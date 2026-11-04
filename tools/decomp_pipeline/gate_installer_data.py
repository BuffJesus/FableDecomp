"""Verify installer BSS/literals and the retail malloc import identity."""
import sys,json,subprocess,tempfile,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
w=Path(tempfile.mkdtemp(prefix='installer-data-'));env=check_one.env()
source=ROOT/'rebuild/src/compiled/01/3c/FileInstaller_Data_013ca818.cpp'
fixture=ROOT/'rebuild/tests/integration/FileInstallerData_test.cpp';objects=[]
for i,path in enumerate((source,fixture)):
    obj=w/('%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],env,w)
    assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);specs=[(0x13ca818,'?g_CFileInstallerSingleton_013CA818@@3UCCountedFileInstaller@@A',bytes(8)),(0x129b448,'?g_FileInstallerCompressedExtension_0129B448@@3QBDB',b'.biz\0'),(0x129b440,'?g_FileInstallerPlainExtension_0129B440@@3QBDB',b'.big\0')]
with pefile.PE(str(EXE))as pe:
    mapped=pe.get_memory_mapped_image()
    for address,symbol,value in specs:
        assert coff_data(c,symbol,len(value))==value
        assert mapped[address-0x400000:address-0x400000+len(value)]==value
    assert pe.get_data(0xbfea0e-0x400000,6)==bytes.fromhex('ff2558014401')
    imports=[(entry.dll.lower(),imp.name)for entry in pe.DIRECTORY_ENTRY_IMPORT for imp in entry.imports if imp.address==0x1440158]
    assert imports==[(b'msvcr71.dll',b'malloc')]
    for address,slot,name in [(0xbfea1a,0x1440150,b'??2@YAPAXI@Z'),(0xbfe9bc,0x1440164,b'??3@YAXPAX@Z')]:
        import struct
        assert pe.get_data(address-0x400000,6)==b'\xff\x25'+struct.pack('<I',slot)
        assert [(entry.dll.lower(),imp.name)for entry in pe.DIRECTORY_ENTRY_IMPORT for imp in entry.imports if imp.address==slot]==[(b'msvcr71.dll',name)]
exe=w/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_DATA_PASS allocations=768'in r.stdout,(r.returncode,r.stdout,r.stderr)
report=dict(data_bytes=18,allocations=768,omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),boundary='Verified singleton zero image and extension literals; descriptive malloc symbol aliases real CRT import target. Data/import integration, no new function coverage.')
(ROOT/'rebuild/compile-gate/installer-data.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
