// lib_bank_file.cpp, retail 0x009CBAA0 / 0x009CBDF0 / 0x009CBE10 (Ego_r
// names and locals): entry-symbol helpers. A symbol "main[subscript]" splits
// at its brackets; one without '[' is public. A file path reduces to its
// file name and extension.
#include "fable_game_text_bank.h"
#include "fable_filesystem.h"

// Retail 009CBAA0. Retail copies the whole symbol, not mainsymb, into
// *mainsymbol; the main part it computes is never used. The slip is kept.
void CBankFile::SplitSymbol(const CCharString& symbol, CCharString* mainsymbol, CCharString* subscript)
{
    long open = symbol.FindCharPos('[');
    long close = symbol.FindCharPos(']');
    CCharString mainsymb;
    CCharString subscr;
    if (open != -1 && close != -1)
    {
        mainsymb = symbol.GetSubString(0, open);
        subscr = symbol.GetSubString(open + 1, close);
    }
    else
    {
        mainsymb = symbol;
        subscr = "";
    }
    if (mainsymbol != 0)
        *mainsymbol = symbol;
    if (subscript != 0)
        *subscript = subscr;
}

// Retail 009CBDF0.
bool CBankFile::IsSymbolPublic(const CCharString& symbol) const
{
    return symbol.FindCharPos('[') == -1;
}

// Retail 009CBE10.
CCharString CBankFile::ConvertPathToFilename(const CCharString& file_path) const
{
    CWideString filename;
    CWideString extension;
    CAFile::SplitPath(file_path.ToWideString(), 0, 0, &filename, &extension);
    filename += extension;
    return filename.ToCharString();
}
