// lib_streaming_font_data_bank.cpp, retail 0x00A5EF30 / 0x00A5F010 (Ego_r ?ReadData@CStreamingFontDataBank@@
// QAEXPAXKK@Z and ?StreamGlyph@...@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@PAXKK_N@Z): reads from the
// streaming font entry, asynchronously (high priority) unless asked not to or the font manager loads glyphs
// synchronously.
#include "fable_font_data_banks.h"
#include "fable_bank_async_entry.h"
#include "fable_system_manager.h"

void CStreamingFontDataBank::ReadData(void* data, unsigned long offset, unsigned long size)
{
    streamingEntry->ReadDataNonAsync(offset, data, size);
}

CCountedPointer<CBankFileAsyncData> CStreamingFontDataBank::StreamGlyph(void* data, unsigned long offset, unsigned long size, bool synchronous)
{
    CSystemManager* system = GFGetSystemManager();
    if (!synchronous && system->PFontManager->GetLoadFontGlyphsAsynchronously())
        return streamingEntry->BeginReadData(offset, data, size, size, THREADED_FILE_PRIORITY_HIGH);
    streamingEntry->ReadDataNonAsync(offset, data, size);
    return CCountedPointer<CBankFileAsyncData>();
}
