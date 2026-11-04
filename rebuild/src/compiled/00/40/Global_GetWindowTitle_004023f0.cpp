#include "fable_window_title.h"
#include "fable_game_text_bank.h"
#include "fable_language.h"
#include "fable_scoped_ptr.h"

// The window title from the text bank's TEXT_GUI_WINDOW_TITLE entry.
CWideString FABLE_FASTCALL GetWindowTitle()
{
    CWideString title(L"Fable - The Lost Chapters");
    boost::scoped_ptr<NGameText::CDataBank> textBank;
    CCharString symbol("TEXT_GUI_WINDOW_TITLE", -1);
    textBank.reset(new NGameText::CDataBank);
    textBank->OpenReadOnly(NLocalisation::GetTextBankName(), 0x100);

    unsigned long index = textBank->FindIndexBySymbolCRC(symbol.GetCRC());
    if (index > 0)
    {
        CIVCountedPointer<const NGameText::CTextEntry> entry =
            textBank->GetPTextEntry(index, true);
        if (!entry.IsNull())
            title = entry->GetText();
    }
    return title + L" ";
}
