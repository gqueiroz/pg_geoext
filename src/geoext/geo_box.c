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
 * \file geoext/geo_box.c
 *
 * \brief A geo box is a type of support used in the indexing method GiST.
 *        It has the upper and lower coordinates of geo types
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
 #include "hexutils.h"
 #include "wkt.h"

 /* PostgreSQL */
 #include <libpq/pqformat.h>
 #include <utils/builtins.h>

/*
* Utility macros.
*/
 #define GEOEXT_GEOBOX_SIZE ( sizeof(struct geo_box) )
 #define GEOEXT_GEOBOX_HEX_LEN ( ( 2 * GEOEXT_GEOBOX_SIZE ) )


/*
* I/O Functions for the geo_box data type
*/

PG_FUNCTION_INFO_V1(geo_box_in);

Datum
geo_box_in(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);

  struct geo_box *gbox = (struct geo_box*) palloc(sizeof(struct geo_box));

  /*elog(NOTICE, "geo_box_in called for: %s", str);*/

  if (!PointerIsValid(str))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_box_in")));

  if (strlen(str) != GEOEXT_GEOBOX_HEX_LEN)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_box", str)));

  hex2binary(str, GEOEXT_GEOBOX_HEX_LEN, (char*)gbox);

  PG_RETURN_GEOBOX_TYPE_P(gbox);
}


PG_FUNCTION_INFO_V1(geo_box_out);

Datum
geo_box_out(PG_FUNCTION_ARGS)
{
  struct geo_box *gbox = PG_GETARG_GEOBOX_TYPE_P(0);

/* alloc a buffer for hex-string plus a trailing '\0' */
  char *hstr = palloc(GEOEXT_GEOBOX_HEX_LEN + 1);

  /*elog(NOTICE, "geo_box_out called");*/

  if (!PointerIsValid(gbox))
    ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                    errmsg("missing argument for geo_box_out")));

  binary2hex((char*)gbox, GEOEXT_GEOBOX_SIZE, hstr);

  PG_RETURN_CSTRING(hstr);
}

/*
* geo_box operations
*/

PG_FUNCTION_INFO_V1(geo_box_from_text);

Datum
geo_box_from_text(PG_FUNCTION_ARGS)
{
 char *str = PG_GETARG_CSTRING(0);

 struct geo_box *gbox = (struct geo_box*) palloc(sizeof(struct geo_box));

 /*elog(NOTICE, "struct geo_box called for: %s", str);*/

 if (!PointerIsValid(gbox))
   ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("missing argument for geo_box_from_text")));

 geo_box_wkt_decode(str, gbox);

 PG_RETURN_GEOBOX_TYPE_P(gbox);
}


PG_FUNCTION_INFO_V1(geo_box_to_str);

Datum
geo_box_to_str(PG_FUNCTION_ARGS)
{
 struct geo_box *gbox = PG_GETARG_GEOBOX_TYPE_P(0);

 /*elog(NOTICE, "geo_box_to_str called");*/

 if (!PointerIsValid(gbox))
   ereport(ERROR, (errcode (ERRCODE_INVALID_PARAMETER_VALUE),
                   errmsg("missing argument for geo_box_to_str")));

 PG_RETURN_CSTRING(geo_box_wkt_encode(gbox));
}
