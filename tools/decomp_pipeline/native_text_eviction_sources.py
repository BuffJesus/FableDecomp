"""Native decoded text/group resource eviction providers."""
from native_text_public_getter_sources import SOURCES as BASE
SOURCES=BASE+[('00/9f/CResourceList_FrameEndStandalone_009fc150.cpp',[])]
ADDRESS={'?UnloadNextResource@CResourceList@@QAE_NK@Z':0x9fc170,'?ShrinkResources@CResourceList@@QAEXK@Z':0x9fc210}

SOURCES.append(("00/9d/STLAsyncDisposalList_push_back_009d7b00.cpp",[]))

SOURCES.extend((p,[]) for p in ["00/9f/CResourceBank_AddToLoadingQueue_009fc3d0.cpp","00/9f/CResourceBank_ProcessLoadingQueue_009fc410.cpp"])

ADDRESS.update({"?GFGetTime@@YINXZ":0x9e1bc0,"?ResourceAbortingEnabled@CResourceBank@@0_NA":0x1396c5c,"?ProcessLoadingQueue@CResourceList@@QAEX_NN@Z":0x9fc250})

# Native movable-pool allocation and cleanup closure.
SOURCES.extend((p,[]) for p in ['00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp', '00/a0/CHandleBasedMemoryPool_CanMoveResource_00a03a40.cpp', '00/a0/CHandleBasedMemoryPool_CombineAreas_00a01e40.cpp', '00/a0/CHandleBasedMemoryPool_Free_00a02b30.cpp', '00/a0/CHandleBasedMemoryPool_GetPointerFromHandle_00a026b0.cpp', '00/a0/CHandleBasedMemoryPool_InsertFreeArea_00a01da0.cpp', '00/a0/CHandleBasedMemoryPool_InternalSetResource_00a028b0.cpp', '00/a0/CHandleBasedMemoryPool_Lock_00a026f0.cpp', '00/a0/CHandleBasedMemoryPool_MoveResource_00a02120.cpp', '00/a0/CHandleBasedMemoryPool_Unlock_00a02710.cpp', '00/a0/CHandleBasedMemoryPool_ValidateResource_00a03a50.cpp', '00/a6/CMovableResourceMemoryPool_AllocateResource_00a62e40.cpp', '00/a6/CMovableResourceMemoryPool_CanMoveResource_00a62f70.cpp', '00/a6/CMovableResourceMemoryPool_Construct_00a62e20.cpp', '00/a6/CMovableResourceMemoryPool_FreeResource_00a62ed0.cpp', '00/a6/CMovableResourceMemoryPool_MoveResource_00a62f30.cpp', '00/a6/CMovableResourceMemoryPool_ValidateResource_00a62ec0.cpp', '00/b3/CBankFileAsync_SetLoadingMemoryPool_00b3e820.cpp'])

# Retain a single provider when the pool closure overlaps existing loading support.
_UNIQUE={}
for _path,_flags in SOURCES:
 if _path in _UNIQUE:assert _UNIQUE[_path]==_flags
 _UNIQUE[_path]=_flags
SOURCES=list(_UNIQUE.items())
del _UNIQUE

SOURCES.extend((p,[]) for p in ["00/9d/CLoadingMemoryFailureHandler_Handler_009d5eb0.cpp","00/9d/CLoadingMemoryFailureHandler_RegisterBank_009d6420.cpp","00/9d/CLoadingMemoryFailureHandler_UnregisterBank_009d5a30.cpp","00/9d/CBankFileAsync_SetLoadingMemoryFailureHandler_009d66a0.cpp"])

ADDRESS.update({"??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z":0xa62920,"??1CCriticalSectionLock@@QAE@XZ":0xa629a0,"?HandleFailedAllocation@CBankFileAsync@@QAE_NXZ":0x9d5bf0})

