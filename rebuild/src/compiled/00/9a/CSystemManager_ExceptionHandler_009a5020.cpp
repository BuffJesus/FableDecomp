// lib_system_manager.cpp, retail 0x009A5020 (Ego_r
// ?ExceptionHandler@CSystemManager@@QAEHH@Z): in exclusive mode, describe the
// exception code in the shared error text and let the search continue (0).
// Retail's FLT_OVERFLOW case shares the divide-by-zero text (the two case
// bodies are merged), so the same literal is used for both here.
#include <stdio.h>
#include "fable_system_manager_state.h"

// The error text buffer (retail 0x013CB0C8; Ego_r name not recovered).
extern char g_FableExceptionText_013CB0C8[];

int CSystemManager::ExceptionHandler(int code)
{
    if (IsValid() == true && ExclusiveMode == true)
    {
        switch (code)
        {
        case 0xC0000005:  // EXCEPTION_ACCESS_VIOLATION
            sprintf(g_FableExceptionText_013CB0C8, "CRITICAL ERROR - An access violation has occured.");
            return 0;
        case 0x80000004:  // EXCEPTION_SINGLE_STEP
            sprintf(g_FableExceptionText_013CB0C8, "STUPID ERROR - The program has encountered a single step signal.");
            return 0;
        case 0x80000003:  // EXCEPTION_BREAKPOINT
            sprintf(g_FableExceptionText_013CB0C8, "STUPID ERROR - The program has encountered a breakpoint.");
            return 0;
        case 0xC0000025:  // EXCEPTION_NONCONTINUABLE_EXCEPTION
            sprintf(g_FableExceptionText_013CB0C8,
                    "CRITICAL ERROR - The program attempted to continue from a non-continuable exception. "
                    "Please quit the application.");
            return 0;
        case 0xC000008E:  // EXCEPTION_FLT_DIVIDE_BY_ZERO
            sprintf(g_FableExceptionText_013CB0C8, "CRITICAL ERROR - The program attempted a floating point divide by zero.");
            return 0;
        case 0xC000008D:  // EXCEPTION_FLT_DENORMAL_OPERAND
            sprintf(g_FableExceptionText_013CB0C8,
                    "CRITICAL ERROR - The program attempted a floating point operation with a denormal operand.");
            return 0;
        case 0xC000008C:  // EXCEPTION_ARRAY_BOUNDS_EXCEEDED
            sprintf(g_FableExceptionText_013CB0C8,
                    "CRITICAL ERROR - The program attempted to access an out of bounds array element.");
            return 0;
        case 0xC000008F:  // EXCEPTION_FLT_INEXACT_RESULT
            sprintf(g_FableExceptionText_013CB0C8,
                    "CRITICAL ERROR - The result of a floating point operation cannot be represented.");
            return 0;
        case 0xC0000090:  // EXCEPTION_FLT_INVALID_OPERATION
            sprintf(g_FableExceptionText_013CB0C8,
                    "CRITICAL ERROR - The program attempted an illegal floating point operation.");
            return 0;
        case 0xC0000091:  // EXCEPTION_FLT_OVERFLOW
            sprintf(g_FableExceptionText_013CB0C8, "CRITICAL ERROR - The program attempted a floating point divide by zero.");
            return 0;
        case 0xC0000092:  // EXCEPTION_FLT_STACK_CHECK
            sprintf(g_FableExceptionText_013CB0C8,
                    "CRITICAL ERROR - A floating point operation overflowed or underflowed the stack.");
            return 0;
        case 0xC0000093:  // EXCEPTION_FLT_UNDERFLOW
            sprintf(g_FableExceptionText_013CB0C8,
                    "CRITICAL ERROR - The exponent of a floating point operation is less than the magnitude allowed.");
            return 0;
        case 0xC0000094:  // EXCEPTION_INT_DIVIDE_BY_ZERO
            sprintf(g_FableExceptionText_013CB0C8, "CRITICAL ERROR - The program attempted an integer divide by zero.");
            return 0;
        case 0xC0000095:  // EXCEPTION_INT_OVERFLOW
            sprintf(g_FableExceptionText_013CB0C8, "CRITICAL ERROR - An integer operation overflowed.");
            return 0;
        case 0xC0000096:  // EXCEPTION_PRIV_INSTRUCTION
            sprintf(g_FableExceptionText_013CB0C8, "CRITICAL ERROR - The program attempted to use an illegal instruction.");
            break;
        }
    }
    return 0;
}
