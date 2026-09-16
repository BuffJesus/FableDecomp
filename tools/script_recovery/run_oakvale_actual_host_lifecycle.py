"""Link a console harness to every object from the validated full DLL build."""
import hashlib
import json
import shutil
import subprocess
import struct
from pathlib import Path
from tools.script_recovery.build_oakvale_staged_runtime import OUT
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def run(registration_check=None,quest_script=None):
    build=json.loads((OUT/'result.json').read_text())
    if build['status']!='passed':raise ValueError('A passing full staged runtime build is required')
    runtime=OUT/'runtime';source=Path(__file__).with_name('oakvale_actual_host_lifecycle_harness.cpp')
    for relative,expected in build['stagedInputs'].items():
        if hashlib.sha256((runtime/relative).read_bytes()).hexdigest()!=expected:raise ValueError('Built runtime input changed: '+relative)
    env=compiler_environment(find_vcvars());cl=shutil.which('cl.exe',path=env['PATH']);objects=sorted((OUT/'obj').glob('*.obj'))
    inputs={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,*objects]}
    if registration_check is not None:
        registration_check=Path(registration_check).resolve()
        inputs[str(registration_check)]=hashlib.sha256(registration_check.read_bytes()).hexdigest()
    if quest_script is not None:
        if registration_check is None:raise ValueError('Quest persistence check also requires the registration inventory')
        quest_script=Path(quest_script).resolve()
        inputs[str(quest_script)]=hashlib.sha256(quest_script.read_bytes()).hexdigest()
        # Real virtual Init resolves the quest through g_fseBasePath, which also
        # owns the runtime log. Keep that writable runtime directory under work/.
        source_fse=quest_script.parent.parent
        fixture_fse=OUT/'host-package/FSE'
        for source_lua in source_fse.rglob('*.lua'):
            destination=fixture_fse/source_lua.relative_to(source_fse)
            destination.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(source_lua,destination)
            expected=hashlib.sha256(source_lua.read_bytes()).hexdigest()
            inputs[str(source_lua)]=expected;inputs[str(destination)]=expected
        quest_script=fixture_fse/quest_script.relative_to(source_fse)
        from tools.script_recovery.native_oakvale_entity_ownership import prove
        from tools.script_recovery.lift_native_lua import RData,RETAIL_EXE
        import pefile
        native=prove();data=RData();pe=pefile.PE(str(RETAIL_EXE))
        relocations=[data.base+entry.rva for block in getattr(pe,'DIRECTORY_ENTRY_BASERELOC',[]) for entry in block.entries if entry.type==3]
        helpers=[row for row in native['regions'] if row[0] in (0x4abe90,0x4aa840,0x99a2e0,0xce1000)]
        from tools.script_recovery.native_oakvale_quiescence import prove as prove_quiescence
        termination=prove_quiescence()['regions'][1]
        helpers.append((termination['address'],termination['size'],termination['sha256']))
        from tools.script_recovery.native_oakvale_entity_callbacks import prove as prove_callbacks
        for callback in prove_callbacks()['regions'][:2]:
            helpers.append((callback['address'],callback['size'],callback['sha256']))
        blob=struct.pack('<I',len(helpers))
        for address,size,_ in helpers:
            fixes=[value-address for value in relocations if address<=value and value+4<=address+size]
            blob+=struct.pack('<III',address,size,len(fixes))+data.bytes_at(address,size)+b''.join(struct.pack('<I',offset) for offset in fixes)
        native_helpers=OUT/'entity-native-test-helpers.bin';native_helpers.write_bytes(blob)
        inputs[str(native_helpers)]=hashlib.sha256(blob).hexdigest()
    command=[cl,'/nologo','/EHsc','/std:c++17','/MT','/O2','/bigobj',
        '/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(runtime/'Vendor/lua'),
        str(source),*map(str,objects),'/Fe:actual-host-lifecycle.exe','/link','/SUBSYSTEM:CONSOLE',
        'kernel32.lib','user32.lib','gdi32.lib','winspool.lib','comdlg32.lib','advapi32.lib','shell32.lib','ole32.lib','oleaut32.lib','uuid.lib','odbc32.lib','odbccp32.lib']
    commands=[]
    test_command=[str(OUT/'actual-host-lifecycle.exe')]+([str(registration_check)] if registration_check is not None else [])
    if quest_script is not None:test_command.extend((str(quest_script),str(native_helpers)))
    for name,args in [('host-harness-build',command),('host-harness-test',test_command)]:
        result=subprocess.run(args,cwd=OUT,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (OUT/(name+'.log')).write_text(result.stdout);commands.append(dict(name=name,exitCode=result.returncode))
        if result.returncode:raise RuntimeError(result.stdout[-10000:])
    for path,expected in inputs.items():
        if hashlib.sha256(Path(path).read_bytes()).hexdigest()!=expected:raise ValueError('Actual-host input changed')
    report=dict(status='passed',commands=commands,inputs=inputs,testOutput=result.stdout.strip(),
        executableSha256=hashlib.sha256((OUT/'actual-host-lifecycle.exe').read_bytes()).hexdigest(),
        limits='Actual complete host/runtime and Lua, engine allocation/timer/base APIs doubled. Real engine scheduler quiescence and live unload remain pending.')
    (OUT/'host-lifecycle-result.json').write_text(json.dumps(report,indent=2)+'\n');return report


if __name__=='__main__':print(run()['testOutput'])
