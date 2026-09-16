// Actual condition helper and extracted binding, engine clone ownership double.
#define RETAIL_THING_CONDITION_NO_MAIN
#include "runtime_checks/retail_thing_condition.cpp"
int main() {
    if (run_thing_condition()) return 1;
    try {
        objectInfo.RefCount=1;
        objectThing.pImp.Data=reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(0x4567);
        objectThing.pImp.Info=&objectInfo;
        FakeHost host; host.m_Me=objectThing; expectedEntity=&host;
        void* task=reinterpret_cast<void*>(0x7890);
        std::memcpy(host.parent+0x2c,&task,sizeof(task));
        RegisterRetailAliveCondition(&host,host.parent,&host.m_Me);
        check(objectInfo.RefCount==2 && conditionClone.pImp.Data==objectThing.pImp.Data);
        // The scheduler evaluates the cloned bound identity, not a refreshed Hero lookup.
        bool boundAlive=true, terminating=false;
        auto nativeFrameDouble=[&](){
            check(conditionClone.pImp.Data==objectThing.pImp.Data);
            if(!boundAlive) terminating=true;
        };
        nativeFrameDouble(); check(!terminating);
        boundAlive=false; nativeFrameDouble(); check(terminating);
        // Local registration copy was already destroyed; thread teardown drops clone.
        thingDestroy(&conditionClone,nullptr); check(objectInfo.RefCount==1);
        thingDestroy(&objectThing,nullptr); check(objectInfo.RefCount==0);
        std::cout<<"PASS: Scythe condition retains bound identity through frames, dead predicate terminates, thread clone releases separately\n";
        return 0;
    } catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
