struct CSub { void Activate(); };
struct CNewFrontendGameComponent {
    unsigned char pad0[0x10];
    void*         field10;
    unsigned char pad14[0xb1 - 0x14];
    unsigned char initialised;
};
extern "C" void  __cdecl  FrontendInitStep1(void);
extern "C" void  __cdecl  FrontendInitStep2(void);
extern "C" CSub* __stdcall FrontendMakeSub(int a, int b, void* ctx);
CNewFrontendGameComponent* g_frontendComponent;
void __fastcall FrontendInit(CNewFrontendGameComponent* self)
{
    g_frontendComponent = self;
    FrontendInitStep1();
    FrontendInitStep2();
    self->initialised = 1;
    CSub* s = FrontendMakeSub(0x60, 0x29, self->field10);
    s->Activate();
}
