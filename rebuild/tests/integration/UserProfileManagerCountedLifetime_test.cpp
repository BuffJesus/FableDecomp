#include "fable_user_profile.h"
#include "fable_boot.h"
#include <stdio.h>
#include <stdlib.h>
#include <new>
static unsigned long allocated,released,destroyed;
void* FABLE_CDECL FableOperatorNew(fable_u32 size){if(size!=12)exit(8);++allocated;return ::operator new(size);}
void FABLE_CDECL FableOperatorDelete(void* p){++released;::operator delete(p);}
class TestObject{public:virtual ~TestObject(){++destroyed;}};
static void require(bool yes){if(!yes)exit(2);}
int main(){for(unsigned long i=0;i<32;++i){CCountedUserProfileManager a={0,0},b={0,0};a=(CUserProfileManager*)0;require(!a.object&&!a.reference&&allocated==i*2);CUserProfileManager* first=(CUserProfileManager*)new TestObject;a=first;FableReferenceCount* shared=(FableReferenceCount*)a.reference;require(shared&&shared->owners==1&&shared->object==first);b.object=a.object;b.reference=a.reference;++shared->owners;CUserProfileManager* second=(CUserProfileManager*)new TestObject;a=second;require(shared->owners==1&&b.object==first&&b.reference==shared&&destroyed==i*2&&released==i*2);FableReferenceCount* replacement=(FableReferenceCount*)a.reference;require(replacement!=shared&&replacement->owners==1&&replacement->object==second);b=(CUserProfileManager*)0;require(!b.object&&!b.reference&&destroyed==i*2+1&&released==i*2+1);a=(CUserProfileManager*)0;require(!a.object&&!a.reference&&destroyed==i*2+2&&released==i*2+2&&allocated==i*2+2);}printf("USER_PROFILE_LIFETIME_OK scenarios=32 destroyed=%lu blocks_released=%lu\n",destroyed,released);return 0;}
