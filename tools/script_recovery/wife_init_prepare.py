"""Compile two unapplied WifeInit capabilities with actual FSE/Lua types."""
import hashlib,json,shutil,subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.wife_init_recovery import generate
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars
def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    source=Path(__file__).parent;output=ROOT/'work/wife_complete_converter/init_proposal';output.mkdir(parents=True,exist_ok=True);generate()
    for name in ('wife_init_methods.inc','wife_conversation_methods.inc','wife_init_harness.cpp'):shutil.copyfile(source/name,output/name)
    header=(runtime/'FableScriptExtender/LuaRetailResources.h').read_text();methods=('InitializeWifeActor','SetWifeDeedReactionsDisabled','AddWifeWhereHusbandConversation')
    for old,new in [('    void DestroyThing(unsigned id)','    #include "wife_init_methods.inc"\n    void DestroyThing(unsigned id)'),('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;','    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join('    type["'+m+'"] = &LuaRetailResources::'+m+';' for m in methods))]:
        if header.count(old)!=1:raise ValueError('WifeInit header anchor changed')
        header=header.replace(old,new)
    header=header.replace('#include "wife_init_methods.inc"','#include "wife_init_methods.inc"\n    #include "wife_conversation_methods.inc"')
    (output/'LuaRetailResources.h').write_text(header);env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);lua=runtime/'Vendor/lua';cs=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'unapplied; lifecycle recovered; host integration pending','commands':[],'limits':['Original Init/callee compared separately; C++ host uses engine API doubles.','Normal copy consumption and injected post-consumption error tested; arbitrary native exceptions before copy consumption remain outside contract.']}
    inputs=[*cs,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',output/'LuaRetailResources.h',output/'wife_init_methods.inc',output/'wife_conversation_methods.inc',output/'wife_init_harness.cpp',output.parent/'INIT.lua'];report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        p=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);(output/(name+'.log')).write_text(p.stdout);report['commands'].append({'name':name,'exitCode':p.returncode})
        if p.returncode:raise RuntimeError(p.stdout)
        return p.stdout.strip()
    run('lua-build',[cl,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,cs)])
    run('capability-build',[cl,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),str(output/'wife_init_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in cs],'/Fe:wife-init-capabilities.exe'])
    report['compiledTest']=run('capability-test',[str(output/'wife-init-capabilities.exe')]);exe=(output/'wife-init-capabilities.exe').read_bytes();pe=int.from_bytes(exe[0x3c:0x40],'little');assert int.from_bytes(exe[pe+4:pe+6],'little')==0x14c;report['peMachine']='0x014c (x86)';report['executableSha256']=hashlib.sha256(exe).hexdigest();(output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report
if __name__=='__main__':print(prepare()['compiledTest'])
