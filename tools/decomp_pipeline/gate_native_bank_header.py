"""Canonical native bank-header source, operand proof and native/model composition."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess
import check_one as V
from native_bank_header_sources import SOURCES,HEADER,HEADER_INDEX
from pe_oracle import EXE
R=Path(__file__).resolve().parents[2]
HELPERS=['native_bank_header_sources.py','native_bank_header_valid.py','native_bank_header_disk.py','native_bank_header_disk_verify.py','native_bank_header_oracle.py','native_bank_header_literals.py']
MODELS=['NativeBankHeader_model.py','NativeBankHeaderThrow_model.py','NativeBankHeaderMutation_model.py']
FIXTURES=['NativeBankHeaderValid_test.cpp','NativeBankHeaderDisk_test.cpp']
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 b=R/'rebuild/build/native-bank-header/00/9d/009d0880'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inputs={Path(__file__),EXE,*[R/'tools/decomp_pipeline'/p for p in HELPERS],*[R/'rebuild/tests/integration'/p for p in MODELS]};seen=set()
 def closure(p):
  p=p.resolve()
  if p in seen:return
  seen.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()),None);assert q is not None,(str(p),name);closure(q)
 for p in SOURCES+[R/'rebuild/tests/integration'/p for p in FIXTURES]+[R/'rebuild/src/compiled/00/9d/CBankFileException_Constructor_009d1e70.cpp']:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));print('NATIVE_BANK_HEADER_BUILD',b,flush=True)
 def invoke(name,args,timeout=180):
  command=[sys.executable,*map(str,args)];r=subprocess.run(command,cwd=R,capture_output=True,text=True,timeout=timeout);(b/(name+'.run.json')).write_text(json.dumps(dict(command=command,code=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));print(r.stdout.strip(),flush=True);assert r.returncode==0,(name,r.returncode,r.stdout,r.stderr)
 valid=b/'valid';invoke('valid',[R/'tools/decomp_pipeline/native_bank_header_valid.py',valid]);obj=valid/('root%d.obj'%HEADER_INDEX)
 invoke('literals',[R/'tools/decomp_pipeline/native_bank_header_literals.py',b,obj]);literal=json.loads((b/'literal-correspondence.json').read_text());assert literal['literal_verified_grade']['near_match'] and len(literal['controls'])==11 and literal['semantic_control']['semantic_oracle_rejected']
 model_dir=b/'models';model_dir.mkdir();command="import sys;from pathlib import Path;sys.path.insert(0,str(Path(sys.argv[1])));import native_bank_header_oracle as M;import json;r=M.run(sys.argv[2],sys.argv[3]);Path(sys.argv[3],'receipt.json').write_text(json.dumps(r,indent=2))";invoke('models',['-c',command,R/'tools/decomp_pipeline',obj,model_dir])
 disk=b/'disk';invoke('disk',[R/'tools/decomp_pipeline/native_bank_header_disk.py',disk,valid]);invoke('disk-proof',[R/'tools/decomp_pipeline/native_bank_header_disk_verify.py',disk,obj,valid/'include'])
 for p,h in pins.items():assert sha(p)==h,('input drift',p)
 native=json.loads((valid/'receipt.json').read_text());assert native['original_header_cases']==3840;assert native['host']['entries']==24520 and native['host']['updates']==12432
 for p,h in native['input_sha256'].items():assert sha(p)==h,('native input drift',p)
 for p,h in native['object_sha256'].items():assert sha(p)==h,('native object drift',p)
 disk_report=json.loads((disk/'receipt.json').read_text());proof=json.loads((disk/'body-proof.json').read_text());assert disk_report['cases']==64 and len(disk_report['controls'])==11 and disk_report['missing_header_rejected'] and disk_report['duplicate_constructor_rejected'];assert proof['header_bytes']==1615 and proof['constructor_bytes']==33
 for p,h in disk_report['object_sha256'].items():assert sha(p)==h
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',build=str(b),input_sha256=pins,source_sha256=sha(HEADER),header_bytes=1615,raw_grade=literal['raw_grade'],verified_literal_grade=literal['literal_verified_grade'],literal_proof=literal,models=json.loads((model_dir/'receipt.json').read_text()),valid_native=native,disk=disk_report,disk_body_proof=proof,boundary='Canonical genuine C++ header provider with same-TU existing33-byte exception ctor, one selected constructor provider. Existing grading unchanged;11 exact full-literal immediate operands separately proven,11 target faults and independent semantic-swap rejection.1512 nonthrow/30 throw-boundary/648 helper-mutation comparisons;3840 valid cases per recreated/original header with native parser/allocation/indexing/packing/queries/strings and64 native disk nonthrow cases/11 source faults. Fixture bank layout/tree headers/manual vector teardown, base/notifications/backing-handle adoption controlled. No native exception unwinding, full Open/lifecycle, arbitrary malformed-file or whole-game claim.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-bank-header.json').write_text(json.dumps(report,indent=2));print('NATIVE_BANK_HEADER_GATE_PASS header=1615 nonthrow=1512 throw=30 mutations=648 valid=3840 original=3840 disk=64 controls=11 literal_controls=11 semantic_controls=1 source_controls=2 near=1 ctor_exact=1')
if __name__=='__main__':main()
