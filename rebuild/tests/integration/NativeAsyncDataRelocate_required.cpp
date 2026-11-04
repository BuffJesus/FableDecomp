#include "fable_game_text_bank.h"
// Genuine qualified virtual call: compiler supplies secondary-this adjustment.
// This linkage executable is never run; no manual complete-this/fastcall shim.
int main(int argc, char** argv)
{
    reinterpret_cast<CBankFileAsyncData*>(argv)->CBankFileAsyncData::RelocateData(
        static_cast<long>(argc), static_cast<void*>(argv),
        static_cast<void*>(argv + 1), static_cast<long>(argc + 1));
    return 0;
}
