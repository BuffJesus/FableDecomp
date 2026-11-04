#include "fable_game_text_bank.h"
static unsigned long AsyncPointerSize(const CCountedPointer<CBankFileAsyncData>& value)
{
    if (value.Data) return sizeof(value) + value.Data->Sizeof();
    return sizeof(value);
}
unsigned long NGameText::CTextEntry::Sizeof() const
{
    unsigned long size = sizeof(*this);
    size += SpeechBank.Sizeof() - sizeof(SpeechBank);
    size += Narrator.Sizeof() - sizeof(Narrator);
    size += SoundName.Sizeof() - sizeof(SoundName);
    size += Text.Sizeof() - sizeof(Text);
    size += Tags.capacity() * sizeof(CTextTag);
    size += AsyncPointerSize(PFileData) - sizeof(PFileData);
    return size;
}
unsigned long NGameText::CGroupEntry::Sizeof() const
{
    unsigned long size = sizeof(*this) + Entries.capacity() * sizeof(long);
    size += AsyncPointerSize(PFileData) - sizeof(PFileData);
    return size;
}
