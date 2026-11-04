#include <stdio.h>
#include "fable_input_manager.h"

// Ego_r CVersion (lib_version.cpp), retail 009EA080..009EA130. Built with /GS
// (GetAsString's text buffer carries a stack cookie).

// Retail 009EA080.
CVersion::CVersion(unsigned char day, unsigned char month, unsigned long year, unsigned long build)
{
    Month = month;
    Day = day;
    Year = year;
    Build = build;
}

// Retail 009EA0A0.
bool CVersion::operator==(const CVersion& version) const
{
    return Day == version.Day && Month == version.Month && Year == version.Year &&
           Build == version.Build;
}

// Retail 009EA0D0: "%02d.%02d.%02d.%04d" (day, month, year, build).
CCharString CVersion::GetAsString() const
{
    char text[256];
    sprintf(text, "%02d.%02d.%02d.%04d", Day, Month, Year, Build);
    return CCharString(text);
}
