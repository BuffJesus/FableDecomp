#include <stdio.h>
#include <string.h>
#include "fable_def_pointer.h"
class CCombatTypeDef;
typedef CDefPointer<const CCombatTypeDef> CombatDefPointer;
extern "C" CombatDefPointer& __fastcall AssignDefPointer(CombatDefPointer&, const CombatDefPointer&);

static CombatDefPointer* g_source;
static CombatDefPointer* g_destination;
static void* g_third;
static int g_mode;
static int g_deletes;
static void* g_deleted;
static bool g_observed;
static void __fastcall DeleteBoundary(void* object, void*)
{
    ++g_deletes; g_deleted=object;
    g_observed=(*reinterpret_cast<unsigned long*>(static_cast<char*>(object)+4)==0 && g_destination->Object==object);
    if(g_mode==1) g_source->Object=0;
    if(g_mode==2) g_source->Object=g_third;
    if(g_mode==3) g_destination->Object=g_third;
}

int main()
{
    void* slots[2]={0,reinterpret_cast<void*>(&DeleteBoundary)};
    const unsigned long counts[4]={0,1,2,0xffffffffUL};
    int cases=0;
    for(int ca=0;ca<4;++ca) for(int cb=0;cb<4;++cb) for(int cc=0;cc<4;++cc)
    for(int destinationIndex=0;destinationIndex<4;++destinationIndex)
    for(int sourceIndex=0;sourceIndex<4;++sourceIndex) for(int mode=0;mode<4;++mode)
    {
        union { unsigned char bytes[sizeof(CDefPointeeBase)]; double alignment; } objects[3];
        memset(objects,0x5a,sizeof(objects));
        void* pointers[4]={0,objects[0].bytes,objects[1].bytes,objects[2].bytes};
        unsigned long expected[3]={counts[ca],counts[cb],counts[cc]};
        for(int i=0;i<3;++i)
        {
            *reinterpret_cast<void***>(objects[i].bytes)=slots;
            *reinterpret_cast<unsigned long*>(objects[i].bytes+4)=expected[i];
        }
        CombatDefPointer destination,source;
        destination.Object=pointers[destinationIndex];source.Object=pointers[sourceIndex];
        g_source=&source;g_destination=&destination;g_third=pointers[3];g_mode=mode;g_deletes=0;g_deleted=0;g_observed=false;
        int expectedSource=sourceIndex, expectedDestination=destinationIndex, deletions=0;
        if(destinationIndex!=sourceIndex)
        {
            if(destinationIndex)
            {
                --expected[destinationIndex-1];
                if(expected[destinationIndex-1]==0)
                {
                    deletions=1;
                    if(mode==1)expectedSource=0;
                    if(mode==2)expectedSource=3;
                }
            }
            expectedDestination=expectedSource;
            if(expectedDestination)++expected[expectedDestination-1];
        }
        CombatDefPointer& result=AssignDefPointer(destination,source);
        if(&result!=&destination || destination.Object!=pointers[expectedDestination] || source.Object!=pointers[expectedSource])return 1;
        if(g_deletes!=deletions || (deletions && (!g_observed || g_deleted!=pointers[destinationIndex])))return 2;
        for(int i=0;i<3;++i)
        {
            if(*reinterpret_cast<unsigned long*>(objects[i].bytes+4)!=expected[i])return 3;
            for(unsigned int b=8;b<sizeof(CDefPointeeBase);++b)if(objects[i].bytes[b]!=0x5a)return 4;
        }
        int previousDeletes=g_deletes;
        CombatDefPointer& self=AssignDefPointer(destination,destination);
        if(&self!=&destination || destination.Object!=pointers[expectedDestination] || g_deletes!=previousDeletes)return 5;
        for(int i=0;i<3;++i)if(*reinterpret_cast<unsigned long*>(objects[i].bytes+4)!=expected[i])return 6;
        // Borrowed raw pointees are released only by the assignment being tested.
        destination.Object=0;source.Object=0;++cases;
    }
    printf("DEF_POINTER_ASSIGNMENT_BEHAVIOR_PASS cases=%d\n",cases);return 0;
}
