# c_unit -- C unit testing rig

A simple testing rig in C that is easy to use but has decent functionality. Developed by Terence Parr, Hanzhou Shi, Shuai Yuan, Yuanyuan Zhang at University of San Francisco.

Sample usage:

```c
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
```

```bash
$ cmake CMakeLists.txt
...
$ make
$ ./demo
PASS stuff
PASS other_stuff
assertion failure in bad_test: strcmp "foo" == "bar" (foo == bar)
FAIL bad_test
test bad_ptr is confused; signal SIGSEGV (11)
```

You can also run with ctest via cmake:

```bash
$ make test
Running tests...
Test project /Users/parrt/github/c_unit
    Start 1: sample_test
1/1 Test #1: sample_test ......................***Failed    0.00 sec

0% tests passed, 1 tests failed out of 1

Total Test time (real) =   0.00 sec

The following tests FAILED:
	  1 - sample_test (Failed)
Errors while running CTest
make: *** [test] Error 8
```

This cmake build uses valgrind for memory checking, which will cause failure upon memory leak.

To see all output, use `-V`:

```bash
$ ctest -V
...
test 1
    Start 1: sample_test

1: Test command: /usr/local/bin/valgrind "--error-exitcode=1" "--tool=memcheck" "--leak-check=full" "./sample_test"
...
1: ==22403== Command: ./sample_test
1: ==22403== 
1: PASS stuff
1: PASS other_stuff
1: assertion failure in bad_test: strcmp "foo" == "bar" (foo == bar)
1: --22403:0:syswrap- WARNING: Ignoring sigreturn( ..., UC_RESET_ALT_STACK );
1: FAIL bad_test
1: ==22403== 
1: ==22403== HEAP SUMMARY:
1: ==22403==     in use at exit: 34,917 bytes in 417 blocks
1: ==22403==   total heap usage: 518 allocs, 101 frees, 41,903 bytes allocated
1: ==22403== 
1: ==22403== LEAK SUMMARY:
1: ==22403==    definitely lost: 0 bytes in 0 blocks
1: ==22403==    indirectly lost: 0 bytes in 0 blocks
1: ==22403==      possibly lost: 0 bytes in 0 blocks
1: ==22403==    still reachable: 0 bytes in 0 blocks
1: ==22403==         suppressed: 34,917 bytes in 417 blocks
1: ==22403== 
1: ==22403== For counts of detected and suppressed errors, rerun with: -v
1: ==22403== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 16 from 16)
1/1 Test #1: sample_test ......................***Failed    0.89 sec

0% tests passed, 1 tests failed out of 1

Total Test time (real) =   0.89 sec

The following tests FAILED:
	  1 - sample_test (Failed)
Errors while running CTest
```
