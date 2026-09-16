"""Compile and stage the Maze opt-in helper entry policy with the Sword proposal."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.maze_entry_policy import stage
from tools.script_recovery.maze_prepare_sword_proposal import prepare as prepare_sword
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment, find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    sword=prepare_sword(runtime)
    source=Path(__file__).parent
    output=ROOT/'work/maze_converter/entry_proposal'
    evidence=stage(output,runtime/'FableScriptExtender')
    for name in ('maze_entry_harness.cpp','maze_entry_adapter.inc'):
        shutil.copyfile(source/name,output/name)
    sword_output=ROOT/'work/maze_converter/sword_proposal'
    shutil.copyfile(sword_output/'MazeResearch.UNLIMBO.lua',output/'MazeResearch.UNLIMBO.lua')
    grave=(sword_output/'EmptyGrave.Main.lua').read_text()
    old='quest:CreateThread("UnLimboSword", {region = ""})'
    if grave.count(old)!=1:raise ValueError('Maze native thread source correspondence changed')
    (output/'EmptyGrave.Main.lua').write_text(grave.replace(old,'quest:CreateMazeUnlimboThread()'))
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';objects=sorted(sword_output.glob('*.obj'))
    objects=[p for p in objects if p.stem not in ('maze_sword_harness',)]
    report={'status':'pending integration; engine restoration and teardown not certified',
        'runtimeChanged':False,'hostSourceEvidence':evidence,'swordGate':sword['compiledTest'],'commands':[],
        'integration':[
            'Review staged maze_entry_host.h and maze_entry_methods.inc changes against actual host; retain default CreateThread policy=false.',
            'Bind CreateMazeUnlimboThread from maze_entry_adapter.inc; it selects native helper entry and explicit empty region with no Lua arguments.',
            'Compose with InitializeMazeSword/UnlimboMazeSword capabilities from sword_proposal. Isolated Lua fragments use both proposals.',
            'Before teardown call BeginNativeEntryTeardown, arrange engine cancellation/drain, and require NativeEntryCallbacksDrained before destroying flags, Sword slot and Lua state.',
            'The counter covers opted-in helper callbacks only. It does not prove no macro/entity callback or engine callback retains parent state; those still require a complete quiescence protocol.',
            'No opt-in flag bypasses closing. Existing default threads keep their termination guards; default Main behavior is unchanged.',
            'Real engine scheduling, save restoration and suspended fiber teardown remain integration requirements.']}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('entry-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'maze_entry_harness.cpp'),*map(str,objects),'/Fe:maze-entry.exe'])
    report['compiledTest']=run('entry-test',[str(output/'maze-entry.exe')])
    inputs=[output/'maze_entry_host.h',output/'maze_entry_methods.inc',output/'maze_entry_harness.cpp',
        output/'MazeResearch.UNLIMBO.lua',output/'EmptyGrave.Main.lua',*objects]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    report['executableSha256']=hashlib.sha256((output/'maze-entry.exe').read_bytes()).hexdigest()
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':
    print(prepare()['compiledTest'])
