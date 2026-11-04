#include "fable_def_string.h"
#include <stdio.h>
#include <string.h>
static const char bytes[]="CRC!FrontEnd\0CRC!Other\0";
static const CMemoryDataOutputStream* expected;
static int peeks,empty,bad;static const char* text;
const void* CMemoryDataOutputStream::PeekData() const{++peeks;if(this!=expected)bad=1;return (void*)bytes;}
CCharString::CCharString(){++empty;text=0;}
CCharString::CCharString(const char* p,long n){text=p;if(n!=-1)bad=2;}
CCharString::~CCharString(){}
int main(){double raw[sizeof(CDefStringTable)/8];const CDefStringTable* table=reinterpret_cast<CDefStringTable*>(raw);expected=&table->StringStream;
 {CCharString s=table->GetString(-1);if(empty!=1 || peeks || text)return 1;}
 {CCharString s=table->GetString(0);if(text!=bytes+4 || strcmp(text,"FrontEnd"))return 2;}
 {CCharString s=table->GetString(13);if(text!=bytes+17 || strcmp(text,"Other"))return 3;}
 if(peeks!=2 || empty!=1 || bad)return 4;puts("DEFSTRING_GET_PASS");return 0;}