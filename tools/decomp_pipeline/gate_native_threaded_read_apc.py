"""Actual Windows APC/file-read/event proof for original and rebuilt callback.

Only private synthetic files and unnamed owned events are used. Manager/file
contexts are raw typed storage; no engine owner, worker, game or save is opened.
"""
import hashlib
import json
import re
import struct
import subprocess
import sys
from datetime import datetime,timezone
from pathlib import Path

import check_one as V
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import gate_native_threaded_read_completion as callback
import threaded_error_policy_oracle as policy

R = Path(__file__).resolve().parents[2]
FIXTURE = R/'rebuild/tests/integration/NativeThreadedReadAPC_test.cpp'
ABI = FIXTURE.with_name('NativeThreadedReadAPC_win32.h')
SOURCES = [FIXTURE,callback.SOURCE,callback.DATA_SOURCE,
           R/'rebuild/src/compiled/00/9c/CFileInstaller_Unpause_009c3250.cpp',
           R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Constructor_00a62920.cpp',
           R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp',policy.SOURCE]


SOURCES.append(R/'rebuild/src/compiled/00/98/CThreadedFileObject_Constructor_0098df00.cpp')

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    assert sys.platform == 'win32','This gate executes an owned Windows x86 probe'
    stamp = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    build = R/'rebuild/build/native-read-apc/00/98/0098e320'/stamp
    build.mkdir(parents=True,exist_ok=False)
    include = build/'include';include.mkdir()
    source_dir = build/'sources';source_dir.mkdir()
    inputs = {Path(__file__),EXE,Path(callback.__file__),Path(policy.__file__),R/'tools/decomp_pipeline/check_one.py',
              R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py'}
    visited = set()
    def closure(path):
        path = path.resolve()
        if path in visited:return
        visited.add(path);inputs.add(path)
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(encoding='utf-8'),re.M):
            target = next((q.resolve() for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert target,(path,name)
            closure(target)
    for path in SOURCES:closure(path)
    toolchain = [V.VC/'bin'/name for name in ('cl.exe','link.exe','c1xx.dll','c2.dll')]
    toolchain += [V.VC/'lib'/name for name in ('libcmt.lib','oldnames.lib','kernel32.lib')]
    toolchain = [path for path in toolchain if path.is_file()]
    inputs.update(toolchain)
    pins = {str(path):sha(path) for path in sorted(inputs)}
    (build/'input-pins.json').write_text(json.dumps(pins,indent=2)+'\n')
    (build/'gate-source.py').write_bytes(Path(__file__).read_bytes())
    for path in visited-set(SOURCES):
        if path.is_relative_to(R/'rebuild/include'):
            target = include/path.relative_to(R/'rebuild/include')
        else:
            assert path.parent == FIXTURE.parent,path
            target = source_dir/path.name
        target.parent.mkdir(parents=True,exist_ok=True)
        target.write_bytes(path.read_bytes())
    snapshots = []
    for index,path in enumerate(SOURCES):
        target = source_dir/('%d_'%index+path.name)
        target.write_bytes(path.read_bytes());snapshots.append(target)
    env = V.env()
    env['INCLUDE'] = str(V.VC/'include')+';'+str(include)+';'+str(source_dir)
    objects = []
    flags = ['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(include),'/I'+str(source_dir)]
    def save(name,data):
        path = build/name
        assert not path.exists(),path
        path.write_text(json.dumps(data,indent=2)+'\n')
    for index,source in enumerate(snapshots):
        obj = build/('provider%d.obj'%index)
        command = [*flags,'/Fo'+str(obj),str(source)]
        result = V.cl(command,env,build)
        save('compile%d.json'%index,dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
        assert result.returncode == 0,result.stdout+result.stderr
        shown = []
        for line in result.stdout.splitlines():
            if 'including file:' in line.lower():
                path = Path(line.split(':',2)[-1].strip()).resolve()
                assert path.is_relative_to(V.VC/'include') or path.is_relative_to(build),path
                shown.append(str(path))
        assert shown,'Missing include provenance'
        objects.append(obj)
    original = read_virtual_data(EXE.read_bytes(),callback.ENTRY,callback.SIZE)
    c = Coff(objects[1]);section,offset = c.symbols[callback.SYMBOL]
    body,relocations = c.sections[section]
    assert offset == 0 and len(body) == 102 and len(relocations) == 7
    patched = bytearray(body)
    relocation_audit = []
    for site,(symbol,kind) in sorted(relocations.items()):
        assert kind in (6,20) and struct.unpack_from('<i',body,site)[0] == 0
        target = callback.ADDRESS[symbol]
        original_operand = struct.unpack_from('<i' if kind == 20 else '<I',original,site)[0]
        assert (callback.ENTRY+site+4+original_operand if kind == 20 else original_operand) == target
        struct.pack_into('<I',patched,site,(target-(callback.ENTRY+site+4 if kind == 20 else 0))&0xffffffff)
        relocation_audit.append(dict(site=site,symbol=symbol,kind=kind,target=hex(target)))
    assert bytes(patched) == original
    policy_audit = []
    pc = Coff(objects[6])
    for root in policy.ROOTS:
        section,offset = pc.symbols[root['symbol']]
        body,relocations = pc.sections[section]
        assert offset == 0 and len(body) == root['size'] and len(relocations) == 1
        patched_policy = bytearray(body)
        for site,(symbol,kind) in relocations.items():
            assert kind == 6 and struct.unpack_from('<I',body,site)[0] == 0
            struct.pack_into('<I',patched_policy,site,policy.ADDRESS[symbol])
        assert bytes(patched_policy) == read_virtual_data(EXE.read_bytes(),root['address'],root['size'])
        policy_audit.append(dict(symbol=root['symbol'],address=hex(root['address']),grade='MATCH'))
    oracle = build/'oracle.bin';oracle.write_bytes(original)
    data = build/'synthetic.bin';data.write_bytes(bytes((i*37+23)&255 for i in range(0x9000)))
    data_hash = sha(data)
    def link(name,providers):
        executable = build/(name+'.exe')
        command = [str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(executable),*map(str,providers),'kernel32.lib']
        result = subprocess.run(command,cwd=build,env=env,capture_output=True,text=True,timeout=30)
        row = dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)
        save(name+'.link.json',row)
        return executable,row
    executable,linked = link('probe',objects)
    assert linked['returncode'] == 0,linked
    with pefile.PE(str(executable)) as pe:
        assert pe.FILE_HEADER.Machine == 0x14c and pe.OPTIONAL_HEADER.Magic == 0x10b
        imports = {(entry.dll.decode().lower(),imp.name.decode()) for entry in pe.DIRECTORY_ENTRY_IMPORT for imp in entry.imports if imp.name}
        for name in ('ReadFileEx','SleepEx','SetEvent','CreateEventA','VirtualAlloc','VirtualProtect'):
            assert ('kernel32.dll',name) in imports,name
    run = subprocess.run([str(executable),str(oracle),str(data)],cwd=build,capture_output=True,text=True,timeout=60)
    save('positive-run.json',dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr))
    assert run.returncode == 0,run.stdout+run.stderr
    match = re.search(r'NATIVE_READ_APC_PASS io_cases=9 delivered=(\d+) immediate_failures=(\d+) error_completions=(\d+) native_controls=24',run.stdout)
    assert match and len(re.findall(r'^IO \d+ ',run.stdout,re.M)) == 9,run.stdout
    delivered,immediate_failures,error_completions = map(int,match.groups())
    assert delivered >= 6 and delivered+immediate_failures == 9
    omissions = []
    required = {1:('_FixtureReadCompletion@12',callback.SYMBOL),
                2:('?m_bDirtyDiscError@CThreadedFileManager@@0_NA',),
                3:('?Unpause@CFileInstaller@@QAEXXZ',),
                4:('??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z',),
                5:('??1CCriticalSectionLock@@QAE@XZ',),
                6:tuple(root['symbol'] for root in policy.ROOTS),7:('??0CFileObject@CThreadedFileManager@@QAE@XZ',)}
    for index,names in required.items():
        _,row = link('omit%d'%index,[obj for n,obj in enumerate(objects) if n != index])
        diagnostic = row['stdout']+row['stderr']
        assert row['returncode'] and re.search(r'LNK(?:2001|2019)',diagnostic),row
        assert any(name in diagnostic for name in names),row
        omissions.append(dict(source=str(SOURCES[index]),required_symbols=names,returncode=row['returncode']))
    noop = source_dir/'no-op-callback.cpp'
    noop.write_text('#include "fable_gfuninitialise.h"\n'
                    'void __stdcall CThreadedFileManager::OnFileReadCompletion(unsigned long,unsigned long,_OVERLAPPED*) {}\n')
    control_obj = build/'no-op-callback.obj'
    control_compile = V.cl([*flags,'/Fo'+str(control_obj),str(noop)],env,build)
    save('runtime-control.compile.json',dict(returncode=control_compile.returncode,stdout=control_compile.stdout,stderr=control_compile.stderr))
    assert control_compile.returncode == 0,control_compile.stdout+control_compile.stderr
    control_objects = [*objects];control_objects[1] = control_obj
    control_exe,control_link = link('runtime-control',control_objects)
    assert control_link['returncode'] == 0,control_link
    control_run = subprocess.run([str(control_exe),str(oracle),str(data)],cwd=build,capture_output=True,text=True,timeout=60)
    save('runtime-control.run.json',dict(returncode=control_run.returncode,stdout=control_run.stdout,stderr=control_run.stderr))
    assert control_run.returncode == 80 and 'NATIVE_APC_FAILURE' in control_run.stderr
    assert all(word in control_run.stderr for word in ('memcmp','request','expected')),control_run.stderr
    assert sha(data) == data_hash,'Owned read-only synthetic file changed'
    assert all(sha(Path(path)) == value for path,value in pins.items()),'Input drift'
    report = dict(status='PASS',build=str(build),original_rebuilt='PASS',io_cases_per_mode=9,
                  delivered_per_mode=delivered,immediate_failures_per_mode=immediate_failures,
                  error_completions_per_mode=error_completions,native_controls_per_mode=24,
                  link_omissions=omissions,runtime_noop_control='DETECTED',input_sha256=pins,
                  object_sha256={str(path):sha(path) for path in objects},executable_sha256=sha(executable),
                  oracle_sha256=sha(oracle),synthetic_file_sha256=data_hash,
                  callback=dict(address='0098e320',grade='MATCH',bytes=102,instructions=31,fixups=relocation_audit),
                  policy=dict(roots=policy_audit,native_dispatches=2,guarded_null_return=True,typed_setter_getter_ingress=True),
                  actual_imports=sorted(name for dll,name in imports if dll == 'kernel32.dll'),
                  boundaries=['Real Windows x86 ReadFileEx/APC/SleepEx/SetEvent and unnamed owned events; one read at a time from a private synthetic file.',
                              'Original callback copied from the private retail oracle with seven independently named relocations; rebuilt callback freshly compiled from frozen source/header copies.',
                              'Raw typed manager/file contexts only; manager owner/queues/OnFinishObject, installer Get/Unpause and engine worker submission not executed here.',
                              'Unpause and scoped-lock providers are linked dependencies with omission checks; their execution is established by separate composition gates.',
                              '24 additional direct native callback controls per mode are distinct from the real IO/APC cases; no game/install/save changes.'],
                  api_contract='https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfileex')
    save('receipt.json',report)
    (R/'rebuild/compile-gate/native-threaded-read-apc.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_THREADED_READ_APC_PASS io_cases=9 original_rebuilt=PASS link_omissions=7 runtime_control=PASS')
    print('RECEIPT',build/'receipt.json')


if __name__ == '__main__':main()
