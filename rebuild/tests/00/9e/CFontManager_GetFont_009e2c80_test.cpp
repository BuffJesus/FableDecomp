#include <stdio.h>
#include "fable_system_manager.h"
#include "fable_symbol_map.h"
#include "test_support/base_class_stubs.h"
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
static unsigned long nameslot;static const CCharString& name=*(CCharString*)&nameslot;
static bool normalExists,streamExists,ok;static int normalChecks,streamChecks,lookups,kind;
static CSymbolMap* normal=(CSymbolMap*)0x1234;static CSymbolMap* streaming=(CSymbolMap*)0x5678;
template<class T>CCountedPointer<T>::CCountedPointer(const CCountedPointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;}
CFontManager::CFontManager(const CCharString&,const CCharString&){}
CFontManager::~CFontManager(){}
struct CFontManagerTestAccess {static void init(CFontManager& m){m.PSymbols.Data=normal;m.PStreamingSymbols.Data=streaming;}};
bool CSymbolMap::SymbolExists(const CCharString& s)const{ok=ok&&&s==&name;if(this==normal){++normalChecks;return normalExists;}ok=ok&&this==streaming;++streamChecks;return streamExists;}
long CSymbolMap::GetSymbolSLONG(const CCharString& s)const{ok=ok&&&s==&name;++lookups;return this==normal?37:81;}
CCountedPointer<CFontBank> CFontManager::GetStaticFont(unsigned long i){ok=ok&&i==37;kind=1;CCountedPointer<CFontBank> p;p.Data=(CFontBank*)0x1000;return p;}
CCountedPointer<CFontBank> CFontManager::GetStreamingFont(unsigned long i){ok=ok&&i==81;kind=2;CCountedPointer<CFontBank> p;p.Data=(CFontBank*)0x2000;return p;}
int main(){CFontManager m(name,name);CFontManagerTestAccess::init(m);for(int n=0;n<4;++n){normalExists=(n&1)!=0;streamExists=(n&2)!=0;ok=true;normalChecks=streamChecks=lookups=kind=0;CCountedPointer<CFontBank> result=m.GetFont(name);if(!ok||normalChecks!=1||streamChecks!=(normalExists?0:1)||lookups!=1||kind!=(!normalExists&&streamExists?2:1)||result.Data!=(CFontBank*)(kind*0x1000)||result.Info)return 1;}puts("FONT_GET_PASS");return 0;}
