"""Stage and compile a condition binding proposal without changing the runtime."""
import difflib
import hashlib
import json
import shutil
import subprocess
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.native_new_oakvale_conditions import verify
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment, find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow'), output=None):
    verify('NOVI_BookTrader')
    output = Path(output or ROOT / 'work/new_oakvale_conditions')
    output.mkdir(parents=True, exist_ok=True)
    source = Path(__file__).parent
    manager = runtime / 'FableScriptExtender/LuaManager.cpp'
    before = manager.read_bytes().decode('utf-8')
    newline = '\r\n' if '\r\n' in before else '\n'
    include = '#include "LuaRetailCondition.h"'
    binding = '    questState_type["RegisterBoundAliveCondition"]'
    if (before.count(include)!=1 or before.count(binding)!=1 or
            'RegisterBoundConsciousCondition' in before):
        raise ValueError('condition binding runtime anchors changed')
    helper = (source / 'retail_conscious_condition.h').read_text()
    registration = (source / 'conscious_condition_registration.inc').read_text()
    after = before.replace(include, include+newline+'#include "LuaRetailConsciousCondition.h"')
    after = after.replace(binding, registration.replace('\n',newline)+newline+binding)
    patch = ''.join(difflib.unified_diff(before.splitlines(True),after.splitlines(True),
        fromfile='a/FableScriptExtender/LuaManager.cpp',tofile='b/FableScriptExtender/LuaManager.cpp'))
    patch += ''.join(difflib.unified_diff([],helper.splitlines(True),
        fromfile='/dev/null',tofile='b/FableScriptExtender/LuaRetailConsciousCondition.h'))
    (output/'condition-integration.patch').write_bytes(patch.encode('utf-8'))
    (output/'LuaManager.cpp').write_bytes(after.encode('utf-8'))
    (output/'LuaRetailConsciousCondition.h').write_bytes(helper.encode('utf-8'))
    shutil.copyfile(source/'scythe_alive_registration.inc',output/'alive-registration.inc')
    witness=json.loads((source/'native_new_oakvale_conditions_witness.json').read_text())
    predicate=bytes.fromhex(witness['predicateBytes'])
    (output/'conscious-predicate.inc').write_text('static const unsigned char nativePredicateBytes[] = {'+
        ','.join(hex(b) for b in predicate)+'};\n')
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    vendor=runtime/'Vendor';lua=vendor/'lua'
    c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'proposal-only-not-applied','runtimeChanged':False,'commands':[],
            'nativeWitness':str(source/'native_new_oakvale_conditions_witness.json')}
    inputs=[manager,source/'retail_conscious_condition.h',source/'conscious_condition_registration.inc',
            source/'conscious_condition_harness.cpp',source/'native_new_oakvale_conditions_witness.json',
            source/'runtime_checks/retail_thing_condition.cpp',source/'runtime_checks/retail_resources_smoke.cpp',
            *c_sources,*(runtime/'FableScriptExtender').glob('*.h')]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout)
        report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:
            (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
            raise RuntimeError(result.stdout)
        return result.stdout
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    objects=[str(output/(p.stem+'.obj')) for p in c_sources]
    run('condition-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),
        '/I'+str(vendor),'/I'+str(lua),str(source/'conscious_condition_harness.cpp'),*objects,
        '/Fe:'+str(output/'condition.exe'),'/link','User32.lib'])
    report['compiledTest']=run('condition-test',[str(output/'condition.exe')])
    run('patch-check',['git','-C',str(runtime),'apply','--check',str(output/'condition-integration.patch')])
    report['binarySha256']=hashlib.sha256((output/'condition.exe').read_bytes()).hexdigest()
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    (output/'INTEGRATION.md').write_text('''# New Oakvale condition proposal

Unapplied. `condition-integration.patch` adds RegisterBoundConsciousCondition to
the entity VM and a 16-byte native cloned-condition helper. The existing
RegisterBoundAliveCondition remains unchanged for DeadFather.

The harness executes checked retail predicate bytes at their real x86 calling
convention against controlled Thing virtual methods. It checks all four
alive/unconscious combinations and short-circuit order, counted clone lifetime,
registration failure cleanup and the actual sol/Lua binding. This is not a full
runtime DLL build or gameplay/scheduler validation.
''')
    return report


if __name__=='__main__':
    result=prepare()
    print(json.dumps({'status':result['status'],'commands':result['commands'],'compiledTest':result['compiledTest']},indent=2))
