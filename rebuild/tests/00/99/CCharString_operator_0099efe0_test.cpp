#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_string.h"
static int unassignCalls,basicCtorCalls;
void CCharString::UnassignString() {
    ++unassignCalls;
    ::operator delete(storage_);storage_=0;
}
template<> CBasicString<char>::CBasicString(const char* text)
    : PData(const_cast<char*>(text)),StringLength(strlen(text)),DataLength(strlen(text)),UseFastExtend(true)
{ ++basicCtorCalls; }
int main() {
    unsigned long storage=0;CCharString& value=*reinterpret_cast<CCharString*>(&storage);
    if(&value.operator=("")!=&value||storage||basicCtorCalls||unassignCalls)return 1;
    if(&value.operator=("abc")!=&value||!storage||basicCtorCalls!=1||unassignCalls)return 2;
    CCharStringData* data=reinterpret_cast<CCharStringData*>(storage);
    if(data->owners!=1||data->unknown04!=3||strcmp(data->text,"abc"))return 3;
    if(&value.operator=("")!=&value||storage||unassignCalls!=1||basicCtorCalls!=1)return 4;
    if(&value.operator=((const char*)0)!=&value||storage||unassignCalls!=1||basicCtorCalls!=1)return 5;
    puts("STRING_ASSIGN_0099efe0_PASS");return 0;
}