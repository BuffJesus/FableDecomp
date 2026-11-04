#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>
template<class T> struct Raw {
 double bytes[(sizeof(T)+7)/8];
 Raw(){memset(bytes,0,sizeof(bytes));}
 T& get(){return *reinterpret_cast<T*>(bytes);}
};
class CResourceTestAccess {
public:
 static void Insert(CResource& r,CResource* prev,CResource* next){r.ListInsert(prev,next);}
 static void Remove(CResource& r){r.ListRemove();}
};
int main(){Raw<CResource> ar,br,cr;CResource& a=ar.get();CResource& b=br.get();CResource& c=cr.get();
 a.ResourceSize=70;a.LastUsedFrame=22;b.NextResource=&c;c.PrevResource=&b;
 CResourceTestAccess::Insert(a,&b,&c);
 if(a.PrevResource!=&b||a.NextResource!=&c||b.NextResource!=&a||c.PrevResource!=&a||a.ResourceSize!=70||a.LastUsedFrame!=22)return 1;
 b.PrevResource=b.NextResource=&b;CResourceTestAccess::Insert(a,&b,&b);
 if(b.PrevResource!=&a||b.NextResource!=&a||a.PrevResource!=&b||a.NextResource!=&b)return 2;
 puts("RESOURCE_INSERT_PASS");return 0;}
