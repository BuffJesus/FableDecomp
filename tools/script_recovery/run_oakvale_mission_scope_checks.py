"""Compare compiled owner scopes against original x86 instruction traces."""
import itertools
import json
from pathlib import Path
from tools.script_recovery.native_oakvale_mission_scopes import execute,SCOPES,prove
from tools.script_recovery.prepare_oakvale_mission_capabilities import prepare,OUTPUT
from tools.script_recovery.run_post_attack_scope_runtime_checks import run
from tools.script_recovery.lift_native_lua import ROOT


def check():
    out=ROOT/'work/oakvale_mission_scope_checks';out.mkdir(parents=True,exist_ok=True)
    rows=[]
    for kind,alias,empty,mutate in itertools.product(SCOPES,range(3),(False,True),(False,True)):
        events=execute(kind,alias,empty,mutate)
        trace='|'.join(':'.join(str(int(v)) if isinstance(v,bool) else str(v) for v in event) for event in events)
        rows.append('{'+','.join((json.dumps(kind),str(alias),str(empty).lower(),str(mutate).lower(),json.dumps(trace)))+'}')
    (out/'mission_native_cases.h').write_text('struct MissionCase {const char* kind;int alias;bool empty,mutate;const char* expected;};\nstatic const MissionCase missionCases[]={\n'+',\n'.join(rows)+'\n};\n')
    # Local generated source makes the quoted generated header include unambiguous.
    harness=out/'mission.cpp';harness.write_text(Path(__file__).with_name('oakvale_mission_scope_harness.cpp').read_text())
    (out/'native-evidence.json').write_text(json.dumps(prove(),indent=2)+'\n')
    return run(harness=harness,output_name='oakvale_mission_scope_checks',prepare_fn=prepare,stage=OUTPUT,
        native_scope='Original DoMission atomic caller instructions compared with real compiled owner and Lua',
        limits='Engine boundaries doubled. Native allocation/scheduler/full mission execution remain separate gates.')


if __name__=='__main__':print(check()['testOutput'])
