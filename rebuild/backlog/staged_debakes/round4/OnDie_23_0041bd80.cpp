// Null-guarded teardown: run the cleanup member, then free the object.
// __fastcall object=ecx, no args.
struct T { void Cleanup(); };
extern "C" void __cdecl Free1(void* p);   // 0x00bfe9bc
extern "C" void __fastcall OnDie(T* p) {
    if (p) {
        p->Cleanup();
        Free1(p);
    }
}
