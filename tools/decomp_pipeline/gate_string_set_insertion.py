"""Validate string-set insertion with actual comparison and red-black balancing."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from string_set_insertion_oracle import compare
ROOT=Path(__file__).resolve()
while not(ROOT/'rebuild').is_dir():ROOT=ROOT.parent
FILES=['STLSetString_InsertUnique_004ac380.cpp','STLSetString_InsertNode_004abc70.cpp']
SPECS=[(0x4ac380,'?insert_unique@',254,254,0),(0x4abc70,'?_M_insert@',206,206,1),(0x4aa220,'?_M_decrement@_Rb_tree_base_iterator@',78,79,0),(0x429950,'??M?$CBasicString@D@@',88,88,0),(0x42971d,'?_Rebalance@',190,190,2),(0x42951b,'?_Rotate_left@',64,64,2),(0x42955b,'?_Rotate_right@',64,64,2)]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--source-dir',type=Path,default=ROOT/'rebuild/src/compiled/00/4a');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/StringSetInsertion_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/string-set-insertion.json');a=ap.parse_args()
 sources=[a.source_dir/f for f in FILES]+[ROOT/'rebuild/src/compiled/00/42/STL_Rb_global_Rebalance_0042971d.cpp'];w=Path(tempfile.mkdtemp(prefix='string-set-insertion-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];coffs=[]
 for i,p in enumerate(sources+[a.test]):
  obj=w/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);coffs.append(Coff(obj))
 selected=[];targets={'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'_malloc':0xbfea0e}
 for addr,prefix,size,nativeSize,index in SPECS:
  names=[s for s in coffs[index].symbols if s.startswith(prefix)];assert len(names)==1;symbol=names[0];selected.append((addr,symbol,size,nativeSize,index));targets[symbol]=addr
 candidate={};native={};records=[]
 with pefile.PE(str(EXE)) as pe:
  for addr,symbol,size,nativeSize,index in selected:
   sec,off=coffs[index].symbols[symbol];assert off==0;b,rel=coffs[index].sections[sec];assert len(b)==size;raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,nativeSize);patched=bytearray(b);checks=[]
   for site,(name,kind) in rel.items():
    assert kind==20 and b[site:site+4]==bytes(4);target=targets[name];value=(target-addr-site-4)&0xffffffff;assert struct.unpack_from('<I',raw,site)[0]==value;struct.pack_into('<I',patched,site,value);checks.append(dict(offset=site,symbol=name,target=target))
   similarity=near_match.compare(b,raw,list(rel))
   # The compiler-emitted predecessor is behavior-checked but is NOT a
   # catalog landing: 78 vs 79 bytes, instruction delta just exceeds NEAR.
   if addr!=0x4aa220:assert patched==raw
   candidate[addr]=patched;native[addr]=raw;records.append(dict(address='%08x'%addr,symbol=symbol,bytes=size,native_bytes=nativeSize,grade='HELPER_BEHAVIOR_ONLY' if addr==0x4aa220 else 'RELOCATION_MATCH',relocations=checks,similarity=similarity.to_dict(),catalog_entry=addr in (0x4ac380,0x4abc70)))
 comparisons=compare(candidate,native);assert all(s not in coffs[-1].symbols for _,s,_,_,_ in selected[:2])
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(3):
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',omit)
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'STRING_SET_INSERTION_BEHAVIOR_PASS scenarios=1088' in r.stdout,r.stdout+r.stderr
 report=dict(entries=records,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=[dict(source=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sources],fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=3,boundaries=['malloc','CCharString copy construction'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('STRING_SET_INSERTION_PASS compiled=1088 comparative='+str(comparisons)+' providers=3')
if __name__=='__main__':main()
