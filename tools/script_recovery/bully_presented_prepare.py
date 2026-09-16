"""Stage/compile the final Rock targeting adapter against actual FSE types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    phase,evidence=recover()
    source=Path(__file__).parent;output=ROOT/'work/bully_converter/presented_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('bully_presented_methods.inc','bully_presented_harness.cpp')
    for name in names:shutil.copyfile(source/name,output/name)
    (output/'bully_presented.lua').write_text(phase)
    original=(runtime/'FableScriptExtender/LuaRetailResources.h').read_text()
    header=original
    edits=[('    void DestroyThing(unsigned id)', '    #include "bully_presented_methods.inc"\n    void DestroyThing(unsigned id)'),
        ('enum class Kind { Resource, ActorMap, Movie, Thing };','enum class Kind { Resource, ActorMap, Movie, Thing, PresentedOutput };'),
        ('        CScriptThing thing{};','        CScriptThing thing{};\n        CCharString presented{};\n        CScriptThing* presentedActor=nullptr;'),
        ('        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;', '        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;\n        case Kind::PresentedOutput: CCharString_Destroy(&e.presented); break;'),
        ('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;', '\n'.join('    type["'+name+'"] = &LuaRetailResources::'+name+';' for name in ('NewPresentedItemOutput','PollPresentedItem','PresentedItemMatches','DestroyPresentedItemOutput','DestroyThing')))]
    for old,new in edits:
        if header.count(old)!=1:raise ValueError('Presented output resource header anchor changed')
        header=header.replace(old,new)
    (output/'LuaRetailResources.h').write_text(header)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'integration':['Review staged resource header: new PresentedOutput entry kind, four methods/bindings, unconditional native CString destructor.', 'No raw CString pointer escapes; output survives false populated polls, normal/error cleanup follows existing resource scope.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        output/'bully_presented.lua',output/'LuaRetailResources.h',*[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'bully_presented_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:bully-presented.exe'])
    report['compiledTest']=run('capability-test',[str(output/'bully-presented.exe')])
    report['executableSha256']=hashlib.sha256((output/'bully-presented.exe').read_bytes()).hexdigest()
    executable=(output/'bully-presented.exe').read_bytes()
    pe=int.from_bytes(executable[0x3c:0x40],'little')
    machine=int.from_bytes(executable[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Expected actual x86 proposal executable')
    report['peMachine']='0x014c (x86)'
    (output/'INTEGRATION.md').write_text("""# Presented-item output ? compiled proposal, unapplied

Reproduce: python -m tools.script_recovery.bully_presented_prepare.
Review the staged LuaRetailResources.h diff: one PresentedOutput entry kind,
CString and borrowed actor fields, four methods/bindings, unconditional native
CString destruction even empty. NewPresentedItemOutput uses99E4B0; Poll passes
the same output to current bound-Thing slot8C each time; Matches negates native
CString inequality99E960. No cached global item name or raw pointer is exposed.

Actual FSE/x86/real Lua verifies false+populated then true+empty, cancellation,
poll/body errors with movie open, resource cleanup and inert escaped IDs.
Mock native trampolines exist only in a private harness allocation. Engine and
runtime checkout are unchanged. Existing noexcept Close assumes nonthrowing native
destructors; callback/poll errors are exercised, destructor failure is not invented.
""")
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
