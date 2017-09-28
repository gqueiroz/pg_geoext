# A Simple Geo-Extension for PostgreSQL 

pg_geoext is a spatial extension prototype for teaching spatial database classes.

## Status
---
Extension in development

## Requeriments
---
The pg_geoext has no dependencies other than PostgreSQL.

## Building & Installation
---
To build it, just do this:

    src/
    make
    make install

## Usage
---

```sql
CREATE EXTENSION geoext;

SELECT point_from_text('POINT(31.3168034143746 29.5994637720287)');
SELECT linestring_from_text('LINESTRING(0 0, 10 10)'));
```

For more examples see doc/geoext-example.sql

## Types:
```c
* POINT (12 45)
* LINESTRING (10 10, 12 13, 10 10)
* POLYGON((0 0, 10 0, 10 10, 0 10, 00))
```
