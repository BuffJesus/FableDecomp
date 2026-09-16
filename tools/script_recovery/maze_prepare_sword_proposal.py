"""Build an isolated retained-Sword capability without changing current candidates."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.maze_lifecycle_audit import audit
from tools.script_recovery.maze_native import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment, find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    evidence = audit()
    source = Path(__file__).parent
    allocation = json.loads((source/'maze_sword_witness.json').read_text())
    data = RData()
    for region in allocation['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Maze Sword allocator/native evidence changed')
    import pefile
    pe=pefile.PE(data=data.data)
    imports={hex(i.address):{'dll':e.dll.decode(),'name':i.name.decode()} for e in pe.DIRECTORY_ENTRY_IMPORT
        for i in e.imports if hex(i.address) in allocation['imports']}
    if imports!=allocation['imports']:raise ValueError('Maze Sword deallocator import changed')
    output = ROOT / 'work/maze_converter/sword_proposal'
    output.mkdir(parents=True, exist_ok=True)
    bodies,_=recover()
    rewrites=[('EmptyGrave.Main','Main','quest, me',
        '    local sword = quest:GetThingWithScriptName("GoodSword")\n    quest:RetainRetailThing("MazeResearch", sword)\n    quest:EntitySetInLimbo(sword, true, true)',
        '    quest:InitializeMazeSword()'),
        ('MazeResearch.UNLIMBO','UnLimboSword','quest',
        '    local sword = quest:GetRetainedRetailThing("MazeResearch")\n    quest:EntitySetInLimbo(sword, false, true)\n    quest:EntitySetAlpha(sword, 0.0, true)',
        '    quest:UnlimboMazeSword()')]
    from lupa.lua54 import LuaRuntime
    for key,name,args,old,new in rewrites:
        body=bodies[key]
        if body.count(old)!=1:raise ValueError('Maze Sword candidate correspondence changed')
        text='-- Disabled capability candidate; see proposal.json.\nfunction '+name+'('+args+')\n'+body.replace(old,new)+'end\n'
        LuaRuntime().execute('return function()\n'+text+'\nend')
        (output/(key+'.lua')).write_text(text)
    names = ('maze_sword_slot.h','maze_sword_adapter.inc','maze_sword_harness.cpp')
    for name in names: shutil.copyfile(source/name,output/name)
    env = compiler_environment(find_vcvars())
    compiler = shutil.which('cl.exe',path=env['PATH'])
    lua = runtime/'Vendor/lua'
    c_sources = sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report = {'status':'pending host integration; original candidate unchanged; isolated capability fragments staged',
        'runtimeChanged':False,'nativeAudit':evidence,'allocationEvidence':allocation,'commands':[],
        'integration':[
            'Add a unique_ptr<MazeSwordSlot> m_mazeSwordSlot to the quest state, allocated during Maze quest construction; never bind the slot or its Thing to Lua.',
            'Construct the slot with interface and ASLR<MazeSwordSlot::DeleteInfo>(0xBFE9BC), native MSVCR71 operator delete. Do not substitute current Game_free (BFEA14 imports free).',
            'Declare/bind InitializeMazeSword and UnlimboMazeSword; include maze_sword_adapter.inc in the quest-state implementation.',
            'Candidate Grave lookup/retain/limbo sequence becomes quest:InitializeMazeSword(). Helper GetRetainedRetailThing/limbo/alpha becomes quest:UnlimboMazeSword(). No existing candidate was changed by this proposal.',
            'Parent teardown must quiesce spawned threads and macro callbacks, destroy the Maze flags map, then call CloseAfterQuiescence before base teardown. Current shared flags and scheduler do not yet establish that precondition.',
            'Do not treat Lua GC, a close method name or field persistence as proof of engine teardown/save restoration ordering.'],
        'exceptionPolicy':'Lookup/consumer exceptions unwind temporary output and key; the retained slot remains owned until explicit close. Native exception equivalence is not claimed.'}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),
        source/'runtime_checks/retail_resources_smoke.cpp',*[output/name for name in names]]
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    report['inputs']={str(p):digest(p) for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout)
        report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('sword-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'maze_sword_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:maze-sword.exe'])
    report['compiledTest']=run('sword-test',[str(output/'maze-sword.exe')])
    report['executableSha256']=digest(output/'maze-sword.exe')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':
    print(prepare()['compiledTest'])
