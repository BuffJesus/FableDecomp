"""Compile an unapplied runoff resource proposal against the actual host types."""
import hashlib,json,shutil,subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.bully_runoff_movie import evidence
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    source=Path(__file__).parent;output=ROOT/'work/bully_converter/runoff_proposal';output.mkdir(parents=True,exist_ok=True)
    for name in ('bully_runoff_methods.inc','bully_runoff_harness.cpp'):shutil.copyfile(source/name,output/name)
    header=(runtime/'FableScriptExtender/LuaRetailResources.h').read_text()
    methods=('TryAcquireThing','NewStringMap','SetString','DestroyStringMap','RunMacroWithStrings','ClearThingHasInformation','SetThingAsAlly','PlayAnimationWithNativeArgument5','NewConversation','AddConversationPerson','AddConversationLine','NewText','FormatBullyIntimidationText','DestroyText','AddConversationText','FaceRetainedThing','ReadBullyRandomModulus','ReadBullyProximityRange','IsDistanceBetweenThingsUnder','GiveBullyTeddyQuestion','AddBullyHealthBar')
    edits=[('    void DestroyThing(unsigned id)','    #include "bully_runoff_methods.inc"\n    void DestroyThing(unsigned id)'),
        ('enum class Kind { Resource, ActorMap, Movie, Thing };','enum class Kind { Resource, ActorMap, Movie, Thing, StringMap, Text };'),
        ('        CScriptThing thing{};','        CScriptThing thing{};\n        CCharString text{};'),
        ('        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;','        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;\n        case Kind::StringMap: StdMap_String_Destroy_API(e.map); Game_free(e.map); e.map=nullptr; break;\n        case Kind::Text: CCharString_Destroy(&e.text); break;'),
        ('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;','    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join('    type["'+m+'"] = &LuaRetailResources::'+m+';' for m in methods))]
    for old,new in edits:
        if header.count(old)!=1:raise ValueError('Runoff proposal header anchor changed')
        header=header.replace(old,new)
    header=header.replace('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;',
        '    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n    type["InitializeBullyActor"] = &LuaRetailResources::InitializeBullyActor;')
    (output/'LuaRetailResources.h').write_text(header)
    env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);lua=runtime/'Vendor/lua'
    cs=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'unapplied; pending integration','native':evidence(),'commands':[]}
    inputs=[*cs,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',output/'LuaRetailResources.h',output/'bully_runoff_methods.inc',output/'bully_runoff_harness.cpp']
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        p=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(p.stdout);report['commands'].append({'name':name,'exitCode':p.returncode})
        if p.returncode:raise RuntimeError(p.stdout)
        return p.stdout.strip()
    run('lua-build',[cl,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,cs)])
    run('capability-build',[cl,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),str(output/'bully_runoff_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in cs],'/Fe:bully-runoff.exe'])
    report['compiledTest']=run('capability-test',[str(output/'bully-runoff.exe')]);exe=(output/'bully-runoff.exe').read_bytes()
    pe=int.from_bytes(exe[0x3c:0x40],'little');assert int.from_bytes(exe[pe+4:pe+6],'little')==0x14c
    report['peMachine']='0x014c (x86)';report['executableSha256']=hashlib.sha256(exe).hexdigest()
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report

if __name__=='__main__':print(prepare()['compiledTest'])
