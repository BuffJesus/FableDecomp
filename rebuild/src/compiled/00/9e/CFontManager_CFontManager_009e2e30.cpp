#include "fable_system_manager.h"
#include "fable_font_data_banks.h"
#include "fable_game_text_bank.h"
#include "fable_symbol_map.h"
#include "fable_font_vertex_buffer.h"

CFontManager::CFontManager(const CCharString& staticHandle,
                           const CCharString& streamingHandle)
    : NumberAsyncGlyphsPending(0),
      LoadFontGlyphsAsynchronously(true)
{
    PFontDataBank.Reset(new CFontDataBank);
    PSymbols.Reset(new CSymbolMap);
    if (!staticHandle.IsEmpty())
    {
        PFontDataBank.Data->OpenReadOnly(staticHandle, 0x48);
        PFontDataBank.Data->CreateSymbolMap(*PSymbols.Data, false);
    }

    PStreamingFontDataBank.Reset(new CStreamingFontDataBank);
    PStreamingSymbols.Reset(new CSymbolMap);
    if (!streamingHandle.IsEmpty())
    {
        PStreamingFontDataBank.Data->OpenReadOnly(streamingHandle, 0x48);
        PStreamingFontDataBank.Data->CreateSymbolMap(*PStreamingSymbols.Data, false);
        PStreamingFontDataBank.Data->InitStreamingEntry(*PStreamingSymbols.Data);
    }

    CurrentVB = 0;
    CVertexBufferWin32* empty = 0;
    VertexBuffers.resize(8, empty);
    for (unsigned i = 0; i < 8; ++i)
        VertexBuffers[i] = new CVertexBufferWin32;
}