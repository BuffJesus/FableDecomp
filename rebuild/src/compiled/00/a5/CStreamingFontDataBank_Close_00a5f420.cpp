#include "fable_font_data_banks.h"

void CStreamingFontDataBank::Close()
{
    CloseFonts();
    CBankFile::Close();
}
