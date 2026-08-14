// Byte-exact reconstruction of CTCInventoryExperience::ResetStatIndicatorValues @ 0x0056e5ab
// Size-optimized zeroing (and mem,0 vs mov mem,0) matches retail; harness also sweeps optimize("s").
#pragma optimize("s",on)
struct CTCInventoryExperience {
    char pad[0x1bc];
    unsigned long field_1bc; // +0x1bc
    unsigned long field_1c0; // +0x1c0
    // this-call @ 0x5bd328 : ecx=self, one stack arg. Real member => native thiscall.
    int StoreIndicator(int arg);
};

// helper @ 0x5bc4b2 : cdecl, no args, returns a value (in eax)
extern int __cdecl HelperGetValue(void);

void __fastcall ResetStatIndicatorValues(CTCInventoryExperience* self)
{
    self->field_1bc = 0;
    self->field_1c0 = self->StoreIndicator(HelperGetValue());
}