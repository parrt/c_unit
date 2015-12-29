/*
The MIT License (MIT)

Copyright (c) 2015 Terence Parr, Hanzhou Shi, Shuai Yuan, Yuanyuan Zhang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <stdio.h>
#include <string.h>
#include "cunit.h"
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

void (*cunit_setup)()		= NULL;
void (*cunit_teardown)()	= NULL;

static const char *current_test_name;

static jmp_buf longjmp_env;

void _assert_true(bool a, const char as[], const char funcname[]) {
	if ( !a ) {
		fprintf(stderr, "assertion failure in %s: %s is false\n", funcname, as);
		longjmp(longjmp_env, 1);
	}
}

void _assert_false(bool a, const char as[], const char funcname[]) {
	if ( a ) {
		fprintf(stderr, "assertion failure in %s: %s is true\n", funcname, as);
		longjmp(longjmp_env, 1);
	}
}

void _assert_equal(unsigned long a, unsigned long b, const char as[], const char bs[], const char funcname[]) {
	if ( a!=b ) {
		fprintf(stderr, "assertion failure in %s: %s == %s (%lu == %lu)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_not_equal(unsigned long a, unsigned long b, const char as[], const char bs[], const char funcname[]) {
	if ( a==b ) {
		fprintf(stderr, "assertion failure in %s: %s != %s (%lu != %lu)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_addr_equal(void *a, void *b, const char as[], const char bs[], const char funcname[]) {
	if ( a!=b ) {
		fprintf(stderr, "assertion failure in %s: %s == %s (%p == %p)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_addr_not_equal(void *a, void *b, const char as[], const char bs[], const char funcname[]) {
	if ( a==b ) {
		fprintf(stderr, "assertion failure in %s: %s != %s (%p != %p)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_float_equal(double a, double b, const char as[], const char bs[], const char funcname[]) {
	if (a != b) {
		fprintf(stderr, "assertion failure in %s: %s == %s (%lf == %lf)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_float_not_equal(double a, double b, const char as[], const char bs[], const char funcname[]) {
	if ( a==b ) {
		fprintf(stderr, "assertion failure in %s: %s != %s (%lf != %lf)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_str_equal(char *a, char *b, const char as[], const char bs[], const char funcname[]) {
	if ( strcmp(a,b)!=0 ) {
		fprintf(stderr, "assertion failure in %s: strcmp %s == %s (%s == %s)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_str_not_equal(char *a, char *b, const char as[], const char bs[], const char funcname[]) {
	if ( strcmp(a,b)==0 ) {
		fprintf(stderr, "assertion failure in %s: strcmp %s != %s (%s != %s)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_strn_equal(char *a, char *b, size_t n, const char as[], const char bs[], const char funcname[]) {
	if ( strncmp(a,b,n)!=0 ) {
		fprintf(stderr, "assertion failure in %s: strcmp %s == %s (%s == %s for %d char)\n", funcname, as, bs, a, b, (int)n);
		longjmp(longjmp_env, 1);
	}
}

void _assert_strn_not_equal(char *a, char *b, size_t n, const char as[], const char bs[], const char funcname[]) {
	if ( strncmp(a,b,n)==0 ) {
		fprintf(stderr, "assertion failure in %s: strcmp %s != %s (%s != %s for %d char)\n", funcname, as, bs, a, b, (int)n);
		longjmp(longjmp_env, 1);
	}
}

static void
handle_sys_errors(int errno)
{
	char *signame = "UNKNOWN";

	if (errno == SIGSEGV)
		signame = "SIGSEGV";
	else if (errno == SIGBUS)
		signame = "SIGBUS";
	fprintf(stderr, "test %s is confused; signal %s (%d)\n", current_test_name, signame, errno);
	exit(errno);
}

static inline void
setup_error_handlers() {
	signal(SIGSEGV, handle_sys_errors);
	signal(SIGBUS, handle_sys_errors);
}

void cunit_test(void (*f)(), const char funcname[]) {
	current_test_name = funcname;
	setup_error_handlers(); // ensure signals are trapped each time
	int exit_code = 0;
	if ( cunit_setup!=NULL ) {
		if ( setjmp(longjmp_env)==0 ) {
			(*cunit_setup)();
		}
		else {
			fprintf(stderr, "FAIL SETUP %s\n", funcname);
			exit_code = -1;
		}
	}
	if ( setjmp(longjmp_env)==0 ) {
		f();
		fprintf(stderr, "PASS %s\n", funcname);
	}
	else {
		fprintf(stderr, "FAIL %s\n", funcname);
		exit_code = -1;
	}
	if ( cunit_teardown!=NULL ) {
		if ( setjmp(longjmp_env)==0 ) {
			(*cunit_teardown)();
		}
		else {
			fprintf(stderr, "FAIL TEARDOWN %s\n", funcname);
			exit_code = -1;
		}
	}
	if ( exit_code != 0 ) exit(exit_code);
}
