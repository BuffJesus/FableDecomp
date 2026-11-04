"""Gate native IME defaults/reset/font fields and real CRT allocation callbacks."""
import json
import subprocess
import tempfile
from pathlib import Path
import pefile
import check_one as V
import oracle_native_ime_support as oracle
from gate_game_component_abi import Coff, DEFAULT_RETAIL
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check

ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-ime-support-'));env=V.env();objects=[]
sources=['rebuild/src/compiled/00/40/CIMEFont_Setters_00404940.cpp',
         'rebuild/src/compiled/00/40/CIME_AllocationCallbacks_00404a60.cpp',
         'rebuild/src/compiled/00/40/CIME_ResetToDefaultSettings_00404a90.cpp',
         'rebuild/src/compiled/00/40/CIME_Shutdown_004051c0.cpp',
         'rebuild/src/compiled/01/3b/CIME_SettingsData_013b7cd8.cpp',
         'rebuild/tests/integration/NativeIMESupport_test.cpp']
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
targets={'_malloc':0xbfea0e,'_free':0xbfea14,'?g_FableIME_013B7CD8@@3VCIME@NUISystem@@A':0x13b7cd8,
         '?ImeUi_FinalizeString@@YIX_N@Z':0xcb2c60,'?ImeUi_EnableIme@@YIX_N@Z':0xcb5130,
         '?ImeUi_Initialize@@YI_NPAUHWND__@@_N@Z':0xcb5180,'??3@YAXPAX@Z':0xbfe9bc,
         '?ResetToDefaultSettings@CIME@NUISystem@@SIXXZ':0x404a90}
specs=[(0,'?SetHeight@CIMEFont@NUISystem@@UAEXI@Z',0x404940,10,0),
       (0,'?SetPosition@CIMEFont@NUISystem@@UAEXHH@Z',0x404950,17,0),
       (1,'?Callback_malloc@CIME@NUISystem@@CIPAXI@Z',0x404a60,10,1),
       (1,'?Callback_free@CIME@NUISystem@@CIXPAX@Z',0x404a70,8,1),
       (2,'?ResetToDefaultSettings@CIME@NUISystem@@SIXXZ',0x404a90,147,19),
       (3,'?Shutdown@CIME@NUISystem@@QAEXXZ',0x4051c0,90,5)]
reports=[check(objects[i],s,a,n,c,targets)for i,s,a,n,c in specs]
symbol='?g_FableIME_013B7CD8@@3VCIME@NUISystem@@A'
assert coff_data(Coff(objects[4]),symbol,84)==bytes(84)
assert symbol not in Coff(objects[5]).symbols
with pefile.PE(str(DEFAULT_RETAIL))as pe:
    mapped=pe.get_memory_mapped_image()
    assert mapped[0x13b7cd8-pe.OPTIONAL_HEADER.ImageBase:0x13b7cd8-pe.OPTIONAL_HEADER.ImageBase+84]==bytes(84)
    imports={imp.address:imp.name for entry in pe.DIRECTORY_ENTRY_IMPORT for imp in entry.imports}
    for address,name in [(0xbfea0e,b'malloc'),(0xbfea14,b'free'),(0xbfe9bc,b'??3@YAXPAX@Z')]:
        import struct
        body=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,6)
        assert body[:2]==b'\xff\x25' and imports[struct.unpack_from('<I',body,2)[0]]==name
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(5):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True)
    assert r.returncode,(omit,'omitted production provider linked')
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30)
assert not r.returncode,(r.returncode,r.stdout,r.stderr,w)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,len(actual),len(expected)))
report.update(providers=reports,data_bytes=84,omission_checks=5,real_allocations=256,
              boundary='Native reset/defaults/font setters and singleton storage; IME API and operator delete recorded with callback mutation. Allocation callbacks execute actual CRT malloc/free. No IME rendering or visible frame.')
(ROOT/'rebuild/compile-gate/native-ime-support.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots')
