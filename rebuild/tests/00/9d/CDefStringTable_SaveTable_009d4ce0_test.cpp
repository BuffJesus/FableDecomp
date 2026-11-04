#include "fable_def_string.h"
#include "fable_disk_file.h"
#include "fable_file_data_output_stream.h"
#include "test_support/base_class_stubs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <new>

class CFileInformation {};
static char events[32];static unsigned eventCount;
static CDefStringTable* table;static CAFile* disk;static bool desiredLock;
static void record(char c){events[eventCount++]=c;}
static void check(bool ok){if(!ok)abort();}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){if(this==disk)record('B');}
CAFile::CAFile(){disk=this;record('F');}
CFileInformation CAFile::GetFileInformation(){return CFileInformation();}
CWideString::CWideString():storage_(0){record('P');}
CWideString::CWideString(const CWideString& x):storage_(x.storage_){}
CWideString::~CWideString(){record('p');}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char*,long):storage_(0){}
CCharString::~CCharString(){if(this!=0){record('s');}}
void CDiskFileWin32::Open(const CWideString& path,FileAccessType mode,unsigned long flags){check(&path==&table->TableFilename&&mode==FILE_ACCESS_CREATE&&flags==4&&!IsOpen());Opened=true;record('O');}
void CDiskFileWin32::Close(){check(IsOpen());Opened=false;record('C');}
void CDiskFileWin32::Read(void*,long,CASuspendableProcess*){}
void CDiskFileWin32::Write(const void*,long,CASuspendableProcess*){}
void CDiskFileWin32::SetPosition(long){}
void CDiskFileWin32::MovePosition(long){}
long CDiskFileWin32::GetPosition()const{return 0;}
void CDiskFileWin32::SetLength(long){}
CAFile* CDiskFileWin32::Clone()const{return 0;}
CFileInformation CDiskFileWin32::GetFileInformation(){return CFileInformation();}
CFileDataOutputStream::CFileDataOutputStream(CAFile* f):File(f){check(f==disk);record('S');}
CFileDataOutputStream::~CFileDataOutputStream(){check(table->Locked==desiredLock);record('D');}
void CFileDataOutputStream::SetPosition(unsigned long){}
unsigned long CFileDataOutputStream::GetPosition()const{return 0;}
void CFileDataOutputStream::Close(){}
void CFileDataOutputStream::Write(const void*,long){}
void CDefStringTable::WriteTable(CDataOutputStream& s){check(!Locked&&static_cast<CFileDataOutputStream&>(s).File==disk);record('W');}
typedef _STL::pair<const CCharString,long> Entry;
typedef _STL::_Rb_tree_node<Entry> Node;
static bool run(bool locked,bool lock,unsigned nodes){
    unsigned char bytes[sizeof(CDefStringTable)];memset(bytes,0,sizeof(bytes));
    table=reinterpret_cast<CDefStringTable*>(bytes);table->Locked=locked;table->ReadOnly=true;
    _STL::_Rb_tree_node_base* header=(_STL::_Rb_tree_node_base*)calloc(1,sizeof(_STL::_Rb_tree_node_base));
    table->StringMap._M_t._M_header._M_data=static_cast<Node*>(header);
    header->_M_left=header;header->_M_right=header;
    Node* root=0;
    for(unsigned i=0;i<nodes;++i){Node* n=(Node*)calloc(1,sizeof(Node));new(const_cast<CCharString*>(&n->_M_value_field.first))CCharString;n->_M_left=root;root=n;}
    header->_M_parent=root;if(root){header->_M_left=root;header->_M_right=root;}
    table->StringMap._M_t._M_node_count=nodes;
    eventCount=0;desiredLock=locked||lock;table->SaveTable(lock);
    check(table->Locked==desiredLock&&table->ReadOnly);
    if(locked)check(eventCount==0);
    else{const char* prefix="FPOSW";check(eventCount>=10&&memcmp(events,prefix,5)==0);unsigned at=5;if(lock)for(unsigned i=0;i<nodes;++i)check(events[at++]=='s');check(eventCount==at+5&&memcmp(events+at,"DsCpB",5)==0);}
    if(!locked&&lock){check(table->StringMap._M_t.size()==0&&header->_M_parent==0&&header->_M_left==header&&header->_M_right==header);}
    else{check(table->StringMap._M_t.size()==nodes&&header->_M_parent==root);table->StringMap.clear();}
    free(header);return true;
}
int main(){run(true,false,0);run(true,true,2);run(false,false,2);run(false,true,0);run(false,true,3);puts("DEFSTRING_SAVE_TABLE_OK cases=5");return 0;}
