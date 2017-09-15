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
 *
 */

/* GeoExt */
#include <geoext/algorithms.h>
#include <geoext/hexutils.h>

/* C Standard Library */
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* PostgreSQL libpq */
#include <libpq-fe.h>

/* big endian */
#define XDR 0

/* little endian */
#define NDR 1

#define MY_MACHINE_BYTE_ORDER NDR



void test_euclidian_distance();

void test_point_in_polygon();

void test_hex_encoding_decoding();

void test_lengh();

void test_intersection();

void test_text_mode();

void test_binary_mode();

void SwapInt32(int32_t *v);

void SwapDouble(char *v);

int main()
{
  /*test_euclidian_distance();

  test_point_in_polygon();

  test_hex_encoding_decoding();

  test_lengh();
  
  test_intersection();

  test_text_mode();*/

  test_binary_mode();

  return EXIT_SUCCESS;
}


void test_euclidian_distance()
{
  {
    struct coord2d c1 = { 0.0, 0.0 };
    struct coord2d c2 = { 1.0, 1.0 };

    double d = euclidian_distance(&c1, &c2);

    printf("%0.2f\n", d);
  }

  {
    struct coord2d c1 = { 1.0, 0.0 };
    struct coord2d c2 = { 1.0, 1.0 };

    double d = euclidian_distance(&c1, &c2);

    printf("%0.2f\n", d);
  }

  {
    struct coord2d c1 = { 1.0, 1.0 };
    struct coord2d c2 = { 2.0, 1.0 };

    double d = euclidian_distance(&c1, &c2);

    printf("%0.2f\n", d);
  }
}


