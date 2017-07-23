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
 * \file geoext/wkt.c
 *
 * \brief Conversion routine between Well-Kown Text respresentation and geometric objects.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 * \warning Although we use the same term WKT from OGC-SF, here we are just
 *          providing a simplified version of this text format
 *          for Points, LineStrings and Simple Polygons.
 *
 */

/* GeoExt */
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
 * WKT delimiters for input/output of geometries.
 */
#define GEOEXT_GEOPOINT_WKT_TOKEN "POINT"
#define GEOEXT_GEOPOINT_WKT_TOKEN_LEN 5

#define GEOEXT_GEOLINESTRING_WKT_TOKEN "LINESTRING"
#define GEOEXT_GEOLINESTRING_WKT_TOKEN_LEN 10

#define GEOEXT_GEOPOLYGON_WKT_TOKEN "POLYGON"
#define GEOEXT_GEOPOLYGON_WKT_TOKEN_LEN 7

#define GEOEXT_GEOM_LDELIM '('
#define GEOEXT_GEOM_RDELIM ')'
#define GEOEXT_GEOM_COLLECTION_DELIM ','


/*
 * \brief Count the number of coordinates in a sequence represented as a string.
 *
 * A sequence o coordinates is represented as:
 * "...(x1 y1, x2 y2, ..., xn yn)..."
 *
 * The number of coordinates will be the number of found commas plus one.
 *
 */
int coord_count(char *s)
{
  int ndelim = 0;

  while ((s = strchr(s, GEOEXT_GEOM_COLLECTION_DELIM)) != NULL)
  {
    ++ndelim;
    ++s;
  }

  return (ndelim + 1);
}


/*
 * \brief Extracts a coordinate pair from a string.
 *
 * \param str         A string containing a pair of double values separated by a 'space'.
 * \param coord       A pointer to a pair of coordinates to write the decoded values.
 * \param endptr      A reference to an already allocated object of type char*, whose value is set by the function to the next character in str after the last numerical value found.
 * \param type_name   The data type name that should be mentioned if an error occurs.
 * \param orig_string If an error occurs, it will prints 'orig_string' as the input.
 *
 * \note This function will skip trailing whitespaces, so endptr will point to first character that is not a whitespace.
 *
 */
static inline void
coord2d_decode(char *str,
               struct coord2d *coord,
               char **endptr,
               const char *type_name,
               const char *orig_string)
{
  assert(str);
  assert(coord);
  assert(endptr);
  assert(type_name);
  assert(orig_string);

  coord->x = float8in_internal(str, &str, type_name, orig_string);

  coord->y = float8in_internal(str, &str, type_name, orig_string);

  *endptr = str;
}


/*
 * \brief Extracts a sequence of coordinates from a string.
 *
 * A sequence o coordinates is represented as:
 * '...(x1 y1, x2 y2, ..., xn yn)...'
 *
 * \param str         A string containing a sequence of coordinates separated by a 'comma'.
 * \param coords      A pointer to a sequence of coordinates to write the decoded values.
 * \param ncoords     The number of coordinate pairs to be read from the input string.
 * \param endptr      A reference to an already allocated object of type char*, whose value is set by the function to the next character in str after the last character in the sequence.
 * \param type_name   The data type name that should be mentioned if an error occurs.
 * \param orig_string If an error occurs, it will prints 'orig_string' as the input.
 *
 *
 * \note This function will skip trailing whitespaces, so endptr will point to first character that is not a whitespace.
 *
 */
static inline void
coord2d_sequence_decode(char *str,
                        struct coord2d *coords,
                        int ncoords,
                        char **endptr,
                        const char *type_name,
                        const char *orig_string)
{
  assert(str);
  assert(coords);
  assert(endptr);
  assert(type_name);
  assert(orig_string);

  while (*str != '\0' && isspace((unsigned char) *str))
    ++str;

  if(*str != GEOEXT_GEOM_LDELIM)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            type_name, orig_string)));

  ++str;

  for(int i = 0; i < ncoords; ++i)
  {
    coord2d_decode(str, coords, &str, type_name, orig_string);

    if(*str == GEOEXT_GEOM_COLLECTION_DELIM)
      ++str;

    ++coords;
  }

  if(*str != GEOEXT_GEOM_RDELIM)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            type_name, orig_string)));

  ++str;

  while (*str != '\0' && isspace((unsigned char) *str))
    ++str;

  *endptr = str;
}


void geo_point_wkt_decode(char *str, struct geo_point* pt)
{
/* search for the occurence of: 'POINT' */
  char *cp = strcasestr(str, GEOEXT_GEOPOINT_WKT_TOKEN);

/* if the substring 'POINT' is not found in the text, we have an invalid WKT */
  if (!PointerIsValid(cp))
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_point", str)));

/* advance cp to one character past the string 'POINT' */
  cp += GEOEXT_GEOPOINT_WKT_TOKEN_LEN;

/* skip spaces, if any */
  while (*cp != '\0' && isspace((unsigned char) *cp))
    ++cp;

/* If we don't find a '(', the WKT is invalid */
  if (*cp != GEOEXT_GEOM_LDELIM)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_point", str)));

/* skip the '(' */
  ++cp;

/* try to decode a pair of coordinates */
  coord2d_decode(cp, &(pt->coord), &cp, "geo_point", str);

/* If we don't find a ')' after reading the x and y coordinates, the WKT is invalid */
  if (*cp != GEOEXT_GEOM_RDELIM)
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_point", str)));

/* skip the ')' */
  ++cp;

/* skip spaces, if any */
  while (*cp != '\0' && isspace((unsigned char) *cp))
    ++cp;

/* if we still have characters, the WKT is invalid */
  if(*cp != '\0')
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type %s: \"%s\"",
            "geo_point", str)));

/*
  WKT doesn't have provision for SRID.
  So, let's prevent instability in unused padded-bytes.
  The DBMS may do wrong decisions if we don't zero all fields!
 */
  pt->srid = 0;
  pt->dummy = 0;
}

char* geo_point_wkt_encode(struct geo_point *pt)
{
  StringInfoData str;

  initStringInfo(&str);

  appendStringInfoString(&str, GEOEXT_GEOPOINT_WKT_TOKEN);

  appendStringInfoChar(&str, GEOEXT_GEOM_LDELIM);

  char *xstr = float8out_internal(pt->coord.x);
  char *ystr = float8out_internal(pt->coord.y);

  appendStringInfo(&str, "%s %s", xstr, ystr);

  pfree(xstr);
  pfree(ystr);

  appendStringInfoChar(&str, GEOEXT_GEOM_RDELIM);

  return str.data;
}
