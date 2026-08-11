#pragma optimize("s",on)
// CTCInventoryItem::OnDie @ 0x00458504
// push esi; mov esi,ecx; mov eax,[esi+0x3c]; test eax,eax; jz L;
// push eax; call free (0xbfea14); pop ecx; L: mov ecx,esi; pop esi; jmp base::OnDie (0x430300)

void __cdecl my_free(void* p);                // -> 0xbfea14
void __fastcall base_OnDie(void* self);       // -> 0x430300

struct CTCInventoryItem {
    char pad[0x3c];
    void* f3c;
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    if (this->f3c)
        my_free(this->f3c);
    base_OnDie(this);
}