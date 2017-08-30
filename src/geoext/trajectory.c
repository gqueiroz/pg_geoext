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
* \file geoext/trajectory.c
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

/* GeoExtension */
#include "geo_point.h"
#include "algorithms.h"
#include "trajectory.h"
#include "hexutils.h"

/* PostgreSQL */
#include <libpq/pqformat.h>
#include <utils/builtins.h>
#include <utils/array.h> /* ArrayType*/
#include <catalog/pg_type.h>
#include <utils/lsyscache.h> /*construct ArrayType*/
#include <parser/parse_type.h> /*get oid from typname*/


/*
 * Utility macros.
 */

/*
  An Hex-String encoding a geo_trajc_elem must have at least:
  - srid: sizeof(int32) ?
  - time: sizeof(Timestamp)
  - 2 coordinates: sizeof(struct coord2d)
  Note that int hex we will have the double of bytes!
 */
#define GEOEXT_MIN_GEOTRAJCE_HEX_LEN \
(2 * (sizeof(Timestamp) + sizeof(struct geo_point) ))

inline
contruct_array_internal(int typlen, int typbyval, int typalign)
{

};

PG_FUNCTION_INFO_V1(trajectory_elem_in);

Datum
trajectory_elem_in(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);

  char *hstr = str;

  struct geo_trajc_elem *traje = (struct geo_trajc_elem*) palloc(sizeof(struct geo_trajc_elem));

  int hstr_size = strlen(str);

  int sizePoint = sizeof(struct geo_point);

  Timestamp time_elem = 0;


  /*elog(NOTICE, "trajectory_elem_in CALL for: %s", str);*/

  if (hstr_size < GEOEXT_MIN_GEOTRAJCE_HEX_LEN)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_trajc_elem", str)));

  /* get the  and advance the hstr pointer */
  hex2binary(hstr, 2 * sizeof(Timestamp), (char*)&time_elem);

  hstr += 2 * sizeof(Timestamp);

  // time_elem = DatumGetTimestamp(DirectFunctionCall3(timestamp_in), PointerGetDatum(hstr), PointerGetDatum(1114), PointerGetDatum(-1));

  /* set the fields of trajectory_elem */
  traje->time_elem = time_elem;

  /* read the coordinates from the hex-string*/
  hex2binary(hstr, 2 * sizePoint, (char*)&(traje->point_elem));

  PG_RETURN_GEOTRAJE_TYPE_P(traje);

};

PG_FUNCTION_INFO_V1(trajectory_elem_out);

Datum
trajectory_elem_out(PG_FUNCTION_ARGS)
{
  struct geo_trajc_elem *traje = PG_GETARG_GEOTRAJE_TYPE_P(0);

  char *timestamp = palloc(2 * sizeof(Timestamp) + 1);
  char *point = palloc(2 * sizeof(struct geo_point) + 1);

  StringInfoData final_hex;
  initStringInfo(&final_hex);

  /*elog(NOTICE, "trajectory_elem_out CALL ");*/

  if (!PointerIsValid(traje))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for trajectory_elem_out")));


  timestamp = DatumGetCString(DirectFunctionCall1(timestamp_out, PointerGetDatum(traje->time_elem)));

  /*elog(NOTICE, "trajectory_elem_out timestamp: %s", timestamp);*/

  point = DatumGetCString(DirectFunctionCall1(geo_point_out, PointerGetDatum(&(traje->point_elem))));

  /*elog(NOTICE, "trajectory_elem_out point point: %s", point);*/

  appendStringInfo(&final_hex, "%s", timestamp);
  appendStringInfo(&final_hex, "%s", point);


  PG_RETURN_CSTRING(final_hex.data);

};

PG_FUNCTION_INFO_V1(get_trajectory_elem);

