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
 * \file geoext/geo_linestring.h
 *
 * \brief A geo_linestring represents a simple polygon line in 2D-space.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */

#ifndef __GEOEXT_GEO_LINESTRING_H__
#define __GEOEXT_GEO_LINESTRING_H__

/* PostgreSQL */
#include <postgres.h>
#include <fmgr.h>

/* GeoExt */
#include "decls.h"


/*
 * A geo_linestring represents a simple polygon line in 2D-space.
 *
 * It is a variable-length data type in PostgreSQL with a double-alignment.
 *
 */
struct geo_linestring
{
  int32 vl_len_;  /* Varlena header.                    */
  int32 dummy;    /* Padding to make it double aligned. */
  int32 srid;     /* The Spatial Reference System ID.   */
  int32 npts;     /* Number of points in the polygon.   */
  struct coord2d coords[FLEXIBLE_ARRAY_MEMBER];   /* The array of polygon vertices.     */
};


/*
 * geo_linestring is a toastable varlena type.
 *
 * Below we have the fmgr interface macros for dealing with a geo_linestring.
 *
 */
#define DatumGetGeoLineStringTypeP(X)      ((struct geo_linestring*) PG_DETOAST_DATUM(X))
#define PG_GETARG_GEOLINESTRING_TYPE_P(n)  DatumGetGeoLineStringTypeP(PG_GETARG_DATUM(n))
#define PG_RETURN_GEOLINESTRING_TYPE_P(x)  PG_RETURN_POINTER(x)


/*
 * geo_linestring operations.
 *
 */
extern Datum geo_linestring_in(PG_FUNCTION_ARGS);
extern Datum geo_linestring_out(PG_FUNCTION_ARGS);

extern Datum geo_linestring_recv(PG_FUNCTION_ARGS);
extern Datum geo_linestring_send(PG_FUNCTION_ARGS);

extern Datum geo_linestring_from_text(PG_FUNCTION_ARGS);
extern Datum geo_linestring_to_str(PG_FUNCTION_ARGS);

extern Datum geo_linestring_is_closed(PG_FUNCTION_ARGS);
extern Datum geo_linestring_length(PG_FUNCTION_ARGS);

/* create a geo_linestring from a pair of points represented by a composite */
extern Datum geo_linestring_make_v1(PG_FUNCTION_ARGS);

/* return the boundary points as a composite pair */
extern Datum geo_linestring_boundary_v1(PG_FUNCTION_ARGS);

/* return the boundary points as a set of points */
extern Datum geo_linestring_boundary_v2(PG_FUNCTION_ARGS);


extern Datum geo_linestring_boundary_points(PG_FUNCTION_ARGS);

/* return the boundary as a composite */
/*extern Datum geo_linestring_boundary_points_v2(PG_FUNCTION_ARGS);*/



extern Datum geo_linestring_make(PG_FUNCTION_ARGS);

extern Datum geo_linestring_intersection(PG_FUNCTION_ARGS);


#endif  /* __GEOEXT_GEO_LINESTRING_H__ */
