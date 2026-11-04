#include "fable_game_text_bank.h"

int main(int argc, char** argv)
{
    const CBankFileAsyncData* data =
        reinterpret_cast<const CBankFileAsyncData*>(argv);
    // Qualified call requires the real override symbol, not virtual dispatch.
    // VC7.1 must supply its genuine secondary-base this adjustment naturally.
    return data->CBankFileAsyncData::CanRelocateData(static_cast<long>(argc));
}
