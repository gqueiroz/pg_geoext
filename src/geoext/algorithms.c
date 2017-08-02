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
 * \file geoext/algorithms.c
 *
 * \brief This file contains the implementation of geometric algorithms used in GeoExt.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */

/* GeoExt */
#include "algorithms.h"


/* C Standard Library */
#include <assert.h>
#include <ctype.h>
#include <math.h>


int equals(struct coord2d *c1, struct coord2d *c2)
{
  return (c1->x == c2->x) && (c1->y == c2->y);
}


double euclidian_distance(struct coord2d *c1, struct coord2d *c2)
{
  double dx = c1->x - c2->x;

  double dy = c1->y - c2->y;

  double result = sqrt((dx * dx) + (dy * dy));

  return result;
}

double length(struct coord2d *c, int num_vertices)
{
  double result = 0.0;
    
  const int n = num_vertices - 1;

  for(int i = 0; i < n; ++i)
  {
    result += euclidian_distance(&c[i], &c[i+1]);

  }

  return result;

}


int point_in_polygon(struct coord2d *pt,
                     struct coord2d *poly,
                     int num_vertices)
{
  struct coord2d *vtx0;  /* The first vertex of the current segment being processed. */
  struct coord2d *vtx1;  /* The second vertex of the current segment being processed. */
  int yflag0;            /* The result of a above/below Y axis test for vertex0. */
  int yflag1;            /* The result of a above/below Y axis test for vertex1. */
  int xflag0;            /* The result of a left/right test for vertex0.*/
  int inside_flag = 0;   /* At each cross of a +X ray starting at pt->x and pt->y
                            this variable will be changed from 0 to 1
                            or, from 1 to 0. */

/* assure that the informed polygon have at least four points */
  assert(num_vertices > 3);

/* get test bit for above/below X axis for first vertex */
  vtx0 = poly;

  yflag0 = ( vtx0->y >= pt->y );

  for( int i = 1 ; i != num_vertices ; ++i)
  {
    vtx1 = poly + i;

    yflag1 = ( vtx1->y >= pt->y );

/* check if endpoints straddle (are on opposite sides) of X axis
   (i.e. the Y's differ); if so, +X ray could intersect this edge. */
    if( yflag0 != yflag1 )
    {
      xflag0 = ( vtx0->x >= pt->x );

/* check if endpoints are on same side of the Y axis (i.e. X's
   are the same); if so, it's easy to test if edge hits or misses. */
      if( xflag0 == ( vtx1->x >= pt->x ) )
      {
/* if edge's X values both right of the point, must hit */
        if( xflag0 )
          inside_flag = !inside_flag;
      }
      else
      {
/* compute intersection of poly segment with +X ray, note
 * if >= point's X; if so, the ray hits it.
 */
        if ( ( vtx1->x - ( vtx1->y - pt->y ) * ( vtx0->x - vtx1->x ) / ( vtx0->y - vtx1->y ) ) >= pt->x )
        {
          inside_flag = !inside_flag;
        }
      }
    }

/* move to next pair of vertices, retaining info as possible */
    yflag0 = yflag1;
    vtx0 = vtx1;
  }

  return inside_flag;
}
