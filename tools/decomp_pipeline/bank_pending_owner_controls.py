"""Runtime defects exercised specifically by pre-read last-owner cancellation."""
import bank_begin_worker_controls as bank
import bank_shared_worker_controls as shared
def controls(sources):
 excluded={'bank-begin-false','bank-begin-no-publish','bank-begin-skip-buffer','bank-buffer-false','bank-missing-finished','bank-keep-uid','bank-wrong-restart'}
 result=[row for row in bank.controls(sources) if row[0] not in excluded]+shared.controls(sources)
 indices={p.name:i for i,p in enumerate(sources)}
 def add(name,file,old,new,expected=1,first=False):
  index=indices[file];text=sources[index].read_text();assert text.count(old)==expected,(name,text.count(old))
  result.append((name,index,text.replace(old,new,1 if first else -1)))
 add('pending-ignore-block','CThreadedFileManager_ProcessObjects_0098ecd0.cpp','if (BlockPendingObjectProcessingRefCount)','if (false)')
 add('pending-wrong-manager-uid','CThreadedFileManager_AddObject_0098f0f0.cpp','    return uid;','    return uid+1;')
 add('pending-no-uid-enrollment','CThreadedFileManager_AddObject_0098f0f0.cpp','    Objects[uid]=object;',';')
 add('pending-no-queue-enrollment','CThreadedFileManager_AddObject_0098f0f0.cpp','    AddObjectToPendingMap(object);',';')
 add('pending-wrong-request-desc','CThreadedFileManager_AddObject_0098f0f0.cpp','    object->Desc=desc;','    object->Desc=desc;++object->Desc.FilePos;')
 remove='CThreadedFileManager_RemoveObject_0098eb20.cpp'
 add('pending-skip-queue-erase',remove,'if (object->LoadingStatus==FILE_LOADING_STATUS_PENDING)','if (false)')
 add('pending-no-request-delete',remove,'delete object;',';')
 add('pending-no-uid-erase',remove,'Objects.erase(it);',';')
 add('pending-false-remove-result',remove,'return true;','return false;')
 file='CThreadedFile_RemoveObject_0098ec90.cpp';index=indices[file]
 result.append(('pending-false-file-remove',index,'#include "fable_threaded_file.h"\nbool CThreadedFile::RemoveObject(unsigned long) {return false;}\n'))
 life='CBankFileAsyncData_Lifetime_009d5410.cpp'
 add('pending-skip-owner-uid',life,'if (ThreadedFileUID) {','if (false) {')
 add('pending-wrong-owner-uid',life,'while (!BankFile.ThreadedFile->RemoveObject(ThreadedFileUID))','while (!BankFile.ThreadedFile->RemoveObject(ThreadedFileUID+1))')
 marker='CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection),false);'
 add('pending-unlock-bank-early',life,marker,marker+'lock.Release();',expected=2,first=True)
 index=indices['CThreadedFileManager_BeginThread_0098f380.cpp']
 result.append(('pending-no-worker-entry',index,'#include "fable_gfuninitialise.h"\nvoid __cdecl CThreadedFileManager::BeginThread(void*) {}\n'))
 for name in ('GetTotalObjectsPendingCount','GetTotalBytesPendingCount','GetTotalObjectsFinishedCount','GetTotalBytesFinishedCount'):
  source=next(p for p in sources if p.name.startswith('CThreadedFileManager_'+name+'_'));index=indices[source.name]
  value=1 if 'Finished' in name else 0
  result.append(('pending-bad-'+name,index,'#include "fable_threaded_file_object.h"\nlong CThreadedFileManager::'+name+'() const {return '+str(value)+';}\n'))
 assert len(result)==41 and len({row[0] for row in result})==41;return result
