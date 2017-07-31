--
-- Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.
--
-- This file is part of pg_geoext, a simple PostgreSQL extension for 
-- for teaching spatial database classes.
--
-- pg_geoext is free software: you can redistribute it and/or modify
-- it under the terms of the GNU Lesser General Public License version 3 as
-- published by the Free Software Foundation.
--
-- pg_geoext is distributed  "AS-IS" in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
-- of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- GNU Lesser General Public License for more details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with pg_geoext. See LICENSE. If not, write to
-- Gilberto Ribeiro de Queiroz at <gribeiro@dpi.inpe.br>.


--
-- Complain if script is sourced in psql, rather than via CREATE EXTENSION
--
\echo Use "CREATE EXTENSION geoext" to load this file. \quit


----------------------------------------
----------------------------------------
-- Introduces the geo_point Data Type --
----------------------------------------
----------------------------------------

--
-- Drop geo_point type if it exists and forward its declaration
--
DROP TYPE IF EXISTS geo_point;
CREATE TYPE geo_point;


--
-- Point Input/Output Functions
--
CREATE OR REPLACE FUNCTION geo_point_in(cstring)
    RETURNS geo_point
    AS 'MODULE_PATHNAME', 'geo_point_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_out(geo_point)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_point_out'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_recv(internal)
    RETURNS geo_point
    AS 'MODULE_PATHNAME','geo_point_recv'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_send(geo_point)
    RETURNS bytea
    AS 'MODULE_PATHNAME', 'geo_point_send'
    LANGUAGE C IMMUTABLE STRICT;


--
-- Point Operators
--
CREATE OR REPLACE FUNCTION point_from_text(cstring)
    RETURNS geo_point
    AS 'MODULE_PATHNAME', 'geo_point_from_text'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION to_str(geo_point)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_point_to_str'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION distance(geo_point, geo_point)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'geo_point_distance'
    LANGUAGE C IMMUTABLE STRICT;

--
-- Register the geo_point Data Type
--
CREATE TYPE geo_point(
    input = geo_point_in,
    output = geo_point_out,
    receive = geo_point_recv,
    send = geo_point_send,
    internallength = 24,
    alignment = double
);


---------------------------------------------
---------------------------------------------
-- Introduces the geo_linestring Data Type --
---------------------------------------------
---------------------------------------------

--
-- Drop geo_linestring type if it exists and forward its declaration
--
DROP TYPE IF EXISTS geo_linestring;
CREATE TYPE geo_linestring;


--
-- LineString Input/Output Functions
--
CREATE OR REPLACE FUNCTION geo_linestring_in(cstring)
    RETURNS geo_linestring
    AS 'MODULE_PATHNAME', 'geo_linestring_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_linestring_out(geo_linestring)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_linestring_out'
    LANGUAGE C IMMUTABLE STRICT;

-- CREATE OR REPLACE FUNCTION geo_linestring_recv(internal)
--     RETURNS geo_linestring
--     AS 'MODULE_PATHNAME','geo_linestring_recv'
--     LANGUAGE C IMMUTABLE STRICT;
-- 
-- CREATE OR REPLACE FUNCTION geo_linestring_send(geo_linestring)
--     RETURNS bytea
--     AS 'MODULE_PATHNAME', 'geo_linestring_send'
--     LANGUAGE C IMMUTABLE STRICT;


--
-- LineString Operators
--
CREATE OR REPLACE FUNCTION linestring_from_text(cstring)
    RETURNS geo_linestring
    AS 'MODULE_PATHNAME', 'geo_linestring_from_text'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION to_str(geo_linestring)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_linestring_to_str'
    LANGUAGE C IMMUTABLE STRICT;

-- CREATE OR REPLACE FUNCTION is_closed(geo_linestring)
--     RETURNS boolean
--     AS 'MODULE_PATHNAME', 'geo_linestring_is_closed'
--     LANGUAGE C IMMUTABLE STRICT;

-- CREATE OR REPLACE FUNCTION length(geo_linestring)
--     RETURNS float8
--     AS 'MODULE_PATHNAME', 'geo_linestring_length'
--     LANGUAGE C IMMUTABLE STRICT;


--
-- Register the geo_linestring Data Type
--
CREATE TYPE geo_linestring
(
    input = geo_linestring_in,
    output = geo_linestring_out,
--    receive = geo_linestring_recv,
--    send = geo_linestring_send,
    internallength = variable,
    storage = extended,
    alignment = double
);


---------------------------------------------
---------------------------------------------
-- Introduces the geo_polygon Data Type --
---------------------------------------------
---------------------------------------------

--
-- Drop geo_polygon type if it exists and forward its declaration
--
DROP TYPE IF EXISTS geo_polygon;
CREATE TYPE geo_polygon;

--
-- Polygon Input/Output Functions
--
CREATE OR REPLACE FUNCTION geo_polygon_in(cstring)
    RETURNS geo_polygon
    AS 'MODULE_PATHNAME', 'geo_polygon_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_polygon_out(geo_polygon)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_polygon_out'
    LANGUAGE C IMMUTABLE STRICT;

---
--- Polygon Operators
---

CREATE OR REPLACE FUNCTION polygon_from_text(cstring)
    RETURNS geo_polygon
    AS 'MODULE_PATHNAME', 'geo_polygon_from_text'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION to_str(geo_polygon)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_polygon_to_str'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION polygon_contains_point(geo_polygon, geo_point)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_polygon_contains_point'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION polygon_area(geo_polygon)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'geo_polygon_area'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION polygon_perimeter(geo_polygon)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'geo_polygon_perimeter'
    LANGUAGE C IMMUTABLE STRICT;

--
-- Register the geo_linestring Data Type
--
CREATE TYPE geo_polygon
(
    input = geo_polygon_in,
    output = geo_polygon_out,
--    receive = geo_linestring_recv,
--    send = geo_linestring_send,
    internallength = variable,
    storage = extended,
    alignment = double
);



