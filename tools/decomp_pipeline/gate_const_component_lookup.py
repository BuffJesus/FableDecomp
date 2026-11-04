"""Check const component-map lookup, signed keys and unchecked missing entries."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_inventory_events import MAPS,QUERIES
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
LOWER_SYMBOL='?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@_STL@@ABW4ETCInterfaceType@@@Z'
SPECS=[dict(address='004365b0',file='CVectorMapThingComponents_LowerBoundConst_004365b0.cpp',symbol=LOWER_SYMBOL,size=60,grade='MATCH'),dict(address='00436990',file='CThing_GetTCConst_00436990.cpp',symbol='?GetTC@CThing@@QBEABVCTCBase@@W4ETCInterfaceType@@@Z',size=45,grade='RELOCATION_MATCH')]
HEADERS=['fable_world.h','fable_vector_map.h']
def execute(codes,keys,query,pattern,present,getter):
    thing,table,stack,stop=0x2000040,0x2100040,0x3008000,0x4000000;u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{0x2000000,0x2100000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for address,code in codes.items():u.mem_write(address,bytes(code))
    regions={base:bytearray(bytes([pattern*71])*0x100) for base in [0x2000000,0x2100000]};struct.pack_into('<I',regions[0x2000000],0x60,0x20000 if present else 0);struct.pack_into('<III',regions[0x2000000],0x84,table,table+len(keys)*8,table+(len(keys)+1)*8)
    for i,key in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',regions[0x2100000],0x40+i*8,key&0xffffffff,0x3400040+i*0x20)
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,query&0xffffffff if getter else stack+0x80));u.mem_write(stack+0x80,struct.pack('<I',query&0xffffffff));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,thing if getter else thing+0x44);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    u.emu_start(0x436990 if getter else 0x4365b0,stop,count=200);index=next((i for i,key in enumerate(keys) if key>=query),len(keys))
    if getter:index=index if index<len(keys) and keys[index]==query else len(keys)
    expected=0x3400040+index*0x20 if getter else table+index*8;assert u.reg_read(UC_X86_REG_EAX)==expected;assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return expected
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ConstComponentLookup_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/const-component-lookup.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='const-component-lookup-gate-'));objects=[];providers=[];candidate={};native={};targets={s['symbol']:int(s['address'],16) for s in SPECS}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS):
            source=a.stage/s['file'] if a.stage else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(symbol,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(targets[symbol]+struct.unpack_from('<I',built,site)[0]-(address+site+4))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for keys in MAPS:
        for query in QUERIES:
            for pattern in range(4):
                assert execute(candidate,keys,query,pattern,False,False)==execute(native,keys,query,pattern,False,False);scenarios+=1
                for present in [False,True]:assert execute(candidate,keys,query,pattern,present,True)==execute(native,keys,query,pattern,present,True);scenarios+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CONST_COMPONENT_LOOKUP_BEHAVIOR_PASS scenarios=600' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=scenarios,compiled_scenarios=600,missing_provider_checks=2,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CONST_COMPONENT_LOOKUP_PASS compiled=600 comparative=%d'%scenarios)
if __name__=='__main__':main()
