"""Reproduce unapplied LiveFather adapters against actual x86 FSE/Lua types."""
import hashlib,json,shutil,subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.live_father_init import generate as init
from tools.script_recovery.live_father_payment import recover as payment
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    source=Path(__file__).parent;output=ROOT/'work/live_father_converter/runtime_proposal';output.mkdir(parents=True,exist_ok=True);init();payment()
    for name in ('live_father_methods.inc','live_father_harness.cpp'):shutil.copyfile(source/name,output/name)
    header=(runtime/'FableScriptExtender/LuaRetailResources.h').read_text()
    methods=('InitializeLiveFatherActor','NewLiteralText','DestroyText','GiveRawHeroGold','LiveFatherHeroHasChocolate','SetActiveQuestObjective','DisplayRawGameInfo','AddLiveFatherGoodDeedCounter')
    edits=[('    void DestroyThing(unsigned id)','    #include "live_father_init.inc"\n    #include "live_father_methods.inc"\n    void DestroyThing(unsigned id)'),
           ('enum class Kind { Resource, ActorMap, Movie, Thing };','enum class Kind { Resource, ActorMap, Movie, Thing, Text };'),
           ('        CScriptThing thing{};','        CScriptThing thing{};\n        CCharString text{};'),
           ('        case Kind::Thing:','        case Kind::Text: CCharString_Destroy(&e.text); break;\n        case Kind::Thing:'),
           ('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;',
            '    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join('    type["'+m+'"] = &LuaRetailResources::'+m+';' for m in methods))]
    for old,new in edits:
        if header.count(old)!=1:raise ValueError('LiveFather proposal header anchor changed')
        header=header.replace(old,new)
    (output/'LuaRetailResources.h').write_text(header)
    env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);lua=runtime/'Vendor/lua';cs=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'unapplied; pending merged owner integration','commands':[],
        'limits':['Actual native argument/destruction paths are separately compared; C++ engine calls here are API doubles.',
                  'Init errors occur after the callee consumes its copied Thing; no arbitrary SEH/destructor contract is inferred.',
                  'Active-quest getter must successfully construct its output; failure before return requires a separately established output-construction guarantee.',
                  'Full actor host composition, state persistence and scheduler teardown remain pending.']}
    inputs=[*cs,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',output/'LuaRetailResources.h',output/'live_father_init.inc',output/'live_father_methods.inc',output/'live_father_harness.cpp']
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        p=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);(output/(name+'.log')).write_text(p.stdout);report['commands'].append({'name':name,'exitCode':p.returncode})
        if p.returncode:raise RuntimeError(p.stdout)
        return p.stdout.strip()
    run('lua-build',[cl,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,cs)])
    run('capability-build',[cl,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),str(output/'live_father_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in cs],'/Fe:live-father-capabilities.exe'])
    report['compiledTest']=run('capability-test',[str(output/'live-father-capabilities.exe')]);exe=(output/'live-father-capabilities.exe').read_bytes();pe=int.from_bytes(exe[0x3c:0x40],'little');assert int.from_bytes(exe[pe+4:pe+6],'little')==0x14c
    report['peMachine']='0x014c (x86)';report['executableSha256']=hashlib.sha256(exe).hexdigest();(output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report

if __name__=='__main__':print(prepare()['compiledTest'])
