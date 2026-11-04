// lib_file_abstract.cpp, retail 0x009975B0-0x009975E0: small path helpers
// (Ego_r names and order).
#include "fable_filesystem.h"
#include "fable_string.h"
#include "fable_string_parser.h"

// The characters a path component may not contain (retail 0x0129A79C: < > :, the cp1252
// left double quote 0x93, / and | and a backslash).
extern const char g_FableForbiddenComponentSymbols_0129A79C[7];

// Retail 009975B0.
long FABLE_FASTCALL CAFile::GetHDSectorSize()
{
    return 0x800;
}

// Retail 009975C0.
long FABLE_FASTCALL CAFile::GetDVDSectorSize()
{
    return 0x800;
}

// Retail 009975E0.
bool FABLE_FASTCALL CAFile::IsForbiddenSymbolInComponent(const CCharString& component)
{
    bool forbidden = false;
    for (long i = 0; i < 7; ++i)
    {
        if (component.FindCharPos(g_FableForbiddenComponentSymbols_0129A79C[i]) > -1)
        {
            forbidden = true;
            break;
        }
    }
    return forbidden;
}
