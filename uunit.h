/* The MIT License (MIT)
*
* Copyright (c) 2015, Mihail K
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
# ifndef __UUNIT_H__
# define __UUNIT_H__

# include <stdint.h>

# if !defined(NDEBUG) && !defined(NUNITTEST)

# ifndef __COUNTER__
#	define __COUNTER__ 0
# endif /* counter */

# define unittest(body) \
	__unittest(__LINE__, __COUNTER__, body)

# define start_tests \
	__start_tests(__LINE__, __COUNTER__)

# define end_tests \
	__end_tests(__LINE__, __COUNTER__)

/* - Internal Macros - */

typedef struct {
	uintptr_t header;
	void (* test)(void);
	uintptr_t footer;
} UnitTest;

# define UNITTEST_HEADER ((uintptr_t)0xBEEF19AC)
# define UNITTEST_FOOTER ((uintptr_t)0xCAFE3821)

# define __concat2(prefix, line, counter) \
	prefix ## line ## counter

# define __concat(prefix, line, counter) \
	__concat2(prefix, line, counter)

# define __unittest(line, counter, body) \
	static void __concat(_test_func_, line, counter)(void) body \
	__UNUSED static UnitTest __concat(_test_, line, counter) = { \
		UNITTEST_HEADER, __concat(_test_func_, line, counter), UNITTEST_FOOTER \
	};

# define __start_tests(line, counter) \
	__UNUSED static UnitTest __concat(_unittests_start_, line, counter) = { \
		UNITTEST_HEADER, 0, UNITTEST_FOOTER \
	}; \
	__CONSTRUCTOR(__concat(_test_start_, line, counter)) { \
		uintptr_t addr = (uintptr_t)&__concat(_unittests_start_, line, counter); \
		for(addr += sizeof(UnitTest);; addr++) { \
			UnitTest *test = (UnitTest *)addr; \
			if(test->header == UNITTEST_HEADER && test->footer == UNITTEST_FOOTER) { \
				if(!test->test) break; \
				test->test(); \
			} \
		} \
	}

# define __end_tests(line, counter) \
	__UNUSED static UnitTest __concat(_unittests_end, line, counter) = { \
		UNITTEST_HEADER, 0, UNITTEST_FOOTER \
	};

/* - Constructor Macro - */

# if defined(_MSC_VER)
#	pragma section(".CRT$XCU", read)
#	define __UNUSED __pragma(warning(suppress:4100))
#	define __CONSTRUCTOR(func) \
	static void __cdecl func(void); \
	__declspec(allocate(".CRT$XCU")) void(__cdecl *__concat(func, _, _))(void) = func; \
	static void __cdecl func(void)
# elif defined(__GNUC__)
#	define __UNUSED __attribute__((unused))
#	define __CONSTRUCTOR(func) \
	static void func(void) __attribute__((constructor)); \
	static void func(void)
# else
#	error Unsupported C compiler.
# endif /* Compiler */

# else /* ndebug || nunittest */

# define unittest(body)
# define start_tests
# define end_tests

# endif /* ndebug || nunittest */

# endif /* uunit.h */
