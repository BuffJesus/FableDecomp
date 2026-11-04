#include <stdio.h>
#include <string.h>
#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
typedef CDefinitionManager::CInstantiatedDefInfo Info;
static Info** g_alternate;
static Info* g_replacement;
static int g_mode,g_loads,g_deletes;
static unsigned long g_index;
static const CDefinitionManager* g_manager;
static void __fastcall DeleteBoundary(void*,void*) { ++g_deletes; }

void CDefinitionManager::LoadDef(unsigned long index) const
{
    ++g_loads;g_index=index;g_manager=this;
    _STL::vector<Info*>& table=const_cast<_STL::vector<Info*>&>(InstantiatedDefs);
    if(g_mode==1)table._M_start=g_alternate;
    if(g_mode==2)table._M_start[index]=g_replacement;
}

int main()
{
    const unsigned long counts[]={0,1,2,0x7fffffffUL,0xffffffffUL};
    void* slots[2]={0,reinterpret_cast<void*>(&DeleteBoundary)};int cases=0;
    for(int repeat=0;repeat<4;++repeat)for(int entry=0;entry<2;++entry)
    for(unsigned int index=0;index<4;++index)for(int selected=0;selected<3;++selected)
    for(int count=0;count<5;++count)for(int mode=0;mode<3;++mode)
    {
        union { unsigned char bytes[sizeof(CDefinitionManager)]; double alignment; } managerStore;
        union { unsigned char bytes[sizeof(Info)]; double alignment; } infoStore[3];
        union { unsigned char bytes[sizeof(CDefClassBase)]; double alignment; } defStore[2];
        memset(&managerStore,0xa5,sizeof(managerStore));memset(infoStore,0x5a,sizeof(infoStore));memset(defStore,0x7c,sizeof(defStore));
        void* pointers[3]={0,defStore[0].bytes,defStore[1].bytes};Info* info[3];
        for(int i=0;i<2;++i){*reinterpret_cast<void***>(defStore[i].bytes)=slots;*reinterpret_cast<unsigned long*>(defStore[i].bytes+4)=counts[count];}
        for(int i=0;i<3;++i){info[i]=reinterpret_cast<Info*>(infoStore[i].bytes);info[i]->PDef.Object=pointers[i];}
        Info* first[4];Info* second[4];
        for(int i=0;i<4;++i){first[i]=info[(selected+1)%3];second[i]=info[selected];}
        if(!mode)first[index]=info[selected];
        CDefinitionManager& manager=*reinterpret_cast<CDefinitionManager*>(managerStore.bytes);
        _STL::vector<Info*>& table=*reinterpret_cast<_STL::vector<Info*>*>(managerStore.bytes+0xa4);
        table._M_start=first;table._M_finish=first+4;table._M_end_of_storage._M_data=first+4;
        g_alternate=second;g_replacement=info[selected];g_mode=mode;g_loads=0;g_deletes=0;
        if(entry)
        {
            CDefPointer<CDefClassBase> result=manager.GetPDefFromGlobalIndex(index);
            if(result.Object!=pointers[selected])return 1;
            if(selected && *reinterpret_cast<unsigned long*>(defStore[selected-1].bytes+4)!=counts[count]+1UL)return 2;
        }
        else
        {
            CDefPointer<const CDefClassBase> result=static_cast<const CDefinitionManager&>(manager).GetPDefFromGlobalIndex(index);
            if(result.Object!=pointers[selected])return 3;
            if(selected && *reinterpret_cast<unsigned long*>(defStore[selected-1].bytes+4)!=counts[count]+1UL)return 4;
        }
        if(g_loads!=1 || g_index!=index || g_manager!=&manager)return 5;
        if(g_deletes!=(selected && counts[count]==0 ? 1 : 0))return 6;
        for(int i=0;i<2;++i)
        {
            if(*reinterpret_cast<unsigned long*>(defStore[i].bytes+4)!=counts[count])return 7;
            for(unsigned int b=8;b<sizeof(CDefClassBase);++b)if(defStore[i].bytes[b]!=0x7c)return 8;
        }
        ++cases;
    }
    printf("GLOBAL_DEF_LOOKUP_BEHAVIOR_PASS cases=%d\n",cases);return 0;
}
