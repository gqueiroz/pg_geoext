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
 * \brief GiST:  R-Tree index support for geo_box
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


// inline
// int geo_box_cmp_internal(double a, double b)
// {
//   if(a > b)
//     return 1;
//   if (a < b )
//     return -1;
//   return 0;
// }

inline
int geo_box_cmp_internal(struct coord2d *first, struct coord2d *second)
{
  if(first->x > second->x)
    return 1;
  else if (first->y > second->y)
    return 1;
  else if (first->x < second->x)
    return -1;
  else if (first->y < second->y)
    return -1;
  else
    return 0;

}


PG_FUNCTION_INFO_V1(geo_box_contain);

Datum
geo_box_contain(PG_FUNCTION_ARGS)
{
  struct geo_box *first = PG_GETARG_GEOBOX_TYPE_P(0);
  struct geo_box *second = PG_GETARG_GEOBOX_TYPE_P(1);

  /* elog(NOTICE, "geo_box_contain CALL"); */

  PG_RETURN_BOOL(geo_box_cmp_internal(&first->high, &second->high) >= 0 &&
                geo_box_cmp_internal(&first->low, &second->low) <= 0);
}
