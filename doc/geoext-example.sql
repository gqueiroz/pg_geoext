--
-- This file contains some examples on how to use pg_geoext.
-- Note: take care with the commands below!
--

DROP DATABASE IF EXISTS pg_geoext;

CREATE DATABASE pg_geoext;

\c pg_geoext

CREATE EXTENSION geoext;

DROP TABLE IF EXISTS t1 CASCADE;

CREATE TABLE t1
(
  id        INT4 PRIMARY KEY,
  location  geo_point
);

CREATE OR REPLACE FUNCTION geo_point_rnd_generator(npts int4)
RETURNS setof record
AS $$
DECLARE
    tupla record;
    longitude numeric;
    latitude numeric;
    pt geo_point;
BEGIN
    RAISE NOTICE 'Computando % pontos aleatórios...', npts;

    FOR i IN 1..npts LOOP
        longitude := 360.0 * random() - 180.0;
        latitude := 180.0 * random() - 90.0;

        pt := point_from_text(format('POINT(%s %s)', longitude, latitude)::cstring);

        tupla := (i, pt);  -- ou: SELECT i, pt INTO tupla;

        RETURN NEXT tupla;

        IF (i % 1000) = 0 THEN
            RAISE NOTICE 'random_pt_generator: iteração %', i;
        END IF;
    END LOOP;

    RETURN;
END;
$$
LANGUAGE plpgsql;

SELECT * FROM geo_point_rnd_generator(1000000) AS t(pid INT4, location geo_point);

INSERT INTO t1 SELECT * FROM geo_point_rnd_generator(1000000) AS t(pid INT4, location geo_point);

--INSERT INTO t1 VALUES(point_from_text('POINT(1 2)')),
--                     (point_from_text('POINT(18 23)'));

SELECT * FROM t1;

SELECT to_str(location) FROM t1;

SELECT * FROM t1 WHERE location = point_from_text('POINT(31.3168034143746 29.5994637720287)');

EXPLAIN ANALYZE SELECT * FROM t1 WHERE location = point_from_text('POINT(31.3168034143746 29.5994637720287)');

CREATE INDEX t1_location_idx ON t1 USING btree(location btree_geo_point_ops);

EXPLAIN ANALYZE SELECT * FROM t1 WHERE location = point_from_text('POINT(31.3168034143746 29.5994637720287)');

EXPLAIN ANALYZE SELECT location FROM t1 WHERE location = point_from_text('POINT(31.3168034143746 29.5994637720287)');

EXPLAIN ANALYZE SELECT * FROM t1 WHERE location < point_from_text('POINT(-170 0)');

EXPLAIN ANALYZE SELECT * FROM t1 WHERE location > point_from_text('POINT(170 0)');

CREATE TABLE t2
(
  c geo_linestring
);

INSERT INTO t2 VALUES(linestring_from_text('LINESTRING(0 0, 10 10)')),
                     (linestring_from_text('LINESTRING(11 11, 12 12, 13 13)'));

SELECT * FROM t2;

SELECT to_str(c) FROM t2;

SELECT line_to_array(linestring_from_text('LINESTRING(11 11, 12 12, 13 13, 14 14, 15 15, 11 11)'));

SELECT unnest((line_to_array(linestring_from_text('LINESTRING(11 11, 12 12, 13 13, 14 14, 15 15, 11 11)')))[1:1]);

SELECT (line_to_array(linestring_from_text('LINESTRING(11 11, 12 12, 13 13, 14 14, 15 15, 11 11)')))[2:2];

SELECT to_str(line_from_arrays(ARRAY[11,12,13,14,15,11], ARRAY[11,12,13,14,15,11]));
