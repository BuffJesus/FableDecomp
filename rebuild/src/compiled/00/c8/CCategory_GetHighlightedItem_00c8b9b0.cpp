struct CCategory {
    char pad[0x50];
    __int64 item;   /* 0x50 */
    long count;     /* 0x58 */
};

__int64 __fastcall CCategory_GetHighlightedItem(const CCategory* self)
{
    if (self->count < 2)
        return (__int64)-131;
    return self->item;
}