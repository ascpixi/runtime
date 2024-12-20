// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

/*++





--*/

#ifndef __PAL_ASSERT_H__
#define __PAL_ASSERT_H__

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
//
// C_ASSERT() can be used to perform many compile-time assertions:
//            type sizes, field offsets, etc.
//
#define C_ASSERT(e) static_assert(e, #e)

//
// CPP_ASSERT() can be used within a class definition, to perform a
// compile-time assertion involving private names within the class.
//
#define CPP_ASSERT(n, e) static_assert(e, #e)

#endif // __cplusplus

extern void __ryujit_pal_asserte_fail(const char* format, ...);

#ifndef _ASSERTE
#if defined(_DEBUG)
#define _ASSERTE(e) do {                                        \
        if (!(e)) {                                             \
            __ryujit_pal_asserte_fail (                         \
                     "ASSERT FAILED\n"                          \
                     "\tExpression: %s\n"                       \
                     "\tLocation:   line %d in %s\n"            \
                     "\tFunction:   %s\n",                      \
                     #e, __LINE__, __FILE__, __FUNCTION__);     \
        }                                                       \
    }while (0)
#else // !DEBUG
#define _ASSERTE(e) ((void)0)
#endif
#endif // _ASSERTE

#ifndef assert
#define assert(e) _ASSERTE(e)
#endif  // assert

#ifdef  __cplusplus
}
#endif

#endif // __PAL_ASSERT_H__
