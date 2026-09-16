"""Compile unapplied Guard resource capabilities against actual FSE types/Lua."""
import hashlib,json,shutil,subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.guard_init import prove
from tools.script_recovery.guard_approach import recover
from tools.script_recovery.guard_capabilities import prove as capabilities
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    source=Path(__file__).parent;output=ROOT/'work/guard_converter/runtime_proposal';output.mkdir(parents=True,exist_ok=True)
    for name in ('guard_methods.inc','guard_harness.cpp'):shutil.copyfile(source/name,output/name)
    header=(runtime/'FableScriptExtender/LuaRetailResources.h').read_text()
    methods=('InitializeGuardActor','ReadGuardAlertRange','ReadGuardLectureRange','FollowThing','GuardFaceHero',
             'AddRawConversationPerson','GuardAddHeroConversationLine','SetRawCutsceneBehaviour')
    edits=[('    void DestroyThing(unsigned id)','    #include "guard_methods.inc"\n    void DestroyThing(unsigned id)'),
           ('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;',
            '    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join('    type["'+m+'"] = &LuaRetailResources::'+m+';' for m in methods))]
    for old,new in edits:
        if header.count(old)!=1:raise ValueError('Guard proposal header anchor changed')
        header=header.replace(old,new)
    (output/'LuaRetailResources.h').write_text(header)
    env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);lua=runtime/'Vendor/lua'
    cs=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'unapplied; pending integration','nativeInit':prove(),'nativeApproach':recover()[1],'nativeCapabilities':capabilities(),'commands':[],
            'limits':['Standalone proposed capabilities, not an integrated DLL or gameplay test.',
                      'Callback-error Init tests inject faults after native argument consumption; no invented engine SEH ownership policy.',
                      'Entity/parent state persistence and scheduler teardown remain pending.']}
    inputs=[*cs,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',output/'LuaRetailResources.h',output/'guard_methods.inc',output/'guard_harness.cpp']
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        p=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(p.stdout);report['commands'].append({'name':name,'exitCode':p.returncode})
        if p.returncode:raise RuntimeError(p.stdout)
        return p.stdout.strip()
    run('lua-build',[cl,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,cs)])
    run('capability-build',[cl,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),str(output/'guard_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in cs],'/Fe:guard-capabilities.exe'])
    report['compiledTest']=run('capability-test',[str(output/'guard-capabilities.exe')]);exe=(output/'guard-capabilities.exe').read_bytes()
    pe=int.from_bytes(exe[0x3c:0x40],'little');assert int.from_bytes(exe[pe+4:pe+6],'little')==0x14c
    report['peMachine']='0x014c (x86)';report['executableSha256']=hashlib.sha256(exe).hexdigest()
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report

if __name__=='__main__':print(prepare()['compiledTest'])
