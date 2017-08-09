/*
  Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.

  This file is part of pg_geoext, a simple PostgreSQL extension for
  for teaching spatial database classes.

  pg_geoext is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 3 as
  published by the Free Software Foundation.

  pg_geoext is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pg_geoext. See LICENSE. If not, write to
  Gilberto Ribeiro de Queiroz at <gribeiro@dpi.inpe.br>.
 */

/*!
 *
 * \file geoext/geo_boc_rtree_gist.c
 *
 * \brief Access methods interface to PostgreSQL R-tree GiST.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */

/*
* Base code: src/backend/access/gist/gistproc.c
* R-Tree functionality for some of the built-in geometric data types
*/


/* GeoExtension */
#include "geo_box.h"

/* PostgreSQL */
#include <utils/builtins.h>

/* PostgreSQL GiST */
#include <access/stratnum.h>
#include <access/gist.h>

/* Convenience macros comparisons */

#define MIN(a,b) (((a)<(b)) ? (a):(b))
#define MAX(a,b) (((a)>(b)) ? (a):(b))

/*
 * Auxiliar function for penalty
 */

 /* Size of a BOX for penalty-calculation purposes */

static inline double
size_gbox (const struct geo_box *gbox)
{

  if(gbox == NULL || gbox->high.x <= gbox->low.x || gbox->high.y <= gbox->low.y){
    return 0.0;
 	}
 	else{
 		return (gbox->high.x - gbox->low.x) * (gbox->high.y - gbox->low.y);
 	}

}


/*
 * Calculates union of two boxes, a and b. The result is stored in *n.
 */

static inline void
rt_gbox_union(struct geo_box *n, const struct geo_box *a, const struct geo_box *b)
{
 	n->high.x = Max(a->high.x, b->high.x);
 	n->high.y = Max(a->high.y, b->high.y);
 	n->low.x = Min(a->low.x, b->low.x);
 	n->low.y = Min(a->low.y, b->low.y);
}


/* Return amount by which the union of the two boxes is larger than the original geo_box's area.*/
static inline double
penalty_geo_box(struct geo_box *orig, const struct geo_box *new)
{
  struct geo_box unionbox;

  rt_gbox_union(&unionbox, orig, new);
	return size_gbox(&unionbox) - size_gbox(orig);

}

/*
 * Auxiliar function for union
 * Increase geo_box pageunion to include tmp
 */

static inline void
adjustGeoBox(struct geo_box *pageunion, const struct geo_box *tmp) {

  if(float8_cmp_internal(pageunion->high.x, tmp->high.x) < 0){
    pageunion->high.x = tmp->high.x;
  }
  if (float8_cmp_internal(pageunion->low.x, tmp->low.x) > 0) {
    pageunion->low.x = tmp->low.x;
  }
  if (float8_cmp_internal(pageunion->high.y, tmp->high.y) < 0) {
    pageunion->high.y = tmp->high.y;
  }
  if (float8_cmp_internal(pageunion->low.y, tmp->low.y) > 0) {
    pageunion->low.y = tmp->low.y;
  }

}



/*	Leaf-level consistency for geo_box with geo_box: just apply the query operator */

static inline bool
gbox_leaf_consistent(struct geo_box *key, struct geo_box *query, StrategyNumber strategy)
{
  bool retval;

  elog(NOTICE, "gbox_leaf_consistent");

	switch (strategy)
  {
    case RTLeftStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_left, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		// case RTOverLeftStrategyNumber:
		// 	retval = DatumGetBool(DirectFunctionCall2(geo_box_overleft, PointerGetDatum(key), PointerGetDatum(query)));
		// 	break;
		case RTOverlapStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_overlap, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		// case RTOverRightStrategyNumber:
		// 	retval = DatumGetBool(DirectFunctionCall2(geo_box_overright, PointerGetDatum(key), PointerGetDatum(query)));
		// 	break;
		case RTRightStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_right, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		case RTSameStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_same, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		case RTContainsStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_contain, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		case RTContainedByStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_contained, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		default:
			elog(ERROR, "Unrecognized strategy number: %d", strategy);
			retval = FALSE;

	}

	return retval;

}


/* Received two geo_box: key and query.
*
* Internal-page consistency for all types.
*
* We can use the same function since these geo_types use bounding boxes both as the
* internal-page representation and also for the query.
*/

