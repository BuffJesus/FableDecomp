#include "fable_game_text_bank.h"

// NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const
// @ 0x009C8890.
CIVCountedPointer<const NGameText::CGroupEntry>
NGameText::CDataBank::GetPGroupEntry(unsigned long index, bool load) const
{
    const CDataBankGroupEntry* group = GetGroupInfo(index);
    if (group)
    {
        const_cast<CDataBank*>(this)->AsyncUpdatePending();

        if (group->IsLoaded() || load)
        {
            if (group->PGroup.IsNull())
            {
                const CDataBankGroupEntry* info = GetGroupInfo(index);
                if (info)
                    info->BeginLoading(index, *const_cast<CDataBank*>(this));
                group->BuildGroup();
                const_cast<CDataBank*>(this)->ResourceList.MakeFreeSpace(0);
            }
            else if (group->PGroup->PFileData.Data)
            {
                group->BuildGroup();
            }

            group->PGroup->TouchResource();
            return group->PGroup;
        }

        BeginLoadingGroup(index);
    }
    return CIVCountedPointer<const CGroupEntry>();
}
