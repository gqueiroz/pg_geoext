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
 * \file unittest/algorithms/main.c
 *
 * \brief Unit-tests for GeoExt algorithms.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 */

/* GeoExt */
#include <geoext/algorithms.h>

/* C Standard Library */
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void test_euclidian_distance();

void test_point_in_polygon();

int main()
{
  test_euclidian_distance();
  
  test_point_in_polygon();

  return EXIT_SUCCESS;
}

void test_euclidian_distance()
{
  {
    coord2d c1 = { 0.0, 0.0 };
    coord2d c2 = { 1.0, 1.0 };
    
    double d = euclidian_distance(&c1, &c2);
    
    printf("%0.2f\n", d);
  }
  
  {
    coord2d c1 = { 1.0, 0.0 };
    coord2d c2 = { 1.0, 1.0 };
    
    double d = euclidian_distance(&c1, &c2);
    
    printf("%0.2f\n", d);
  }
  
  {
    coord2d c1 = { 1.0, 1.0 };
    coord2d c2 = { 2.0, 1.0 };
    
    double d = euclidian_distance(&c1, &c2);
    
    printf("%0.2f\n", d);
  }
}

void test_point_in_polygon()
{
  {
    coord2d poly [] = { { 0.0, 0.0 }, { 0.0, 10.0 }, { 10.0, 10.0 },
                        { 10.0, 0.0 }, { 0.0, 0.0 } };
    
    int num_vertices = sizeof(poly) / sizeof(coord2d);
    
    {
      coord2d pt = { 5.0, 5.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { -1.0, 5.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 11.0, 5.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 0.0, 0.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 0.0, 5.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 0.0, 10.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 5.0, 10.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 10.0, 10.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 10.0, 5.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 10.0, 0.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 5.0, 0.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
  }
  
  {
    coord2d poly [] = { { 2.0, 2.0 }, { 4.0, 6.0 }, { 6.0, 4.0 },
                        { 8.0, 8.0 }, { 12.0, 2.0 }, { 4.0, 0.0 },
                        { 2.0, 2.0 } };
    
    int num_vertices = sizeof(poly) / sizeof(coord2d);
    
    {
      coord2d pt = { 6.0, 9.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 2.0, 7.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 6.0, 1.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 8.0, 3.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 2.0, 3.5 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 6.0, 5.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 11.0, 6.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 6.0, 8.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 2.0, 4.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 1.0, 2.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 6.0, 8.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 4.0, 4.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 5.0, 2.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 8.0, 8.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 4.0, 6.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 2.0, 2.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }
    
    {
      coord2d pt = { 12.0, 2.0 };
    
      int result = point_in_polygon(&pt, poly, num_vertices);
    
      printf("O ponto encontra-se dentro do polígono? %s\n", (result ? "sim" : "não"));
    }

  }
}


