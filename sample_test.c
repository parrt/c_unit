#include <stdio.h>
#include <stdlib.h>
#include "c_unit.h"

static void setup() {
}

static void teardown() {
}

void stuff() {
	assert_float_equal(1.0, 1.0);
	assert_true(1);
	assert_false(0);
	char *p = malloc(10);
	char *q = p;
	assert_addr_equal(p,q);
	char *r = NULL;
	assert_addr_not_equal(p,r);
	free(p); // leave this off and valgrind will complain
}

void other_stuff() {
	assert_str_not_equal("hi", "mom");
}

void bad_ptr() {
	char *p = (char *)0x100;
	*p = 'a';
}

void bad_test() {
	assert_str_equal("foo", "bar");
}

int main(int argc, char *argv[])
{
	c_unit_setup = setup;
	c_unit_teardown = teardown;

	test(stuff);
	test(other_stuff);

	test(bad_test);

	// uncomment next line to test a bad memory reference, forcing program to bail out with non-0 error code
//	test(bad_ptr);

	return c_unit_fails; // return how many test()s failed.  ctest likes 0 return value to mean no failures
}
