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
#include <setjmp.h>
#include <stdbool.h>

extern void (*cunit_setup)();
extern void (*cunit_teardown)();

#define test(f)						cunit_test(f, #f)
#define assert_true(a)			    _assert_true(a,#a,__func__)
#define assert_false(a)			    _assert_false(a,#a,__func__)
#define assert_equal(a,b)			_assert_equal((unsigned long)a,(unsigned long)b,#a,#b,__func__)
#define assert_not_equal(a,b)		_assert_not_equal((unsigned long)a,(unsigned long)b,#a,#b,__func__)
#define assert_addr_equal(a,b)		_assert_addr_equal(a,b,#a,#b,__func__)
#define assert_addr_not_equal(a,b)	_assert_addr_not_equal(a,b,#a,#b,__func__)
#define assert_str_equal(a,b)		_assert_str_equal(a,b,#a,#b,__func__)
#define assert_str_not_equal(a,b)	_assert_str_not_equal(a,b,#a,#b,__func__)
#define assert_float_equal(a,b)		_assert_float_equal(a,b,#a,#b,__func__)
#define assert_float_not_equal(a,b)	_assert_float_not_equal(a,b,#a,#b,__func__)
#define assert_strn_equal(a,b,n)	_assert_strn_equal(a,b,n,#a,#b,__func__)
#define assert_strn_not_equal(a,b,n)_assert_strn_not_equal(a,b,n,#a,#b,__func__)

void _assert_true(bool a, const char as[], const char funcname[]);
void _assert_false(bool a, const char as[], const char funcname[]);
void _assert_equal(unsigned long a, unsigned long b, const char as[], const char bs[], const char funcname[]);
void _assert_not_equal(unsigned long a, unsigned long b, const char as[], const char bs[], const char funcname[]);
void _assert_addr_equal(void *a, void *b, const char as[], const char bs[], const char funcname[]);
void _assert_addr_not_equal(void *a, void *b, const char as[], const char bs[], const char funcname[]);
void _assert_float_equal(double a, double b, const char as[], const char bs[], const char funcname[]);
void _assert_float_not_equal(double a, double b, const char as[], const char bs[], const char funcname[]);
void _assert_str_equal(char *a, char *b, const char as[], const char bs[], const char funcname[]);
void _assert_str_not_equal(char *a, char *b, const char as[], const char bs[], const char funcname[]);
void _assert_strn_equal(char *a, char *b, size_t n, const char as[], const char bs[], const char funcname[]);
void _assert_strn_not_equal(char *a, char *b, size_t n, const char as[], const char bs[], const char funcname[]);

void cunit_test(void (*f)(), const char funcname[]);
