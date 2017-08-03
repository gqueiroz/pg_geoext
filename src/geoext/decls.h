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
 * \file geoext/decls.h
 *
 * \brief GeoExt basic data types.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */

#ifndef __GEOEXT_DECLS_H__
#define __GEOEXT_DECLS_H__

/*
 * \brief A coord2d represents a pair of coordinates in a 2D-space.
 *
 * It is just an utility type for building more complex data types.
 * So, this type will not be available in the PostgreSQL type system.
 * Instead, look at type geo_point, geo_linestring and geo_polygon.
 *
 */
struct coord2d
{
  double x;
  double y;
};

/*
 * \brief The type of spatial relation between two segments.
 *
 */
enum segment_relation_type
{
  DISJOINT, /**< Segments do not intersect.                               */
  CROSS,    /**< Segments crosses in a single inner point.                */
  TOUCH,    /**< Segments touches in one of their end-points.             */
  OVERLAP   /**< Segments overlap: their intersection is another segment. */
};

#endif  /* __GEOEXT_DECLS_H__ */
