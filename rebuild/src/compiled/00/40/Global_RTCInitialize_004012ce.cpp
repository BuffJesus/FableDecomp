typedef void (__cdecl* RtcInitializer)();

extern "C" RtcInitializer DAT_01368e58[];
extern "C" RtcInitializer DAT_01368f58[];

// EXCEPTION_EXECUTE_HANDLER == 1: the __except filter that unconditionally
// runs the handler. Retail wraps each initializer call in __try/__except so a
// faulting init routine cannot abort the whole init table walk.
extern "C" void __cdecl __RTC_Initialize()
{
    for (RtcInitializer* entry = DAT_01368e58; entry < DAT_01368f58; ++entry)
    {
        __try
        {
            if (*entry != 0)
                (*entry)();
        }
        __except (1)
        {
        }
    }
}
