#include "retail_oakvale_timers.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
DWORD g_fableBase=0;
static CGameScriptInterfaceBase games[4]{};
static void* table[91]{};
static int ids[2]{},reads=0,registrations=0,closes=0,failRegister=0,failClose=0;
static std::vector<std::string> events;
static void check(bool value){if(!value)throw std::runtime_error("timer check failed");}
static CGameScriptInterfaceBase* current(){check(reads<4);return &games[reads++];}
static int __fastcall create(CGameScriptInterfaceBase* self,void*) {
    check(self==&games[reads-1]);int index=registrations++;events.push_back("register:"+std::to_string(index));
    if(registrations==failRegister)throw std::runtime_error("REGISTER");return ids[index];
}
static void __fastcall destroy(CGameScriptInterfaceBase* self,void*,int id) {
    check(self==&games[reads-1]);++closes;events.push_back("close:"+std::to_string(id));
    if(closes==failClose)throw std::runtime_error("CLOSE");
}
static void reset(){reads=registrations=closes=failRegister=failClose=0;events.clear();}
int main(){try{
    table[0x15c/4]=reinterpret_cast<void*>(&create);table[0x160/4]=reinterpret_cast<void*>(&destroy);
    for(auto& game:games)*reinterpret_cast<void***>(&game)=table;
    const int values[]={std::numeric_limits<int>::min(),-1,0,1,std::numeric_limits<int>::max()};
    int cases=0;
    for(int ambient:values)for(int watch:values)for(bool explicitClose:{false,true}) {
        reset();ids[0]=ambient;ids[1]=watch;
        {RetailOakvaleTimers owner(&current);check(owner.Ambient()==ambient && owner.Watch()==watch);
            if(explicitClose){owner.Close();owner.Close();bool rejected=false;try{owner.Watch();}catch(...){rejected=true;}check(rejected);}
        }
        check(reads==4 && registrations==2 && closes==2);
        check(events==std::vector<std::string>{"register:0","register:1","close:"+std::to_string(watch),"close:"+std::to_string(ambient)});++cases;
    }
    for(int registrationFailure:{1,2})for(int cleanupFailure:{0,1}){
        reset();ids[0]=-1;ids[1]=0;failRegister=registrationFailure;failClose=cleanupFailure;
        std::string error;try{RetailOakvaleTimers owner(&current);}catch(const std::exception& e){error=e.what();}
        check(error=="REGISTER" && registrations==registrationFailure && closes==registrationFailure-1);++cases;
    }
    for(int failure:{1,2}){
        reset();ids[0]=ids[1]=0;failClose=failure;
        {RetailOakvaleTimers owner(&current);std::string error;try{owner.Close();}catch(const std::exception& e){error=e.what();}
            check(error=="CLOSE" && closes==2);owner.Close();check(closes==2);
        }
        check(reads==4 && closes==2);++cases;
    }
    std::cout<<"PASS: "<<cases<<" timer ownership policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
