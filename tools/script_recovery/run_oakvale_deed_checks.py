"""Compile deed objective and live morality against the composed actual owner."""
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_oakvale_deed_capabilities import prepare,OUTPUT
from tools.script_recovery.run_post_attack_scope_runtime_checks import run


if __name__=='__main__':
    out=ROOT/'work/oakvale_deed_objective_checks';out.mkdir(parents=True,exist_ok=True)
    source=Path(__file__).with_name('oakvale_objective_runtime_harness.cpp').read_text()
    if source.count('TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01')!=1:raise ValueError('Objective harness literal changed')
    source=source.replace('TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01','TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02').replace('SetInitialOakvaleObjective','SetOakvaleDeedObjective')
    source=source.replace('static int objectiveStep=',
        'static void __fastcall unexpectedDeedMap(void*,void*){throw std::runtime_error("unexpected map");}\n'
        'decltype(StdMap_String_Destroy_API) StdMap_String_Destroy_API=reinterpret_cast<decltype(StdMap_String_Destroy_API)>(&unexpectedDeedMap);\nstatic int objectiveStep=',1)
    harness=out/'deed_objective_harness.cpp';harness.write_text(source)
    for filename,output in ((harness,'oakvale_deed_objective_checks'),('oakvale_deed_morality_harness.cpp','oakvale_deed_morality_checks')):
        result=run(harness=filename,output_name=output,prepare_fn=prepare,stage=OUTPUT,
            native_scope='Complete native deed helper execution and reviewed SCRIPT_DEF +D64 load',
            limits='Actual composed resource owner/FSE types/Lua with engine doubles. Finite, signed-zero and infinity morality checked; signaling-NaN/FPU exception parity not claimed. Full gameplay and scheduler remain pending.')
        print(result['testOutput'])
