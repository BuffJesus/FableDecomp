// Typed disk-file ownership used by bank read-open. Retail Reset009D1EE0; callback009A9030.
#include "fable_counted_pointer.h"
#include "fable_disk_file.h"
template<> void CCountedPointer<CDiskFileWin32>::Reset(CDiskFileWin32* data)
{
 if(Info) {
  --Info->RefCount;
  if(!Info->RefCount) {
   Info->DeleteFunc(Info->Data);
   delete Info;
  }
 }
 Info=0;
 Data=data;
 if(Data)Info=new CCPPointerInfo(&DeleteData,Data);
}
