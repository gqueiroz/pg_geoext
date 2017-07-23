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
 * \file geoext/geoext.h
 *
 * \brief GeoExt spatial types for PostgreSQL.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */

#ifndef __GEOEXT_GEOEXT_H__
#define __GEOEXT_GEOEXT_H__

/* PostgreSQL */
#include <postgres.h>
#include <fmgr.h>

/* GeoExt */
#include "algorithms.h"


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
 * A geo_linestring represents a simple polygon line in 2D-space.
 *
 * It is a variable-length data type in PostgreSQL with a double-alignment.
 *
 */
struct geo_linestring
{
  int32 vl_len_;  /* Varlena header.                    */
  int32 npts;     /* Number of points in the polygon.   */
  int32 srid;     /* The Spatial Reference System ID.   */
  int32 dummy;    /* Padding to make it double aligned. */
  struct coord2d coords[FLEXIBLE_ARRAY_MEMBER];   /* The array of polygon vertices.     */
};


/*
 * A geo_polygon represents a simple polygon in 2D-space.
 *
 * It is a variable-length data type in PostgreSQL with a double-alignment.
 *
 */
struct geo_polygon
{
  int32 vl_len_;  /* Varlena header.                    */
  int32 npts;     /* Number of points in the polygon.   */
  int32 srid;     /* The Spatial Reference System ID.   */
  int32 dummy;    /* Padding to make it double aligned. */
  struct coord2d coords[FLEXIBLE_ARRAY_MEMBER];   /* The array of polygon vertices.     */
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
 * geo_polygon is a toastable varlena type.
 *
 * Below we have the fmgr interface macros for dealing with a geo_polygon.
 *
 */
#define DatumGetGeoPolygonTypeP(X)      ((struct geo_polygon*) PG_DETOAST_DATUM(X))
#define PG_GETARG_GEOPOLYGON_TYPE_P(n)  DatumGetGeoPolygonTypeP(PG_GETARG_DATUM(n))
#define PG_RETURN_GEOPOLYGON_TYPE_P(x)  PG_RETURN_POINTER(x)


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


/*
 * geo_polygon operations.
 *
 */
/*extern Datum geo_polygon_in(PG_FUNCTION_ARGS);
extern Datum geo_polygon_out(PG_FUNCTION_ARGS);

extern Datum geo_polygon_recv(PG_FUNCTION_ARGS);
extern Datum geo_polygon_send(PG_FUNCTION_ARGS);

extern Datum geo_polygon_from_text(PG_FUNCTION_ARGS);
extern Datum geo_polygon_to_str(PG_FUNCTION_ARGS);

extern Datum geo_polygon_area(PG_FUNCTION_ARGS);
extern Datum geo_polygon_perimeter(PG_FUNCTION_ARGS);
extern Datum geo_polygon_contains_point(PG_FUNCTION_ARGS);*/


#endif  /* __GEOEXT_H__ */
