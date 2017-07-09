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

#ifndef __GEOEXT_DECLS_H__
#define __GEOEXT_DECLS_H__


/*
 * A coord2d represents a pair of coordinates in a 2D-space.
 *
 * It is just an utility type for building more complex data types.
 * So, this type will not be available in the PostgreSQL type system.
 * Instead, look at type geo_point, geo_linestring and geo_polygon.
 *
 */
typedef struct coord2d
{
  double x;
  double y;
} coord2d;


#endif  /* __GEOEXT_DECLS_H__ */
