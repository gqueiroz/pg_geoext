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
* \file geoext/trajectory.h
*
* \brief
*
*
* \author Gilberto Ribeiro de Queiroz
* \author Fabiana Zioti
*
* \date 2017
*
* \copyright GNU Lesser Public License version 3
*
*/

#ifndef __GEOEXT_TRAJECTORY_H__
#define __GEOEXT_TRAJECTORY_H__

/* PostgreSQL */
#include <postgres.h>
#include <fmgr.h>
#include <utils/timestamp.h>

/* GeoExt */
#include "decls.h"

/*
 * A geo_trajc_elem is
 *
 * It is a fixed-size data type in PostgreSQL with a double-alignment.
 *
 */
struct geo_trajc_elem
{
  Timestamp time_elem;     /*Time of object*/
  struct geo_point point_elem;  /* 2D coordinate.*/
};

#define DatumGetGeoTrajETypeP(X) ((struct geo_trajc_elem*) DatumGetPointer(X))
#define PG_GETARG_GEOTRAJE_TYPE_P(n)  DatumGetGeoTrajETypeP(PG_GETARG_DATUM(n))
#define PG_RETURN_GEOTRAJE_TYPE_P(x)  PG_RETURN_POINTER(x)




/*
 * trajectory operations.
 *
 */

inline contruct_array_internal(int typlen, int typbyval, int typalign);

extern Datum trajectory_elem_in(PG_FUNCTION_ARGS);
extern Datum trajectory_elem_out(PG_FUNCTION_ARGS);
extern Datum get_trajectory_elem(PG_FUNCTION_ARGS);

extern Datum trajectory_to_array(PG_FUNCTION_ARGS);
extern Datum trajectory_to_array_final(PG_FUNCTION_ARGS);

#endif  /* __GEOEXT_H__ */
