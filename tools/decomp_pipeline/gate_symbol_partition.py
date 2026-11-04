"""Validate native symbol partition and pair swap with string ownership."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from symbol_partition_oracle import compare
ROOT=Path(__file__).resolve()
while not(ROOT/'rebuild').is_dir():ROOT=ROOT.parent
FILES=['STL_Swap_SymbolEntry_009e6480.cpp','STL_Partition_SymbolEntry_009e6c80.cpp']
SPECS=[(0x9e6480,'??$swap@VCCharString@@',168,0),(0x9e6c80,'??$__unguarded_partition@',133,1),(0x9e6530,'?FastCompare@CCharString@@QBE_NABV1@@Z',108,2)]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--source-dir',type=Path,default=ROOT/'rebuild/src/compiled/00/9e');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/SymbolPartitionChain_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/symbol-partition.json');a=ap.parse_args()
 sources=[a.source_dir/f for f in FILES]+[ROOT/'rebuild/src/compiled/00/9e/CCharString_FastCompare_009e6530.cpp'];w=Path(tempfile.mkdtemp(prefix='symbol-partition-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];coffs=[]
 for i,p in enumerate(sources+[a.test]):
  obj=w/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);coffs.append(Coff(obj))
 selected=[];targets={'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??1CCharString@@QAE@XZ':0x99eae0,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0}
 for addr,prefix,size,index in SPECS:
  matches=[s for s in coffs[index].symbols if s.startswith(prefix)];assert len(matches)==1;symbol=matches[0];selected.append((addr,symbol,size,index));targets[symbol]=addr
 candidate={};native={};records=[]
 with pefile.PE(str(EXE)) as pe:
  for addr,symbol,size,index in selected:
   sec,off=coffs[index].symbols[symbol];assert off==0;b,rel=coffs[index].sections[sec];assert len(b)==size;raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,size);patched=bytearray(b);checks=[]
   for site,(name,kind) in rel.items():
    assert kind==20 and b[site:site+4]==bytes(4);target=targets[name];value=(target-addr-site-4)&0xffffffff;assert struct.unpack_from('<I',raw,site)[0]==value;struct.pack_into('<I',patched,site,value);checks.append(dict(offset=site,symbol=name,target=target))
   similarity=near_match.compare(b,raw,list(rel));assert similarity.near_match
   if index!=2:assert patched==raw
   candidate[addr]=patched;native[addr]=raw;records.append(dict(address='%08x'%addr,symbol=symbol,bytes=size,grade='NEAR_MATCH' if index==2 else 'RELOCATION_MATCH',relocations=checks,similarity=similarity.to_dict()))
 comparisons=compare(candidate,native);assert all(s not in coffs[-1].symbols for _,s,_,_ in selected)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(3):
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode, ("provider omission linked",omit)
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'SYMBOL_PARTITION_CHAIN_BEHAVIOR_PASS scenarios=768' in r.stdout,r.stdout+r.stderr
 report=dict(entries=records,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=[dict(source=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sources],fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=3,boundaries=['CCharString copy, assignment, destruction'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('SYMBOL_PARTITION_PASS compiled=768 comparative='+str(comparisons)+' providers=3')
if __name__=='__main__':main()
