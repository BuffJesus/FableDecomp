"""Validate feedback member-pointer dispatch and package confirmation against retail."""
import argparse
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ROOT = Path(__file__).resolve().parents[2]
TABLE_SYMBOL = '?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B'
SPECS = [
    dict(address='00434a30', file='CDisplayEngine_ReceiveFeedbackGameEvent_00434a30.cpp', symbol='?ReceiveFeedbackGameEvent@CDisplayEngine@@QAEXPBVCGameEvent@@@Z', size=36),
    dict(address='00434a60', file='CDisplayEngine_ConfirmFeedbackGameEventPackage_00434a60.cpp', symbol='?ConfirmFeedbackGameEventPackage@CDisplayEngine@@QAEXPBVCGameEventPackage@@@Z', size=74),
]
READERS = [
    dict(address='009f16d0', file='CGameEventPackage_GetEvent_009f16d0.cpp', symbol='?GetEvent@CGameEventPackage@@QBEPBVCGameEvent@@K@Z', size=14),
    dict(address='009f16e0', file='CGameEventPackage_GetNoEvents_009f16e0.cpp', symbol='?GetNoEvents@CGameEventPackage@@QBEKXZ', size=4),
]
TARGETS = {s['symbol']: int(s['address'], 16) for s in READERS}
TARGETS[TABLE_SYMBOL] = 0x013B9288


def execute(codes, entry, types, variation):
    obj, package, stack, stop, callback = 0x2000040, 0x2100010, 0x3008000, 0x4000000, 0x5000000
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for base, size in [(0x434000,4096),(0x9f1000,4096),(0x13b9000,8192),(0x2000000,4096),(0x2100000,4096),(0x3000000,65536),(stop,4096),(callback,4096)]:
        u.mem_map(base,size)
    for address, code in codes.items(): u.mem_write(address, bytes(code))
    table = bytearray(b'\x5a'*5568)
    confirm = entry == 0x434a60
    expected = []
    for t in range(87):
        delta = [-16,0,16,32][(t+variation)%4]
        for field in [32,48]:
            active = t%8 in ((1,3,4,6) if field==32 else (2,3,5,6))
            struct.pack_into('<Ii',table,t*64+field,callback if active else 0,delta)
    u.mem_write(TARGETS[TABLE_SYMBOL], bytes(table))
    initial = bytes([variation*55+1])*0x180
    u.mem_write(obj-0x40, initial)
    data = bytearray(b'\x7d'*0x648)
    struct.pack_into('<I',data,4,len(types))
    for i,t in enumerate(types):
        struct.pack_into('<I',data,8+i*40,t)
        delta = [-16,0,16,32][(t+variation)%4]
        if t%8 in ((2,3,5,6) if confirm else (1,3,4,6)):
            expected.append((obj+delta,package+8+i*40))
    u.mem_write(package, bytes(data))
    arg=package if confirm else package+8
    u.mem_write(stack,struct.pack('<II',stop,arg))
    u.reg_write(UC_X86_REG_ECX,obj);u.reg_write(UC_X86_REG_ESP,stack)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
    trace=[]
    def hook(uc,pc,size,user):
        if pc!=callback:return
        sp=uc.reg_read(UC_X86_REG_ESP)
        trace.append((uc.reg_read(UC_X86_REG_ECX),struct.unpack('<I',uc.mem_read(sp+4,4))[0]))
        # Clobber caller-saved registers to exercise the loop's preserved state.
        uc.reg_write(UC_X86_REG_EAX,0xabababab);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef)
        uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+8)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(entry,stop,count=5000)
    assert trace==expected,(trace,expected)
    assert u.reg_read(UC_X86_REG_ESP)==stack+8
    assert all(u.reg_read(r)==0x12340000+i for i,r in enumerate(saved))
    assert bytes(u.mem_read(obj-0x40,len(initial)))==initial
    assert bytes(u.mem_read(package,len(data)))==data
    assert bytes(u.mem_read(TARGETS[TABLE_SYMBOL],len(table)))==table
    return trace


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--stage',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/DisplayFeedback_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/display-feedback.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='display-feedback-'));objects=[];providers=[];entries=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+READERS):
            source=args.stage/s['file'] if args.stage and s in SPECS else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file']
            obj=work/('source%d.obj'%i)
            r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
            objects.append(obj);providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
            c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'],(s,len(built))
            address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in (6,20);addend=struct.unpack_from('<I',built,site)[0]
                struct.pack_into('<I',patched,site,(TARGETS[name]+addend-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,len(built));assert patched==retail,s
            candidate[address]=patched;native[address]=retail
            entries.append(dict(s,grade='RELOCATION_MATCH' if rel else 'MATCH'))
    comparisons=0
    for variation in range(4):
        for type in range(87):
            for entry in [0x434a30,0x434a60]:
                assert execute(candidate,entry,[type],variation)==execute(native,entry,[type],variation);comparisons+=1
        for n in range(41):
            for rotation in range(8):
                types=[(i+rotation)%87 for i in range(n)]
                assert execute(candidate,0x434a60,types,variation)==execute(native,0x434a60,types,variation);comparisons+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(args.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
    fixture=Coff(test);assert TABLE_SYMBOL in fixture.symbols and not any(s['symbol'] in fixture.symbols for s in SPECS+READERS)
    objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(len(objects)-1):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True)
        assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert not r.returncode and 'DISPLAY_FEEDBACK_BEHAVIOR_PASS scenarios=16064' in r.stdout,r.stdout+r.stderr
    headers={n:hashlib.sha256((args.include/n).read_bytes()).hexdigest() for n in ['fable_game.h','fable_game_event_dispatch.h']}
    report=dict(entries=entries,providers=providers,comparative_scenarios=comparisons,behavior=r.stdout.strip(),
                fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),headers=headers,missing_provider_checks=4,
                dispatch_table_status='external boundary; table initialization not reconstructed')
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('DISPLAY_FEEDBACK_PASS providers=2 bytes=110 compiled=16064 comparative=%d'%comparisons)


if __name__=='__main__':main()
