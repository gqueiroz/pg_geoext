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
#include "geo_point.h"
#include "hexutils.h"
#include "wkt.h"


/* PostgreSQL */
#include <access/htup_details.h> /* for heap_form_tuple() */
#include <catalog/pg_type.h>
#include <executor/executor.h> /* for GetAttributeByNum and GetAttributeByName */
#include "access/htup_details.h" /* for heap_form_tuple() */
#include <libpq/pqformat.h>
#include <utils/builtins.h>
#include <utils/array.h>
#include <utils/lsyscache.h>
#include <funcapi.h>


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
  An hex-string used to encode a LineString must have at least:
  - srid: sizeof(int32)
  - npts: sizeof(int32)
  - 2 coordinates: 2 * sizeof(struct coord2d)
  Note that in hex we will have the double of bytes!
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

  int size = offsetof(struct geo_linestring, srid) + (hstr_size / 2);

  /*elog(NOTICE, "geo_linestring_in called for: %s", str);*/

  if (hstr_size < GEOEXT_MIN_GEOLINESTRING_HEX_LEN)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_linestring", str)));

  /*elog(NOTICE, "number of bytes required for LineString '%s': %d", str, size);*/

  line = (struct geo_linestring*) palloc(size);

  SET_VARSIZE(line, size);
  line->dummy = 0;

/* decode the hex-string */
  hex2binary(hstr, hstr_size, (char*)(&line->srid));

  assert(line->npts >= 2);

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

  /*elog(NOTICE, "geo_linestring_out called");*/

/* encode from srid on */
  binary2hex((char*)(&line->srid), size, hstr);

  PG_RETURN_CSTRING(hstr);
}


PG_FUNCTION_INFO_V1(geo_linestring_recv);

Datum
geo_linestring_recv(PG_FUNCTION_ARGS)
{
  StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);

  struct geo_linestring *line = NULL;

  int32 npts = 0;

  int32 srid = 0;

  int base_size = 0;

  int size = 0;

  /*elog(NOTICE, "geo_linestring_recv called");*/

  if (!PointerIsValid(buf))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_linestring_recv")));

  srid = pq_getmsgint(buf, sizeof(int32));
  npts = pq_getmsgint(buf, sizeof(int32));

  assert(npts >= 2);

  base_size = npts * sizeof(struct coord2d);

  size = offsetof(struct geo_linestring, coords) + base_size;

  line = (struct geo_linestring*) palloc(size);

  SET_VARSIZE(line, size);
  line->dummy = 0;   /* prevent instability in unused pad bytes!
                        the DBMS may do wrong decisions if we don't zero all fields! */
  line->srid = srid;
  line->npts = npts;

  for (int i = 0; i < npts; ++i)
  {
    line->coords[i].x = pq_getmsgfloat8(buf);
    line->coords[i].y = pq_getmsgfloat8(buf);

  }

  PG_RETURN_GEOLINESTRING_TYPE_P(line);
}


PG_FUNCTION_INFO_V1(geo_linestring_send);

Datum
geo_linestring_send(PG_FUNCTION_ARGS)
{
  struct geo_linestring *line = PG_GETARG_GEOLINESTRING_TYPE_P(0);

  StringInfoData buf;

  /*elog(NOTICE, "geo_linestring_send called");*/

 if (!PointerIsValid(line))
   ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("missing argument for geo_linestring_send")));

  pq_begintypsend(&buf);

  pq_sendint(&buf, line->srid, sizeof(int32));
  pq_sendint(&buf, line->npts, sizeof(int32));

  for (int i = 0; i < line->npts; ++i)
  {
    pq_sendfloat8(&buf, line->coords[i].x);
    pq_sendfloat8(&buf, line->coords[i].y);
  }

  PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}


/*
 * geo_linestring operations
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

PG_FUNCTION_INFO_V1(geo_linestring_is_closed);

Datum
geo_linestring_is_closed(PG_FUNCTION_ARGS)
{
  struct geo_linestring *line = PG_GETARG_GEOLINESTRING_TYPE_P(0);

  int result;

  /*elog(NOTICE, "geo_linestring_is_closed called");*/

  int final_position = line->npts - 1;

  result = equals(&(line->coords[0]), &(line->coords[final_position]));

  PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(geo_linestring_length);

Datum
geo_linestring_length(PG_FUNCTION_ARGS)
{
  struct geo_linestring *line = PG_GETARG_GEOLINESTRING_TYPE_P(0);

  /*elog(NOTICE, "geo_linestring_length called");*/

  float8 result = 0.0;

  if(!PointerIsValid(line))
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("LineString argument for length is not valid.")));

  result = length((line->coords), (line->npts));

  PG_RETURN_FLOAT8(result);
}


PG_FUNCTION_INFO_V1(geo_linestring_make_v2);

