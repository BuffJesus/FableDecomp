#include <cstdio>
typedef void (__fastcall *AutoTinySuspendCallback)(void*);
struct AutoTinySuspendableProcess
{
    unsigned char unknown00[5];
    bool suspended05;
    unsigned char unknown06[0x2e];
    AutoTinySuspendCallback callback34;
    void* context38;
};
static int g_AutoTinySuspendCalls = 0;
static bool g_AutoTinySuspendObservedPriorState = true;
void __fastcall AutoTinySuspendCallbackImpl(void* context)
{
    AutoTinySuspendableProcess* self =
        static_cast<AutoTinySuspendableProcess*>(context);
    ++g_AutoTinySuspendCalls;
    g_AutoTinySuspendObservedPriorState = self->suspended05;
}
void __fastcall SuspendableProcess(AutoTinySuspendableProcess* self)
{
    self->callback34(self->context38);
    self->suspended05 = true;
}
int main()
{
    AutoTinySuspendableProcess process = {0};
    process.callback34 = AutoTinySuspendCallbackImpl;
    process.context38 = &process;
    SuspendableProcess(&process);
    if (!process.suspended05 || g_AutoTinySuspendCalls != 1 ||
        g_AutoTinySuspendObservedPriorState)
        return 1;
    std::printf("AUTO_TINY_00e62970_TEST PASS\n");
    return 0;
}