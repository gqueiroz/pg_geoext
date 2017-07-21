#
# Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.
#
# This file is part of pg_geoext, a simple PostgreSQL extension for 
# for teaching spatial database classes.
#
# pg_geoext is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License version 3 as
# published by the Free Software Foundation.
#
# pg_geoext is distributed  "AS-IS" in the hope that it will be useful,
# but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with pg_geoext. See LICENSE. If not, write to
# Gilberto Ribeiro de Queiroz at <gribeiro@dpi.inpe.br>.
#
# Author: Gilberto Ribeiro de Queirox
#         Fabiana Zioti
#

cmake_minimum_required(VERSION 2.8.12)

project(pg_geoext)

get_filename_component(PG_GEOEXT_ABSOLUTE_ROOT_DIR "../../" ABSOLUTE)

include_directories(${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src)

find_package(PostgreSQL)

add_subdirectory(unittest_algorithms)
