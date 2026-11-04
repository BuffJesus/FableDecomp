#include "fable_string.h"
CWideString::CWideString(const CCharString& text){
 ++g_CWideStringInstanceCount_013BCA20;
 long length=text.GetLength();
 storage_=new CWideStringData(length,L' ');
 for(long i=0;i<length;++i)storage_->text[i]=text.GetConstCharPtr()[i]&0xff;
}
