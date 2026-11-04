#include <new>
#include "fable_console.h"

// CConsoleCommandLine's header-inline constructors as retail emitted them out of
// line (lib_console.cpp callers that did not expand them): default 009EA2B0,
// copy 009EA2E0 (the manifest's _Cons_val label is its STLport list caller),
// from-text 009EEA10, and the scalar deleting destructor 009EE390 (vtable
// 0x0129C400 slot 0; RTTI .?AVCConsoleCommandLine@@).

#pragma inline_depth(0)
void CConsoleCommandLine_EmitConstructors(void* defaultLine, void* copyLine, void* textLine,
                                          const CConsoleCommandLine& other, const CCharString& text)
{
    ::new (defaultLine) CConsoleCommandLine();
    ::new (copyLine) CConsoleCommandLine(other);
    ::new (textLine) CConsoleCommandLine(text);
}
