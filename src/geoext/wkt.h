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
 * \file geoext/wkt.h
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

#ifndef __GEOEXT_WKT_H__
#define __GEOEXT_WKT_H__


/* GeoExt */
#include "geoext.h"


/*
 * \brief Count the number of coordinates in a sequence represented as a string.
 *
 * \param str A sequence o coordinates is represented as: '...(x1 y1, x2 y2, ..., xn yn)...'.
 *
 * \note The number of coordinates will be the number of found commas plus one.
 *
 */
int coord_count(char *str);


/*
 * \brief Convert a WKT string to a geo_point.
 *
 * \param str A string with the following format: 'POINT(x y)'.
 * \param pt  A geo_point data.
 *
 */
void geo_point_wkt_decode(char *str, struct geo_point *pt);


/*
 * \brief Convert a geo_point to a WKT string.
 *
 * \param str A string with the following format: 'POINT(x y)'.
 * \param pt  A geo_point data.
 *
 */
char* geo_point_wkt_encode(struct geo_point *pt);


/*
 * \brief Convert a WKT string to a geo_linestring.
 *
 * \param str  A string with the following format: 'LINESTRING(X1 Y1, X2 Y2, ..., Xn Yn)'.
 * \param lstr A geo_linestring data.
 *
 */
/*void geo_linestring_wkt_decode(char *str, struct geo_linestring* lstr);*/


/*
 * \brief Convert a WKT string to a geo_polygon.
 *
 * \param str  A string with the following format: 'POLYGON(X1 Y1, X2 Y2, ..., Xn Yn)'.
 * \param poly A geo_polygon data.
 *
 */
/*void geo_polygon_wkt_decode(char *str, struct geo_polygon* poly);*/


#endif  /* __GEOEXT_WKT_H__ */
