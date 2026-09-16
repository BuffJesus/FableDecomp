#pragma once
#include "FableAPI.h"
#include "GameInterface.h"
#include <stdexcept>
#include <string>

// Proposal: one native counted Thing owned exclusively by the quest. Never
// expose this class or a shared_ptr to its Thing as Lua userdata.
class MazeSwordSlot {
public:
    using DeleteInfo=void(__cdecl*)(void*);
    MazeSwordSlot(CGameScriptInterfaceBase* game,DeleteInfo deleteInfo):game_(game),deleteInfo_(deleteInfo) {
        static_assert(sizeof(void*)==4 && sizeof(CScriptThing)==12,"Retail x86 Thing layout");
        if(!game_ || !RetailThing_Destroy_API || !g_pCScriptThingVTable || !deleteInfo_)
            throw std::runtime_error("Maze retained Thing APIs unavailable");
        sword_.pVTable=g_pCScriptThingVTable;
    }
    ~MazeSwordSlot() { CloseAfterQuiescence(); }
    MazeSwordSlot(const MazeSwordSlot&)=delete;
    MazeSwordSlot& operator=(const MazeSwordSlot&)=delete;

    void LookupAndSetInLimbo(const std::string& name,bool limbo,bool extra) {
        RequireOpen();
        if(!GetThingWithScriptName_ByName_API || !EntitySetInLimbo_API)
            throw std::runtime_error("Maze named Thing APIs unavailable");
        {
            FableString key(name.c_str());
            struct Output {
                CScriptThing value{};
                Output(){value.pVTable=g_pCScriptThingVTable;}
                ~Output(){RetailThing_Destroy_API(&value);}
            } output;
            GetThingWithScriptName_ByName_API(game_,&output.value,key);
            // EA81BC compares Info, not Data. Equal Info leaves both fields
            // unchanged. Different Info releases old before acquiring new.
            auto* incoming=output.value.pImp.Info;
            if(sword_.pImp.Info!=incoming) {
                auto* old=sword_.pImp.Info;
                if(old && --old->RefCount==0) {
                    old->DeleteFunc(old->Data);
                    // Native BFE9BC imports MSVCR71 operator delete. Existing
                    // Game_free points at BFEA14/free and is not this contract.
                    deleteInfo_(old);
                }
                sword_.pImp=output.value.pImp;
                if(incoming) ++incoming->RefCount;
            }
        } // EA81F4..EA8239: output Thing then key, before consuming parent slot.
        EntitySetInLimbo_API(game_,&sword_,limbo,extra);
    }

    void SetInLimboAndAlpha(bool limbo,bool limboExtra,float alpha,bool alphaExtra) {
        RequireOpen();
        if(!EntitySetInLimbo_API || !EntitySetAlpha_API)
            throw std::runtime_error("Maze retained Thing consumers unavailable");
        EntitySetInLimbo_API(game_,&sword_,limbo,limboExtra);
        EntitySetAlpha_API(game_,&sword_,alpha,alphaExtra);
    }

    // Caller must first quiesce native/Lua threads and macro callbacks, and
    // destroy the Maze flag map. This is an integration precondition, not a
    // claim that the current host already establishes it.
    void CloseAfterQuiescence() {
        if(!closed_) { closed_=true; RetailThing_Destroy_API(&sword_); }
    }
private:
    void RequireOpen() const {if(closed_)throw std::runtime_error("Maze retained Thing slot is closed");}
    CGameScriptInterfaceBase* game_;
    DeleteInfo deleteInfo_;
    CScriptThing sword_{};
    bool closed_=false;
};
