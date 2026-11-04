from pathlib import Path
import sys,tempfile,json,struct,subprocess,argparse,hashlib
root=next(p for p in Path(__file__).resolve().parents if (p/'rebuild/include').is_dir());sys.path.insert(0,str(root/'tools/decomp_pipeline'))
import check_one,verify_and_land as V,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_const_component_lookup import LOWER_SYMBOL,execute
from gate_inventory_events import MAPS,QUERIES
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
FIND_SYMBOL='?Find@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@_STL@@ABW4ETCInterfaceType@@@Z'

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',type=Path);ap.add_argument('--test',type=Path,default=root/'rebuild/tests/integration/ConstComponentFind_test.cpp');ap.add_argument('--output',type=Path,default=root/'rebuild/compile-gate/const-component-find.json');a=ap.parse_args();symbol=FIND_SYMBOL;work=Path(tempfile.mkdtemp(prefix='const-component-find-gate-'));env=check_one.env()
    sources=[(a.stage/'CVectorMapThingComponents_FindConst_00436860.cpp' if a.stage else root/'rebuild/src/compiled/00/43/CVectorMapThingComponents_FindConst_00436860.cpp'),root/'rebuild/src/compiled/00/43/CVectorMapThingComponents_LowerBoundConst_004365b0.cpp'];symbols=[symbol,LOWER_SYMBOL];addresses=[0x436860,0x4365b0];sizes=[34,60];objects=[];codes={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,source in enumerate(sources):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[symbols[i]];code,rel=c.sections[sec];assert off==0 and len(code)==sizes[i];patched=bytearray(code)
            for site,(sym,kind) in rel.items():assert kind==20 and sym==LOWER_SYMBOL;struct.pack_into('<I',patched,site,(0x4365b0+struct.unpack_from('<I',code,site)[0]-(addresses[i]+site+4))&0xffffffff)
            retail=pe.get_data(addresses[i]-0x400000,sizes[i]);assert patched==retail;codes[addresses[i]]=patched;native[addresses[i]]=retail
    def run(code,keys,query,pattern):
        u=Uc(UC_ARCH_X86,UC_MODE_32);table=0x2100040;stack=0x3008000;stop=0x4000000
        for page in [0x436000,0x2000000,0x2100000,stop]:u.mem_map(page,0x1000)
        u.mem_map(0x3000000,0x10000)
        for a,b in code.items():u.mem_write(a,bytes(b))
        mapdata=bytearray(bytes([pattern*71])*128);struct.pack_into('<III',mapdata,0x40,table,table+len(keys)*8,table+(len(keys)+1)*8);entries=bytearray(bytes([pattern*71])*128)
        for i,k in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',entries,0x40+i*8,k&0xffffffff,0x3400040+i*0x20)
        u.mem_write(0x2000000,bytes(mapdata));u.mem_write(0x2100000,bytes(entries));u.mem_write(stack,struct.pack('<II',stop,stack+0x80));u.mem_write(stack+0x80,struct.pack('<I',query&0xffffffff));u.reg_write(UC_X86_REG_ECX,0x2000040);u.reg_write(UC_X86_REG_ESP,stack)
        saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
        for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
        u.emu_start(0x436860,stop,count=200);index=next((i for i,k in enumerate(keys) if k==query),len(keys));assert u.reg_read(UC_X86_REG_EAX)==table+index*8;assert bytes(u.mem_read(0x2000000,128))==mapdata and bytes(u.mem_read(0x2100000,128))==entries;assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return index
    count=0
    for keys in MAPS:
        for query in QUERIES:
            for pattern in range(4):assert run(codes,keys,query,pattern)==run(native,keys,query,pattern);count+=1
    test=work/'test.obj';fixture=a.test;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(fixture.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s not in Coff(test).symbols for s in symbols);objects.append(test);exe=work/'test.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],capture_output=True,text=True,env=env);assert not r.returncode,r.stdout+r.stderr
    print(r.stdout.strip(),'comparative=',count,'omissions=2');a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(native=count,compiled=200,omissions=2,work=str(work),providers=[dict(source=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sources],fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),headers={h:hashlib.sha256((root/'rebuild/include'/h).read_bytes()).hexdigest() for h in ['fable_world.h','fable_vector_map.h','fable_thing_components.h']}),indent=2)+'\n');print('CONST_COMPONENT_FIND_PASS')

if __name__=='__main__':main()
