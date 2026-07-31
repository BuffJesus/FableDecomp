
struct PDef { void* vtbl; int refcount; };
struct Entry { int tag; PDef* pdef; };
struct CTCInventoryAbilities {
    char pad[0x160];
    Entry* table;
    PDef** GetAbilityPDef(PDef** out, int index);
};
PDef** CTCInventoryAbilities::GetAbilityPDef(PDef** out, int index)
{
    PDef* p = this->table[index].pdef;
    *out = p;
    if (p) p->refcount++;
    return out;
}