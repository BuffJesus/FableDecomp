struct CTCMapwho;

// direct (__fastcall) helper at 0xc8ce80 in retail (this in ecx)
void __fastcall Mapwho_Cleanup(CTCMapwho* self);

typedef void (*GlobalDieFn)(CTCMapwho*);
GlobalDieFn g_dieFn = 0; // resolves to [0x1440154]

// this-in-ecx via __fastcall; harness maps __fastcall->__fastcall
int __fastcall CTCMapwho_OnDie(CTCMapwho* self) {
    if (self) {
        Mapwho_Cleanup(self);
        g_dieFn(self);
    }
    return 0;
}