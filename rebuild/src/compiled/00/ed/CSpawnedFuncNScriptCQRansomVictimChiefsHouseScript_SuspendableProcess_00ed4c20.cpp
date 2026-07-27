typedef void (__fastcall *AutoTinySuspendCallback)(void*);
struct AutoTinySuspendableProcess
{
    unsigned char unknown00[5];
    bool suspended05;
    unsigned char unknown06[0x2e];
    AutoTinySuspendCallback callback34;
    void* context38;
};
void __fastcall SuspendableProcess(AutoTinySuspendableProcess* self)
{
    self->callback34(self->context38);
    self->suspended05 = true;
}