# Forward actual ReadFileEx through a test-only post-submission barrier.
SOURCES=[(p,f+["/DReadFileEx=FixtureReadFileEx"] if p=="00/98/CThreadedFileManager_ProcessObjects_0098ecd0.cpp" else f) for p,f in SOURCES]
SOURCES.extend((p,[]) for p in ["00/9d/CBankFileAsync_DiscardAsyncData_009d67e0.cpp","00/9d/CBankFileAsyncData_DiscardAsyncData_009d68a0.cpp"])

ADDRESS.update({"?InsertFreeArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@@Z":0xa01da0,"__imp__EnterCriticalSection@4":0x143fe0c,"__imp__LeaveCriticalSection@4":0x143fe10,"_memmove":0xbfeae6})

from gate_native_pool_allocation_policy import ADDRESS as _POOL_POLICY_ADDRESS
for _symbol,_address in _POOL_POLICY_ADDRESS.items():
 if _symbol in ADDRESS:assert ADDRESS[_symbol]==_address
 ADDRESS[_symbol]=_address

from gate_native_bank_async_data_buffer import ADDRESS as _BUFFER_ADDRESS
for _symbol,_address in _BUFFER_ADDRESS.items():
 if _symbol in ADDRESS:assert ADDRESS[_symbol]==_address
 ADDRESS[_symbol]=_address
ADDRESS.update({"?g_FableAllocatorRangeStart_013CAAA4@@3KA":0x13caaa4,"?g_FableAllocatorRangeEnd_013CAAA8@@3KA":0x13caaa8,"?InitDebugInfo@CMemoryAllocator@@IAEXPAX_NJ@Z":0x9f28d0})

from gate_native_bank_async_data_teardown import ADDRESS as _TEARDOWN_ADDRESS
for _symbol,_address in _TEARDOWN_ADDRESS.items():
 if _symbol in ADDRESS:assert ADDRESS[_symbol]==_address
 ADDRESS[_symbol]=_address

# The retail-shaped TU emits the same standalone frame helper andbank coordinator.
SOURCES=[("00/9f/CResourceList_FrameEnd_009fc150.cpp",f) if p=="00/9f/CResourceList_FrameEndStandalone_009fc150.cpp" else (p,f) for p,f in SOURCES]

ADDRESS["?DiscardAsyncData@CBankFileAsync@@QAEXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z"]=0x9d67e0

SOURCES.extend((p,[]) for p in ["00/9c/NGameText_CDataBank_GetEnumerationType_009c79e0.cpp","00/9c/NGameText_CDataBank_GetLastModifiedTime_009cb710.cpp","00/9e/CDateAndTime_Constructor_009e9a70.cpp"] if p not in dict(SOURCES))
ADDRESS.update({"??0CCharString@@QAE@PBDJ@Z":0x99ebf0,"??_C@_09KANGMNGK@EGameText?$AA@":0x129b464,"??_C@_0L@HNPBNKBJ@ETextGroup?$AA@":0x129b458,"??_C@_00CNPNBAHC@?$AA@":0x122d70e,"??0CDateAndTime@@QAE@JKKKKKK@Z":0x9e9a70})

SOURCES.extend((p,[]) for p in ["00/99/CDiskFileWin32_GetFileInformation_0099a5a0.cpp","00/9e/CDateAndTime_Initialise_009e9cb0.cpp","00/9e/CDateAndTime_CDateAndTime_009e9ab0.cpp"] if p not in dict(SOURCES))
ADDRESS.update({"??0CDateAndTime@@QAE@XZ":0x9e9ab0,"?Initialise@CDateAndTime@@QAEXABU_FILETIME@@@Z":0x9e9cb0,"__imp__GetFileInformationByHandle@8":0x143fdac,"__imp__FileTimeToSystemTime@8":0x143fd50})

SOURCES.append(("00/99/CDiskFileWin32_Clone_0099ae00.cpp",[]))
ADDRESS.update({"??0CAFile@@QAE@XZ":0x99a2f0,"??_7CDiskFileWin32@@6B@":0x122d06c,"??0CWideString@@QAE@XZ":0x99aed0,"?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z":0x99a6a0})

