import json,sys,tempfile,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
from gate_game_component_abi import Coff
work=Path(tempfile.mkdtemp(prefix='installer-active-list-'));obj=work/'list.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/9c/InstallerActiveFileList_Clear_009c71a0.cpp')],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
symbol='?Clear@InstallerActiveFileList@@QAEXXZ';c=Coff(obj);section,offset=c.symbols[symbol];body,relocs=c.sections[section]
print('bytes='+str(len(body))+' relocations='+str(len(relocs)))
report=check(obj,symbol,0x9c71a0,264,7,{'_free':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc,'??1CCompressorZlib@@UAE@XZ':0xa3acc0})
(ROOT/'rebuild/compile-gate/installer-active-file-list-parity.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
source=ROOT/'rebuild/tests/integration/InstallerActiveFileListClear_test.cpp';fixture=work/'fixture.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fixture),str(source)],check_one.env(),work);assert r.returncode==0,r.stdout+r.stderr
blob=obj.read_bytes()
for old,new in [('_free','_Fnod'),('??3@YAXPAX@Z','_InfoDelete'),('??1CCompressorZlib@@UAE@XZ','@CDtor@4')]:
 needle=old.encode()+b'\0';value=new.encode()+b'\0';assert blob.count(needle)==1 and len(value)<=len(needle);blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
obj.write_bytes(blob)
exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(link+[str(fixture)],env=check_one.env(),capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(link+[str(obj),str(fixture)],env=check_one.env(),capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_ACTIVE_LIST_OK cases=1080' in r.stdout,(r.returncode,r.stdout,r.stderr)
(ROOT/'rebuild/compile-gate/installer-active-file-list-behavior.json').write_text(json.dumps(dict(cases=1080,distinct_cases=135,omitted_provider_checks=1,boundary='nested/outer nodes actual malloc/free behind recording release hooks; compressor/object destruction recorded; exact cleanup event order, cached outer next and fresh file reference after compressor callback; absent/count0/1/2/ffffffff controls'),indent=2)+'\n')
print(r.stdout.strip())
