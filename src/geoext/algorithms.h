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
 * \file geoext/algorithms.h
 *
 * \brief Geometric algorithms used in GeoExt
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 * \warning The algorithms implemented here are not robust enough for
 *          a production environment. However, they will attend the purpose
 *          of a beginner's spatial database course.
 *
 */

#ifndef __GEOEXT_ALGORITHMS_H__
#define __GEOEXT_ALGORITHMS_H__


/* GeoExt */
#include "decls.h"


/*
 * \brief Tells if c1 and c2 are coincidents.
 *
 */
int equals(struct coord2d* c1, struct coord2d* c2);


/*
 * \brief Computes the euclidean distance between two points.
 *
 * \note This function is based on the naive formula: \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
 *
 */
double euclidian_distance(struct coord2d* c1, struct coord2d* c2);


/*
 * \brief Computes the length of points.
 *
 */
double length(struct coord2d* c1, int num_vertices);


/*
 * \brief Tells if a point is inside a polygon.
 *
 * This algorithm is based on crossings test.
 * For a detailed explanation of the algorithm, please, see:
 *   Haines, Eric. Point in Polygon Strategies. Graphics Gems IV, ed. Paul Heckbert, Academic Press, p. 24-46, 1994.
 *
 * See also the nice homepage of W. Randolph Franklin:
 *   https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
 *
 * \note This implementation follows the code presented at: http://erich.realtimerendering.com/ptinpoly.
 *
 */
int point_in_polygon(struct coord2d* pt,
                     struct coord2d* poly,
                     int num_vertices);

double area(struct coord2d *coord, int npts);
double perimeter (struct coord2d *coord, int npts);

/*
 * \brief Computes the intersection point(s) between two line segments.
 *
 * If the segments overlap this function returns two points representing
 * the extreme points of the intersection.
 *
 * \param p1  The start point of segment p.
 * \param p2  The end point of segment p.
 * \param q1  The start point of segment q.
 * \param q2  The end point of segment q.
 * \param ip1 Intersection point of p and q.
 * \param ip2 If the segment overlap returns a second intersection point between p and q.
 *
 * \return The type of intersection between line segments.
 *
 * \note Based on the algorithm of Franklin Antonio (1992) in Graphics Gems III.
 *
 * \warning This function does not perform bounding box comparison between segment pair.
 *
 */
enum segment_relation_type
compute_intersection(struct coord2d* p1, struct coord2d* p2,
                     struct coord2d* q1, struct coord2d* q2,
                     struct coord2d* ip1, struct coord2d* ip2);

#endif  /* __GEOEXT_ALGORITHMS_H__ */
