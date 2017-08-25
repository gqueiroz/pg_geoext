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

CREATE OR REPLACE FUNCTION same_position(record,cstring, geo_point)
    RETURNS boolean
    AS 'MODULE_PATHNAME', 'geo_point_same_position'
    LANGUAGE C;
--
-- Point Operators to interface to B-tree
--
CREATE OR REPLACE FUNCTION geo_point_cmp(geo_point, geo_point)
    RETURNS int4
    AS 'MODULE_PATHNAME', 'geo_point_cmp'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_eq(geo_point, geo_point)
    RETURNS boolean
    AS 'MODULE_PATHNAME', 'geo_point_eq'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_ne(geo_point, geo_point)
    RETURNS boolean
    AS 'MODULE_PATHNAME', 'geo_point_ne'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_lt(geo_point, geo_point)
    RETURNS boolean
    AS 'MODULE_PATHNAME', 'geo_point_lt'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_gt(geo_point, geo_point)
    RETURNS boolean
    AS 'MODULE_PATHNAME', 'geo_point_gt'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_le(geo_point, geo_point)
    RETURNS boolean
    AS 'MODULE_PATHNAME', 'geo_point_le'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_point_ge(geo_point, geo_point)
    RETURNS boolean
    AS 'MODULE_PATHNAME', 'geo_point_ge'
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


--
-- Create operators to interface geo_point to B-tree index
--
CREATE OPERATOR = (
    LEFTARG = geo_point,
    RIGHTARG = geo_point,
    PROCEDURE = geo_point_eq,
    COMMUTATOR = '=',
    NEGATOR = '<>',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel
);

CREATE OPERATOR <> (
    LEFTARG = geo_point,
    RIGHTARG = geo_point,
    PROCEDURE = geo_point_ne,
    COMMUTATOR = '<>',
    NEGATOR = '=',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel
);

CREATE OPERATOR <
(
    LEFTARG = geo_point,
    RIGHTARG = geo_point,
    PROCEDURE = geo_point_lt,
    COMMUTATOR = >,
    NEGATOR = >=,
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel
);

CREATE OPERATOR > (
    LEFTARG = geo_point,
    RIGHTARG = geo_point,
    PROCEDURE = geo_point_gt,
    COMMUTATOR = < ,
    NEGATOR = <=,
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel
);

CREATE OPERATOR <= (
    LEFTARG = geo_point,
    RIGHTARG = geo_point,
    PROCEDURE = geo_point_le,
    COMMUTATOR = >= ,
    NEGATOR = >,
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel
);

CREATE OPERATOR >= (
    LEFTARG = geo_point,
    RIGHTARG = geo_point,
    PROCEDURE = geo_point_ge,
    COMMUTATOR = <= ,
    NEGATOR = <,
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel
);


--
-- Create an operator class for geo_point to interface to B-tree index
--
CREATE OPERATOR CLASS btree_geo_point_ops
    DEFAULT FOR TYPE geo_point USING btree AS
        OPERATOR        1       <  ,
        OPERATOR        2       <= ,
        OPERATOR        3       =  ,
        OPERATOR        4       >= ,
        OPERATOR        5       >  ,
        FUNCTION        1       geo_point_cmp(geo_point, geo_point);


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

CREATE OR REPLACE FUNCTION geo_linestring_recv(internal)
    RETURNS geo_linestring
    AS 'MODULE_PATHNAME','geo_linestring_recv'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_linestring_send(geo_linestring)
    RETURNS bytea
    AS 'MODULE_PATHNAME', 'geo_linestring_send'
    LANGUAGE C IMMUTABLE STRICT;


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

CREATE OR REPLACE FUNCTION is_closed(geo_linestring)
   RETURNS boolean
   AS 'MODULE_PATHNAME', 'geo_linestring_is_closed'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION length(geo_linestring)
   RETURNS float8
   AS 'MODULE_PATHNAME', 'geo_linestring_length'
   LANGUAGE C IMMUTABLE STRICT;


