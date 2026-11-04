"""Validate the active/focus setters and native character-ring insertion."""
import argparse
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
ITEMS=[('CSystemManager_SetAsActive_009a5270',0x9a5270,60,'?SetAsActive@CSystemManager@@AAEX_N@Z','active_test.cpp','ACTIVE_STATE_PASS scenarios=16'),
       ('CSystemManager_SetHasInputFocus_009a5a30',0x9a5a30,26,'?SetHasInputFocus@CSystemManager@@AAEX_N@Z','focus_test.cpp','SYSTEM_FOCUS_PASS scenarios=16'),
       ('CInkeyBuffer_AddChar_00a5fa70',0xa5fa70,76,'?AddChar@CInkeyBuffer@@QAEXG_N@Z','inkey_test.cpp','INKEY_ADD_CHAR_PASS scenarios=120000')]

def active_execution(code,flags):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x9a5000,4096),(0x2000000,4096),(0x2100000,4096),(0x3000000,65536)]:uc.mem_map(base,size)
    uc.mem_write(0x9a5270,bytes(code));uc.mem_write(0x2000000,b'\xa5'*0x180)
    for offset,bit in [(9,1),(0xdc,2),(0xea,3)]:uc.mem_write(0x2000000+offset,bytes([int(bool(flags&(1<<bit)))]))
    sp=0x3008000;uc.mem_write(sp,struct.pack('<II',0x2100000,flags&1))
    uc.reg_write(UC_X86_REG_ESP,sp);uc.reg_write(UC_X86_REG_ECX,0x2000000)
    saved={UC_X86_REG_EBX:0x11111111,UC_X86_REG_ESI:0x22222222,UC_X86_REG_EDI:0x33333333,UC_X86_REG_EBP:0x44444444}
    for r,v in saved.items():uc.reg_write(r,v)
    stopped=[]
    def hook(uc,address,size,data):
        if address==0x2100000:stopped.append(True);uc.emu_stop()
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0x9a5270,0x2100001,count=100)
    assert stopped and uc.reg_read(UC_X86_REG_ESP)==sp+8
    assert all(uc.reg_read(r)==v for r,v in saved.items())
    return bytes(uc.mem_read(0x2000000,0x180))

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--stage',type=Path)
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/window-leaves.json')
    args=ap.parse_args();env=check_one.env();work=Path(tempfile.mkdtemp(prefix='window-leaves-'))
    pe=pefile.PE(str(EXE));reports=[]
    for name,address,size,symbol,test_name,marker in ITEMS:
        shard=f'{address:08x}'[:2]+'/'+f'{address:08x}'[2:4]
        source=args.stage/(name+'.cpp') if args.stage else ROOT/'rebuild/src/compiled'/shard/(name+'.cpp')
        test_source=args.stage/test_name if args.stage else ROOT/'rebuild/tests'/shard/(name+'_test.cpp')
        obj,test=work/(name+'.obj'),work/(name+'_test.obj')
        for src,target in [(source,obj),(test_source,test)]:
            r=check_one.cl(['/nologo','/c','/O2','/Oy','/MT','/Fo'+str(target),str(src.resolve())],env,work)
            assert r.returncode==0,r.stdout+r.stderr
        coff=Coff(obj);sec,off=coff.symbols[symbol];built,relocs=coff.sections[sec]
        assert off==0 and len(built)==size
        retail=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,size)
        candidate=bytearray(built)
        if address==0x9a5a30:
            assert relocs=={19:('?SetHasInputFocus@CInputManager@@QAEX_N@Z',20)}
            assert candidate[19:23]==b'\0'*4
            struct.pack_into('<I',candidate,19,0x9f4e20-(address+23))
        else:assert not relocs
        score=compare(built,retail,relocs,retail_relocations(f'{address:08x}',size))
        assert score.near_match,score
        if address==0x9a5270:
            for flags in range(16):assert active_execution(candidate,flags)==active_execution(retail,flags),flags
        else:assert candidate==retail
        exe=work/(name+'.exe');cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
        alone=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True)
        assert alone.returncode!=0,'fixture must require candidate'
        linked=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True)
        assert linked.returncode==0,linked.stdout+linked.stderr
        result=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
        assert result.returncode==0 and marker in result.stdout,result.stdout+result.stderr
        grade=('RELOCATION_MATCH' if relocs else 'MATCH') if candidate==retail else 'NEAR_MATCH'
        report=dict(address=f'{address:08x}',grade=grade,bytes=size,similarity=score.to_dict(),behavior=result.stdout.strip(),
                    source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(test_source.read_bytes()).hexdigest())
        reports.append(report);print('WINDOW_LEAF_PASS',report['address'],grade,report['behavior'])
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps({'functions':reports},indent=2)+'\n')
    print('WINDOW_LEAVES_PASS functions=3 compiled=120032 comparative_active=16')

if __name__=='__main__':main()
