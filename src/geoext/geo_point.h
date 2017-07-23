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
 * \file geoext/geo_point.h
 *
 * \brief A geo_point is bidimensional point that can be associated to a spatial reference system.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */

#ifndef __GEOEXT_GEO_POINT_H__
#define __GEOEXT_GEO_POINT_H__

/* PostgreSQL */
#include <postgres.h>
#include <fmgr.h>

/* GeoExt */
#include "decls.h"


/*
 * A geo_point is bidimensional point that can be associated to a spatial reference system.
 *
 * It is a fixed-size data type in PostgreSQL with a double-alignment.
 *
 */
struct geo_point
{
  struct coord2d coord;  /* 2D coordinate.                          */
  int32 srid;            /* The Spatial Reference System ID.        */
  int32 dummy;           /* Just to keep the struct double-aligned. */
};


/*
 * geo_point is a fixed-size pass-by-reference type.
 *
 * Below we have the fmgr interface macros for dealing with a geo_point.
 *
 */
#define DatumGetGeoPointTypeP(X)      ((struct geo_point*) DatumGetPointer(X))
#define PG_GETARG_GEOPOINT_TYPE_P(n)  DatumGetGeoPointTypeP(PG_GETARG_DATUM(n))
#define PG_RETURN_GEOPOINT_TYPE_P(x)  PG_RETURN_POINTER(x)


/*
 * geo_point operations.
 *
 */
extern Datum geo_point_in(PG_FUNCTION_ARGS);
extern Datum geo_point_out(PG_FUNCTION_ARGS);

extern Datum geo_point_recv(PG_FUNCTION_ARGS);
extern Datum geo_point_send(PG_FUNCTION_ARGS);

extern Datum geo_point_from_text(PG_FUNCTION_ARGS);
extern Datum geo_point_to_str(PG_FUNCTION_ARGS);

extern Datum geo_point_distance(PG_FUNCTION_ARGS);


#endif  /* __GEOEXT_H__ */
