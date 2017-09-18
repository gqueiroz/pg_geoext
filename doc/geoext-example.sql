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

SELECT linestring_to_array(linestring_from_text('LINESTRING(11 11, 12 12, 13 13, 14 14, 15 15, 11 11)'));

SELECT unnest((linestring_to_array(linestring_from_text('LINESTRING(11 11, 12 12, 13 13, 14 14, 15 15, 11 11)')))[1:1]);

SELECT (linestring_to_array(linestring_from_text('LINESTRING(11 11, 12 12, 13 13, 14 14, 15 15, 11 11)')))[2:2];

SELECT to_str(linestring_from_arrays(ARRAY[11,12,13,14,15,11], ARRAY[11,12,13,14,15,11]));

------------
CREATE TABLE tpoly
(
  geom geo_polygon
);

INSERT INTO tpoly VALUES(polygon_from_text('POLYGON((0 0, 10 0, 10 10, 0 10, 00))')),
                        (polygon_from_text('POLYGON((20 0, 30 0, 25 10, 20 0))'));

SELECT * FROM t2;

SELECT to_str(c) FROM t2;
------------

-- teste for operations geo_box --
SELECT geo_box_contain(box_from_text('BOX (7 7, 1 2)'), box_from_text('BOX (6 6, 2 2)'));
SELECT geo_box_contain(box_from_text('BOX (15 7, 1 2)'), box_from_text('BOX (14 7, 10 4)'));
SELECT geo_box_contained(box_from_text('BOX (1 2, 2 1)'), box_from_text('BOX (3 2, 1 0)'));
SELECT geo_box_contained(box_from_text('BOX (2 2, 0 2)'), box_from_text('BOX (6 6, 3 3)'));
SELECT geo_box_left(box_from_text('BOX (5 4, 2 0)'), box_from_text('BOX (-2 5, -4 2)'));
SELECT geo_box_left(box_from_text('BOX (-1 2, -4 0)'),box_from_text('BOX (2 3, 1 0)'));
SELECT geo_box_right(box_from_text('BOX (5 4, 2 0)'),box_from_text('BOX (-2 5, -4 2)'));
SELECT geo_box_overlap(box_from_text('BOX (4 3, 1 1)'),box_from_text('BOX (2 2, -1 -1)'));
SELECT geo_box_overlap(box_from_text('BOX (15 14, 20 10)'),box_from_text('BOX (2 2, 1 1)'));
SELECT box_from_text('BOX (7 7, 1 2)') @> box_from_text('BOX (6 6, 2 2)');


SELECT buoy_id, same_position(buoy,'geom', point_from_text('POINT(37.3168034143746 30.5994637720287)')) AS same FROM buoy;
SELECT linestring_intersection_points_v1(linestring_from_text('LINESTRING(11 11, 12 12, 13 13, 14 14, 15 15, 11 11)'));
--SELECT array_trajectory_agg(time, position) as trajec FROM boia;


CREATE TABLE buoy_obs(
 id SERIAL PRIMARY KEY,
 buoy_id INTEGER,
 time TIMESTAMP,
 position geo_point
);

INSERT INTO buoy_obs (buoy_id,time,position) VALUES (1, '2015-05-18 10:00:00'::timestamp, point_from_text('POINT(33 22)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (1, '2015-05-18 14:00:00'::timestamp, point_from_text('POINT(37 30)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (1, '2015-05-18 15:00:00'::timestamp, point_from_text('POINT(37 31)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (1, '2015-05-18 16:00:00'::timestamp, point_from_text('POINT(38 32)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (2, '2015-05-19 21:00:00'::timestamp, point_from_text('POINT(37 30)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (2, '2015-05-19 22:00:00'::timestamp, point_from_text('POINT(37 32)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (2, '2015-05-19 23:00:00'::timestamp, point_from_text('POINT(38 33)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (3, '2015-05-18 13:00:00'::timestamp, point_from_text('POINT(30 20)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (3, '2015-05-18 12:00:00'::timestamp, point_from_text('POINT(31 20)') );
INSERT INTO buoy_obs (buoy_id,time,position) VALUES (1, '2015-05-18 11:00:00'::timestamp, point_from_text('POINT(32 21)') );


SELECT array_trajectory_agg(time, position) as trajec FROM buoy_obs;
SELECT buoy_id, array_trajectory_agg(time, position) FROM buoy_obs GROUP BY buoy_id;
