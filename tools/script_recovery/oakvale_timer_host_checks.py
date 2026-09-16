"""Compile staged host TUs and exercise metadata plus extracted state accessors."""
import hashlib,json,shutil,subprocess
from pathlib import Path
from tools.script_recovery.oakvale_timer_host_prepare import prepare
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

def run(*,prepare_fn=prepare):
    out,report=prepare_fn();runtime=Path('D:/Code/ForgeFSE-retail-shadow');source=Path(__file__).parent
    shutil.copyfile(source/'oakvale_timer_host_harness.cpp',out/'oakvale_timer_host_harness.cpp')
    state=(out/'LuaQuestState.cpp').read_text();parts=[]
    for signature in ('int LuaQuestState::GetStateInt(', 'void LuaQuestState::SetStateInt('):
        start=state.index(signature);end=state.index('\n}',start)+2;parts.append(state[start:end].replace('LuaQuestState::','GeneratedState::'))
    (out/'state_int_methods.inc').write_text('\n'.join(parts))
    dll=(out/'dllmain.cpp').read_text();start=dll.index('template<int N>\nvoid* __fastcall QuestAllocator(');end=dll.index('\n}',start)+2
    (out/'allocator.inc').write_text(dll[start:end])
    env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);lua=runtime/'Vendor/lua';cs=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    opts=[cl,'/nologo','/EHsc','/std:c++17','/MD','/O2','/bigobj','/I'+str(out),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua)]
    report['commands']=[]
    inputs=[source/'oakvale_timer_host_harness.cpp',*out.glob('*.h'),*out.glob('*.cpp'),*out.glob('*.inc'),*(runtime/'FableScriptExtender').glob('*.h'),*cs]
    report['inputs'].update({str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs})
    def command(name,args):
        p=subprocess.run(args,cwd=out,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);(out/(name+'.log')).write_text(p.stdout);report['commands'].append({'name':name,'command':args,'exitCode':p.returncode})
        if p.returncode:raise RuntimeError(p.stdout[-12000:])
        return p.stdout.strip()
    command('lua-build',[cl,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,cs)])
    for name in ('dllmain','LuaQuestHost','LuaQuestState'):command(name+'-compile',opts+['/c',str(out/(name+'.cpp'))])
    command('harness-build',opts+[str(out/'oakvale_timer_host_harness.cpp'),*[str(out/(p.stem+'.obj')) for p in cs],'/Fe:oakvale-timer-host.exe'])
    report['compiledTest']=command('harness-test',[str(out/'oakvale-timer-host.exe')]);raw=(out/'oakvale-timer-host.exe').read_bytes();pe=int.from_bytes(raw[0x3c:0x40],'little');assert int.from_bytes(raw[pe+4:pe+6],'little')==0x14c
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==sha for p,sha in report['inputs'].items()),'Timer integration inputs changed during checks'
    report['peMachine']='0x014c (x86)';report['executableSha256']=hashlib.sha256(raw).hexdigest();(out/'result.json').write_text(json.dumps(report,indent=2)+'\n');return report
if __name__=='__main__':print(run()['compiledTest'])
