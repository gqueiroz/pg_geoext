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
 * \file geoext/geo_box_op.c
 *
 * \brief GiST:  R-Tree operations support for geo_box
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */

/* GeoExtension */
#include "geo_box.h"

/* PostgreSQL */
#include <libpq/pqformat.h>
#include <utils/builtins.h>

/* C Standard Library */
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <string.h>

static inline bool
geo_box_cmp_internal_overlap(struct geo_box *first, struct geo_box *second)
{
  return (DatumGetBool(DirectFunctionCall2(geo_box_left, PointerGetDatum(first), PointerGetDatum(second))) ||
          DatumGetBool(DirectFunctionCall2(geo_box_right, PointerGetDatum(first), PointerGetDatum(second))) ||
          DatumGetBool(DirectFunctionCall2(geo_box_above, PointerGetDatum(first), PointerGetDatum(second))) ||
          DatumGetBool(DirectFunctionCall2(geo_box_below, PointerGetDatum(first), PointerGetDatum(second))));

}


PG_FUNCTION_INFO_V1(geo_box_contain);

Datum
geo_box_contain(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_contain CALL"); */

  PG_RETURN_BOOL(float8_cmp_internal(first->high.x, second->high.x) >= 0 &&
                 float8_cmp_internal(first->high.y, second->high.y) >= 0 &&
                 float8_cmp_internal(first->low.x, second->low.x) <= 0 &&
                 float8_cmp_internal(first->low.y, second->low.y) <= 0
               );

}


PG_FUNCTION_INFO_V1(geo_box_contained);

Datum
geo_box_contained(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_contained CALL"); */

  PG_RETURN_BOOL(float8_cmp_internal(first->high.x, second->high.x) <= 0 &&
                 float8_cmp_internal(first->high.y, second->high.y) <= 0 &&
                 float8_cmp_internal(first->low.x, second->low.x) >= 0 &&
                 float8_cmp_internal(first->low.y, second->low.y) >= 0
               );

}


PG_FUNCTION_INFO_V1(geo_box_left);

Datum
geo_box_left(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_left CALL"); */

  PG_RETURN_BOOL(float8_cmp_internal(first->high.x, second->low.x) < 0);

}


PG_FUNCTION_INFO_V1(geo_box_same);

Datum
geo_box_same(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_same CALL"); */

  PG_RETURN_BOOL(float8_cmp_internal(first->high.x, second->high.x) == 0 &&
                 float8_cmp_internal(first->high.y, second->high.y) == 0 &&
                 float8_cmp_internal(first->low.x, second->low.x) == 0 &&
                 float8_cmp_internal(first->low.y, second->low.y) == 0
               );
}


PG_FUNCTION_INFO_V1(geo_box_right);

Datum
geo_box_right(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_right CALL"); */

  PG_RETURN_BOOL(float8_cmp_internal(first->low.x, second->high.x) > 0);

}

PG_FUNCTION_INFO_V1(geo_box_below);

Datum
geo_box_below(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_below CALL"); */

  PG_RETURN_BOOL(float8_cmp_internal(first->high.y, second->low.y) < 0);
}


PG_FUNCTION_INFO_V1(geo_box_above);

Datum
geo_box_above(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_above CALL"); */

  PG_RETURN_BOOL(float8_cmp_internal(first->low.y, second->high.y) > 0);

}


PG_FUNCTION_INFO_V1(geo_box_overlap);

Datum
geo_box_overlap(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_overlap CALL"); */

  PG_RETURN_BOOL(!geo_box_cmp_internal_overlap(first, second));
}

/*
PG_FUNCTION_INFO_V1(geo_box_overleft);

Datum
geo_box_overleft(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  if(DatumGetBool(DirectFunctionCall2(geo_box_overlap, PointerGetDatum(first), PointerGetDatum(second))))
  {

  }
}

*/
