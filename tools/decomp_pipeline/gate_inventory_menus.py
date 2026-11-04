"""Validate five original inventory menu handlers through the real component lookup."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
import gate_inventory_events as G
ROOT=G.ROOT
SPECS=[dict(address=a,file='CWorld_'+m+'_'+a+'.cpp',symbol='?'+m+'@CWorld@@QAEXPBVCGameEvent@@@Z',size=80,grade='RELOCATION_MATCH',leaf=target,method=m,inventory_method=leaf) for a,m,leaf,target in [('0062c640','EAOpenPCScoreboard','OpenPCScoreboard',0x587b93),('0062c690','EAOpenPCSkillsMenu','OpenPCSkillsMenu',0x587bff),('0062c700','EAOpenPCInventoryMenu','OpenPCInventoryMenu',0x58807d),('0062c750','EAOpenPCOptionsMenu','OpenPCOptionsMenu',0x58af10),('0062c7a0','EAOpenPCPhotoJournalMenu','OpenPCPhotojournal',0x58f9bc)]]
EXTRA=G.EXTRA+[G.SPECS[-1]]
TARGETS=dict(G.TARGETS)
TARGETS.update({s['symbol']:int(s['address'],16) for s in SPECS})
TARGETS.update({'?'+s['inventory_method']+'@CTCInventory@@QAEXXZ':s['leaf'] for s in SPECS})

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/InventoryMenus_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/inventory-menus.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='inventory-menus-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<len(SPECS) else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0;leaves=[s['leaf'] for s in SPECS]
    for value in range(256):
        for s in SPECS:
            for has in [False,True]:
                for keys in G.MAPS:
                    for pattern in range(2):assert G.execute(candidate,s,value,has,keys,pattern,inventory_leaves=leaves)==G.execute(native,s,value,has,keys,pattern,inventory_leaves=leaves);scenarios+=1
        if value%32==0:print('INVENTORY_MENUS_PROGRESS comparisons=%d'%scenarios,flush=True)
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(8):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'INVENTORY_MENUS_BEHAVIOR_PASS scenarios=51400' in r.stdout,r.stdout+r.stderr
    report=dict(entries=SPECS,providers=providers,comparative_scenarios=scenarios,compiled_scenarios=51400,missing_provider_checks=8,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={'fable_thing_components.h':hashlib.sha256((a.include/'fable_thing_components.h').read_bytes()).hexdigest()});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('INVENTORY_MENUS_PASS providers=8 compiled=51400 comparative=%d'%scenarios)
if __name__=='__main__':main()
