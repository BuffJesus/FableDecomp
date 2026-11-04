// lib_wstringparser: CWParsedItem::GetString, retail 0x00A0CB80. Retail keeps the
// Ego_r module order InitAsSymbol 0x00A0CA00, InitAsInteger 0x00A0CA20,
// InitAsFloat 0x00A0CA90, InitAsString 0x00A0CB50, GetSymbol 0x00A0CB70,
// GetString 0x00A0CB80, GetInteger 0x00A0CB90 (Ego_r 0x00B77530..0x00B776F0;
// FableWin ?GetString@CWParsedItem@@QBEABVCWideString@@XZ). GetSymbol and
// GetString are separate byte-identical out-of-line bodies (no ICF in retail).
// Ego_r.pdb layout: CWParsedItem 0x0C { EParsedItemType Type; union {long;float} +4;
// CWideString String +8 }. No shared engine/ header models CWParsedItem.
#include "fable_string.h"

class CWParsedItem
{
public:
    enum EParsedItemType {};
    const CWideString& GetSymbol() const;
    const CWideString& GetString() const;

private:
    EParsedItemType Type;     // +0x00
    union
    {
        long Integer;
        float Float;
    };                        // +0x04
    CWideString String;       // +0x08
};

const CWideString& CWParsedItem::GetString() const
{
    return String;
}
