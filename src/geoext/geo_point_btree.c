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
 * \file geoext/geo_point_btree.c
 *
 * \brief Extension interface to PostgreSQL B-tree.
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
#include "geo_point.h"
#include "algorithms.h"


/* PostgreSQL */
#include <utils/builtins.h>


/* C Standard Library */
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <string.h>

/*
 * Internal B-tree comparison support function.
 */
static inline int
geo_point_cmp_i(struct geo_point *first,
                struct geo_point *second)
{
  if(first->srid != second->srid)
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
            errmsg("The point arguments have different SRIDs: %d e %d .",
                   first->srid, second->srid)));
  
  if(first->coord.x < second->coord.x)
    return -1;
  else if(first->coord.x > second->coord.x)
    return 1;
  else if(first->coord.y < second->coord.y)
    return -1;
  else if(first->coord.y > second->coord.y)
    return 1;
  else
    return 0;
}


/*
 * B-tree operators for geo_point
 */

PG_FUNCTION_INFO_V1(geo_point_cmp);

Datum
geo_point_cmp(PG_FUNCTION_ARGS)
{
  struct geo_point *first = PG_GETARG_GEOPOINT_TYPE_P(0);
  struct geo_point *second = PG_GETARG_GEOPOINT_TYPE_P(1);
  
  PG_RETURN_INT32(geo_point_cmp_i(first, second));
}


PG_FUNCTION_INFO_V1(geo_point_eq);

Datum
geo_point_eq(PG_FUNCTION_ARGS)
{
  struct geo_point *first = PG_GETARG_GEOPOINT_TYPE_P(0);
  struct geo_point *second = PG_GETARG_GEOPOINT_TYPE_P(1);
  
  PG_RETURN_BOOL(geo_point_cmp_i(first, second) == 0);
}


PG_FUNCTION_INFO_V1(geo_point_ne);

Datum
geo_point_ne(PG_FUNCTION_ARGS)
{
  struct geo_point *first = PG_GETARG_GEOPOINT_TYPE_P(0);
  struct geo_point *second = PG_GETARG_GEOPOINT_TYPE_P(1);
  
  PG_RETURN_BOOL(geo_point_cmp_i(first, second) != 0);
}


PG_FUNCTION_INFO_V1(geo_point_lt);

Datum
geo_point_lt(PG_FUNCTION_ARGS)
{
  struct geo_point *first = PG_GETARG_GEOPOINT_TYPE_P(0);
  struct geo_point *second = PG_GETARG_GEOPOINT_TYPE_P(1);
  
  PG_RETURN_BOOL(geo_point_cmp_i(first, second) == -1);
}


PG_FUNCTION_INFO_V1(geo_point_gt);

Datum
geo_point_gt(PG_FUNCTION_ARGS)
{
  struct geo_point *first = PG_GETARG_GEOPOINT_TYPE_P(0);
  struct geo_point *second = PG_GETARG_GEOPOINT_TYPE_P(1);
  
  PG_RETURN_BOOL(geo_point_cmp_i(first, second) == 1);
}


PG_FUNCTION_INFO_V1(geo_point_le);

Datum
geo_point_le(PG_FUNCTION_ARGS)
{
  struct geo_point *first = PG_GETARG_GEOPOINT_TYPE_P(0);
  struct geo_point *second = PG_GETARG_GEOPOINT_TYPE_P(1);
  
  int result = geo_point_cmp_i(first, second);
  
  PG_RETURN_BOOL(result <= 0);
}


PG_FUNCTION_INFO_V1(geo_point_ge);

Datum
geo_point_ge(PG_FUNCTION_ARGS)
{
  struct geo_point *first = PG_GETARG_GEOPOINT_TYPE_P(0);
  struct geo_point *second = PG_GETARG_GEOPOINT_TYPE_P(1);
  
  int result = geo_point_cmp_i(first, second);
  
  PG_RETURN_BOOL(result >= 0);
}
