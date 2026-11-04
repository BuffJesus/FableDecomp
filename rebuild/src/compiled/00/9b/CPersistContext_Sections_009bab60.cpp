// lib_persist_context.cpp, retail 0x009BAB60 / 0x009BABF0: named save
// sections. A binary save writes the name and a length slot, then patches
// the length (bytes after the slot) when the section ends; a text save
// writes START_/END_ lines.
#include "fable_persist_context.h"
#include "fable_string.h"

// Retail 009BAB60.
void CPersistContext::StartSection(CPersistContextWriteSection& section, const CCharString& name)
{
    section.Name = name;
    if (Mode == MODE_BINARY_SAVE)
    {
        PSaveStream->WriteNullTerminatedString(name);
        PSaveStream->WriteSLONG(0);
        section.StartPosition = PSaveStream->GetPosition();
    }
    else if (Mode == MODE_TEXT_SAVE)
    {
        *PStringToSaveTo += "START_" + section.Name + "\r\n";
    }
}

// Retail 009BABF0.
void CPersistContext::EndSection(CPersistContextWriteSection& section)
{
    if (Mode == MODE_BINARY_SAVE)
    {
        unsigned long end = PSaveStream->GetPosition();
        long length = end - section.StartPosition;
        PSaveStream->SetPosition(section.StartPosition - 4);
        PSaveStream->WriteSLONG(length);
        PSaveStream->SetPosition(end);
    }
    else if (Mode == MODE_TEXT_SAVE)
    {
        *PStringToSaveTo += "END_" + section.Name + "\r\n\r\n";
    }
}
