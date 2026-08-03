struct CWorldMapField4Obj;

extern "C" __declspec(naked) void __stdcall HelperPrepare_7a7bf1(CWorldMapField4Obj* obj, int flag) {
    __asm {
        ret
    }
}

struct CWorldMap {
    void* field0;
    CWorldMapField4Obj* field4;

    void __fastcall PrepareForLoad();
};

void __fastcall CWorldMap::PrepareForLoad() {
    HelperPrepare_7a7bf1(field4, 0);
}