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
    test(bad_ptr);

    return 0;
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

And now with valgrind checking:

```bash
$ ctest -D ExperimentalMemCheck
```