#include "fable_game_text_bank.h"

// NGameText::CDataBank::GetPTextEntry(unsigned long, bool) const
// @ 0x009C9070.
CIVCountedPointer<const NGameText::CTextEntry>
NGameText::CDataBank::GetPTextEntry(unsigned long index, bool load) const
{
    const CDataBankTextEntry* text = GetTextInfo(index);
    if (text)
    {
        const_cast<CDataBank*>(this)->AsyncUpdatePending();

        if (text->IsLoaded() || load)
        {
            if (text->PText.IsNull())
            {
                const CDataBankTextEntry* info = GetTextInfo(index);
                if (info)
                    info->BeginLoading(index, *const_cast<CDataBank*>(this));
                text->BuildText();
                const_cast<CDataBank*>(this)->ResourceList.MakeFreeSpace(0);
            }
            else if (text->PText->PFileData.Data)
            {
                text->BuildText();
            }

            text->PText->TouchResource();
            return text->PText;
        }

        BeginLoadingText(index);
    }
    return CIVCountedPointer<const CTextEntry>();
}
