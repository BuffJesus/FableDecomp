"""Synthetic reader tests and optional frozen-object differential audit.

No source compilation/linking/execution. Objects are read-only inputs. Existing
historical receipts remain unchanged: any corrected-reader evidence is new.
"""
import argparse,hashlib,importlib.util,json,struct,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'tools/decomp_pipeline'))

def load(path,name):
 spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module);return module.Coff

def synthetic(path,raw_pointer,raw_size,virtual_size,flags,payload=b'',relocate=False):
 # One symbol/node at offset3 and optional relocation to that exact symbol.
 # Preserve a distinct header prefix so legacy BSS slicing cannot pass.
 symbol_pointer=60+len(payload)+(10 if relocate else 0)
 header=struct.pack('<HHIIIHH',0x14c,1,0x12345678,symbol_pointer,1,0,0)
 section=struct.pack('<8sIIIIIIHHI',b'.test\0\0\0',virtual_size,0,raw_size,raw_pointer,60+len(payload) if relocate else 0,0,1 if relocate else 0,0,flags)
 reloc=struct.pack('<IIH',0,0,6) if relocate else b''
 symbol=struct.pack('<8sIhHBB',b'node\0\0\0\0',3,1,0,2,0)
 path.write_bytes(header+section+payload+reloc+symbol+struct.pack('<I',4))

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--reader',type=Path,default=R/'tools/decomp_pipeline/gate_game_component_abi.py');ap.add_argument('--baseline',type=Path);ap.add_argument('--objects',type=Path,nargs='*',default=[]);ap.add_argument('--report',type=Path);args=ap.parse_args()
 sys.path.insert(0,str(args.baseline.parent if args.baseline else args.reader.parent))
 paths=[Path(__file__),args.reader,*args.objects]+([args.baseline] if args.baseline else [])
 before={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}
 Coff=load(args.reader,'corrected_coff_reader');Legacy=load(args.baseline,'baseline_coff_reader') if args.baseline else None
 cases=[]
 with tempfile.TemporaryDirectory(prefix='coff-reader-tests-') as temp:
  p=Path(temp)/'synthetic.obj'
  for size,virtual in ((0,0),(1,0),(4096,8192)):
   synthetic(p,0,size,virtual,0xc0000080);c=Coff(p)
   assert c.sections=={1:(b'',{})} and c.symbols=={'node':(1,3)} and c.indices=={0:'node'}
   assert c.section_metadata=={1:dict(raw_size=size,raw_pointer=0,virtual_size=virtual,characteristics=0xc0000080)}
   cases.append(dict(kind='BSS',size=size,virtual_size=virtual))
  # No raw pointer is payload-less even if flags are not BSS: do not guess
  # zero initialization or infer section payload from characteristics alone.
  synthetic(p,0,8,3,0x60000020);assert Coff(p).sections[1][0]==b'';cases.append(dict(kind='rawptr0_nonBSS'))
  for payload,flags in ((b'\x00\x00\x00\x00\xc3',0x60000020),(b'\x00\x00\x00\x00\xfe\xff',0xc0000040)):
   synthetic(p,60,len(payload),123,flags,payload,True);c=Coff(p)
   assert c.sections=={1:(payload,{0:('node',6)})} and c.symbols=={'node':(1,3)} and c.indices=={0:'node'}
   assert c.section_metadata[1]==dict(raw_size=len(payload),raw_pointer=60,virtual_size=123,characteristics=flags)
   if Legacy:
    old=Legacy(p);assert old.sections==c.sections and old.symbols==c.symbols and old.indices==c.indices
   cases.append(dict(kind='initialized',flags=hex(flags),payload_sha256=hashlib.sha256(payload).hexdigest()))
 audits=[]
 for path in args.objects:
  c=Coff(path);old=Legacy(path) if Legacy else None;initialized=[];uninitialized=[]
  if old:assert c.symbols==old.symbols and c.indices==old.indices and c.sections.keys()==old.sections.keys()
  for number,(payload,relocations) in c.sections.items():
   metadata=c.section_metadata[number]
   if metadata['raw_pointer']:
    assert len(payload)==metadata['raw_size']
    if old:assert c.sections[number]==old.sections[number]
    initialized.append(number)
   else:
    assert payload==b''
    if old:assert relocations==old.sections[number][1]
    uninitialized.append(dict(section=number,metadata=metadata,legacy_payload_bytes=len(old.sections[number][0]) if old else None))
  audits.append(dict(path=str(path),initialized_sections_unchanged=initialized,payloadless_sections=uninitialized))
 after={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths};assert before==after
 report=dict(status='NATIVE_COFF_READER_PASS',synthetic_cases=len(cases),cases=cases,frozen_object_audits=audits,input_sha256_before=before,input_sha256_after=after,boundary='Reader-only new audit; no compilation or executable run. Rawptr0 empty file payload, not fabricated zero initializer. Additive section metadata preserves raw/virtual extent. Symbol/relocation behavior unchanged; historical object receipts/tool hashes not rewritten.')
 if args.report:args.report.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report,indent=2))

if __name__=='__main__':main()
