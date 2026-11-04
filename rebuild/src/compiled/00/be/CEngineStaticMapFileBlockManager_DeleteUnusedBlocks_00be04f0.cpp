#include "engine/CEngineStaticMapFileBlockManager.h"
#include "fable_game_text_bank.h"
#include "fable_stlport_tree_erase.h"
void CEngineStaticMapFileBlockManager::DeleteUnusedBlocks() {
    _STL::map<long,CFileBlock>::iterator current=Blocks.begin();
    while(current!=Blocks.end()) {
        _STL::map<long,CFileBlock>::iterator next=current;
        ++next;
        if(current->second.RefCount==0) {
            if(!current->second.LoadingData.Data->IsFinished())
                CBankFileAsyncData::DiscardAsyncData(current->second.LoadingData);
            Blocks.erase(current);
        }
        current=next;
    }
}
