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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99 -Winline")

include_directories(${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext)
include_directories(${PostgreSQL_INCLUDE_DIR})

set(PG_GEOEXT_SRC_FILES "${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/algorithms.c"
                        "${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/hexutils.c"
                        "${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/unittest/algorithms/main.c")

set(PG_GEOEXT_HDR_FILES "${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/algorithms.h"
                        "${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/hexutils.h"
                        "${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/decls.h")

#file(GLOB PG_GEOEXT_SRC_FILES ${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/*.c)
#file(GLOB PG_GEOEXT_HDR_FILES ${PG_GEOEXT_ABSOLUTE_ROOT_DIR}/src/geoext/*.hpp)

source_group("Source Files"  FILES ${PG_GEOEXT_SRC_FILES})
source_group("Header Files"  FILES ${PG_GEOEXT_HDR_FILES})

add_executable(unittest_algorithms ${PG_GEOEXT_SRC_FILES} ${PG_GEOEXT_HDR_FILES})

target_link_libraries(unittest_algorithms ${PostgreSQL_LIBRARY})
