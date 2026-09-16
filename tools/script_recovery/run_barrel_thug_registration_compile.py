"""Compile all registrations in the composed BarrelThug proposal with actual FSE types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars
from tools.script_recovery.prepare_barrel_thug_resource_extension import prepare


def run():
    proposal=prepare();out=ROOT/'work/barrel_thug_resource_integration';runtime=Path('D:/Code/ForgeFSE-retail-shadow')
    unit=out/'registration-check.cpp'
    condition=Path(__file__).with_name('conscious_condition_registration.inc').read_text()
    if condition.strip() not in (out/'LuaManager.cpp').read_text():raise ValueError('Staged condition registration changed')
    unit.write_text('#include "LuaRetailResources.h"\n#include "LuaQuestState.h"\n#include "LuaEntityHost.h"\n#include "LuaRetailConsciousCondition.h"\nvoid CheckAllBarrelThugRegistrations(sol::state& lua, LuaEntityHost* pEntityHost) { RegisterRetailResources(lua); auto questState_type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor); questState_type["GetVillagerSpeechLists"]=&LuaQuestState::GetVillagerSpeechLists;\n'+condition+'\n}\n')
    paths=[unit,*out.glob('*.h'),*(runtime/'FableScriptExtender').glob('*.h')]
    hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    command=[compiler,'/nologo','/c','/EHsc','/std:c++17','/MD','/O2',
             '/I'+str(out),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),
             '/I'+str(runtime/'Vendor/lua'),str(unit),'/Fo:registration-check.obj']
    result=subprocess.run(command,cwd=out,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    (out/'registration-compile.log').write_text(result.stdout)
    if result.returncode:raise RuntimeError(result.stdout)
    for path,value in hashes.items():
        if hashlib.sha256(Path(path).read_bytes()).hexdigest()!=value:raise ValueError('Registration input changed')
    obj=(out/'registration-check.obj').read_bytes()
    if int.from_bytes(obj[:2],'little')!=0x14C:raise ValueError('Registration object must be x86')
    report=dict(status='passed',exitCode=result.returncode,machine=0x14C,inputs=hashes,
                objectSha256=hashlib.sha256(obj).hexdigest(),candidateSha256=proposal['candidateSha256'],
                limits='All composed registration templates compile; no link, full DLL or gameplay validation.')
    (out/'registration-result.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(json.dumps(run(),indent=2))
