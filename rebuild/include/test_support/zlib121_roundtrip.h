#pragma once
#include "test_support/zlib121_data.h"
static const char ztest_phrase[]="Fable native reconstruction: CRC, def, cache. ";
static int CheckOutput(const unsigned char* output,unsigned n){
 for(unsigned i=0;i<n;++i)if(output[i]!=(unsigned char)ztest_phrase[i%(sizeof(ztest_phrase)-1)])return 1;
 return 0;
}
static int Decode(z_stream* stream,const ZTestCase& test,bool chunked){
 unsigned char output[16384];memset(output,0xa5,sizeof(output));unsigned fed=0,given=0;
 int status=0;unsigned loops=0;
 while(status!=Z_STREAM_END){
  if(++loops>40000)return 1;
  if(!stream->avail_in && fed<test.size){unsigned n=chunked?7:test.size;n=n<test.size-fed?n:test.size-fed;
   stream->next_in=(Bytef*)test.input+fed;stream->avail_in=n;fed+=n;}
  if(!stream->avail_out && given<test.output){unsigned n=chunked?19:test.output;n=n<test.output-given?n:test.output-given;
   stream->next_out=output+given;stream->avail_out=n;given+=n;}
  status=inflate(stream,chunked?Z_NO_FLUSH:Z_FINISH);
  if(status<0)return 2;
 }
 if(stream->total_out!=test.output || stream->total_in!=test.size || CheckOutput(output,test.output))return 3;
 return 0;
}
int main(){
 ztest_allocs=ztest_frees=ztest_fail=0;
 for(int i=0;i<4;++i){
  z_stream s;memset(&s,0,sizeof(s));
  if(inflateInit2_(&s,ztest_cases[i].window,"1.2.1",sizeof(s)))return 1;
  if(Decode(&s,ztest_cases[i],false))return 2;
  if(inflateReset(&s) || s.total_in || s.total_out)return 3;
  s.avail_in=s.avail_out=0;
  if(Decode(&s,ztest_cases[i],true))return 4;
  if(inflateEnd(&s) || s.state)return 5;
 }
 z_stream s;memset(&s,0,sizeof(s));
 if(inflateInit_(0,"1.2.1",sizeof(s))!=Z_STREAM_ERROR || inflateReset(0)!=Z_STREAM_ERROR || inflateEnd(0)!=Z_STREAM_ERROR)return 6;
 if(inflateInit_(&s,"2.0",sizeof(s))!=Z_VERSION_ERROR || inflateInit_(&s,"1.2.1",sizeof(s)-1)!=Z_VERSION_ERROR)return 7;
 if(inflateInit2_(&s,7,"1.2.1",sizeof(s))!=Z_STREAM_ERROR || s.state)return 8;
 ztest_fail=1;if(inflateInit_(&s,"1.2.1",sizeof(s))!=Z_MEM_ERROR)return 9;ztest_fail=0;
 if(inflateInit_(&s,"1.2.1",sizeof(s)))return 10;
 if(Decode(&s,ztest_cases[2],false))return 14;
 if(inflateEnd(&s) || inflateInit_(&s,"1.2.1",sizeof(s)))return 15;
 unsigned char bad[sizeof(ztest_data2)];memcpy(bad,ztest_data2,sizeof(bad));bad[sizeof(bad)-1]^=1;
 unsigned char output[16384];s.next_in=bad;s.avail_in=sizeof(bad);s.next_out=output;s.avail_out=sizeof(output);
 if(inflate(&s,Z_FINISH)!=Z_DATA_ERROR)return 11;
 if(inflateEnd(&s) || ztest_allocs!=ztest_frees)return 12;
 if(adler32(0,0,0)!=1 || crc32(0,0,0)!=0 || crc32(0,(const Bytef*)"123456789",9)!=0xcbf43926UL || adler32(1,(const Bytef*)"Wikipedia",9)!=0x11e60398UL)return 13;
 puts("ZLIB_PASS");return 0;
}
