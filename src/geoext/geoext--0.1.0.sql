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


--
-- Drop GeoExt types if they exists and forward their declaration
--
DROP TYPE IF EXISTS geo_point;
CREATE TYPE geo_point;


--
-- Point Input/Output functions
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
-- Point functions
--
CREATE OR REPLACE FUNCTION from_text(cstring)
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
-- Register GeoExt data types
--
CREATE TYPE geo_point(
    input = geo_point_in,
    output = geo_point_out,
    receive = geo_point_recv,
    send = geo_point_send,
    internallength = 24,
    alignment = double
);

