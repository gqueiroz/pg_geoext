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
* \file geoext/geo_box.h
*
* \brief A geo box is a type of support used in the indexing method GiST.
*        It has the upper and lower coordinates of geo types
*
* \author Gilberto Ribeiro de Queiroz
* \author Fabiana Zioti
*
* \date 2017
*
* \copyright GNU Lesser Public License version 3
*
*/

#ifndef __GEOEXT_GEO_BOX_H__
#define __GEOEXT_GEO_BOX_H__

/* PostgreSQL */
#include <postgres.h>
#include <fmgr.h>

/* GeoExt */
#include "decls.h"


/*
* A geo_box is bidimensional box used in the indexing method GiST
*
* It is a fixed-size data type in PostgreSQL with a double-alignment.
*
*/
struct geo_box
{
 struct coord2d high;  /* 2D coordinate. */
 struct coord2d low;  /* 2D coordinate. */
};

/*
* geo_box is a fixed-size pass-by-reference type.
*
* Below we have the fmgr interface macros for dealing with a geo_box.
*
*/

#define DatumGetGeoBoxTypeP(X)      ((struct geo_box*) DatumGetPointer(X))
#define PG_GETARG_GEOBOX_TYPE_P(n)  DatumGetGeoBoxTypeP(PG_GETARG_DATUM(n))
#define PG_RETURN_GEOBOX_TYPE_P(x)  PG_RETURN_POINTER(x)

/*
 * geo_box operations.
 *
 */

/* Operações suportadas pelo tipo geo_box */

extern Datum geo_box_in(PG_FUNCTION_ARGS);
extern Datum geo_box_out(PG_FUNCTION_ARGS);
extern Datum geo_box_from_text(PG_FUNCTION_ARGS);
extern Datum geo_box_to_str(PG_FUNCTION_ARGS);

/*
** support routines for the GiST access method for geo_box
*/
/*
extern Datum geo_box_consistent(PG_FUNCTION_ARGS);
extern Datum geo_box_union(PG_FUNCTION_ARGS);
extern Datum geo_box_compress(PG_FUNCTION_ARGS);
extern Datum geo_box_decompress(PG_FUNCTION_ARGS);
extern Datum geo_box_penalty(PG_FUNCTION_ARGS);
extern Datum geo_box_picksplit(PG_FUNCTION_ARGS);
extern Datum geo_box_same(PG_FUNCTION_ARGS);

*/


/*
*  GiST:  R-Tree index support
*
*/

/*
extern Datum geo_box_contain(PG_FUNCTION_ARGS);
extern Datum geo_box_contained(PG_FUNCTION_ARGS);
extern Datum geo_box_left(PG_FUNCTION_ARGS);
extern Datum geo_box_same_op(PG_FUNCTION_ARGS);
extern Datum geo_box_right(PG_FUNCTION_ARGS);
extern Datum geo_box_below(PG_FUNCTION_ARGS);
extern Datum geo_box_above(PG_FUNCTION_ARGS);
extern Datum geo_box_overlap(PG_FUNCTION_ARGS);

extern Datum geo_box_overleft(PG_FUNCTION_ARGS);
extern Datum geo_box_overright(PG_FUNCTION_ARGS);
// extern Datum geo_box_overbelow(PG_FUNCTION_ARGS);
// extern Datum geo_box_overabove(PG_FUNCTION_ARGS);
extern Datum geo_box_below_eq(PG_FUNCTION_ARGS); // Is obsolete
extern Datum geo_box_above_eq(PG_FUNCTION_ARGS); // Is obsolete

*/


#endif  /* __GEOEXT_H__ */
