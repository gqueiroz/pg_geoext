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


# The name of the shared library to be built from the multiple
# source files in the GeoExt codebase.
MODULE_big = geoext

# As our extension uses multiple files, we have to
# set OBJS
OBJS = algorithms.o geo_linestring.o geo_point.o geo_polygon.o geoext.o hexutils.o wkt.o

# The extension name: geoext
EXTENSION = geoext

# Files that will be installed under prefix/share/geoext
DATA = geoext--0.1.0.sql  # the script used to install the extension

# Files that will be installed under prefix/doc/geoext
DOCS = README.geoext

# Build based on pg_config framework
PG_CONFIG = pg_config

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