Datum
geo_linestring_make_v2(PG_FUNCTION_ARGS)
{
  HeapTupleHeader pt_pair = PG_GETARG_HEAPTUPLEHEADER(0);

  bool isnull = false;

  struct geo_point *pt1 = NULL;
  struct geo_point *pt2 = NULL;

  struct geo_linestring *line = NULL;

  Datum first = 0;
  Datum second = 0;

  int size = 0;

  first = GetAttributeByName(pt_pair, "first", &isnull);

  if(isnull)
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("linestring_make_v2 accept a pair of points: first component not provided.")));

  second = GetAttributeByNum(pt_pair, 2, &isnull);

  if(isnull)
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("linestring_make_v2 accept a pair of points: second component not provided.")));

  pt1 = DatumGetGeoPointTypeP(first);
  pt2 = DatumGetGeoPointTypeP(second);

  if(pt1->srid != pt2->srid)
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("linestring_make_v2: first (%d) and second (%d) components have different SRIDs.",
                   pt1->srid, pt2->srid)));

  size = offsetof(struct geo_linestring, coords) + 2 * sizeof(struct coord2d);

  line = (struct geo_linestring*) palloc(size);

  SET_VARSIZE(line, size);

  line->dummy = 0;
  line->srid = pt1->srid;
  line->npts = 2;
  line->coords[0] = pt1->coord;
  line->coords[1] = pt2->coord;

  PG_RETURN_GEOLINESTRING_TYPE_P(line);
}


PG_FUNCTION_INFO_V1(geo_linestring_boundary_v1);

Datum geo_linestring_boundary_v1(PG_FUNCTION_ARGS)
{
  elog(NOTICE, "geo_linestring_boundary_v1 CALL ");

  struct geo_linestring *line = PG_GETARG_GEOLINESTRING_TYPE_P(0);

  HeapTuple tuple;

  TupleDesc tupdesc;

  Oid resultTypeId;

  Datum values[2]; /* first geo_point, second geo_point */

  Datum result;

  struct geo_point *pt1 = (struct geo_point*) palloc(sizeof(struct geo_point));

  struct geo_point *pt2 =  (struct geo_point*) palloc(sizeof(struct geo_point));

  int final_position = line->npts - 1;

  bool isnull[2];


  if(!PointerIsValid(line))
    ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("LineString argument for boundary is not valid.")));


  pt1->coord = line->coords[0];
  pt1->srid = line->srid;

  pt2->coord = line->coords[final_position];
  pt2->srid = line->srid;

  /*elog(NOTICE, "First boundary %g , %g ", pt1->coord.x, pt1->coord.y);*/
  /*elog(NOTICE, "Last boundary  %g , %g ", pt2->coord.x, pt2->coord.y);*/


  /* Build a tuple descriptor for our result type and
   * check that SQL function definition is set up to return a record*/
  if (get_call_result_type(fcinfo, &resultTypeId, &tupdesc) != TYPEFUNC_COMPOSITE)
      ereport(ERROR,
              (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
               errmsg("function returning record called in context "
                      "that cannot accept type record")));

  /* make the tuple descriptor known to postgres as valid return type*/
  BlessTupleDesc(tupdesc);

  values[0] = PointerGetDatum(pt1);
  values[1] = PointerGetDatum(pt2);

  isnull[0] = FALSE;
  isnull[1] = FALSE;

  tuple = heap_form_tuple( tupdesc, values, isnull );

  result = HeapTupleGetDatum(tuple);

  PG_RETURN_DATUM(result);

}




PG_FUNCTION_INFO_V1(geo_linestring_boundary_points);

Datum
geo_linestring_boundary_points(PG_FUNCTION_ARGS)
{
  ArrayType  *result_array;

  Datum *datum_elems;

  struct geo_linestring *line  = PG_GETARG_GEOLINESTRING_TYPE_P(0);

  bool *isnull;

  int ndims, dims[MAXDIM],lbs[MAXDIM], i, j;

  int16 typlen;

  bool typbyval;

  char typalign;

  isnull = (bool *) palloc((line->npts * 2) * sizeof(bool));
  datum_elems = (Datum *) palloc((line->npts * 2) * sizeof(Datum));

  if (!PointerIsValid(line))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_linestring")));

  ndims = 2;
  lbs[0] = 1;
  dims[0] = 2;
  lbs[1] = 1;
  dims[1] = line->npts;

  j = 0;
  for (i = 0; i < line->npts; i++)
  {
    datum_elems[j] = Float8GetDatum(line->coords[i].x);
    isnull[j] = false;
    j++;
  }

  for (i = 0 ; i < line->npts; i++)
  {
    datum_elems[j] = Float8GetDatum(line->coords[i].y);
    isnull[j] = false;
    j++;
  }

  get_typlenbyvalalign(FLOAT8OID, &typlen, &typbyval, &typalign);

  /* construct 1-D array*/
  // result_array = construct_array(datum_elems, line->npts*2, FLOAT8OID, typlen, typbyval, typalign);

  /*construct ndims-D array*/
  result_array = construct_md_array(datum_elems, isnull, ndims, dims, lbs, FLOAT8OID, typlen, typbyval, typalign );

  PG_RETURN_ARRAYTYPE_P(result_array);
}


