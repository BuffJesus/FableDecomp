#include <stdio.h>
struct Data { unsigned long Bytes; unsigned long Sizeof() const{return Bytes;} };
struct Info { unsigned char base[0x0c]; Data* DataObject; float Duration,NonLoopingDuration;
 unsigned long Sizeof() const; };
static unsigned long SizeofCounted(const Data* data){if(data)return data->Sizeof()+sizeof(data);return sizeof(data);}
unsigned long Info::Sizeof() const {return SizeofCounted(DataObject)+0x14;}
int main(){Info info;info.DataObject=0;if(info.Sizeof()!=sizeof(info.DataObject)+0x14)return 1;
 Data data;data.Bytes=40;info.DataObject=&data;if(info.Sizeof()!=40+0x18)return 2;
 printf("ANIMATION_INFO_SIZEOF PASS\n");return 0;}