Datum
get_trajectory_elem(PG_FUNCTION_ARGS)
{

  Timestamp timestamp = PG_GETARG_TIMESTAMP(0);

  struct geo_point *pt = PG_GETARG_GEOPOINT_TYPE_P(1);


  struct geo_trajc_elem *traje = (struct geo_trajc_elem *)palloc(sizeof(struct geo_trajc_elem));

  /*elog(NOTICE,"get_trajectory_elem CALL ");*/

  /*fazer as verificacoes*/

  /*get time_elem*/
  traje->time_elem = timestamp;

  /*get point element*/
  traje->point_elem.coord = pt->coord;
  traje->point_elem.srid = pt->srid;
  traje->point_elem.dummy = 0;

  /*elog(NOTICE,"get_trajectory_elem getall ");*/

  PG_RETURN_GEOTRAJE_TYPE_P(traje);
};

PG_FUNCTION_INFO_V1(tst_trajectory);

Datum
tst_trajectory(PG_FUNCTION_ARGS)
{
  /*funcao final da agregacao*/
  /*recebe um array de trajectory_elem*/
  /*criar o tipo trajetoria */
  /*ordena o array*/
  /*criar line*/
  /*pega o tempo final e inicial*/
}

PG_FUNCTION_INFO_V1(trajectory_to_array);

Datum
trajectory_to_array(PG_FUNCTION_ARGS)
{

  elog(NOTICE, "trajectory_to_array CALL");

  /* Agregate array result*/
  ArrayType *result_array;
  struct geo_trajc_elem *traje = (struct geo_trajc_elem *)palloc(sizeof(struct geo_trajc_elem));

  /*get args*/
  Timestamp time_el = PG_GETARG_TIMESTAMP(1);
  struct geo_point *pt = PG_GETARG_GEOPOINT_TYPE_P(2);

  int16 typlen;
  bool typbyval;
  char typalign;

  Datum datum_element;

  const char *aux1 = "geo_trajc_elem";
  TypeName *typname;
  typname = typeStringToTypeName(aux1);

  /*creating trajectory_elem type*/
  traje = DatumGetGeoTrajETypeP(DirectFunctionCall2(get_trajectory_elem, PointerGetDatum(time_el), PointerGetDatum(pt)));
  /*pass to datum for using in ArrayType*/
  datum_element = PointerGetDatum(traje);

  /*from parser/parse_type.h*/
  Oid element_type = typenameTypeId(NULL, typname);

  /*elog(NOTICE, "get element_type %d", element_type);*/

  if (!OidIsValid(element_type))
     elog(ERROR, "could not determine data type of geo_trajc_elem input");


  get_typlenbyvalalign(element_type, &typlen, &typbyval, &typalign);


  bool isnull = PG_ARGISNULL(0);

  // float8 isnull = PG_GETARG_FLOAT8(0);

  elog(NOTICE, "isnull %d", isnull);


  // If argument 0 (agg_state) is null this is the first value provided to the aggregate.
  if(isnull) {

    elog(NOTICE, "eh null");
    result_array = construct_array(&datum_element, 1, element_type, typlen, typbyval, typalign);
    /*elog(NOTICE, "construct_array ok");*/
    PG_RETURN_ARRAYTYPE_P(result_array);

  }

  ArrayType *array_in = PG_GETARG_ARRAYTYPE_P(0);
  ArrayType *aux;
  /*elog(NOTICE, "dims array_in %d", ARR_NDIM(array_in));*/

  aux = construct_array(&datum_element, 1, element_type, typlen, typbyval, typalign);
  /*elog(NOTICE, "construct_array ok");*/

  result_array = DatumGetArrayTypeP(DirectFunctionCall2(array_cat, PointerGetDatum(array_in), PointerGetDatum(aux)));

  /*elog(NOTICE, "concat ok");
  elog(NOTICE, "return result");*/

  PG_RETURN_ARRAYTYPE_P(result_array);

}

PG_FUNCTION_INFO_V1(trajectory_to_array_final);

Datum
trajectory_to_array_final(PG_FUNCTION_ARGS)
{
  elog(NOTICE, "trajectory_to_array_final CALL");

  ArrayType *array_in;
  ArrayType *array_out_order;

  /* variables for "deconstructed" array*/
  Datum *datums_time;

  bool *nulls_time;
  int count_time;

  int16 typlen;
  bool typbyval;
  char typalign;

  array_in = PG_GETARG_ARRAYTYPE_P(0);

  elog(NOTICE, "get argument");

  PG_RETURN_ARRAYTYPE_P(array_in);
}
