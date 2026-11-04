#include <new>
#include "fable_retail_banks.h"
extern "C" __declspec(align(8)) unsigned char managerStorage[sizeof(CBankFileManager)]={0};
#pragma comment(linker,"/alternatename:?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A=_managerStorage")
// Storage ownership belongs to this host; the actual ctor, registration map and dtor are native.
CBankFileManager& Manager(){return *reinterpret_cast<CBankFileManager*>(managerStorage);}
extern "C" void ManagerCreate(const CWideString& path,unsigned long flags){
 new (managerStorage) CBankFileManager;
 Manager().UseRetailBanks=(flags&4)!=0;
 Manager().Banks[CCharString("owned-native-bank")].DevBankFilePath=path;
}
extern "C" void ManagerDestroy(){Manager().~CBankFileManager();}

extern "C" void UnusedBankBoundary();
bool CBankFileManager::FindContainedBank(const CCharString&,NBankFileManager::CContainedBank&,CCountedPointer<NBankFileManager::CRetailBank>&){UnusedBankBoundary();return false;}

#include "fable_progress_display.h"
namespace NProgressDisplay { CCountedPointer<CAProgressDisplay> PProgressDisplay; }
