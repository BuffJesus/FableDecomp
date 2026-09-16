"""Merge only TeddyGirl's required staged owner capabilities; never apply them."""
import hashlib,json,shutil,subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.teddy_girl_prepare import prepare as specific_prepare
from tools.script_recovery.teddy_girl_candidate import generate
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

def method(path,name):
    source=path.read_text()
    if source.count(name+'(')!=1:raise ValueError('TeddyGirl shared method correspondence changed: '+name)
    start=source.index(name+'(');start=source.rfind('\n',0,start)+1;brace=source.index('{',start);depth=1;end=brace+1
    while depth:
        depth+=(source[end]=='{')-(source[end]=='}');end+=1
    return source[start:end]+'\n'

def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    specific=specific_prepare(runtime);source=Path(__file__).parent;base=ROOT/'work/teddy_girl_converter/runtime_proposal';output=ROOT/'work/teddy_girl_converter/owner_proposal';output.mkdir(parents=True,exist_ok=True)
    for name in ('teddy_girl_methods.inc','teddy_girl_distance_bytes.inc'):shutil.copyfile(base/name,output/name)
    (output/'teddy_girl_specific_harness.cpp').write_text((source/'teddy_girl_harness.cpp').read_text().replace('int main(){','int teddySpecificMain(){'))
    shutil.copyfile(source/'teddy_girl_owner_harness.cpp',output/'teddy_girl_owner_harness.cpp');shutil.copyfile(source/'bully_presented_methods.inc',output/'bully_presented_methods.inc')
    selected=[('retail_resource_actions.inc','IsHitByHeroExceptAbility'),('bully_runoff_methods.inc','NewConversation'),('bully_runoff_methods.inc','SetThingAsAlly'),('guard_methods.inc','AddRawConversationPerson')]
    (output/'teddy_girl_common_methods.inc').write_text('\n'.join(method(source/file,name) for file,name in selected))
    header=(base/'LuaRetailResources.h').read_text()
    names=['NewPresentedItemOutput','PollPresentedItem','PresentedItemMatches','DestroyPresentedItemOutput',*[name for file,name in selected]]
    edits=[('            table->GetScriptThing(expert, &e.thing);','            try { table->GetScriptThing(expert, &e.thing); }\n            catch (...) { Destroy(e); throw; } // Roll back the unreturned owned Thing before outer movie cleanup.'),
        ('    void DestroyThing(unsigned id)','    #include "bully_presented_methods.inc"\n    #include "teddy_girl_common_methods.inc"\n    void DestroyThing(unsigned id)'),
        ('enum class Kind { Resource, ActorMap, Movie, Thing };','enum class Kind { Resource, ActorMap, Movie, Thing, PresentedOutput };'),
        ('        CScriptThing thing{};','        CScriptThing thing{};\n        CCharString presented{};\n        CScriptThing* presentedActor=nullptr;'),
        ('        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;','        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;\n        case Kind::PresentedOutput: CCharString_Destroy(&e.presented); break;'),
        ('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;','    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join('    type["'+name+'"] = &LuaRetailResources::'+name+';' for name in names))]
    for old,new in edits:
        if header.count(old)!=1:raise ValueError('TeddyGirl merged owner anchor changed')
        header=header.replace(old,new)
    from tools.script_recovery.readable_lua import readable_source,wrap_local_declarations
    (output/'LuaRetailResources.h').write_text(header);candidate,proof=generate();readable,_=readable_source(candidate,inline_literals=True);readable,_=wrap_local_declarations(readable);(output/'candidate.lua').write_text(readable)
    for obj in base.glob('*.obj'):shutil.copyfile(obj,output/obj.name)
    env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);lua=runtime/'Vendor/lua';cs=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'unapplied merged TeddyGirl owner proposal','specificAdapterGate':specific['compiledTest'],'candidateSha256':proof['sourceSha256'],'readableSha256':hashlib.sha256(readable.encode()).hexdigest(),'commands':[],
        'sharedMethods':selected,'sharedSourceHashes':{file:hashlib.sha256((source/file).read_bytes()).hexdigest() for file,name in selected},
        'ownerCorrection':'NewThingFromResource rolls back an unreturned Thing if the native getter throws, before surrounding movie cleanup.',
        'limits':['Full readable candidate uses actual owner and adapters with engine doubles; native helper AddGoodDeed/BadDeed and scheduler remain explicit test boundaries.','Native destructors are assumed nonthrowing as required by existing noexcept Close; no engine teardown-quiescence claim.','Getter rollback tests inject a C++ error after output construction; they do not model arbitrary engine SEH or partially invalid native objects.']}
    inputs=[*(output.glob('*.inc')),*(output.glob('*.cpp')),output/'LuaRetailResources.h',output/'candidate.lua',source/'runtime_checks/retail_resources_smoke.cpp']
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,args):
        p=subprocess.run(args,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);(output/(name+'.log')).write_text(p.stdout);report['commands'].append({'name':name,'exitCode':p.returncode})
        if p.returncode:raise RuntimeError(p.stdout)
        return p.stdout.strip()
    run('owner-build',[cl,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),str(output/'teddy_girl_owner_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in cs],'/Fe:teddy-girl-owner.exe'])
    report['compiledTest']=run('owner-test',[str(output/'teddy-girl-owner.exe')]);exe=(output/'teddy-girl-owner.exe').read_bytes();pe=int.from_bytes(exe[0x3c:0x40],'little');assert int.from_bytes(exe[pe+4:pe+6],'little')==0x14c
    report['peMachine']='0x014c (x86)';report['executableSha256']=hashlib.sha256(exe).hexdigest();(output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report

if __name__=='__main__':print(prepare()['compiledTest'])
