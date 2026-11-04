"""Source defects detected by independent bank-worker state and lifetime checks."""
def controls(sources):
 indices={p.name:i for i,p in enumerate(sources)}
 result=[]
 def add(name,file,old,new,expected=1,first=False):
  index=indices[file];text=sources[index].read_text()
  assert text.count(old)==expected,(name,text.count(old))
  result.append((name,index,text.replace(old,new,1 if first else -1)))
 begin='CBankFileAsyncEntry_BeginReadData_009d6060.cpp'
 setup='CBankFileAsyncEntry_SetupRead_009d5970.cpp'
 ctor='CBankFileAsyncData_CBankFileAsyncData_009d5df0.cpp'
 onbegin='CBankFileAsyncData_OnFileReadBegin_009d5440.cpp'
 complete='CBankFileAsyncData_OnFileReadCompletion_009d54a0.cpp'
 lifetime='CBankFileAsyncData_Lifetime_009d5410.cpp'
 add('bank-wrong-buffer',begin,'new CBankFileAsyncData(buffer, size, BankFile)','new CBankFileAsyncData(0, size, BankFile)')
 add('bank-wrong-length',begin,'new CBankFileAsyncData(buffer, size, BankFile)','new CBankFileAsyncData(buffer, size+1, BankFile)')
 add('bank-missing-info',begin,'Info(data ? new CCPPointerInfo(DeleteData, data) : 0)','Info(data && data->Length==0 ? new CCPPointerInfo(DeleteData, data) : 0)')
 add('bank-wrong-refcount',begin,'if (Info) ++Info->RefCount;','if (Info) Info->RefCount+=2;')
 add('bank-missing-delete-func',begin,'new CCPPointerInfo(DeleteData, data)','new CCPPointerInfo(data->Length ? 0 : DeleteData, data)')
 add('bank-skip-request',begin,'SetupRead(offset, *data, size, bufferSize, 0, priority);',';')
 add('bank-wrong-uid',setup,'data.ThreadedFileUID=data.BankFile.ThreadedFile->AddObject(desc);','data.ThreadedFileUID=1+data.BankFile.ThreadedFile->AddObject(desc);')
 add('bank-retain-request',setup,'desc.QueueWhenFinished=false;','desc.QueueWhenFinished=true;')
 add('bank-missing-callback',setup,'desc.PCallback=&data;','desc.PCallback=0;')
 add('bank-wrong-offset',setup,'desc.FilePos=BankFile.GetEntryDataOffset(Index)+offset;','desc.FilePos=BankFile.GetEntryDataOffset(Index)+offset+1;',expected=2)
 add('bank-begin-false',onbegin,'return true;','return false;')
 add('bank-begin-no-publish',onbegin,'*output = Buffer;',';')
 add('bank-begin-skip-buffer',onbegin,'SetupBuffer(false)','true')
 add('bank-buffer-false','CBankFileAsyncData_SetupBuffer_009d52d0.cpp','if(Buffer)return true;','if(Buffer)return false;')
 add('bank-missing-finished',complete,'Finished = true;','Finished = false;')
 add('bank-keep-uid',complete,'ThreadedFileUID = 0;',';')
 add('bank-wrong-restart',complete,'if (MemoryHandle != -1)','if (true)')
 add('bank-omit-unregister',lifetime,'BankFile.AsyncDataInstances.erase(this);',';',expected=2,first=True)
 add('bank-no-node-free','STLAsyncBankTree_erase_009d7430.cpp','free(node);',';')
 add('bank-wrong-node-value','STLAsyncBankTree_Minsert_009d7990.cpp','_Construct(&node->_M_value_field, value);','_Construct(&node->_M_value_field, static_cast<CBankFileAsyncData*>(0));')
 add('bank-early-size-store',ctor,'    CCriticalSectionLock lock(','    InternalAllocatedBufferSize=0;\n    CCriticalSectionLock lock(',expected=2,first=True)
 add('bank-no-registration',ctor,'BankFile.AsyncDataInstances.insert(this);',';',expected=2,first=True)
 add('bank-dtor-try-lock',lifetime,'CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection),false);','CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection),true);',expected=2,first=True)
 return result
