"""Check recovered window activation with mocked input and sound boundaries."""
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
from near_match import compare, retail_relocations
from pe_oracle import EXE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ROOT=Path(__file__).resolve().parents[2]
ENTRY=0x009a5a50
SYMBOL='?ActivateWindow@CSystemManager@@CIXPAV1@_N@Z'
TARGETS={'?SetHasInputFocus@CInputManager@@QAEX_N@Z':0x009f4e20,
         '?ClearEvents@CInputManager@@QAEXXZ':0x009f5540,
         '?UnknownSetEnabled_009915e0@CASoundManager@@QAEX_N@Z':0x009915e0,
         '?FableSavedSoundVolume_013ca60c@@3MA':0x013ca60c,
         '?FableSoundDeactivated_013ca610@@3_NA':0x013ca610}


def execute(code, scenario):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for page,size in [(0x009a5000,4096),(0x009f4000,8192),(0x00991000,4096),
                      (0x013ca000,4096),(0x02000000,4096),(0x02100000,4096),(0x03000000,65536)]:
        uc.mem_map(page,size)
    manager=0x02000000
    inp,altinp,sound,altsound,table=manager+0x400,manager+0x500,manager+0x600,manager+0x700,manager+0x800
    stop,setvol,getvol,offset=0x02100000,0x02100010,0x02100020,0x02100030
    def put(a,v): uc.mem_write(a,struct.pack('<I',v & 0xffffffff))
    def get(a): return struct.unpack('<I',uc.mem_read(a,4))[0]
    def byte(a,v): uc.mem_write(a,bytes([v]))
    active,prior,ever,paused,have_input,have_sound,offset_ok,mut_input,mut_sound=[bool(scenario & (1<<n)) for n in range(9)]
    uc.mem_write(ENTRY,bytes(code))
    # Boundary stubs stop decoding at mapped-page edges. GetVolume returns
    # through a real x87 load so both bodies exercise their FSTP instruction.
    for a in [0x009f4e20,0x009f5540,0x009915e0,setvol,offset]: uc.mem_write(a,b'\xc3')
    uc.mem_write(getvol,b'\xd9\x05'+struct.pack('<I',0x02100080)+b'\xc2\x04\x00')
    uc.mem_write(0x02100080,struct.pack('<f',-0.375))
    uc.mem_write(manager,b'\xa5'*0x180)
    for o,v in [(9,prior),(10,not active),(0xdc,ever),(0xea,0),(0x16c,0)]: byte(manager+o,v)
    put(manager+0x58,inp if have_input else 0)
    put(manager+0x7c,sound if have_sound else 0)
    uc.mem_write(manager+0x170,struct.pack('<f',-17.5))
    uc.mem_write(0x013ca60c,struct.pack('<f',0.625)); byte(0x013ca610,paused)
    put(sound,table); put(altsound,table)
    for slot,address in [(46,setvol),(47,getvol),(49,offset)]: put(table+slot*4,address)
    trace=[]
    done=False
    def hook(uc,address,size,data):
        nonlocal done
        if address==stop:
            done=True; uc.emu_stop(); return
        if address not in [0x009f4e20,0x009f5540,0x009915e0,setvol,getvol,offset]: return
        sp=uc.reg_read(UC_X86_REG_ESP); receiver=uc.reg_read(UC_X86_REG_ECX)
        result=0xdeadbe00
        if address==0x009f4e20:
            argc=1; trace.append(('focus',receiver,get(sp+4)&255))
            assert receiver==inp and (get(sp+4)&255)==active
            if mut_input: put(manager+0x58,altinp)
        elif address==0x009f5540:
            argc=0; trace.append(('clear',receiver))
            assert receiver==get(manager+0x58)
        elif address==0x009915e0:
            argc=1; trace.append(('enabled',receiver,get(sp+4)&255))
            assert receiver==get(manager+0x7c) and (get(sp+4)&255)==active
        elif address==setvol:
            argc=3; trace.append(('set',receiver,get(sp+4),get(sp+8),get(sp+12)))
            assert receiver==sound and get(sp+4)==0x8000 and get(sp+12)==0
            assert get(sp+8)==(0x3f200000 if active else 0)
            if mut_sound: put(manager+0x7c,altsound)
        elif address==getvol:
            trace.append(('get',receiver,get(sp+4)))
            assert receiver==sound and get(sp+4)==0x8000
            return
        else:
            argc=1; trace.append(('offset',receiver,get(sp+4)))
            assert receiver==get(manager+0x7c) and get(sp+4)==manager+0x170
            uc.mem_write(get(sp+4),struct.pack('<f',42.25)); result=int(offset_ok)
        uc.reg_write(UC_X86_REG_EAX,result)
        uc.reg_write(UC_X86_REG_ECX,0xcccccccc); uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_ESP,sp+4+4*argc); uc.reg_write(UC_X86_REG_EIP,get(sp))
    uc.hook_add(UC_HOOK_CODE,hook)
    sp=0x03008000; put(sp,stop)
    saved={UC_X86_REG_EBX:0x11111111,UC_X86_REG_ESI:0x22222222,UC_X86_REG_EDI:0x33333333,UC_X86_REG_EBP:0x44444444}
    for r,v in saved.items(): uc.reg_write(r,v)
    uc.reg_write(UC_X86_REG_ECX,manager); uc.reg_write(UC_X86_REG_EDX,int(active)); uc.reg_write(UC_X86_REG_ESP,sp)
    uc.emu_start(ENTRY,stop+1,count=2000)
    assert done and uc.reg_read(UC_X86_REG_ESP)==sp+4
    assert all(uc.reg_read(r)==v for r,v in saved.items())
    return trace,bytes(uc.mem_read(manager,4096)).hex(),bytes(uc.mem_read(0x013ca60c,5)).hex()


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_ActivateWindow_009a5a50.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9a/CSystemManager_ActivateWindow_009a5a50_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/system-activation.json')
    args=ap.parse_args()
    work=Path(tempfile.mkdtemp(prefix='system-activation-'))
    env=check_one.env(); env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    obj,test=work/'candidate.obj',work/'test.obj'
    for source,target in [(args.source,obj),(args.test,test)]:
        r=check_one.cl(['/nologo','/c','/O2','/Oy','/MT','/Fo'+str(target),str(source.resolve())],env,work)
        assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj); section,start=coff.symbols[SYMBOL]; built,relocs=coff.sections[section]
    assert start==0 and len(relocs)==12
    pe=pefile.PE(str(EXE)); retail=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,268)
    score=compare(built,retail,relocs,retail_relocations('009a5a50',268)); assert score.near_match,score
    candidate=bytearray(built)
    checked=[]
    for site,(symbol,kind) in relocs.items():
        target=TARGETS[symbol]; assert kind==(6 if 'Fable' in symbol else 20)
        assert int.from_bytes(built[site:site+4],'little')==0
        value=target if kind==6 else target-(ENTRY+site+4)
        struct.pack_into('<I',candidate,site,value & 0xffffffff)
        checked.append({'offset':site,'symbol':symbol,'target':f'{target:08x}'})
    assert candidate==retail,'exact relocated body required for this gate'
    for slot,target in [(46,0x00a3e9d0),(47,0x00a3ea20),(49,0x00a3e480)]:
        assert struct.unpack('<I',pe.get_data(0x0129d2c4+4*slot-pe.OPTIONAL_HEADER.ImageBase,4))[0]==target
    traces=[]
    for scenario in range(512):
        native=execute(retail,scenario); recovered=execute(candidate,scenario)
        assert native==recovered,scenario
        traces.append({'scenario':scenario,'events':native[0]})
    exe=work/'fixture.exe'; cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    standalone=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True)
    assert standalone.returncode!=0,'fixture must require candidate'
    linked=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True)
    assert linked.returncode==0,linked.stdout+linked.stderr
    run=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert run.returncode==0 and 'ACTIVATION_BEHAVIOR_PASS scenarios=512' in run.stdout,run.stdout+run.stderr
    report={'address':'009a5a50','grade':'RELOCATION_MATCH','retail_bytes':268,'built_bytes':len(built),
            'similarity':score.to_dict(),'relocations':checked,'behavior':run.stdout.strip(),
            'source_sha256':hashlib.sha256(args.source.read_bytes()).hexdigest(),
            'fixture_sha256':hashlib.sha256(args.test.read_bytes()).hexdigest(),'comparative_scenarios':traces}
    args.output.parent.mkdir(parents=True,exist_ok=True); args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('SYSTEM_ACTIVATION_PASS grade=RELOCATION_MATCH bytes=268 compiled=512 comparative=512')


if __name__=='__main__': main()
