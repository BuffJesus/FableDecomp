#pragma once
// Unapplied proposal. Quest lifetime must own this object across entity frames.
#include "FableAPI.h"
#include <array>
#include <cstdint>
#include <memory>
#include <stdexcept>

class RetailVillagerSpeechLists {
    struct Vector { CCharString* begin=nullptr; CCharString* end=nullptr; CCharString* capacity=nullptr; };
    static_assert(sizeof(Vector)==12,"Retail speech vectors require x86");
    std::array<Vector,8> vectors{};
    bool closed=false;
    Vector& Get(const std::string& category,bool male) {
        if(closed)throw std::runtime_error("Villager speech lists are closed");
        unsigned index;
        if(category=="good")index=0;
        else if(category=="bad")index=1;
        else if(category=="both")index=2;
        else if(category=="none")index=3;
        else throw std::runtime_error("Unknown Villager speech category");
        return vectors[index+(male?0:4)];
    }
public:
    RetailVillagerSpeechLists()=default;
    RetailVillagerSpeechLists(const RetailVillagerSpeechLists&)=delete;
    RetailVillagerSpeechLists& operator=(const RetailVillagerSpeechLists&)=delete;
    ~RetailVillagerSpeechLists(){Close();}

    void Append(const std::string& category,bool male,const std::string& literal) {
        auto& vector=Get(category,male);
        if(!CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail speech string APIs unavailable");
        CCharString temporary{};
        CCharString_Construct_Literal(&temporary,literal.c_str(),-1);
        try {
            if(vector.end!=vector.capacity) {
                if(vector.end) {
                    using Copy=CCharString*(__thiscall*)(CCharString*,const CCharString*);
                    ASLR<Copy>(0x0099EC30)(vector.end,&temporary);
                }
                vector.end=reinterpret_cast<CCharString*>(reinterpret_cast<uintptr_t>(vector.end)+4);
            } else {
                // 433530's third stack argument is unused; count=1 and append=true.
                unsigned char allocator=0;
                using Insert=void(__thiscall*)(Vector*,CCharString*,const CCharString*,void*,unsigned,bool);
                ASLR<Insert>(0x00433530)(&vector,vector.end,&temporary,&allocator,1,true);
            }
        } catch(...) {
            try {CCharString_Destroy(&temporary);}catch(...) {}
            throw;
        }
        CCharString_Destroy(&temporary);
    }
    int Count(const std::string& category,bool male) {
        auto& vector=Get(category,male);
        return static_cast<int>((reinterpret_cast<uintptr_t>(vector.end)-reinterpret_cast<uintptr_t>(vector.begin))/4);
    }
    void CopySelectedTo(CCharString* destination,const std::string& category,bool male,int index) {
        auto& vector=Get(category,male);
        if(!destination || index<0 || index>=Count(category,male))
            throw std::runtime_error("Invalid Villager speech index");
        using Assign=CCharString*(__thiscall*)(CCharString*,const CCharString*);
        ASLR<Assign>(0x0099EFB0)(destination,vector.begin+index);
    }
    void Close() noexcept {
        if(closed)return;
        closed=true;
        // DBEFC0: descending vectors, ascending entries within each vector.
        for(auto vector=vectors.rbegin();vector!=vectors.rend();++vector) {
            for(auto item=vector->begin;item!=vector->end;++item)
                try {CCharString_Destroy(item);}catch(...) {}
            if(vector->begin) {
                using Free=void(__cdecl*)(void*);
                try {ASLR<Free>(0x00BFEA14)(vector->begin);}catch(...) {}
            }
            *vector=Vector{};
        }
    }
};

// One owner belongs to LuaQuestState, which entity hosts already share.
// Lua references may outlive that state; they then observe a closed list object.
class RetailVillagerSpeechListOwner {
    std::shared_ptr<RetailVillagerSpeechLists> lists;
public:
    RetailVillagerSpeechListOwner()=default;
    RetailVillagerSpeechListOwner(const RetailVillagerSpeechListOwner&)=delete;
    RetailVillagerSpeechListOwner& operator=(const RetailVillagerSpeechListOwner&)=delete;
    ~RetailVillagerSpeechListOwner(){if(lists)lists->Close();}
    std::shared_ptr<RetailVillagerSpeechLists> Get() {
        if(!lists)lists=std::make_shared<RetailVillagerSpeechLists>();
        return lists;
    }
};