PG_FUNCTION_INFO_V1(geo_linestring_make);

Datum
geo_linestring_make(PG_FUNCTION_ARGS)
{
  ArrayType *array_x;

  ArrayType *array_y;

  /* variables for "deconstructed" array*/
  Datum *datums_x;
  Datum *datums_y;
  bool *nulls_x;
  bool *nulls_y;
  int count_x, count_y;

  int16 typlen;

  bool typbyval;

  char typalign;

  struct geo_linestring *line = NULL;

  int32 npts;

  int32 srid = 0;

  int base_size;

  int size;

  /*elog(NOTICE, "geo_linestring_from_array called);*/

  array_x = PG_GETARG_ARRAYTYPE_P(0);
  array_y = PG_GETARG_ARRAYTYPE_P(1);


  if (!PointerIsValid(array_x))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument one for geo_linestring_from_array")));


  if (!PointerIsValid(array_y))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument two for geo_linestring_from_array")));


  /*elog(NOTICE, "geo_linestring_from_array insert numbers in line");*/

  /*values found in table pg_type*/
  get_typlenbyvalalign(FLOAT8OID, &typlen, &typbyval, &typalign);

  deconstruct_array(array_x, FLOAT8OID, typlen, typbyval, typalign , &datums_x, &nulls_x, &count_x);
  deconstruct_array(array_y, FLOAT8OID, typlen, typbyval, typalign , &datums_y, &nulls_y, &count_y);

  /* elog(NOTICE, "dimension of arrays %d", count_x); */
  if(count_x != count_y)
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                  errmsg("different dimension of arrays geo_linestring_from_array")));

  npts = count_x;

  base_size = npts * sizeof(struct coord2d);

  size = offsetof(struct geo_linestring, coords) + base_size;

  if(npts < 2)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid syntax for geo_linestring! ")));

  line = (struct geo_linestring*) palloc(size);

  SET_VARSIZE(line, size);

  line->npts = npts;
  line->srid = srid;

  for(int i = 0; i < npts; i++)
  {
    line->coords[i].x = DatumGetFloat8(datums_x[i]);
    /*elog(NOTICE, "geo_linestring x %g",line->coords[i].x );*/

    line->coords[i].y = DatumGetFloat8(datums_y[i]);
    /*elog(NOTICE, "geo_linestring y %g",line->coords[i].y );*/
  }

  PG_RETURN_GEOLINESTRING_TYPE_P(line);

}


PG_FUNCTION_INFO_V1(geo_linestring_intersection);

Datum
geo_linestring_intersection(PG_FUNCTION_ARGS)
{
    FuncCallContext     *funcctx;
    int                  call_cntr;
    int                  max_calls;
    TupleDesc            tupdesc;
    AttInMetadata       *attinmeta;
    struct geo_linestring *line = PG_GETARG_GEOLINESTRING_TYPE_P(0);
    // struct geo_linestring *line2;

    /* elog(NOTICE, "geo_linestring_intersection_points_v1 called");*/

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL())
    {
        MemoryContext   oldcontext;

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        /* total number of tuples to be returned */
        funcctx->max_calls = line->npts;

        /* Build a tuple descriptor for our result type */
        if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                            "that cannot accept type record")));

        /*
         * generate attribute metadata needed later to produce tuples from raw
         * C strings
         */
        attinmeta = TupleDescGetAttInMetadata(tupdesc);
        funcctx->attinmeta = attinmeta;

        MemoryContextSwitchTo(oldcontext);
    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    attinmeta = funcctx->attinmeta;

    if (call_cntr < max_calls)    /* do when there is more left to send */
    {
        char       **values;
        HeapTuple    tuple;
        Datum        result;

        values = (char **) palloc(2 * sizeof(char *));
        values[0] = (char *) palloc(8 * sizeof(char));
        values[1] = (char *) palloc(8 * sizeof(char));

        snprintf(values[0], 8, "%g", line->coords[call_cntr].x);
        snprintf(values[1], 8, "%g", line->coords[call_cntr].y);

        /* build a tuple */
        tuple = BuildTupleFromCStrings(attinmeta, values);

        /* make the tuple into a datum */
        result = HeapTupleGetDatum(tuple);

        SRF_RETURN_NEXT(funcctx, result);
    }
    else    /* do when there is no more left */
    {
        SRF_RETURN_DONE(funcctx);
    }
}
