"""Compile actual chain/launcher and exercise Windows loading in a harmless fixture."""
import argparse,json,shutil,subprocess,sys,time,hashlib,ctypes
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

def main():
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True)
    p.add_argument('--launcher',type=Path,help='Exercise this exact existing launcher instead of compiling it')
    a=p.parse_args()
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH']);assert compiler
    sources=Path(__file__).parent
    commands=[]
    def compile(src,target,dll=False):
        cmd=[compiler,'/nologo','/EHsc','/std:c++17','/Od',str(src),'/Fe:'+str(out/target),'/Fo:'+str(out/(target+'.obj'))]
        if dll:cmd+=['/LD']
        r=subprocess.run(cmd,cwd=out,env=env,capture_output=True,text=True)
        (out/(target+'.build.log')).write_text(r.stdout+r.stderr)
        commands.append(dict(command=cmd,returncode=r.returncode));r.check_returncode()
    compile(sources/'runtime_checks/novi_compat_chain.cpp','chain-check.exe')
    r=subprocess.run([str(out/'chain-check.exe')],capture_output=True,text=True,check=True)
    (out/'chain-check.log').write_text(r.stdout+r.stderr)
    if a.launcher:shutil.copy2(a.launcher,out/'NoviLocalLauncher.exe')
    else:compile(sources/'novi_compat_launcher.cpp','NoviLocalLauncher.exe')
    original=out/'original.cpp';original.write_text('#include <windows.h>\nextern "C" __declspec(dllexport) int Ready(){return 17;}\nBOOL WINAPI DllMain(HMODULE,DWORD,LPVOID){return TRUE;}\n')
    addon=out/'addon.cpp';addon.write_text('''#include <windows.h>
static bool ready=false;
extern "C" __declspec(dllexport) DWORD WINAPI NoviCompatibilityStart(void*) {
auto h=GetModuleHandleA("FableScriptExtender.dll");
auto f=h?(int(*)())GetProcAddress(h,"Ready"):nullptr;
ready=f && f()==17;return ready?1:99;
}
extern "C" __declspec(dllexport) int Ready(){return ready?23:0;}
BOOL WINAPI DllMain(HMODULE,DWORD,LPVOID){return TRUE;}
''')
    game=out/'game.cpp';game.write_text('''#include <windows.h>
#include <cstdio>
int main(){FILE* entered=nullptr;fopen_s(&entered,"primary-entered.txt","wx");
if(entered){fputs("main",entered);fclose(entered);}
auto h=GetModuleHandleA("NoviCompatibility.dll");
auto f=h?(int(*)())GetProcAddress(h,"Ready"):nullptr;
if(!f || f()!=23)return 1;
FILE* file=nullptr;fopen_s(&file,"fixture-main-ran.txt","wx");
if(!file)return 2;fputs("Both DLLs initialized before primary thread resumed.",file);fclose(file);return 0;}
''')
    compile(original,'FableScriptExtender.dll',True);compile(addon,'NoviCompatibility.dll',True);compile(game,'FixtureGame.exe')
    reject=out/'reject.cpp';reject.write_text('#include <windows.h>\nextern "C" __declspec(dllexport) DWORD WINAPI NoviCompatibilityStart(void*){return 99;}\nBOOL WINAPI DllMain(HMODULE,DWORD,LPVOID){return TRUE;}\n')
    hang=out/'hang.cpp';hang.write_text('#include <windows.h>\nextern "C" __declspec(dllexport) DWORD WINAPI NoviCompatibilityStart(void*){Sleep(45000);return 1;}\nBOOL WINAPI DllMain(HMODULE,DWORD,LPVOID){return TRUE;}\n')
    compile(reject,'RejectedAddon.dll',True);compile(hang,'StalledAddon.dll',True)
    cmd=[str(out/'NoviLocalLauncher.exe'),str(out/'FixtureGame.exe'),str(out),str(out/'FableScriptExtender.dll'),str(out/'NoviCompatibility.dll')]
    r=subprocess.run(cmd,capture_output=True,text=True,timeout=40)
    (out/'loader.log').write_text(r.stdout+r.stderr);r.check_returncode()
    for _ in range(50):
        if (out/'fixture-main-ran.txt').exists():break
        time.sleep(.1)
    assert (out/'fixture-main-ran.txt').exists()
    failures=[]
    for name,expected,original_path,addon_path in [
        ('missing-original',7,out/'missing.dll',out/'NoviCompatibility.dll'),
        ('missing-addon',3,out/'FableScriptExtender.dll',out/'missing-addon.dll'),
        ('addon-rejected',9,out/'FableScriptExtender.dll',out/'RejectedAddon.dll'),
        ('addon-timed-out',9,out/'FableScriptExtender.dll',out/'StalledAddon.dll')]:
        directory=out/name;directory.mkdir()
        failure_cmd=[str(out/'NoviLocalLauncher.exe'),str(out/'FixtureGame.exe'),str(directory),str(original_path),str(addon_path)]
        failure=subprocess.run(failure_cmd,capture_output=True,text=True,timeout=40)
        assert failure.returncode==expected,(name,failure.returncode)
        assert not (directory/'fixture-main-ran.txt').exists()
        assert not (directory/'primary-entered.txt').exists()
        info=json.loads(failure.stdout) if failure.stdout.strip() else None
        if info:
            kernel=ctypes.WinDLL('kernel32',use_last_error=True)
            kernel.OpenProcess.argtypes=[ctypes.c_uint,ctypes.c_int,ctypes.c_uint]
            kernel.OpenProcess.restype=ctypes.c_void_p
            kernel.WaitForSingleObject.argtypes=[ctypes.c_void_p,ctypes.c_uint]
            kernel.CloseHandle.argtypes=[ctypes.c_void_p]
            handle=kernel.OpenProcess(0x100000,False,info['pid'])
            if handle:
                try:assert kernel.WaitForSingleObject(handle,0)==0,(name,'process still alive')
                finally:kernel.CloseHandle(handle)
            else:assert ctypes.get_last_error()==87,(name,ctypes.get_last_error())
        failures.append(dict(name=name,returncode=failure.returncode,output=failure.stdout,child_terminated=bool(info)))
    inputs=[sources/'novi_compat_launcher.cpp',sources/'novi_compat_chain.inl',sources/'runtime_checks/novi_compat_chain.cpp',Path(__file__)]
    if a.launcher:inputs.append(a.launcher)
    (out/'result.json').write_text(json.dumps(dict(passed=True,commands=commands,loader_rejections=failures,
        inputs={str(f.resolve()):hashlib.sha256(f.read_bytes()).hexdigest() for f in inputs},
        limits='Synthetic Windows processes and callback; actual Fable coexistence is not tested.'),indent=2))
    print('Chain checks and suspended-process two-DLL loader fixture passed.')
if __name__=='__main__':main()
