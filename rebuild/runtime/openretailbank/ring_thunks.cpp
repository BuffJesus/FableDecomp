// ABI-transparent names used by the raw OpenRetailBank body.
//
// The anchor deliberately calls C-linkage names so its 44 call relocations
// stay stable when it is linked outside Fable.exe.  These alternates bind
// those names to the already reconstructed ring functions without adding a
// wrapper frame (important for thiscall/fastcall functions and hidden C++
// return buffers).

#pragma comment(linker, "/alternatename:_FableOpenRetailPathExists_009A8840=_FableHostPathExists_009A8840")
#pragma comment(linker, "/alternatename:_FableOpenRetailOperatorNew_009A8840=_FableHostOperatorNew_009A8840")
#pragma comment(linker, "/alternatename:_FableOpenRetailDiskBaseConstruct_009A8840=??0CBase@@QAE@XZ")
#pragma comment(linker, "/alternatename:_FableOpenRetailCharConstruct_009A8840=??0CWideString@@QAE@XZ")
#pragma comment(linker, "/alternatename:_FableOpenRetailDiskOwnerConstruct_009A8840=@FableHostDiskOwnerConstruct_009A8840@12")
#pragma comment(linker, "/alternatename:_FableOpenRetailFileStreamConstruct_009A8840=@CFileDataInputStream_CFileDataInputStream_00994700@16")
#pragma comment(linker, "/alternatename:_FableOpenRetailReadOverflow_009A8840=@FableHostReadOverflow_009A8840@16")
#pragma comment(linker, "/alternatename:_FableOpenRetailFileStreamDestroy_009A8840=@CFileDataInputStream_dtor_00994780@8")
#pragma comment(linker, "/alternatename:_FableOpenRetailDiskOwnerDestroy_009A8840=@Dest_val_CCountedPointer@8")
#pragma comment(linker, "/alternatename:_FableOpenRetailBankConstruct_009A8840=??0CRetailBank@NBankFileManager@@QAE@XZ")
#pragma comment(linker, "/alternatename:_FableOpenRetailBankOwnerConstruct_009A8840=@CCountedPtr_ctor@12")
#pragma comment(linker, "/alternatename:_FableOpenRetailBankListInsert_009A8840=?Buynode_CSpawnedFuncBase@@YIPAU_Node@@PAU_List_val@@HPBUCCountedPtr@@@Z")
#pragma comment(linker, "/alternatename:_FableOpenRetailWideToChar_009A8840=?ConvertToCharString@CWideString@@QBE?AVCCharString@@XZ")
#pragma comment(linker, "/alternatename:_FableOpenRetailCharAssign_009A8840=?CCharString_operator_assign@@YIPAUCCharString@@PAU1@PAX0@Z")
#pragma comment(linker, "/alternatename:_FableOpenRetailCharDestroy_009A8840=??1CCharString@@QAE@XZ")
#pragma comment(linker, "/alternatename:_FableOpenRetailDiskOwnerAssign_009A8840=@CCountedPtr_assign@12")
#pragma comment(linker, "/alternatename:_FableOpenRetailBufferAllocate_009A8840=?Buffer_AllocateAndZero@@YIPAUBufferOverlay@@PAU1@PAXI@Z")
#pragma comment(linker, "/alternatename:_FableOpenRetailSetPosition_009A8840=?SetPosition@CDataInputStream@@QAEXK@Z")
#pragma comment(linker, "/alternatename:_FableOpenRetailMemoryStreamConstruct_009A8840=@FableHostMemoryStreamConstruct@16")
#pragma comment(linker, "/alternatename:_FableOpenRetailReadNullString_009A8840=?ReadNullTerminatedString@CMemoryDataInputStream@@QAE?AUCCharString@@XZ")
#pragma comment(linker, "/alternatename:_FableOpenRetailContainedBankIndex_009A8840=??ACContainedBankMap@NBankFileManager@@QAEAAUCContainedBank@1@ABVCCharString@@@Z")
#pragma comment(linker, "/alternatename:_FableOpenRetailMemoryStreamDestroy_009A8840=??1CChunkedFileChunk@@QAE@XZ")
#pragma comment(linker, "/alternatename:_FableOpenRetailFree_009A8840=_free_00bfea14")
#pragma comment(linker, "/alternatename:_FableOpenRetailThreadedConstruct_009A8840=@CThreadedFile_ctor@4")
#pragma comment(linker, "/alternatename:_FableOpenRetailThreadedReset_009A8840=?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z")
#pragma comment(linker, "/alternatename:_FableOpenRetailThreadedOpen_009A8840=?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z")
#pragma comment(linker, "/alternatename:_FableOpenRetailOperatorDelete_009A8840=_operator_delete")
