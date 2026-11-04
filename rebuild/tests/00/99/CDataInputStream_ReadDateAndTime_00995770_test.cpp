#include "fable_data_input_stream.h"
#include "fable_date_and_time.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static unsigned constructed,overflows;
static CDataInputStream* input;
static unsigned char bytes[24];
CDateAndTime::CDateAndTime(long year,unsigned long month,unsigned long day,unsigned long hour,unsigned long minute,unsigned long second,unsigned long millisecond)
 : Year(year),Month(month),Day(day),Hour(hour),Minute(minute),Second(second),Millisecond(millisecond) {++constructed;}
void CDataInputStream::ReadWithSrcChunkOverflow(void* target,long size) {
    if(this!=input || size<=0 || StreamPos+size>19)abort();
    memcpy(target,bytes+StreamPos,size);StreamPos+=size;
    CurrentSourcePtr=bytes+StreamPos;SourceChunkBytesRemaining=0;++overflows;
}
int main() {
    memset(bytes,0xA5,sizeof(bytes));
    long year=-123;memcpy(bytes+7,&year,4);
    bytes[11]=0xFF;bytes[12]=0x80;bytes[13]=23;bytes[14]=59;
    unsigned short second=0xFEDC,ms=0xABCD;memcpy(bytes+15,&second,2);memcpy(bytes+17,&ms,2);
    for(unsigned available=0;available<=12;++available) {
        unsigned char raw[sizeof(CDataInputStream)];memset(raw,0xA5,sizeof(raw));input=reinterpret_cast<CDataInputStream*>(raw);
        input->StreamPos=7;input->StreamSize=19;input->CurrentSourcePtr=bytes+7;input->SourceChunkStreamPos=7;input->SourceChunkBytesRemaining=available;
        constructed=overflows=0;
        CDateAndTime result=input->ReadDateAndTime();
        if(constructed!=1 || result.Year!=-123 || result.Month!=255 || result.Day!=128 || result.Hour!=23 || result.Minute!=59 || result.Second!=0xFEDC || result.Millisecond!=0xABCD)return 1;
        if(input->StreamPos!=19 || input->StreamSize!=19 || input->SourceChunkStreamPos!=7 || input->CurrentSourcePtr!=bytes+19 || input->SourceChunkBytesRemaining!=0)return 2;
        if((available==12)!=(overflows==0) || *reinterpret_cast<unsigned long*>(raw)!=0xA5A5A5A5)return 3;
    }
    for(unsigned i=0;i<7;++i)if(bytes[i]!=0xA5)return 4;
    for(unsigned i=19;i<24;++i)if(bytes[i]!=0xA5)return 5;
    puts("READ_DATE_00995770_PASS");return 0;
}
