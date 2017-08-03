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

/*
 * Auxiliary Functions
 *
 */
static inline enum segment_relation_type
overlap_intersection(struct coord2d *p1, struct coord2d *p2,
                     struct coord2d *q1, struct coord2d *q2,
                     struct coord2d *ip1, struct coord2d *ip2)
{
  struct coord2d *min_p = 0;
  struct coord2d *max_p = 0;

  struct coord2d *min_q = 0;
  struct coord2d *max_q = 0;

/* are the segments vertical? */
  if (p1->x == p2->x)
  {
/* order points along y-axis */
    if (p1->y < p2->y)
    {
      min_p = p1;
      max_p = p2;
    }
    else
    {
      min_p = p2;
      max_p = p1;
    }

    if (q1->y < q2->y)
    {
      min_q = q1;
      max_q = q2;
    }
    else
    {
      min_q = q2;
      max_q = q1;
    }

/* is p below q? */
    if (max_p->y < min_q->y)
      return DISJOINT;

/* is p above q? */
    if (min_p->y > max_q->y)
      return DISJOINT;

/* do the segments touch? */
    if(max_p->y == min_q->y)
    {
      *ip1 = *max_p;

      return TOUCH;
    }

    *ip1 = min_p->y > min_q->y ? *min_p : *min_q;
    *ip2 = max_p->y < max_q->y ? *max_p : *max_q;

    return OVERLAP;
  }
  else
  {
/* order points along the x-axis! */
    if (p1->x < p2->x)
    {
      min_p = p1;
      max_p = p2;
    }
    else
    {
      min_p = p2;
      max_p = p1;
    }

    if (q1->x < q2->x)
    {
      min_q = q1;
      max_q = q2;
    }
    else
    {
      min_q = q2;
      max_q = q1;
    }

/* is p left of q? */
    if (max_p->x < min_q->x)
      return DISJOINT;

/* is p right of q? */
    if (min_p->x > max_q->x)
      return DISJOINT;

/* do the segments touch? */
    if(max_p->x == min_q->x)
    {
      *ip1 = *max_p;

      return TOUCH;
    }

    *ip1 = min_p->x > min_q->x ? *min_p : *min_q;
    *ip2 = max_p->x < max_q->x ? *max_p : *max_q;

    return OVERLAP;
  }
}


/*
 * Geometric Primitives
 *
 */
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

/* to get a number and if this number is negative, transform to positive value*/
double absolute(double value) {
    if (value < 0)
      return -value;

    return value;
}


double area(struct coord2d *coord, int npts)
{
  double area = 0;         // Accumulates area in the loop

  for (int i = 0; i < npts-1; i++)
    {

      /*Use a formules to calculates the area from polygon points
      area = (x1 * y2 - y1 * x2) + (x2 * y3 - y2 * x3) + ..... n / 2 */

      area += ((coord[i].x * coord[i+1].y) -
                    (coord[i].y * coord[i+1].x));
    }
    area /= 2;

  return absolute(area);
}

enum segment_relation_type
compute_intersection(struct coord2d* p1, struct coord2d* p2,
                     struct coord2d* q1, struct coord2d* q2,
                     struct coord2d* ip1, struct coord2d* ip2)
{
  double ax = p2->x - p1->x;
  double ay = p2->y - p1->y;

  double bx = q1->x - q2->x;
  double by = q1->y - q2->y;

  double den = ay * bx - ax * by;

  if (den == 0.0) /* are they collinear? */
  {
    return overlap_intersection(p1, p2, q1, q2, ip1, ip2);
  }
  else
  {
/* they are not collinear, let's see if they intersects */
    double cx = p1->x - q1->x;
    double cy = p1->y - q1->y;

/* is alpha in the range [0..1]? */
    double num_alpha = by * cx - bx * cy;

    if (den > 0.0)
    {
/* is alpha before the range [0..1] or after it? */
      if((num_alpha < 0.0) || (num_alpha > den))
        return DISJOINT;
    }
    else /* den < 0 */
    {
/* is alpha before the range [0..1] or after it? */
      if ( (num_alpha > 0.0) || (num_alpha < den) )
        return DISJOINT;
    }

/* is beta in the range [0..1]? */
    double num_beta = ax * cy - ay * cx;

    if (den > 0.0)
    {
/* is beta before the range [0..1] or after it? */
      if((num_beta < 0.0) || (num_beta > den))
        return DISJOINT;
    }
    else /* den < 0 */
    {
/* is beta before the range [0..1] or after it? */
      if ( (num_beta > 0.0) || (num_beta < den) )
        return DISJOINT;
    }

    assert(num_alpha != 0);
    assert(num_beta != 0);

/* compute intersection point */
    double alpha = num_alpha / den;

    ip1->x = p1->x + alpha * (p2->x - p1->x);
    ip1->y = p1->y + alpha * (p2->y - p1->y);

    return CROSS;
  }
}
