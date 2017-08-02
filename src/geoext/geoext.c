/*
  Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.

  This file is part of pg_geoext, a simple PostgreSQL extension for 
  for teaching spatial database classes.

  pg_geoext is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 3 as
  published by the Free Software Foundation.

  pg_geoext is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pg_geoext. See LICENSE. If not, write to
  Gilberto Ribeiro de Queiroz at <gribeiro@dpi.inpe.br>.
 */

/*!
 *
 * \file geoext/geoext.c
 *
 * \brief GeoExt spatial types for PostgreSQL.
 *
 * \author Gilberto Ribeiro de Queiroz
 * \author Fabiana Zioti
 *
 * \date 2017
 *
 * \copyright GNU Lesser Public License version 3
 *
 */


/* PostgreSQL */
#include <postgres.h>
#include <fmgr.h>


/* Prototype definitions */
void _PG_init(void);

void _PG_fini(void);


/*
  Note: assure that this is the only PG_MODULE_MAGIC definition
  in the whole extension!
 */
PG_MODULE_MAGIC;


void _PG_init()
{
  /*elog(NOTICE, "GeoExtension initialized!");*/
}


void _PG_fini()
{
  /*elog(NOTICE, "GeoExtension finalized!");*/
}
