"""Validate creature combat-definition getter and retained component getter."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile,near_match
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE
from creature_combat_oracle import compare
ROOT=check_one.ROOT
DATA='?PCurrentThingManager@CThing@@0PAVCThingManager@@A'
SPECS=[(0x664d40,'?GetPCombatTypeDef@CThingCreatureBase@@',268,0),(0x436990,'?GetTC@CThing@@QBE',45,0),(0x4365b0,'?LowerBound@?$CVectorMap@W4ETCInterfaceType@@',60,1),(0x4c7990,'?PeekDefinitionManager@CThing@@',9,2),(0x4c74f0,'?SetCurrentThingManager@CThing@@',7,3)]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/66/CThingCreatureBase_GetPCombatTypeDef_00664d40.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CreatureCombatGetter_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/creature-combat.json');a=ap.parse_args()
 sources=[a.source,ROOT/'rebuild/src/compiled/00/43/CVectorMapThingComponents_LowerBoundConst_004365b0.cpp',ROOT/'rebuild/src/compiled/00/4c/CThing_PeekDefinitionManager_004c7990.cpp',ROOT/'rebuild/src/compiled/00/4c/CThing_SetCurrentThingManager_004c74f0.cpp']
 work=Path(tempfile.mkdtemp(prefix='creature-combat-gate-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[]
 for i,p in enumerate(sources+[a.test]):
  obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);coffs.append(Coff(obj))
 selected=[];targets={DATA:0x13b8a1c}
 for addr,prefix,size,index in SPECS:
  matches=[s for s in coffs[index].symbols if s.startswith(prefix)];assert len(matches)==1,(addr,matches);selected.append((addr,matches[0],size,index));targets[matches[0]]=addr
 literal=next(s for s in coffs[0].symbols if s.startswith('??_C@_0BB@') and 'COMBAT_TYPE_NULL' in s);targets[literal]=0x125aba0
 targets.update({'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??1CCharString@@QAE@XZ':0x99eae0})
 for s in coffs[0].symbols:
  if s.startswith('??$GetDef@VCCombatTypeDef@@'):
   targets[s]=0x669770 if 'ABVCCharString' in s else 0x669640
 candidate={};native={};records=[]
 with pefile.PE(str(EXE)) as pe:
  sec,off=coffs[0].symbols[literal];data,rel=coffs[0].sections[sec];assert off==0 and not rel and data==pe.get_data(0x125aba0-pe.OPTIONAL_HEADER.ImageBase,17)==b'COMBAT_TYPE_NULL\0'
  assert coff_data(coffs[3],DATA,4)==bytes(4)
  for addr,sym,size,index in selected:
   sec,off=coffs[index].symbols[sym];assert off==0;built,rel=coffs[index].sections[sec];patched=bytearray(built);checks=[]
   for site,(name,kind) in rel.items():
    assert built[site:site+4]==bytes(4),(sym,site)
    target=(0x669770 if 'ABVCCharString' in name else 0x669640) if name.startswith('??$GetDef@VCCombatTypeDef@@') else targets[name];assert kind in (6,20),(sym,name,kind)
    value=(target-addr-site-4)&0xffffffff if kind==20 else target
    struct.pack_into('<I',patched,site,value);checks.append(dict(offset=site,symbol=name,target=target))
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,size);similarity=near_match.compare(built,raw,list(rel));assert similarity.near_match,(sym,similarity.to_dict())
   exact=patched==raw
   if addr!=0x664d40:assert exact,(sym,len(built),size)
   candidate[addr]=patched;native[addr]=raw;records.append(dict(address='%08x'%addr,symbol=sym,retail_bytes=size,built_bytes=len(built),grade='RELOCATION_MATCH' if exact else 'NEAR_MATCH',similarity=similarity.to_dict(),relocations=checks))
 cases=compare(candidate,native)
 assert not any(sym in coffs[-1].symbols for _,sym,_,_ in selected)
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(4):
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',omit)
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'CREATURE_COMBAT_GETTER_BEHAVIOR_PASS cases=2592' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=records,comparative_scenarios=cases,behavior=r.stdout.strip(),providers=[dict(source=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sources],fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=4,header_sha256={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in ('fable_creature_combat.h','fable_thing_components.h')},boundaries=['CCharString constructor/destructor','CDefinitionManager GetDef index/name','definition virtual deletion'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CREATURE_COMBAT_PASS native='+str(cases)+' compiled=2592 providers=4')
if __name__=='__main__':main()
