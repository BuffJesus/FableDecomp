// Behaviour of CStartupDefinitionManager::ErrorCallback 0x0043368C: ignores the
// message (reads and writes nothing).
#include "fable_definition_manager.h"
#include <stdio.h>

struct Probe : CStartupDefinitionManager
{
    static ErrorCallbackFunction Callback() { return &ErrorCallback; }
};

int main()
{
    unsigned long messageRaw[2] = {0xC0FFEE11, 0xC0FFEE22};
    const CCharString& message = *reinterpret_cast<const CCharString*>(messageRaw);
    Probe::Callback()(message);
    if (messageRaw[0] != 0xC0FFEE11 || messageRaw[1] != 0xC0FFEE22) return 1;
    puts("OK_0043368c");
    return 0;
}
