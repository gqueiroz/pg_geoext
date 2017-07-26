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
 * \file geoext/geo_linestring.c
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

/* GeoExtension */
#include "geo_linestring.h"
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
 * Utility macros.
 */

/*
  An Hex-String encoding a LineString must have at least:
  - srid: sizeof(int32)
  - npts: sizeof(int32)
  - 2 coordinates: 2 * sizeof(struct coord2d)
  Note that int hex we will have the double of bytes!
 */
#define GEOEXT_MIN_GEOLINESTRING_HEX_LEN \
(2 * ((2 * sizeof(int32)) + (2 * sizeof(struct coord2d))))


/*
 * I/O Functions for the geo_point data type
 */

PG_FUNCTION_INFO_V1(geo_linestring_in);

Datum
geo_linestring_in(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);

  char *hstr = str;

  struct geo_linestring *line = NULL;

  int hstr_size = strlen(str);

  int32 srid = 0;

  int32 npts = 0;

  int size = 0;

  int base_size = 0;

  /*elog(NOTICE, "geo_linestring_in called for: %s", str);*/

  if (hstr_size < GEOEXT_MIN_GEOLINESTRING_HEX_LEN)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_linestring", str)));

/* get the srid from the hex-string and advance the hstr pointer */
  hex2binary(hstr, 2 * sizeof(int32), (char*)&srid);

  hstr += 2 * sizeof(int32);

/* get the npts from the hex-string and advance the hstr pointer */
  hex2binary(hstr, 2 * sizeof(int32), (char*)&npts);

  hstr += 2 * sizeof(int32);

/* compute the number of bytes required to store the linestring */
  base_size = npts * sizeof(struct coord2d);

  size = offsetof(struct geo_linestring, coords) + base_size;

  /*elog(NOTICE, "number of bytes required for LineString '%s': %d", str, size);*/

  line = (struct geo_linestring*) palloc(size);

  SET_VARSIZE(line, size);

/* set the fields of linestring */
  line->srid = srid;
  line->npts = npts;
  line->dummy = 0;

/* read the coordinates from the hex-string*/
  hex2binary(hstr, 2 * base_size, (char*)(line->coords));

  PG_RETURN_GEOLINESTRING_TYPE_P(line);
}


PG_FUNCTION_INFO_V1(geo_linestring_out);

Datum
geo_linestring_out(PG_FUNCTION_ARGS)
{
  struct geo_linestring *line = PG_GETARG_GEOLINESTRING_TYPE_P(0);

/* compute the LineString size in bytes */
  int size = ( 2 * sizeof(int32) ) + ( line->npts * sizeof(struct coord2d) );

/* allocate a buffer for an hex-string (with room for a trailing '\0') */
  char *hstr = palloc(2 * size + 1);

  char* cp = NULL;

  /*elog(NOTICE, "geo_linestring_out called");*/

/* encode the srid and npts */
  binary2hex((char*)(&line->srid), 2 * sizeof(int32), hstr);

/* advance the bytes used for encoding srid and npts */
  cp = hstr + 2 * (2 * sizeof(int32));

/* encode the coordinates */
  binary2hex((char*)line->coords, line->npts * sizeof(struct geo_linestring), cp);

  PG_RETURN_CSTRING(hstr);
}

/*extern Datum geo_linestring_recv(PG_FUNCTION_ARGS);*/
/*extern Datum geo_linestring_send(PG_FUNCTION_ARGS);*/


/*
 * geo_point operations
 */

PG_FUNCTION_INFO_V1(geo_linestring_from_text);

Datum
geo_linestring_from_text(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);

  struct geo_linestring *line = NULL;

  int npts = coord_count(str);

  int base_size = npts * sizeof(struct coord2d);

  int size = offsetof(struct geo_linestring, coords) + base_size;

  /*elog(NOTICE, "geo_linestring_from_text called for: %s", str);*/

  /*elog(NOTICE, "number of bytes required for LineString '%s': %d", str, size);*/

  if(npts < 2)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_linestring", str)));

  line = (struct geo_linestring*) palloc(size);

  SET_VARSIZE(line, size);

  line->npts = npts;

  geo_linestring_wkt_decode(str, line);

  PG_RETURN_GEOLINESTRING_TYPE_P(line);
}


PG_FUNCTION_INFO_V1(geo_linestring_to_str);

Datum
geo_linestring_to_str(PG_FUNCTION_ARGS)
{
  struct geo_linestring *line = PG_GETARG_GEOLINESTRING_TYPE_P(0);

  /*elog(NOTICE, "geo_linestring_to_str called");*/

  PG_RETURN_CSTRING(geo_linestring_wkt_encode(line));
}

/*extern Datum geo_linestring_is_closed(PG_FUNCTION_ARGS);*/
/*extern Datum geo_linestring_length(PG_FUNCTION_ARGS);*/
/*extern Datum geo_linestring_intersection_points(PG_FUNCTION_ARGS);*/