CREATE FUNCTION linestring_to_array(geo_linestring)
   RETURNS float8[]
   AS 'MODULE_PATHNAME', 'geo_linestring_to_array'
   LANGUAGE C IMMUTABLE;

CREATE OR REPLACE FUNCTION linestring_from_arrays(float8[], float8[])
   RETURNS geo_linestring
   AS 'MODULE_PATHNAME', 'geo_linestring_from_array'
   LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION linestring_intersection_points_v1(IN geo_linestring,
   OUT x float8, OUT y float8)
   RETURNS SETOF record
   AS 'MODULE_PATHNAME', 'geo_linestring_intersection_points_v1'
   LANGUAGE C IMMUTABLE STRICT;


--
-- Register the geo_linestring Data Type
--
CREATE TYPE geo_linestring
(
    input = geo_linestring_in,
    output = geo_linestring_out,
    receive = geo_linestring_recv,
    send = geo_linestring_send,
    internallength = variable,
    storage = extended,
    alignment = double
);

----------------------------------------
----------------------------------------
-- Introduces the geo_box Data Type --
----------------------------------------
----------------------------------------

--
-- Drop geo_box type if it exists and forward its declaration
--
DROP TYPE IF EXISTS geo_box;
CREATE TYPE geo_box;

--
-- Box Input/Output Functions
--
CREATE OR REPLACE FUNCTION geo_box_in(cstring)
    RETURNS geo_box
    AS 'MODULE_PATHNAME', 'geo_box_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION geo_box_out(geo_box)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_box_out'
    LANGUAGE C IMMUTABLE STRICT;

--
-- Box Operators
--
CREATE OR REPLACE FUNCTION box_from_text(cstring)
    RETURNS geo_box
    AS 'MODULE_PATHNAME', 'geo_box_from_text'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION to_str(geo_box)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'geo_box_to_str'
    LANGUAGE C IMMUTABLE STRICT;


--
-- Box Operators to interface to R-tree GiST
--

CREATE FUNCTION geo_box_contain(geo_box, geo_box)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'geo_box_contain'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION geo_box_contained(geo_box, geo_box)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'geo_box_contained'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION geo_box_left(geo_box,geo_box)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'geo_box_left'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION geo_box_same(geo_box, geo_box)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'geo_box_same'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION geo_box_right(geo_box, geo_box)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'geo_box_right'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION geo_box_below(geo_box, geo_box)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'geo_box_below'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION geo_box_above(geo_box, geo_box)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'geo_box_above'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION geo_box_overlap(geo_box, geo_box)
    RETURNS bool
    AS 'MODULE_PATHNAME','geo_box_overlap'
    LANGUAGE C IMMUTABLE STRICT;

--
-- Register the geo_box Data Type
--
CREATE TYPE geo_box(
    input = geo_box_in,
    output = geo_box_out,
    --receive = geo_box_recv,
    --send = geo_box_send,
    internallength = 32,
    alignment = double
);

--
-- Create operators to interface geo_box to R-tree index
--

--1 RTLeftStrategyNumber
CREATE OPERATOR <<
(
  PROCEDURE = geo_box_left,
  LEFTARG = geo_box,
  RIGHTARG = geo_box,
  RESTRICT = areasel,
  JOIN = areajoinsel
);

-- 3 RTOverlapStrategyNumber
CREATE OPERATOR &&
(
  PROCEDURE = geo_box_overlap,
  LEFTARG = geo_box,
  RIGHTARG = geo_box,
  RESTRICT = areasel,
  JOIN = areajoinsel
);

-- 5 RTRightStrategyNumber
CREATE OPERATOR >>
(
  PROCEDURE = geo_box_right,
  LEFTARG = geo_box,
  RIGHTARG = geo_box,
  --COMMUTATOR = << ,
  RESTRICT = areasel,
  JOIN = areajoinsel
);

