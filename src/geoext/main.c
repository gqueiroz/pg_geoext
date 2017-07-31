#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <string.h>

/* PostgreSQL */
// #include <postgres.h>
// #include <fmgr.h>
// #include <libpq/pqformat.h>
// #include <utils/builtins.h>


int main (){

 double d;

 d = d + (sqrt((pow((55 - 44), 2)) +
           (pow((55 - 44), 2))));

  printf("%f",d);

  // printf("%s",str);
  // char *cp = (char*)strcasestr("POINT(44 44)", "POINT");
  // cp += 6;
  //
  // printf ("%s",cp);
  //printf("%d", strcasestr("POINT(44 44)", "POINT"));

//  coord2d_decode(cp, &cp, "geo_point", str);
}


// coord2d_decode(char *str,
//                char **endptr,
//                const char *type_name,
//                const char *orig_string)
// {
//   assert(str);
//   assert(endptr);
//   assert(type_name);
//   assert(orig_string);
//
// float8in_internal(str, &str, type_name, orig_string);
//
//  float8in_internal(str, &str, type_name, orig_string);
//   printf("asas");
//   *endptr = str;
// }
