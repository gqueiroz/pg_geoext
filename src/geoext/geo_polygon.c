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
 * \file geoext/geo_polygon.c
 *
 * \brief A geo_polygon represents a simple polygon in 2D-space.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 * \author Rafael Monteiro Mariano
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */

/* GeoExtension */
#include "geo_polygon.h"
#include "geo_point.h"
#include "algorithms.h"
#include "hexutils.h"
#include "wkt.h"


/* PostgreSQL */
#include <libpq/pqformat.h>
#include <utils/builtins.h>


/* C Standard Library */
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <string.h>

/*
  An Hex-String encoding a Polygon must have at least:
  - srid: sizeof(int32)
  - npts: sizeof(int32)
  - 2 coordinates: 2 * sizeof(struct coord2d)
  Note that int hex we will have the double of bytes!
 */
#define GEOEXT_MIN_GEOPOLYGONSTRING_HEX_LEN \
(2 * ((2 * sizeof(int32)) + (2 * sizeof(struct coord2d))))

PG_FUNCTION_INFO_V1(geo_polygon_from_text);

Datum
geo_polygon_from_text(PG_FUNCTION_ARGS){

  char *str = PG_GETARG_CSTRING(0);
  int npts = coord_count(str);
  struct geo_polygon *poly = NULL;
  int base_size = npts * sizeof(struct coord2d);
  int size = offsetof(struct geo_polygon, coords) + base_size;

  if(npts < 4)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("Polygon requires more points: minimum 4")));

 poly = (struct geo_polygon*) palloc(size);

  SET_VARSIZE(poly, size);

  poly->npts = npts;

  geo_polygon_wkt_decode(str, poly);

  PG_RETURN_GEOPOLYGON_TYPE_P(poly);
// /elog(NOTICE, "teste 1");
}


PG_FUNCTION_INFO_V1(geo_polygon_in);

Datum
geo_polygon_in(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);

  char *hstr = str;

  struct geo_polygon *poly = NULL;

  int hstr_size = strlen(str);

  int32 srid = 0;

  int32 npts = 0;

  int size = 0;

  int base_size = 0;


  if (hstr_size < GEOEXT_MIN_GEOPOLYGONSTRING_HEX_LEN)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_polygon", str)));

/* get the srid from the hex-string and advance the hstr pointer */
  hex2binary(hstr, 2 * sizeof(int32), (char*)&srid);

  hstr += 2 * sizeof(int32);

/* get the npts from the hex-string and advance the hstr pointer */
  hex2binary(hstr, 2 * sizeof(int32), (char*)&npts);

  hstr += 2 * sizeof(int32);

/* compute the number of bytes required to store the linestring */
  base_size = npts * sizeof(struct coord2d);

  size = offsetof(struct geo_polygon, coords) + base_size;

  /*elog(NOTICE, "number of bytes required for Polygon '%s': %d", str, size);*/

  poly = (struct geo_polygon*) palloc(size);

  SET_VARSIZE(poly, size);

/* set the fields of polygon */
  poly->srid = srid;
  poly->npts = npts;
  poly->dummy = 0;

/* read the coordinates from the hex-string*/
  hex2binary(hstr, 2 * base_size, (char*)(poly->coords));

  PG_RETURN_GEOPOLYGON_TYPE_P(poly);
}


PG_FUNCTION_INFO_V1(geo_polygon_out);

Datum
geo_polygon_out(PG_FUNCTION_ARGS)
{
  struct geo_polygon *poly = PG_GETARG_GEOPOLYGON_TYPE_P(0);

/* compute the Polygon size in bytes */
  int size = ( 2 * sizeof(int32) ) +
      ( poly->npts * sizeof(struct coord2d) );

/* allocate a buffer for an hex-string (with room for a trailing '\0') */
  char *hstr = palloc(2 * size + 1);

  char* cp = NULL;

  /*elog(NOTICE, "geo_linestring_out called");*/

/* encode the srid and npts */
  binary2hex((char*)(&poly->srid), 2 * sizeof(int32), hstr);

/* advance the bytes used for encoding srid and npts */
  cp = hstr + 2 * (2 * sizeof(int32));

/* encode the coordinates */
  binary2hex((char*)poly->coords, poly->npts * sizeof(struct geo_polygon), cp);

  PG_RETURN_CSTRING(hstr);
}

PG_FUNCTION_INFO_V1(geo_polygon_to_str);

Datum
geo_polygon_to_str(PG_FUNCTION_ARGS){

  struct geo_polygon *poly = PG_GETARG_GEOPOLYGON_TYPE_P(0);

  PG_RETURN_CSTRING(geo_polygon_wkt_encode(poly));

}

PG_FUNCTION_INFO_V1(geo_polygon_contains_point);

Datum
geo_polygon_contains_point(PG_FUNCTION_ARGS){

  struct geo_polygon *poly = PG_GETARG_GEOPOLYGON_TYPE_P(0);
  struct geo_point *point = PG_GETARG_GEOPOINT_TYPE_P(1);

  int boolean = point_in_polygon(&point->coord, &poly->coords, poly->npts);

  if (boolean == 1)
    PG_RETURN_CSTRING("TRUE");
  PG_RETURN_CSTRING("FALSE");
}

PG_FUNCTION_INFO_V1(geo_polygon_area);

Datum
geo_polygon_area(PG_FUNCTION_ARGS){

  struct geo_polygon *poly = PG_GETARG_GEOPOLYGON_TYPE_P(0);
  float8 result = area(&poly->coords, poly->npts);

  PG_RETURN_FLOAT8(result);
}
