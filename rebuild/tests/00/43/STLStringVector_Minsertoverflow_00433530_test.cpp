#include "test_support/string_vector_fixture.h"
static int run(int oldCount,int pos,int count,bool atEnd){
 const char* words[]={"alpha","beta","gamma","delta"};
 _STL::vector<CCharString> v;
 if(oldCount){v._M_start=(CCharString*)malloc(oldCount*sizeof(CCharString));v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+oldCount;
  for(int i=0;i<oldCount;++i)new(v._M_finish++)CCharString(words[i]);}
 CCharString fill("new");const CCharString& value=oldCount?v[oldCount-1]:fill;const char* expectedFill=oldCount?words[oldCount-1]:"new";
 CCharString* oldStart=v._M_start;int oldDeaths=deaths;
 _STL::__false_type tag;v._M_insert_overflow(v._M_start+pos,value,tag,count,atEnd);
 if(v.size()!=oldCount+count||v.capacity()!=oldCount+(oldCount>count?oldCount:count)||deaths-oldDeaths!=oldCount||live!=oldCount+count+1)return 1;
 if(oldCount && v._M_start==oldStart)return 2;
 for(int i=0;i<oldCount+count;++i){const char* expected=i<pos?words[i]:i<pos+count?expectedFill:words[i-count];if(strcmp(v[i].GetConstCharPtr(),expected))return 3;}
 return 0;
}
int main(){
 if(run(0,0,1,true)||live||dataLive)return 1;
 if(run(0,0,3,false)||live||dataLive)return 2;
 if(run(4,4,1,true)||live||dataLive)return 3;
 if(run(4,1,1,false)||live||dataLive)return 4;
 if(run(4,3,6,false)||live||dataLive)return 5;
 if(run(4,2,0,false)||live||dataLive)return 6;
 puts("STRING_OVERFLOW_PASS");return 0;
}