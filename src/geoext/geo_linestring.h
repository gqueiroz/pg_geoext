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
  int32 npts;     /* Number of points in the polygon.   */
  int32 srid;     /* The Spatial Reference System ID.   */
  int32 dummy;    /* Padding to make it double aligned. */
  struct coord2d coords[FLEXIBLE_ARRAY_MEMBER];   /* The array of polygon vertices.     */
};


#endif  /* __GEOEXT_GEO_LINESTRING_H__ */
