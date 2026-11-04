"""Fresh native bank recovery path/open/reader and owned stream/disk lifetime."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,subprocess,hashlib
import check_one as V
from file_input_stream_sources import SOURCES
import gate_native_bank_recovery_path_lifetime as P
R=Path(__file__).resolve().parents[2]
BASE=R/'rebuild/build/native-bank-recovery-path-lifetime/00/9c/009cbfe0/20261020T001219980117Z/acceptance.json'
SCRIPTS=[R/'tools/decomp_pipeline'/n for n in ('native_bank_open_wrapper_build.py','native_bank_recovery_owned_build.py','native_bank_open_file_verify.py','native_bank_open_wrapper_verify.py','native_bank_recovery_path_verify.py','native_bank_recovery_owned_verify.py','native_bank_recovery_path_controls.py','native_bank_recovery_owned_controls.py')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sha(BASE)=='0b5a70ec12b435ef97cc6371aa42d99fc94fbecb8e9bd93b9a314b49eb902c3e'
 previous=json.loads(BASE.read_text())
 inputs={Path(p) for p in previous['input_sha256']}|{BASE,Path(__file__),*SCRIPTS,*SOURCES,
  R/'rebuild/tests/integration/NativeBankRecoveryOwned_test.cpp',R/'rebuild/tests/integration/NativeBankRecoveryColdAliases.h',R/'rebuild/tests/integration/NativeBankRecovery_model.py',R/'rebuild/src/compiled/00/9d/CBankFile_ReadRecoveryFile_009d0380.cpp'}
 inputs.update(R/'rebuild/src/compiled/00/99'/n for n in ('CDiskFileWin32_Read_0099a960.cpp','CDiskFileWin32_SetPosition_0099abf0.cpp','CDiskFileWin32_GetPosition_0099abd0.cpp'))
 inputs.update(p for p in (R/'rebuild/include').rglob('*') if p.is_file())
 inputs.add(R/'tools/decomp_pipeline/file_input_stream_sources.py')
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-recovery-owned/00/9d/009d0380'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
 def save(name,data):
  q=b/name;assert not q.exists();q.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for script in SCRIPTS:
  run=subprocess.run([sys.executable,str(script),str(b/'parent' if script.name=='native_bank_open_wrapper_build.py' else b)],cwd=R,capture_output=True,text=True,timeout=240)
  save(script.stem+'.run.json',dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr));assert not run.returncode,run.stdout+run.stderr;print(script.stem+' PASS',flush=True)
 report=json.loads((b/'receipt.json').read_text());reader=json.loads((b/'reader-verification.json').read_text());core=json.loads((b/'linked-verification.json').read_text());wrapper=json.loads((b/'wrapper-verification.json').read_text())
 assert report['fresh'] and len(report['objects'])==157 and report['recovery_cases']==216
 assert core['linked_bodies']==153 and core['fixups']==713 and wrapper['wrapper_bodies']==18 and wrapper['wrapper_fixups']==88
 assert reader['recovery_bytes']==679 and reader['recovery_fixups']==29 and len(reader['stream_bodies'])==8
 controls=json.loads((b/'controls-verification.json').read_text())['controls'];path_controls=json.loads((b/'recovery-controls.json').read_text())['controls'];assert len(controls)==9 and len(path_controls)==3
 command=json.loads((b/'link.json').read_text())['command'];omissions=[];redundant=[]
 required={b/'new0.obj',*(b/('recovery%d.obj'%n) for n in range(13) if n!=2)}
 for n,name in enumerate(report['objects']):
  obj=Path(name)
  if obj.name=='recovery13.obj':continue
  exe=b/('omit%d.exe'%n);cmd=['/out:'+str(exe) if s.startswith('/out:') else s for s in command if s!=name and not s.startswith('/map:')]
  result=subprocess.run(cmd,cwd=b,env=V.env(),capture_output=True,text=True,timeout=30);save('omit%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));(omissions if result.returncode else redundant).append(name)
  if obj in required and not result.returncode:
   run=subprocess.run([str(exe),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=15);save('omit%d.run.json'%n,dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr));assert run.returncode==80 and 'NATIVE_WORKER_FAILURE' in run.stderr,name
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p,h in report['objects'].items():assert sha(Path(p))==h
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 fixups=830+reader['stream_fixups']+29+sum(d['fixups'] for d in reader['disk_bodies'] if d['bytes']!=52)
 final=dict(status='PASS',build=str(b),fresh=True,processes=24,recovery_cases=216,reads=24,bytes=888,linked_bodies=182,fixups=fixups,controls=controls+path_controls,oracle_controls=reader['model']['controls'],link_omissions=omissions,redundant_objects=redundant,input_sha256=pins,object_sha256=report['objects'],executable_sha256=sha(b/'probe.exe'),new_roots=0,
  boundary='Native recovery path/existence/disk Open/Read/Seek/Position/Close, exact679B reader and native buffered lifetime. Entry-list decoding and bank header parser controlled. All157 linked providers fresh.216 recovery cases include missing file, bad signature/validation, ignored entry-reader false, buffer crossing and exclusive reopen after cleanup. Real CreateFileW observer forwards calls and checks flags. Prior87 wrapper controls remain previous evidence. Existing near-match roots retain grades; no exception-unwinding claim or game/save changes.')
 save('acceptance.json',final);(R/'rebuild/compile-gate/native-bank-recovery-owned.json').write_text(json.dumps(final,indent=2)+'\n');print('NATIVE_BANK_RECOVERY_OWNED_PASS processes=24 recovery=216 bodies=182 controls=16')

if __name__=='__main__':main()
