"""Compose scratch installer constructor with recovered strings/map and real OS.

Only the constructor's CRT thread entry is redirected in its test object.
Vtable identity and worker entry bodies are fixture boundaries, not recovered.
"""
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path
import check_one
import pefile
from pe_oracle import EXE
from gate_retail_bank_alias_insertion import SOURCES as ALIAS_SOURCES

ROOT = Path(__file__).resolve().parents[2]
STRINGS = [
    '00/99/CCharString_Constructor_0099ebf0.cpp',
    '00/99/CCharString_AllocStringData_0099ea60.cpp',
    '00/99/CCharString_Destructor_0099eae0.cpp',
    '00/99/CCharString_UnassignString_0099e9b0.cpp',
    '00/99/CCharString_operator_0099efe0.cpp',
    '00/99/CCharString_CCharString_0099ec30.cpp',
    '00/99/CCharString_DefaultConstructor_0099e4b0.cpp',
    '00/99/CCharString_ConversionOperator_0099e4c0.cpp',
    '00/9a/CCharString_InitFromCStr_009a0590.cpp',
    '00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp',
    '00/9a/CBasicRetailBankAliasMap_Erase_009aa830.cpp',
    '01/3b/CharStringInstanceCount_013bd800.cpp',
]

def main():
    work = Path(tempfile.mkdtemp(prefix='installer-ctor-os-'))
    env = check_one.env()
    env['INCLUDE'] = str(work) + ';' + env['INCLUDE']
    literals = {}
    with pefile.PE(str(EXE)) as pe:
        for address in (0x129b448, 0x129b440):
            raw = pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase, 64).split(b'\0',1)[0]
            assert raw and len(raw) < 64
            literals[address] = raw.decode('ascii')
    (work/'installer_literals.h').write_text('\n'.join(
        'extern const char g_FileInstaller%sExtension_%08X[] = %s;' %
        (kind,address,json.dumps(literals[address]))
        for kind,address in [('Compressed',0x129b448),('Plain',0x129b440)])+'\n')
    # These STL helpers are already emitted from authoritative inline headers.
    sources = [ROOT/'work/FileInstallerConstructor_candidate.cpp'] + [p for p in ALIAS_SOURCES[:-1] if p.stem not in ('STL_Mfind_0042b467','STL_Rb_global_Rebalance_0042971d')] + [
        ROOT/'rebuild/src/compiled'/name for name in STRINGS] + [
        ROOT/'rebuild/tests/integration/FileInstallerConstructorOS_test.cpp']
    assert len(sources) == len(set(sources))
    objects = []
    for source in sources:
        obj = work/(source.stem+'.obj')
        result = check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
        assert result.returncode == 0, result.stdout+result.stderr
        objects.append(obj)
    raw = bytearray(objects[0].read_bytes())
    ptr,count = struct.unpack_from('<II',raw,8)
    start = ptr+count*18
    needle = b'__beginthread\0'
    value = b'_ThreadRecording\0'
    pos = raw.index(needle,start)
    offset = pos-start
    delta = len(value)-len(needle)
    for i in range(count):
        at = ptr+i*18
        if raw[at:at+4] == b'\0'*4:
            old = struct.unpack_from('<I',raw,at+4)[0]
            if old > offset: struct.pack_into('<I',raw,at+4,old+delta)
    struct.pack_into('<I',raw,start,struct.unpack_from('<I',raw,start)[0]+delta)
    objects[0].write_bytes(raw[:pos]+value+raw[pos+len(needle):])
    exe = work/'fixture.exe'
    link = [str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    result = subprocess.run(link+[str(o) for o in objects],env=env,capture_output=True,text=True)
    assert result.returncode == 0, result.stdout+result.stderr
    omitted = 0
    for index in range(len(objects)-1):
        result = subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=index],env=env,capture_output=True,text=True)
        assert result.returncode != 0, 'omitted provider linked: '+str(sources[index])
        omitted += 1
    result = subprocess.run(link+[str(o) for o in objects],env=env,capture_output=True,text=True)
    assert result.returncode == 0, result.stdout+result.stderr
    result = subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert result.returncode == 0 and 'FILE_INSTALLER_CTOR_OS_OK' in result.stdout, (result.returncode,result.stdout,result.stderr)
    report = dict(literals={hex(k):v for k,v in literals.items()}, sources=[str(p.relative_to(ROOT)) for p in sources],
        source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},
        omitted_provider_checks=omitted,cycles=32,events=128,worker_launches=0,
        boundary='real CRT malloc/string allocation, production string/map providers, real Windows CS/events; recorded thread calls; fixture vtable/worker identities; explicit cleanup, no installer destructor claim')
    (ROOT/'work/file_installer_constructor/os-composition.json').write_text(json.dumps(report,indent=2)+'\n')
    print(result.stdout.strip())
    print('omitted_provider_checks='+str(omitted))

if __name__ == '__main__': main()
