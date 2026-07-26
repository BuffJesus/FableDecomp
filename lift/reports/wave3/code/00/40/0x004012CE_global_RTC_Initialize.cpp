using RtcInitializer = void (__cdecl*)();

extern "C" RtcInitializer DAT_01368e58[];
extern "C" RtcInitializer DAT_01368f58[];

extern "C" void __cdecl __RTC_Initialize()
{
    for (RtcInitializer* entry = DAT_01368e58; entry < DAT_01368f58; ++entry) {
        if (*entry != nullptr) {
            (*entry)();
        }
    }
}