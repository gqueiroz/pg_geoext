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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wmissing-prototypes -Wpointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdeclaration-after-statement -Wendif-labels")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-format-attribute -Wformat-security")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -fwrapv")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-command-line-argument")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.7")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Winline")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -bundle")
set(CMAKE_SHARED_LINKER_FLAGS "-multiply_defined suppress")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-undefined,dynamic_lookup")

include_directories("/Applications/Postgres.app/Contents/Versions/9.6/include/postgresql/server")

include_directories(${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext)

file(GLOB PG_GEOEXT_SRC_FILES ${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/*.c)
file(GLOB PG_GEOEXT_HDR_FILES ${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/*.h)

source_group("Source Files"  FILES ${PG_GEOEXT_SRC_FILES})
source_group("Header Files"  FILES ${PG_GEOEXT_HDR_FILES})

add_library(geoext SHARED ${PG_GEOEXT_SRC_FILES} ${PG_GEOEXT_HDR_FILES})

set_target_properties(geoext PROPERTIES PREFIX "")
set_target_properties(geoext PROPERTIES SUFFIX ".so")
