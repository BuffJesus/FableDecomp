#include "fable_font_data_banks.h"

void CFontDataBank::Close()
{
    CloseFonts();
    CBankFile::Close();
}