void test_point_in_polygon()
{
  {
    struct coord2d poly [] = { { 0.0, 0.0 }, { 0.0, 10.0 }, { 10.0, 10.0 },
                               { 10.0, 0.0 }, { 0.0, 0.0 } };

    int num_vertices = sizeof(poly) / sizeof(struct coord2d);

    {
      struct coord2d pt = { 5.0, 5.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { -1.0, 5.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 11.0, 5.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 0.0, 0.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 0.0, 5.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 0.0, 10.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 5.0, 10.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 10.0, 10.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 10.0, 5.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 10.0, 0.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 5.0, 0.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }
  }

  {
    struct coord2d poly [] = { { 2.0, 2.0 }, { 4.0, 6.0 }, { 6.0, 4.0 },
                               { 8.0, 8.0 }, { 12.0, 2.0 }, { 4.0, 0.0 },
                               { 2.0, 2.0 } };

    int num_vertices = sizeof(poly) / sizeof(struct coord2d);

    {
      struct coord2d pt = { 6.0, 9.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 2.0, 7.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 6.0, 1.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 8.0, 3.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 2.0, 3.5 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 6.0, 5.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 11.0, 6.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 6.0, 8.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 2.0, 4.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 1.0, 2.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 6.0, 8.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 4.0, 4.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 5.0, 2.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 8.0, 8.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 4.0, 6.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 2.0, 2.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

    {
      struct coord2d pt = { 12.0, 2.0 };

      int result = point_in_polygon(&pt, poly, num_vertices);

      printf("Point in polygon? %s\n", (result ? "yes" : "no"));
    }

  }
}


void test_hex_encoding_decoding()
{
  struct coord2d pt1 = { 5.0, 2.0 };
  struct coord2d pt2 = { 7.0, 9.0 };

  char hex_str[2 * sizeof(struct coord2d) + 1];

  binary2hex((const char*)&pt1, sizeof(struct coord2d), hex_str);

  printf("%s\n", hex_str);

  printf("(%0.2f, %0.2f)\n", pt2.x, pt2.y);

  hex2binary(hex_str, strlen(hex_str), (char*)&pt2);

  printf("(%0.2f, %0.2f)\n", pt2.x, pt2.y);
}


void test_lengh()
{
  {
    struct coord2d linestring [] = { { 2.0, 1.0 }, { 4.0, 3.0 }, { 5.0, 2.0 },
                               { 7.0, 6.0 }, { 7.0, 4.0 } };

    int num_vertices = sizeof(linestring) / sizeof(struct coord2d);

    double l = length(linestring, num_vertices);

    printf("%0.2f\n", l);

  }
}


void test_intersection()
{
  {
    struct coord2d p[] = { {1.0, 1.0}, {3.0, 3.0} };
    
    struct coord2d q[] = { {3.0, 1.0}, {1.0, 3.0} };
    
    struct coord2d ips[2];
    
    enum segment_relation_type rel = compute_intersection(p, p + 1,
                                                          q, q + 1,
                                                          ips, ips + 1);
    
    printf("%d\n", rel == CROSS);
  }
  
  {
    struct coord2d p[] = { {1.0, 1.0}, {3.0, 3.0} };
    
    struct coord2d q[] = { {1.0, 3.0}, {3.0, 1.0} };
    
    struct coord2d ips[2];
    
    enum segment_relation_type rel = compute_intersection(p, p + 1,
                                                          q, q + 1,
                                                          ips, ips + 1);
    
    printf("%d\n", rel == CROSS);
  }
  
  {
    struct coord2d p[] = { {1.0, 1.0}, {3.0, 3.0} };
    
    struct coord2d q[] = { {0.0, 4.0}, {1.0, 3.0} };
    
    struct coord2d ips[2];
    
    enum segment_relation_type rel = compute_intersection(p, p + 1,
                                                          q, q + 1,
                                                          ips, ips + 1);
    
    printf("%d\n", rel == CROSS);
  }

}


void test_text_mode()
{
  PGconn *conn = PQconnectdb("host=localhost port=5432 dbname=pg_geoext user=gribeiro connect_timeout=4");

  ConnStatusType status = PQstatus(conn);

  if(status != CONNECTION_OK)
  {
    PQfinish(conn);
    return;
  }

  PGresult *result = PQexec(conn, "SELECT * FROM t2");

  if(PQresultStatus(result) != PGRES_TUPLES_OK)
  {
    PQclear(result);
    PQfinish(conn);
    return;
  }

  int nrows = PQntuples(result);

  for(int i = 0; i < nrows; ++i)
  {
    char* value = PQgetvalue(result, i, 0);

    printf("%s\n", value);
  }

  PQclear(result);

  PQfinish(conn);
}

void test_binary_mode()
{
  PGconn *conn = PQconnectdb("host=localhost port=5432 dbname=pg_geoext user=gribeiro connect_timeout=4");

  ConnStatusType status = PQstatus(conn);

  if(status != CONNECTION_OK)
  {
    PQfinish(conn);
    return;
  }

/* Insert data in binary mode */
  {
/* convert data representation to big-endian */
    int32_t srid = 4326;
    int32_t npts = 4;
    struct coord2d coords[] = { {1, 2}, {3, 4}, {5, 6}, {7, 8} };

    char buff[2 * sizeof(int32_t) + sizeof(coords)];

    char *dst = buff + 2 * sizeof(int32_t);

    for(int i = 0; i < npts; ++i)
    {
      double x = coords[i].x;
      double y = coords[i].y;

      SwapDouble((char*)&x);
      SwapDouble((char*)&y);

      memcpy(dst, &x, sizeof(double));
      dst += sizeof(double);

      memcpy(dst, &y, sizeof(double));
      dst += sizeof(double);
    }

    SwapInt32(&srid);
    SwapInt32(&npts);

    memcpy(buff, &srid, sizeof(int32_t));
    memcpy(buff + sizeof(int32_t), &npts, sizeof(int32_t));

/* insert data through binary interface */
    const Oid paramTypes[1] = { 23811 };
    char const * paramValues[1];
    paramValues[0] = buff;
    int paramLengths[1] = {2 * sizeof(int32_t) + sizeof(coords)};
    int paramFormats[1] = { 1 };

    PGresult *result = PQexecParams(conn, "INSERT INTO t2 VALUES($1)", 1,
                                    paramTypes, paramValues, paramLengths, paramFormats, 1);

    if(PQresultStatus(result) != PGRES_COMMAND_OK)
    {
      PQclear(result);
      PQfinish(conn);
      return;
    }

    PQclear(result);
  }
  

/* Retrieve data from the database */
  {
    PGresult *result = PQexecParams(conn, "SELECT * FROM t2", 0, NULL, NULL, NULL, NULL, 1);

    if(PQresultStatus(result) != PGRES_TUPLES_OK)
    {
      PQclear(result);
      PQfinish(conn);
    return;
    }

    int nrows = PQntuples(result);

    for(int i = 0; i < nrows; ++i)
    {
      char* value = PQgetvalue(result, i, 0);

      int32_t srid = -1;
      int32_t npts = -1;

      memcpy(&srid, value, sizeof(int32_t));
      memcpy(&npts, value + sizeof(int32_t), sizeof(int32_t));

#if MY_MACHINE_BYTE_ORDER == NDR
      SwapInt32(&srid);
      SwapInt32(&npts);
#endif

      printf("srid = %d; npts = %d\n", srid, npts);
    }

    PQclear(result);
  }

  PQfinish(conn);
}

void SwapInt32(int32_t *v)
{
  char vIn[4], vOut[4];

  memcpy(vIn, v, sizeof(int32_t));

  vOut[0] = vIn[3];
  vOut[1] = vIn[2];
  vOut[2] = vIn[1];
  vOut[3] = vIn[0];

  memcpy(v, vOut, sizeof(int32_t));
}

void SwapDouble(char *v)
{
  char v0 = v[0];
  v[0] = v[7];
  v[7] = v0;

  char v1 = v[1];
  v[1] = v[6];
  v[6] = v1;

  char v2 = v[2];
  v[2] = v[5];
  v[5] = v2;

  char v3 = v[3];
  v[3] = v[4];
  v[4] = v3;
}

