#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_disk_file.h"
extern CBankFileManager g_FableBankFileManager_013CA79C;
extern CWideString g_CAFileCurrentPath_013BCA10;
extern "C" void* _ReturnAddress();
#pragma intrinsic(_ReturnAddress)
extern "C" void ColdBoundary(){fprintf(stderr,"RETAIL_COLD_BOUNDARY return=%p\n",_ReturnAddress());exit(80);}
static void require(bool ok,int line){if(!ok){fprintf(stderr,"RETAIL_NATIVE_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
extern void InspectRetailToc(NBankFileManager::CRetailBank&,const NBankFileManager::CContainedBank&,const CCharString&);
static NBankFileManager::CRetailBank* selectedBank;
static unsigned Walk(NBankFileManager::CContainedBankMap::CNode* node,unsigned budget){
 if(!node)return 0;CHECK(budget>0);unsigned left=Walk(node->left_,budget-1);const NBankFileManager::CContainedBank& x=node->value_.second;
 printf("SUB\t%s\t%lu\t%lu\t%lu\t%lu\t%lu\n",(const char*)node->value_.first,x.dataVersion_,x.entryCount_,x.infoOffset_,x.infoSize_,x.alignment_);InspectRetailToc(*selectedBank,x,node->value_.first);
 return left+1+Walk(node->right_,budget-1);
}
int main(int argc,char**argv){
 CHECK(argc==2);
 {CCharString narrow(argv[1]);CWideString pathname=narrow.ToWideString();CBankFileManager& manager=g_FableBankFileManager_013CA79C;manager.UseRetailBanks=true;
 CHECK(manager.RetailBanks.node_ && manager.RetailBanks.node_->next_==manager.RetailBanks.node_);
 manager.OpenRetailBank(pathname,false);
 CBankFileManager::CRetailBankList::CNode* head=manager.RetailBanks.node_;CHECK(head->next_!=head && head->next_==head->prev_ && head->next_->next_==head && head->next_->prev_==head);
 CCountedPointer<NBankFileManager::CRetailBank>& owner=head->next_->value_;CHECK(owner.Data && owner.Info && owner.Info->RefCount==1);
 NBankFileManager::CRetailBank* bank=owner.Data;CHECK(bank->diskFile_.Data && bank->diskFile_.Info && bank->diskFile_.Info->RefCount==1 && bank->diskFile_.Data->IsOpen());
 CHECK(bank->threadedFile_.data_ && bank->threadedFile_.control_ && bank->threadedFile_.control_->RefCount==1 && bank->threadedFile_.data_->openFlag_ && !bank->threadedFile_.data_->openedForWrite_ && !bank->useDma_);
 CHECK(!strcmp((const char*)bank->filePath_,argv[1]));
 selectedBank=bank;unsigned count=Walk(bank->containedBanks_.head_->parent_,1000);CHECK(count==bank->containedBanks_.size_);printf("RETAIL\t%u\t%lu\n",count,bank->threadedFile_.data_->length_);
 manager.Close();manager.Close();CHECK(head->next_==head && head->prev_==head);
 }
 g_CAFileCurrentPath_013BCA10=L"";
 printf("COUNTERS\t%ld\t%ld\n",g_CCharStringInstanceCount_013BD800,g_CWideStringInstanceCount_013BCA20);
 CHECK(g_CCharStringInstanceCount_013BD800==0 && g_CWideStringInstanceCount_013BCA20==2);return 0;
}
#include "NativeFileInputStreamColdAliases.h"
#pragma comment(linker,"/alternatename:?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?Clone@CDiskFileWin32@@UBEPAVCAFile@@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?SetLength@CDiskFileWin32@@UAEXJ@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?MovePosition@CDiskFileWin32@@UAEXJ@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?Write@CDiskFileWin32@@UAEXPBXJPAVCASuspendableProcess@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?ValidatePathnameIsOnWriteableDrive@CAFile@@SIXABVCWideString@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?CreateDirectoryForFile@CAFile@@SIXABVCWideString@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GFGetLastWindowsError@@YI?AVCCharString@@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@PBDABV0@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetAbsoluteFilePath@CAFile@@SI?AVCWideString@@ABV2@0@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetSubString@CWideString@@QBE?AV1@KK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@ABV0@0@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@ABV0@PBD@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GFIntToCharString@@YI?AVCCharString@@J@Z=_ColdBoundary")
