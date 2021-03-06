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
* \file geoext/geo_box.h
*
* \brief A geo box is a type of support used in the indexing method GiST.
*        It has the upper and lower coordinates of geo types
*
* \author Gilberto Ribeiro de Queiroz
* \author Fabiana Zioti
*
* \date 2017
*
* \copyright GNU Lesser Public License version 3
*
*/

/*
 * R-Tree Bibliography
 * [1] A. Guttman. R-tree: a dynamic index structure for spatial searching.
 *    Proceedings of the ACM SIGMOD Conference, pp 47-57, June 1984.
 */


#ifndef __GEOEXT_GEO_BOX_H__
#define __GEOEXT_GEO_BOX_H__

/* PostgreSQL */
#include <postgres.h>
#include <fmgr.h>

/* GeoExt */
#include "decls.h"


/*
* A geo_box is bidimensional box used in the indexing method GiST
*
* It is a fixed-size data type in PostgreSQL with a double-alignment.
*
*/
struct geo_box
{
 struct coord2d high;  /* 2D coordinate. */
 struct coord2d low;  /* 2D coordinate. */
};

/*
* geo_box is a fixed-size pass-by-reference type.
*
* Below we have the fmgr interface macros for dealing with a geo_box.
*
*/

#define DatumGetGeoBoxTypeP(X)      ((struct geo_box*) DatumGetPointer(X))
#define PG_GETARG_GEOBOX_TYPE_P(n)  DatumGetGeoBoxTypeP(PG_GETARG_DATUM(n))
#define PG_RETURN_GEOBOX_TYPE_P(x)  PG_RETURN_POINTER(x)

/*
 * geo_box operations.
 *
 */

/* Support operations for geo_box type */

extern Datum geo_box_in(PG_FUNCTION_ARGS);
extern Datum geo_box_out(PG_FUNCTION_ARGS);
extern Datum geo_box_from_text(PG_FUNCTION_ARGS);
extern Datum geo_box_to_str(PG_FUNCTION_ARGS);

/*
*  GiST:  R-Tree operations support
*
*/

extern Datum geo_box_contain(PG_FUNCTION_ARGS);
extern Datum geo_box_contained(PG_FUNCTION_ARGS);
extern Datum geo_box_left(PG_FUNCTION_ARGS);
extern Datum geo_box_same(PG_FUNCTION_ARGS);
extern Datum geo_box_right(PG_FUNCTION_ARGS);
extern Datum geo_box_below(PG_FUNCTION_ARGS);
extern Datum geo_box_above(PG_FUNCTION_ARGS);
extern Datum geo_box_overlap(PG_FUNCTION_ARGS);
extern Datum geo_box_overleft(PG_FUNCTION_ARGS);
/*
extern Datum geo_box_overright(PG_FUNCTION_ARGS);
extern Datum geo_box_overbelow(PG_FUNCTION_ARGS);
extern Datum geo_box_overabove(PG_FUNCTION_ARGS);
*/
// extern Datum geo_box_below_eq(PG_FUNCTION_ARGS); // Is obsolete
// extern Datum geo_box_above_eq(PG_FUNCTION_ARGS); // Is obsolete



/* Support procedures for GiST */

/*
 * \brief The GiST Consistent method for geo_box
 * \note Given an index entry p and a query value q,
 *        this function determines whether the index entry is "consistent" with the query
 */

extern Datum geo_box_consistent(PG_FUNCTION_ARGS);


/*
 * \brief The GiST Union method for geo_box
 * \note This method consolidates information in the tree. Given a set of entries,
 *       this function generates a new index entry that represents all the given entries.
 */

extern Datum geo_box_union(PG_FUNCTION_ARGS);


/*
 * \brief GiST Compress method for geo_box
 * \note For geo_box do not do anything.
 *       Converts the data item into a format suitable for physical storage in an index page.
 */

extern Datum geo_box_compress(PG_FUNCTION_ARGS);


/*
 * \brief GiST DeCompress method for geo_box
 * \note For geo_box do not do anything. Just use the stored geo_box as is.
 *
 *       Converts the index representation of the data item into a format
 *     that can be manipulated by the other GiST methods in the operator class
 *
 */

extern Datum geo_box_decompress(PG_FUNCTION_ARGS);


/*
 * \brief GiST Penalty method for geo_box
 * \note Returns a value indicating the "cost" of inserting the new entry into a particular branch of the tree.
 *       Items will be inserted down the path of least penalty in the tree.
 */

extern Datum geo_box_penalty(PG_FUNCTION_ARGS);


/*
 * \brief GiST Picksplit method for geo_box
 * \note Split an overflowing node into two new nodes.
 */

extern Datum geo_box_picksplit(PG_FUNCTION_ARGS);

/*
 * \brief GiST Same method for geo_box
 * \note Equality method
 *
 */

extern Datum g_box_same(PG_FUNCTION_ARGS);




#endif  /* __GEOEXT_H__ */
