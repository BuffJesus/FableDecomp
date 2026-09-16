"""Compile the owned position binding with actual x86 runtime types and vendor Lua."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars
from tools.script_recovery.prepare_woman_resource_extension import prepare


def run():
    proposal=ROOT/'work/woman_resource_integration'; metadata=prepare(output=proposal)
    output=ROOT/'work/woman_owned_position_checks';output.mkdir(parents=True,exist_ok=True)
    runtime=Path('D:/Code/ForgeFSE-retail-shadow');source=Path(__file__).parent
    vendor=runtime/'Vendor';lua=vendor/'lua'
    sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    inputs=[proposal/'LuaRetailResources.h',source/'owned_thing_position_harness.cpp',
            source/'runtime_checks/retail_resources_smoke.cpp',*sources,*(runtime/'FableScriptExtender').glob('*.h')]
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    report={'status':'running','inputs':{str(p):digest(p) for p in inputs},'commands':[]}
    def execute(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout)
        report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    execute('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,sources)])
    execute('position-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(proposal),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),
        '/I'+str(vendor),'/I'+str(lua),str(source/'owned_thing_position_harness.cpp'),
        *[str(output/(p.stem+'.obj')) for p in sources],'/Fe:owned-position.exe'])
    report['result']=execute('position-test',[str(output/'owned-position.exe')])
    if any(digest(Path(p))!=value for p,value in report['inputs'].items()):raise ValueError('Compiled inputs changed')
    report['status']='passed';(output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    metadata['ownedPositionValidation']=str(output/'result.json');metadata['status']='proposal-only-not-applied'
    (proposal/'proposal.json').write_text(json.dumps(metadata,indent=2)+'\n')
    return report


if __name__=='__main__':print(json.dumps(run(),indent=2))
