"""Validate pointer fill-insert and its complete native growth/fill chain."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,capstone,check_one,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from shader_pointer_fill_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
INSERT='?_M_fill_insert@?$vector@PAVCShaderBase@@V?$allocator@PAVCShaderBase@@@_STL@@@_STL@@QAEXPAPAVCShaderBase@@IABQAV3@@Z'
FILL='??$fill@PAPAVCShaderBase@@PAV1@@_STL@@YAXPAPAVCShaderBase@@0ABQAV1@@Z'
OVERFLOW='?_M_insert_overflow@?$vector@PAVCShaderBase@@V?$allocator@PAVCShaderBase@@@_STL@@@_STL@@QAEXPAPAVCShaderBase@@ABQAV3@ABU__true_type@2@I_N@Z'
SPECS=[(0xa5e3b0,INSERT,276),(0xa5db30,FILL,31),(0xa5e1b0,OVERFLOW,216)]
TARGETS={symbol:entry for entry,symbol,length in SPECS};TARGETS.update({'_malloc':0xbfea0e,'_free':0xbfea14,'_memmove':0xbfeae6})
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a5/CShaderResource_PointerFillInsert_00a5e3b0.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a5/CShaderResource_PointerFillInsert_00a5e3b0_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/shader-pointer-fill.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='shader-pointer-fill-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 c=Coff(obj);f=Coff(test);assert all(symbol not in f.symbols for entry,symbol,length in SPECS)
 candidate={};native={};records=[];md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
 with pefile.PE(str(EXE)) as pe:
  for entry,symbol,length in SPECS:
   sec,off=c.symbols[symbol];assert off==0;built,relocs=c.sections[sec];original=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
   for site,(name,kind) in sorted(relocs.items()):
    assert kind==20 and built[site:site+4]==b'\0'*4;target=TARGETS[name];struct.pack_into('<I',patched,site,(target-entry-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target))
   score=near_match.compare(built,original,list(relocs),[])
   if entry==0xa5e3b0:
    assert score.near_match and len(built)==289
    calls=[i.operands[0].imm for i in md.disasm(original,entry) if i.mnemonic=='call' and i.operands[0].type==capstone.x86_const.X86_OP_IMM];assert calls==[r['target'] for r in checked];grade='NEAR_MATCH'
   elif entry==0xa5db30:
    # Shared STLport fill is cdecl here; retail's helper is fastcall.
    # Only complete matching caller/helper pairs are executed. No separate ABI claim.
    assert not relocs and len(built)==30 and built[-1:]==b'\xc3' and original[-3:]==b'\xc2\x04\x00';grade='INTERNAL_ABI_VARIANT'
   else:assert patched==original;grade='RELOCATION_MATCH'
   candidate[entry]=patched;native[entry]=original;records.append(dict(address='%08x'%entry,grade=grade,built_bytes=len(built),retail_bytes=length,similarity=score.to_dict(),relocations=checked))
 comparative=compare(candidate,native)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(command+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(command+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'SHADER_POINTER_FILL_BEHAVIOR_PASS scenarios=2736' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(entries=records,behavior=r.stdout.strip(),comparative_scenarios=comparative,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('SHADER_POINTER_FILL_PASS compiled=2736 comparative='+str(comparative))
if __name__=='__main__':main()