-- 6 RTSameStrategyNumber
CREATE OPERATOR ~=
(
  PROCEDURE = geo_box_same,
  LEFTARG = geo_box,
  RIGHTARG = geo_box,
  --COMMUTATOR =  ,
  RESTRICT = areasel,
  JOIN = areajoinsel
);

-- 7 RTContainsStrategyNumber
CREATE OPERATOR @>
(
  PROCEDURE = geo_box_contain,
  LEFTARG = geo_box,
  RIGHTARG = geo_box,
  RESTRICT = areasel,
  JOIN = areajoinsel
);

-- 8 RTContainedByStrategyNumber
CREATE OPERATOR <@
(
  PROCEDURE = geo_box_contained,
  LEFTARG = geo_box,
  RIGHTARG = geo_box,
  RESTRICT = areasel,
  JOIN = areajoinsel
);

--10 RTBelowStrategyNumber
CREATE OPERATOR <<|
(
  PROCEDURE = geo_box_below,
  LEFTARG = geo_box,
  RIGHTARG = geo_box,
  COMMUTATOR = |>>,
  RESTRICT = areasel,
  JOIN = areajoinsel
);

-- 11 RTAboveStrategyNumber
CREATE OPERATOR |>>
(
  PROCEDURE = geo_box_above,
  LEFTARG = geo_box,
  RIGHTARG = geo_box,
  COMMUTATOR = <<| ,
  RESTRICT = areasel,
  JOIN = areajoinsel
);


---
-- Define GiST methods
---

CREATE OR REPLACE FUNCTION geo_box_consistent(internal, geo_box, smallint, oid, internal)
    RETURNS bool
    AS 'MODULE_PATHNAME', 'geo_box_consistent'
    LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION geo_box_union(internal, internal)
    RETURNS geo_box
    AS 'MODULE_PATHNAME', 'geo_box_union'
    LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION geo_box_compress(internal)
    RETURNS internal
    AS 'MODULE_PATHNAME', 'geo_box_compress'
    LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION geo_box_decompress(internal)
    RETURNS internal
    AS 'MODULE_PATHNAME', 'geo_box_decompress'
    LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION geo_box_penalty(internal, internal, internal)
    RETURNS internal
    AS 'MODULE_PATHNAME', 'geo_box_penalty'
    LANGUAGE C STRICT;

/*CREATE OR REPLACE FUNCTION geo_box_picksplit(internal, internal)
    RETURNS internal
    AS 'MODULE_PATHNAME', 'geo_box_picksplit'
    LANGUAGE C STRICT;*/

CREATE OR REPLACE FUNCTION g_box_same(geo_box, geo_box, internal)
    RETURNS internal
    AS 'MODULE_PATHNAME', 'g_box_same'
    LANGUAGE C STRICT;


--
-- Create an operator class for geo_box to interface to R-tree index
--

CREATE OPERATOR CLASS gist_gbox_ops
    DEFAULT FOR TYPE geo_box USING gist AS
        OPERATOR        1        <<  ,
      	OPERATOR        3        &&  ,
      	OPERATOR        5        >>	 ,
        OPERATOR	      6	       ~=  ,
        OPERATOR        7        @>  ,
        OPERATOR        8        <@  ,
      	OPERATOR        10       <<| ,
      	OPERATOR        11       |>> ,

        FUNCTION  1 geo_box_consistent(internal, geo_box, smallint, oid, internal),
      	FUNCTION	2	geo_box_union (internal, internal),
      	FUNCTION	3	geo_box_compress (internal),
      	FUNCTION	4	geo_box_decompress (internal),
      	FUNCTION	5	geo_box_penalty (internal, internal, internal),
      	--FUNCTION	6	geo_box_picksplit (internal, internal),
      	FUNCTION	7	g_box_same (geo_box, geo_box, internal);
      	--FUNCTION	8	geo_box_distance (internal, cube, smallint, oid, internal);
