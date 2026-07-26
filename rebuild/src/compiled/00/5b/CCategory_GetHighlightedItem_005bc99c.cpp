struct CCategoryItem { long value; long pad1; long pad2; long pad3; long pad4; };
struct CCategory {
    char pad0[8];
    CCategoryItem* items; /* +0x8 */
    char pad2[0x20 - 0xC];
    long highlighted; /* +0x20 */
};
long __fastcall CCategory_GetHighlightedItem(const CCategory* self)
{
    long idx = self->highlighted;
    if (idx >= 0)
        return self->items[idx].value;
    return 0;
}