"""Validate native definition-name lookup with real CRC and symbol providers."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from name_index_oracle import compare
ROOT=Path(__file__).resolve()
while not(ROOT/'rebuild').is_dir():ROOT=ROOT.parent
FILES=['CVectorMapNameIndex_LowerBoundConst_009b21f0.cpp','CDefinitionManager_GetDefGlobalIndexFromName_009ad410.cpp']
SPECS=[(0x9b21f0,'?LowerBound@',60,0),(0x9ad410,'?GetDefGlobalIndexFromName@',141,1),(0x4014a0,'?FableCRC_Calc_004014A0@@',56,2),(0x9e6910,'?LowerBound@',77,3),(0x9e6530,'?FastCompare@',108,4),(0x9e5170,'?FindSymbol@',76,5)]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--stage',type=Path);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/NameIndexChain_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/name-index.json');a=ap.parse_args()
 sources=[(a.stage if a.stage else ROOT/'rebuild/src/compiled'/('00/9b' if i==0 else '00/9a'))/f for i,f in enumerate(FILES)]
 sources += [ROOT/'rebuild/src/compiled'/p for p in ['00/40/Global_CCRC_Calc_004014a0.cpp','00/9e/CVectorMap_SymbolMap_LowerBound_009e6910.cpp','00/9e/CCharString_FastCompare_009e6530.cpp','00/9e/CSymbolMap_FindSymbol_009e5170.cpp']]
 w=Path(tempfile.mkdtemp(prefix='name-index-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[]
 for i,p in enumerate(sources+[a.test]):
  obj=w/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);coffs.append(Coff(obj))
 selected=[];targets={'g_FableCRCTable_0129A168':0x129a168,'?g_FableCRCTable_0129A168@@3PAIA':0x129a168}
 for addr,prefix,size,index in SPECS:
  matches=[s for s in coffs[index].symbols if s.startswith(prefix)];assert len(matches)==1,matches;symbol=matches[0];selected.append((addr,symbol,size,index));targets[symbol]=addr
 candidate={};native={};records=[]
 with pefile.PE(str(EXE)) as pe:
  table=pe.get_data(0x129a168-pe.OPTIONAL_HEADER.ImageBase,1024)
  words=[]
  for i in range(256):
   x=i
   for bit in range(8):x=(x>>1)^0xedb88320 if x&1 else x>>1
   words.append(x)
  assert table==struct.pack('<256I',*words)
  for addr,symbol,size,index in selected:
   sec,off=coffs[index].symbols[symbol];assert off==0;b,rel=coffs[index].sections[sec];raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,size);patched=bytearray(b);checks=[]
   for site,(name,kind) in rel.items():
    assert b[site:site+4]==bytes(4);target=targets[name];value=(target-addr-site-4)&0xffffffff if kind==20 else target;assert kind in (6,20)
    struct.pack_into('<I',patched,site,value);checks.append(dict(offset=site,symbol=name,target=target))
   similarity=near_match.compare(b,raw,list(rel));assert similarity.near_match,(symbol,similarity.to_dict())
   exact=patched==raw
   if index not in (1,4):assert exact,(symbol,len(b),size)
   candidate[addr]=patched;native[addr]=raw;records.append(dict(address='%08x'%addr,symbol=symbol,bytes=size,built_bytes=len(b),grade='RELOCATION_MATCH' if exact else 'NEAR_MATCH',relocations=checks,similarity=similarity.to_dict()))
 comparisons=compare(candidate,native,table);assert all(s not in coffs[-1].symbols for _,s,_,_ in selected)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(6):
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',omit)
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'NAME_INDEX_CHAIN_BEHAVIOR_PASS scenarios=3060' in r.stdout,r.stdout+r.stderr
 report=dict(entries=records,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=[dict(source=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sources],fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=6,boundaries=[],header_sha256=hashlib.sha256((a.include/'fable_definition_manager.h').read_bytes()).hexdigest())
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('NAME_INDEX_PASS compiled=3060 comparative='+str(comparisons)+' providers=6')
if __name__=='__main__':main()
