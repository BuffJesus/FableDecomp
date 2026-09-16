"""Compile two unapplied Marker capabilities with actual FSE/Lua types."""
import hashlib,json,shutil,subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.manage_quest_core_markers import generate
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars
def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    source=Path(__file__).parent;output=ROOT/'work/manage_quest_core_markers/runtime_proposal';output.mkdir(parents=True,exist_ok=True);generate()
    for name in ('manage_quest_core_markers_methods.inc','manage_quest_core_markers_harness.cpp'):shutil.copyfile(source/name,output/name)
    header=(runtime/'FableScriptExtender/LuaRetailResources.h').read_text();methods=('AddCoreQuestMarker','RemoveCoreQuestMarker')
    for old,new in [('    void DestroyThing(unsigned id)','    #include "manage_quest_core_markers_methods.inc"\n    void DestroyThing(unsigned id)'),('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;','    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join('    type["'+m+'"] = &LuaRetailResources::'+m+';' for m in methods))]:
        if header.count(old)!=1:raise ValueError('Marker header anchor changed')
        header=header.replace(old,new)
    (output/'LuaRetailResources.h').write_text(header);env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);lua=runtime/'Vendor/lua';cs=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'unapplied; lifecycle recovered; host integration pending','commands':[],'limits':['Whole original helper and real counted destruction compared separately; engine APIs are doubles.','Engine registration/state/teardown/save-restore and pre-return lookup exceptions remain integration gates.']}
    inputs=[*cs,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',output/'LuaRetailResources.h',output/'manage_quest_core_markers_methods.inc',output/'manage_quest_core_markers_harness.cpp',output.parent/'CANDIDATE.lua'];report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        p=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);(output/(name+'.log')).write_text(p.stdout);report['commands'].append({'name':name,'exitCode':p.returncode})
        if p.returncode:raise RuntimeError(p.stdout)
        return p.stdout.strip()
    run('lua-build',[cl,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,cs)])
    run('capability-build',[cl,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),str(output/'manage_quest_core_markers_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in cs],'/Fe:manage-quest-core-markers-capabilities.exe'])
    report['compiledTest']=run('capability-test',[str(output/'manage-quest-core-markers-capabilities.exe')]);exe=(output/'manage-quest-core-markers-capabilities.exe').read_bytes();pe=int.from_bytes(exe[0x3c:0x40],'little');assert int.from_bytes(exe[pe+4:pe+6],'little')==0x14c;report['peMachine']='0x014c (x86)';report['executableSha256']=hashlib.sha256(exe).hexdigest();(output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report
if __name__=='__main__':print(prepare()['compiledTest'])