static inline bool
rtree_internal_consistent(struct geo_box *key, struct geo_box *query, StrategyNumber strategy)
{
  bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
		case RTOverLeftStrategyNumber:
			// retval = DatumGetBool(DirectFunctionCall2(geo_box_overleft, PointerGetDatum(key), PointerGetDatum(query)));
			// break;
		case RTOverlapStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_overlap, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		case RTOverRightStrategyNumber:
		case RTRightStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_right, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		case RTSameStrategyNumber:
		case RTContainsStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_contain, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		case RTContainedByStrategyNumber:
			retval = DatumGetBool(DirectFunctionCall2(geo_box_overlap, PointerGetDatum(key), PointerGetDatum(query)));
			break;
		default:
			retval = FALSE;
	}
	return (retval);

}


PG_FUNCTION_INFO_V1(geo_box_consistent);

Datum
geo_box_consistent(PG_FUNCTION_ARGS)
{
  /* Contains the key, as well as its own location (rel,page,offset) which can supply the matching pointer*/
  GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);

  struct geo_box *query = PG_GETARG_GEOBOX_TYPE_P(1);

  StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);

  /* Oid		subtype = PG_GETARG_OID(3); */

  bool *recheck = (bool *) PG_GETARG_POINTER(4);

  bool retval;

  /* elog(NOTICE, "geo_box_consistent CALL"); */

  /* All cases served by this function are exact */
	*recheck = false;


  if (!(DatumGetPointer(entry->key) != NULL && query))
    PG_RETURN_BOOL(FALSE);

	/* If the value is on the final page, then it's exact comparison. */
  /* If it is inside, we should return true if the values ​​in the later pages (children) can satisfy the condition */
	if (GIST_LEAF(entry)){
    retval = gbox_leaf_consistent((struct geo_box *) DatumGetPointer(entry->key), query, strategy);
  }
	else{
    retval = rtree_internal_consistent((struct geo_box *) DatumGetPointer(entry->key), query, strategy);
  }

  PG_RETURN_BOOL(retval);

}


PG_FUNCTION_INFO_V1(geo_box_union);

Datum
geo_box_union(PG_FUNCTION_ARGS)
{
  /* Vector of GISTENTRY structs; user-defined methods union and picksplit */
  GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);

  int *sizep = (int *) PG_GETARG_POINTER(1);

	int			numranges, i;

	struct geo_box *tmp, *pageunion;


  numranges = entryvec->n;

	pageunion = (struct geo_box *) palloc(sizeof(struct geo_box));

	tmp = DatumGetGeoBoxTypeP(entryvec->vector[0].key);

	memcpy((void *) pageunion, (void *) tmp, sizeof(struct geo_box));

	for(i = 1; i < numranges; i++)
	{
		tmp = DatumGetGeoBoxTypeP(entryvec->vector[i].key);
		adjustGeoBox(pageunion, tmp);
	}

	*sizep = sizeof(struct geo_box);

	/*The union function should return a pointer to newly palloc()ed memory.*/
	PG_RETURN_POINTER(pageunion);

}


PG_FUNCTION_INFO_V1(geo_box_compress);

Datum
geo_box_compress(PG_FUNCTION_ARGS)
{
  PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}


PG_FUNCTION_INFO_V1(geo_box_decompress);

Datum
geo_box_decompress(PG_FUNCTION_ARGS)
{
  PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}


PG_FUNCTION_INFO_V1(g_box_same);

Datum
g_box_same(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);

	struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  bool *result = (bool *) PG_GETARG_POINTER(2);

  if (first && second)
    *result = DatumGetBool(DirectFunctionCall2(geo_box_same, PointerGetDatum(first), PointerGetDatum(second)));
	else{
		if (first == NULL && second == NULL)
      *result = TRUE;
		else
      *result = FALSE;
	}

   PG_RETURN_POINTER(result);

}

PG_FUNCTION_INFO_V1(geo_box_penalty);

Datum
geo_box_penalty(PG_FUNCTION_ARGS)
{
  GISTENTRY *origentry = (GISTENTRY *) PG_GETARG_POINTER(0);

  GISTENTRY *newentry = (GISTENTRY *) PG_GETARG_POINTER(1);

	float	*penalty = (float *) PG_GETARG_POINTER(2);

  struct geo_box *orig = DatumGetGeoBoxTypeP(origentry->key);
  struct geo_box *new = DatumGetGeoBoxTypeP(newentry->key);

  *penalty = (float) penalty_geo_box(orig, new);

  PG_RETURN_POINTER(penalty);

}
