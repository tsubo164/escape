/*
Copyright (c) 2011-2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEST(expr) \
	do { if (expr) {\
	    Test_Pass( #expr, __FILE__, __LINE__ ); \
	} else { \
	    Test_Fail( #expr, __FILE__, __LINE__ ); \
	} } while (0)

#define TEST_INT(a, b) \
	do { if ((a)==(b)) {\
	    Test_Pass( #a" == "#b, __FILE__, __LINE__ ); \
	} else { \
	    Test_Fail( #a" == "#b, __FILE__, __LINE__ ); \
		printf("*   actual:   %d\n", (a)); \
		printf("*   expected: %d\n", (b)); \
	} } while (0)

#define TEST_FLOAT(a, b) \
	do { if (Test_FloatEq((a),(b))) {\
	    Test_Pass( #a" == "#b, __FILE__, __LINE__ ); \
	} else { \
	    Test_Fail( #a" == "#b, __FILE__, __LINE__ ); \
		printf("*   actual:   \"%g\"\n", (a)); \
		printf("*   expected: \"%g\"\n", (b)); \
	} } while (0)

#define TEST_DOUBLE(a, b) \
	do { if (Test_DoubleEq((a),(b))) {\
	    Test_Pass( #a" == "#b, __FILE__, __LINE__ ); \
	} else { \
	    Test_Fail( #a" == "#b, __FILE__, __LINE__ ); \
		printf("*   actual:   \"%g\"\n", (double)(a)); \
		printf("*   expected: \"%g\"\n", (double)(b)); \
	} } while (0)

#define TEST_STR(a, b) \
	do { if (strcmp((a),(b))==0) {\
	    Test_Pass( #a" == "#b, __FILE__, __LINE__ ); \
	} else { \
	    Test_Fail( #a" == "#b, __FILE__, __LINE__ ); \
		printf("*   actual:   \"%s\"\n", (a)); \
		printf("*   expected: \"%s\"\n", (b)); \
	} } while (0)

extern void Test_Pass( const char *expr, const char *file, int line );
extern void Test_Fail( const char *expr, const char *file, int line );

extern int Test_GetPassCount();
extern int Test_GetFailCount();
extern int Test_GetTotalCount();

extern int Test_DoubleEq(double a, double b);
extern int Test_FloatEq(float a, float b);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XXX_H */