SOURCES.append(("00/99/CDiskFileWin32_MovePosition_0099ac90.cpp",[]))
ADDRESS.update({"__imp__SetFilePointer@16":0x143fde8,"__imp__FlushFileBuffers@4":0x143fda8,"??_C@_0CF@ENJLLKFE@Cannot?5move?5before?5beginning?5of?5@":0x129a8b8,"??_C@_0BN@BOIGJIHO@Cannot?5seek?5past?5end?5of?5file?$AA@":0x129a898,"??0CCharString@@QAE@ABV0@@Z":0x99ec30,"??BCCharString@@QBEPBDXZ":0x99e4c0,"??1CCharString@@QAE@XZ":0x99eae0,"__TI2?AVCBBBFileException@@":0x1369380,"__TI1?AVCBBBFileException@@":0x1369380,"__CxxThrowException@8":0xbfeb84})

SOURCES.append(("00/98/CThreadedFile_SetObjectPriority_0098f0e0.cpp",[]))
ADDRESS.update({"?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A":0x13bc870,"?SetObjectPriority@CThreadedFileManager@@QAEXKW4EThreadedFilePriority@@@Z":0x98eff0})

SOURCES.append(("00/98/CThreadedFileManager_SetDeviceTimeSlice_0098e170.cpp",[]))

SOURCES.extend((p,[]) for p in ["00/98/CThreadedFile_ResizeFile_0098e090.cpp","00/98/CThreadedFile_FinishAndGetFileHandle_0098ec70.cpp"])
from threaded_file_open_write_oracle import ADDRESS as _WRITE_ADDRESS
for _symbol,_address in _WRITE_ADDRESS.items():
 if _symbol in ADDRESS:assert ADDRESS[_symbol]==_address
 ADDRESS[_symbol]=_address
ADDRESS["?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z"]=0x98e7d0

SOURCES=[(p,f+["/DWriteFileEx=FixtureWriteFileEx"] if p=="00/98/CThreadedFileManager_ProcessObjects_0098ecd0.cpp" else f) for p,f in SOURCES]
SOURCES.extend((p,[]) for p in ["00/98/CThreadedFileManager_Queries_0098e590.cpp","00/99/STLThreadedUIDTree_ConstFind_00990320.cpp"])

from threaded_file_resize_oracle import ADDRESS as _RESIZE_ADDRESS
for _symbol,_address in _RESIZE_ADDRESS.items():
 if _symbol in ADDRESS:assert ADDRESS[_symbol]==_address
 ADDRESS[_symbol]=_address

from threaded_query_oracle import ADDRESS as _QUERY_ADDRESS
for _symbol,_address in _QUERY_ADDRESS.items():
 if _symbol in ADDRESS:assert ADDRESS[_symbol]==_address
 ADDRESS[_symbol]=_address

SOURCES.extend(("00/98/CThreadedFileManager_%s_%08x.cpp"%(method,address),[]) for method,address in [("GetTotalObjectsPendingCount",0x98e390),("GetTotalBytesPendingCount",0x98e410),("GetTotalObjectsFinishedCount",0x98e490),("GetTotalBytesFinishedCount",0x98e510)])

from threaded_read_completion_oracle import ADDRESS as _COMPLETION_ADDRESS
for _symbol,_address in _COMPLETION_ADDRESS.items():
 if _symbol in ADDRESS:assert ADDRESS[_symbol]==_address
 ADDRESS[_symbol]=_address

assert ADDRESS.get("__imp__CloseHandle@4",0x143fe30)==0x143fe30
ADDRESS["__imp__CloseHandle@4"]=0x143fe30

# Public native error policy is exercised by real EOF callback/self-clear cases.
SOURCES.append(("00/40/CThreadedFileManager_ErrorCallbacks_00401460.cpp",[]))
from threaded_error_policy_oracle import ADDRESS as _POLICY_ADDRESS
for _symbol,_address in _POLICY_ADDRESS.items():
 if _symbol in ADDRESS:assert ADDRESS[_symbol]==_address
 ADDRESS[_symbol]=_address
