"""Compile both objective variants using the established ownership/error harness."""
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_oakvale_progress_capabilities import prepare,OUTPUT
from tools.script_recovery.run_post_attack_scope_runtime_checks import run


def check():
    out=ROOT/'work/oakvale_progress_checks';out.mkdir(parents=True,exist_ok=True)
    source=Path(__file__).with_name('oakvale_objective_runtime_harness.cpp').read_text()
    source=source.replace('static int objectiveStep=',
        'static void __fastcall unexpectedProgressMap(void*,void*){throw std::runtime_error("unexpected map");}\n'
        'decltype(StdMap_String_Destroy_API) StdMap_String_Destroy_API=reinterpret_cast<decltype(StdMap_String_Destroy_API)>(&unexpectedProgressMap);\n'
        'static void* progressTable[1024]{},*progressOther[1024]{};\n'
        'static bool hasGold=false,changeTable=false;\nstatic int objectiveStep=',1)
    source=source.replace('"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01"',
        '(hasGold?"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03":"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06")')
    source=source.replace('static CCharString* __fastcall objectiveGet',
        'static void __fastcall objectiveSetChanged(CGameScriptInterfaceBase*,void*,const CCharString*,const CCharString*,const CCharString*,const CCharString*);\n'
        'static CCharString* __fastcall objectiveGet',1)
    source=source.replace('step("get");ownedName=out;',
        'step("get");ownedName=out;if(changeTable){*reinterpret_cast<void***>(&game)=progressOther;progressTable[0x4a0/4]=reinterpret_cast<void*>(&objectiveSetChanged);}',1)
    source=source.replace('static void __fastcall objectiveDestroy',
        'static void __fastcall objectiveSetChanged(CGameScriptInterfaceBase* self,void*,const CCharString* name,const CCharString* objective,const CCharString* r1,const CCharString* r2){check(changeTable);objectiveSet(self,nullptr,name,objective,r1,r2);}\n'
        'static void __fastcall objectiveDestroy',1)
    source=source.replace('SetInitialOakvaleObjective','SetOakvaleProgressObjective')
    source=source.replace('scope:SetOakvaleProgressObjective()','scope:SetOakvaleProgressObjective(hasGold)')
    source=source.replace('for(int alias:{0,1,2})',
        'for(bool gold:{false,true})for(bool changed:{false,true})for(int alias:{0,1,2})',1)
    source=source.replace('objectiveAlias=alias;',
        'hasGold=gold;changeTable=changed;lua["hasGold"]=gold;*reinterpret_cast<void***>(&game)=progressTable;'
        'progressTable[0xa3c/4]=reinterpret_cast<void*>(&objectiveGet);progressTable[0x4a0/4]=reinterpret_cast<void*>(&objectiveSet);'
        'objectiveAlias=alias;',1)
    harness=out/'progress.cpp';harness.write_text(source)
    return run(harness=harness,output_name='oakvale_progress_checks',prepare_fn=prepare,stage=OUTPUT,
        native_scope='Complete gold watcher and attack callers checked by test_oakvale_progress',
        limits='Actual composed owner/Lua, engine and CString boundaries doubled. Live scheduling/gameplay pending.')


if __name__=='__main__':print(check()['testOutput'])
