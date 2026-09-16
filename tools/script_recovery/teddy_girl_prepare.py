"""Compile staged TeddyGirl adapters against actual FSE types and real Lua."""
import hashlib,json,shutil,subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.teddy_girl_lifecycle import prove
from tools.script_recovery.teddy_girl_movement import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    source=Path(__file__).parent;output=ROOT/'work/teddy_girl_converter/runtime_proposal';output.mkdir(parents=True,exist_ok=True)
    for name in ('teddy_girl_methods.inc','teddy_girl_harness.cpp'):shutil.copyfile(source/name,output/name)
    header=(runtime/'FableScriptExtender/LuaRetailResources.h').read_text()
    methods=('NewMovie','StartOwnedMovie','InitializeTeddyGirlActor','TakeTeddyFromHero','ClearRawInformation','GiveTeddyGirlQuestion','TeddyGirlAddHeroLine','IsActorPositionOnScreen','TeddyGirlMoveToThing','IsDistanceUnderThing','IsDistanceOver','IsTalkedToByHero','TeddyGirlTalkedWithTeddy','RemoveRawThing')
    for old,new in [('    void DestroyThing(unsigned id)','    #include "teddy_girl_methods.inc"\n    void DestroyThing(unsigned id)'),('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;','    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join('    type["'+name+'"] = &LuaRetailResources::'+name+';' for name in methods))]:
        if header.count(old)!=1:raise ValueError('TeddyGirl header insertion correspondence changed')
        header=header.replace(old,new)
    (output/'LuaRetailResources.h').write_text(header)
    from tools.script_recovery.lift_native_lua import RData
    native=RData();recover(native)
    (output/'teddy_girl_distance_bytes.inc').write_text('\n'.join('static const unsigned char '+name+'[]={'+','.join(hex(byte) for byte in native.bytes_at(address,114))+'};' for name,address in (('underBytes',0xcbe2ff),('overBytes',0xcbe3ea))))
    env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);lua=runtime/'Vendor/lua'
    cs=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'unapplied; pending merged capability integration','nativeLifecycle':prove(),'nativeMovement':recover()[1],'commands':[],
        'limits':['Actual FSE type and Lua binding tests use engine doubles; no gameplay or engine teardown claim.','Common resource/presented/talk/distance/movie capabilities must be merged separately.']}
    inputs=[*cs,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',output/'LuaRetailResources.h',output/'teddy_girl_methods.inc',output/'teddy_girl_harness.cpp',output/'teddy_girl_distance_bytes.inc']
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        p=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(p.stdout);report['commands'].append({'name':name,'exitCode':p.returncode})
        if p.returncode:raise RuntimeError(p.stdout)
        return p.stdout.strip()
    run('lua-build',[cl,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,cs)])
    run('capability-build',[cl,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),str(output/'teddy_girl_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in cs],'/Fe:teddy-girl-capabilities.exe'])
    report['compiledTest']=run('capability-test',[str(output/'teddy-girl-capabilities.exe')]);exe=(output/'teddy-girl-capabilities.exe').read_bytes()
    pe=int.from_bytes(exe[0x3c:0x40],'little');assert int.from_bytes(exe[pe+4:pe+6],'little')==0x14c
    report['peMachine']='0x014c (x86)';report['executableSha256']=hashlib.sha256(exe).hexdigest()
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report

if __name__=='__main__':print(prepare()['compiledTest'])
