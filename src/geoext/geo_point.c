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
 * \file geoext/geo_point.c
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

/* GeoExtension */
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
 * Utility macros.
 */
#define GEOEXT_GEOPOINT_SIZE ( sizeof(struct geo_point) - sizeof(int32) )
#define GEOEXT_GEOPOINT_HEX_LEN ( ( 2 * GEOEXT_GEOPOINT_SIZE ) )


/*
 * I/O Functions for the geo_point data type
 */

PG_FUNCTION_INFO_V1(geo_point_in);

Datum
geo_point_in(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);

  struct geo_point *pt = (struct geo_point*) palloc(sizeof(struct geo_point));

  /*elog(NOTICE, "geo_point_in called for: %s", str);*/

  if (!PointerIsValid(str))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_point_in")));

  if (strlen(str) != GEOEXT_GEOPOINT_HEX_LEN)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_point", str)));

  hex2binary(str, GEOEXT_GEOPOINT_HEX_LEN, (char*)pt);

/*
  prevent instability in unused pad bytes!
  the DBMS may do wrong decisions if we don't zero all fields!
 */
  pt->srid = 0;
  pt->dummy = 0;

  PG_RETURN_GEOPOINT_TYPE_P(pt);
}


PG_FUNCTION_INFO_V1(geo_point_out);

Datum
geo_point_out(PG_FUNCTION_ARGS)
{
  struct geo_point *pt = PG_GETARG_GEOPOINT_TYPE_P(0);

/* alloc a buffer for hex-string plus a trailing '\0' */
  char *hstr = palloc(GEOEXT_GEOPOINT_HEX_LEN + 1); 

  /*elog(NOTICE, "geo_point_out called");*/

  if (!PointerIsValid(pt))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_point_out")));

  binary2hex((char*)pt, GEOEXT_GEOPOINT_SIZE, hstr);

  PG_RETURN_CSTRING(hstr);
}


PG_FUNCTION_INFO_V1(geo_point_recv);

Datum
geo_point_recv(PG_FUNCTION_ARGS)
{
  StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);

  struct geo_point *result = (struct geo_point*) palloc(sizeof(struct geo_point));

  /*elog(NOTICE, "geo_point_recv called");*/

  if (!PointerIsValid(buf))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_point_recv")));

  result->coord.x = pq_getmsgfloat8(buf);
  result->coord.y = pq_getmsgfloat8(buf);
  result->srid = pq_getmsgint(buf, sizeof(int32));

/*
  prevent instability in unused pad bytes!
  the DBMS may do wrong decisions if we don't zero all fields!
 */
  result->dummy = 0;

  PG_RETURN_GEOPOINT_TYPE_P(result);
}


PG_FUNCTION_INFO_V1(geo_point_send);

Datum
geo_point_send(PG_FUNCTION_ARGS)
{
  struct geo_point *pt = PG_GETARG_GEOPOINT_TYPE_P(0);

  StringInfoData buf;

  /*elog(NOTICE, "geo_point_send called");*/

  if (!PointerIsValid(pt))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_point_send")));

  pq_begintypsend(&buf);

  pq_sendfloat8(&buf, pt->coord.x);
  pq_sendfloat8(&buf, pt->coord.y);
  pq_sendint(&buf, pt->srid, sizeof(int32));

  PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}


/*
 * geo_point operations
 */

PG_FUNCTION_INFO_V1(geo_point_from_text);

Datum
geo_point_from_text(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);

  struct geo_point *pt = (struct geo_point*) palloc(sizeof(struct geo_point));

  /*elog(NOTICE, "geo_point_from_text called for: %s", str);*/

  if (!PointerIsValid(pt))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_point_from_text")));

  geo_point_wkt_decode(str, pt);

  PG_RETURN_GEOPOINT_TYPE_P(pt);
}


PG_FUNCTION_INFO_V1(geo_point_to_str);

Datum
geo_point_to_str(PG_FUNCTION_ARGS)
{
  struct geo_point *pt = PG_GETARG_GEOPOINT_TYPE_P(0);

  /*elog(NOTICE, "geo_point_to_str called");*/

  if (!PointerIsValid(pt))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_point_to_str")));

  PG_RETURN_CSTRING(geo_point_wkt_encode(pt));
}


PG_FUNCTION_INFO_V1(geo_point_distance);

Datum
geo_point_distance(PG_FUNCTION_ARGS)
{
  struct geo_point *pt1 = PG_GETARG_GEOPOINT_TYPE_P(0);

  struct geo_point *pt2 = PG_GETARG_GEOPOINT_TYPE_P(1);

  float8 dist = 0.0;

  /*elog(NOTICE, "calling geo_point_distance!");*/

  if(!PointerIsValid(pt1))
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("First argument for distance between points is not valid.")));

  if(!PointerIsValid(pt2))
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("Second argument for distance between points is not valid.")));

  if(pt1->srid != pt2->srid)
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("The point arguments have different SRIDs: %d e %d .", pt1->srid, pt2->srid)));

  dist = euclidian_distance(&(pt1->coord), &(pt2->coord));

  PG_RETURN_FLOAT8(dist);
}
