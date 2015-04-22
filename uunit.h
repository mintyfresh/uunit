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

# define unittest __unittest(__LINE__, __COUNTER__)

# if !defined(NDEBUG) && !defined(NUNITTEST)

# define __unittest(line, counter) \
	__CONSTRUCTOR(__concat(_test_func_, line, counter))

# else /* ndebug || nunittest */

# define __unittest(line, counter) \
	__UNUSED(__concat(_test_func_, line, counter))

# endif /* ndebug || nunittest */

/* - Internal Macros - */

# ifndef __COUNTER__
#	define __COUNTER__ 0
# endif /* counter */

# define __concat2(prefix, line, counter) \
	prefix ## line ## counter

# define __concat(prefix, line, counter) \
	__concat2(prefix, line, counter)

/* - Compiler Specific Macros - */

# if defined(_MSC_VER)
#	pragma section(".CRT$XCU", read)
#	define __UNUSED(func) \
	__pragma(warning(suppress:4100)) \
	static void func(void)
#	define __CONSTRUCTOR(func) \
	static void __cdecl func(void); \
	__declspec(allocate(".CRT$XCU")) void(__cdecl *__concat(func, _, _))(void) = func; \
	static void __cdecl func(void)
# elif defined(__GNUC__)
#	define __UNUSED(func) \
	static void func(void) __attribute__((unused)); \
	static void func(void)
#	define __CONSTRUCTOR(func) \
	static void func(void) __attribute__((constructor)); \
	static void func(void)
# else
#	error Unsupported C compiler.
# endif /* Compiler */

# endif /* uunit.h */
