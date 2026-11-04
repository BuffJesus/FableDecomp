#include "fable_game_text_bank.h"
// Qualified virtual call is a genuine direct reference to the exact root.
// Linkage only: the resulting executable must never be run.
int main(int argc, char** argv)
{
    reinterpret_cast<CBankFileAsyncData*>(argv)->CBankFileAsyncData::OnFileReadCompletion(argc != 0);
    return 0;
}
