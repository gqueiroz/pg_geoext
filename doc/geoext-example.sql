--
-- This file contains some examples on how to use pg_geoext.
-- Note: take care with the commands below!
--

DROP DATABASE IF EXISTS pg_geoext;

CREATE DATABASE pg_geoext;

\c pg_geoext

CREATE EXTENSION geoext;

CREATE TABLE t1
(
  c geo_point
);

INSERT INTO t1 VALUES(point_from_text('POINT(1 2)')),
                     (point_from_text('POINT(18 23)'));

SELECT * FROM t1;

SELECT to_str(c) FROM t1;

CREATE TABLE t2
(
  c geo_linestring
);

INSERT INTO t2 VALUES(linestring_from_text('LINESTRING(0 0, 10 10)')),
                     (linestring_from_text('LINESTRING(11 11, 12 12, 13 13)'));

SELECT * FROM t2;

SELECT to_str(c) FROM t2;
