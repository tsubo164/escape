/*
Copyright (c) 2011-2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "Test.h"
#include <stdio.h>

static int n_pass = 0;
static int n_fail = 0;
static int n_total = 0;

void Test_Pass(const char *expr, const char *file, int line)
{
	fprintf(stdout, "  :PASS :%s:%d: %s\n", file, line, expr);
	n_pass++;
	n_total++;
}

void Test_Fail(const char *expr, const char *file, int line)
{
	fprintf(stdout, "* :FAIL :%s:%d: %s\n", file, line, expr);
	n_fail++;
	n_total++;
}

int Test_GetPassCount()
{
	return n_pass;
}

int Test_GetFailCount()
{
	return n_fail;
}

int Test_GetTotalCount()
{
	return n_total;
}

/* the following codes from comp.lang.c FAQ list · Question 14.5
   http://c-faq.com/fp/fpequal.html */
#define Abs(x)    ((x) < 0 ? -(x) : (x))
#define Max(a, b) ((a) > (b) ? (a) : (b))

static double RelDif(double a, double b)
{
  double c = Abs(a);
  double d = Abs(b);

  d = Max(c, d);

  return d == 0.0 ? 0.0 : Abs(a - b) / d;
}

int Test_DoubleEq(double a, double b)
{
	return RelDif(a, b) <= 1e-15 ? 1 : 0;
}

int Test_FloatEq(float a, float b)
{
	return RelDif(a, b) <= 1e-7 ? 1 : 0;
}

