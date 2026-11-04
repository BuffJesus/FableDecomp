#include "fable_symbol_map.h"
#include "fable_disk_file.h"
#include "fable_carray.h"
#include "fable_comments.h"
#include "fable_stlport_vector_impl.h"

// lib_symbol_map.cpp, retail 009E5C90 (Ego_r ?LoadFromFile@CSymbolMap@@QAE_NABVCWideString@@@Z;
// locals success, file, text): replace the map with the enums of one symbol
// file. Clear and UpdateDirty are expanded; the file being parsed is kept in
// a CWideString static for the parser's errors (retail 0x013CAA34, beside the
// CCharString group name CurrentFilename 0x013CAA30; its name is not
// recovered).
extern CWideString g_FableSymbolMapParseFile_013CAA34;

// Expanded here (out-of-line copies 0x009E5250 and 0x009E5C30).
inline void CSymbolMap::Clear()
{
    SLONGMap.DeallocateMemory();
    NamesUsed._M_t.clear();
}

inline void CSymbolMap::UpdateDirty() const
{
    CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> >& map =
        const_cast<CSymbolMap*>(this)->SLONGMap;
    map.Update();
    map.DeallocateUnusedMemory();
}

bool CSymbolMap::LoadFromFile(const CWideString& name)
{
    bool success = false;
    Clear();
    g_FableSymbolMapParseFile_013CAA34 = name;
    CDiskFileWin32 file;
    file.Open(name, FILE_ACCESS_READ, 2);
    CArray<char> text(file.GetLength() + 1);
    text[file.GetLength()] = 0;
    file.Read(&text[0], file.GetLength(), 0);
    GFRemoveComments(&text[0], "//", "\n");
    GFRemoveComments(&text[0], "//", "\r");
    GFRemoveComments(&text[0], "/*", "*/");
    if (ParseBuffer(&text[0]))
        success = true;
    UpdateDirty();
    g_FableSymbolMapParseFile_013CAA34 = L"";
    return success;
}
