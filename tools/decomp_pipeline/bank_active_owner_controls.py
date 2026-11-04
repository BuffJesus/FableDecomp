"""Defects exercised by last-owner release while the native request is READING."""
import bank_begin_worker_controls as C
def controls(sources):
 controls=[row for row in C.controls(sources) if row[0] != 'bank-early-size-store']
 def add(name,file,old,new):
  i=next(i for i,p in enumerate(sources) if p.name==file);s=sources[i].read_text();assert s.count(old)==1,(name,s.count(old));controls.append((name,i,s.replace(old,new)))
 life='CBankFileAsyncData_Lifetime_009d5410.cpp'
 add('active-skip-cancellation',life,'if (ThreadedFileUID) {','if (false) {')
 add('active-no-unlock',life,'                lock.Release();','                ;')
 add('active-no-relock',life,'                lock.Lock();','                ;')
 add('active-single-attempt',life,'while (!BankFile.ThreadedFile->RemoveObject(ThreadedFileUID))','if (!BankFile.ThreadedFile->RemoveObject(ThreadedFileUID))')
 add('active-wrong-cancel-uid',life,'while (!BankFile.ThreadedFile->RemoveObject(ThreadedFileUID))','while (!BankFile.ThreadedFile->RemoveObject(ThreadedFileUID+1))')
 add('active-allow-reading-removal','CThreadedFileManager_RemoveObject_0098eb20.cpp','object->LoadingStatus==FILE_LOADING_STATUS_READING ||','false ||')
 add('active-callback-keeps-manager-lock','CThreadedFileManager_OnFinishObject_0098ebc0.cpp','        LeaveCriticalSection(section);','        ;')
 add('active-wrong-transfer-size','CThreadedFileManager_ProcessObjects_0098ecd0.cpp','ReadFileEx(object->PFile->fileHandle_,object->Desc.PDst,object->Desc.Size,object->Overlapped,OnFileReadCompletion);','ReadFileEx(object->PFile->fileHandle_,object->Desc.PDst,object->Desc.Size-1,object->Overlapped,OnFileReadCompletion);')
 assert len(controls)==30 and len({row[0] for row in controls})==30
 return controls
