"""Check retail application-name parsing with mocked command line and string ABI."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from near_match import compare,retail_relocations
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
ENTRY=0x9a5340
SYMBOL='?InitApplicationName@CSystemManager@@AAEXXZ'
TARGETS={'___security_cookie':0x139c8a8,'__imp__GetCommandLineW@0':0x143fd74,
         '_wcsncpy':0xbfeb04,'_wcslen':0xbfed94,'_wcsrchr':0xbfedb2,'_wcscpy':0xbfed76,
         '??0CWideString@@QAE@PBG@Z':0x99b6b0,'?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,
         '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??1CCharString@@QAE@XZ':0x99eae0,
         '??1CWideString@@QAE@XZ':0x99b510,'@__security_check_cookie@4':0xbfe9f9}
INPUTS=['','fable.exe','fable.exe args','C:\\Games\\Fable.exe -x','"C:\\Games With Spaces\\Fable.exe" -x',
        ' "C:\\Fable.exe"','C:/Games/Fable.exe','\\\\server\\share\\Fable.exe','C:\\Name WithSpace\\file.exe','"C:\\Name WithSpace\\file.exe"',
        'C:\\','"C:\\"','"C:\\file.exe','C:\\foo"bar.exe ignored','C:\\foo.exe\tother','"C:\\f.exe"ignored','""','"',
        'C:\\中文.exe','C:\\path\\..\\file.exe','C:\\path\\..\\','C:\\.exe','C:\\a\\b\\c','C:\\'+'x'*257]

def execute(code,text,alignment,initial):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x9a5000,4096),(0xbfe000,4096),(0x99b000,4096),(0x99e000,4096),
                      (0x139c000,4096),(0x143f000,4096),(0x2000000,4096),(0x2100000,4096),(0x3000000,65536)]:uc.mem_map(base,size)
    manager,command,stop,get_command=0x2000000,0x2000800,0x2100000,0x2100010
    def put(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def wide(a):
        values=[]
        for n in range(262):
            value=struct.unpack('<H',uc.mem_read(a+2*n,2))[0]
            if not value:return values
            values.append(value)
        raise AssertionError('unterminated wide string beyond tested retail copy bound')
    def write_wide(a,values):uc.mem_write(a,struct.pack('<'+'H'*(len(values)+1),*values,0))
    def decode(values):return struct.pack('<'+'H'*len(values),*values).decode('utf-16-le')
    uc.mem_write(ENTRY,bytes(code));uc.mem_write(manager,b'\xa5'*0x180);put(manager+0x168,initial)
    uc.mem_write(command,text.encode('utf-16-le')+b'\0\0');put(0x143fd74,get_command);put(0x139c8a8,0x12345678)
    callbacks=set(TARGETS.values())-{0x139c8a8,0x143fd74}
    for address in callbacks|{get_command}:uc.mem_write(address,b'\xc3')
    events=[];active_wide=[];active_char=[];done=[]
    def hook(uc,address,size,data):
        if address==stop:done.append(True);uc.emu_stop();return
        if address not in callbacks|{get_command}:return
        sp=uc.reg_read(UC_X86_REG_ESP);receiver=uc.reg_read(UC_X86_REG_ECX);argc=0;result=0
        if address==get_command:events.append(('command',));result=command
        elif address==0xbfeb04:
            dest,src,count=get(sp+4),get(sp+8),get(sp+12);assert src==command and count==261
            value=wide(src);uc.mem_write(dest,struct.pack('<'+'H'*count,*(value+[0]*(count-len(value)))));result=dest
            events.append(('copy',count))
        elif address==0xbfed94:result=len(wide(get(sp+4)));events.append(('length',result))
        elif address==0xbfedb2:
            pointer,character=get(sp+4),get(sp+8);assert character==92
            value=wide(pointer);indices=[i for i,v in enumerate(value) if v==character]
            result=pointer+2*indices[-1] if indices else 0;events.append(('last_slash',decode(value),bool(result)))
        elif address==0xbfed76:
            dest,src=get(sp+4),get(sp+8);value=wide(src);write_wide(dest,value);result=dest;events.append(('filename',decode(value)))
        elif address==0x99b6b0:
            assert not active_wide;active_wide.append(receiver);put(receiver,0)
            events.append(('wide',decode(wide(get(sp+4)))));argc=1;result=receiver
        elif address==0x99b2c0:
            assert active_wide==[receiver] and not active_char
            result=get(sp+4);active_char.append(result);put(result,0);argc=1;events.append(('convert',))
        elif address==0x99efb0:
            assert receiver==manager+0x168 and active_char==[get(sp+4)] and active_wide
            put(receiver,0x12345678);argc=1;result=receiver;events.append(('assign',))
        elif address==0x99eae0:
            assert active_char==[receiver] and active_wide;active_char.clear();events.append(('char_destroy',))
        elif address==0x99b510:
            assert active_wide==[receiver] and not active_char;active_wide.clear();events.append(('wide_destroy',))
        else:
            assert address==0xbfe9f9 and receiver==0x12345678;events.append(('cookie',))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_ESP,sp+4+argc*4);uc.reg_write(UC_X86_REG_EIP,get(sp))
    uc.hook_add(UC_HOOK_CODE,hook)
    sp=0x3008000+alignment;put(sp,stop);uc.reg_write(UC_X86_REG_ESP,sp);uc.reg_write(UC_X86_REG_ECX,manager)
    saved={UC_X86_REG_EBX:0x11111111,UC_X86_REG_ESI:0x22222222,UC_X86_REG_EDI:0x33333333,UC_X86_REG_EBP:0x44444444}
    for r,v in saved.items():uc.reg_write(r,v)
    uc.emu_start(ENTRY,stop+1,count=5000)
    assert done and not active_wide and not active_char and uc.reg_read(UC_X86_REG_ESP)==sp+4
    assert all(uc.reg_read(r)==v for r,v in saved.items())
    return events,bytes(uc.mem_read(manager,0x180)).hex()

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_InitApplicationName_009a5340.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9a/CSystemManager_InitApplicationName_009a5340_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/application-name.json')
    args=ap.parse_args();env=check_one.env();work=Path(tempfile.mkdtemp(prefix='application-name-'));obj,test=work/'candidate.obj',work/'test.obj'
    for source,target in [(args.source,obj),(args.test,test)]:
        result=check_one.cl(['/nologo','/c','/O2','/Oy','/GS','/MT','/Fo'+str(target),str(source.resolve())],env,work)
        assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);sec,off=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert off==0 and len(relocs)==12
    pe=pefile.PE(str(EXE));retail=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,274)
    score=compare(built,retail,relocs,retail_relocations('009a5340',274));assert score.near_match,score
    imports={entry.address:entry.name.decode() for group in pe.DIRECTORY_ENTRY_IMPORT for entry in group.imports if entry.name}
    assert imports[0x143fd74]=='GetCommandLineW'
    for symbol in ['_wcsncpy','_wcslen','_wcsrchr','_wcscpy']:
        thunk=pe.get_data(TARGETS[symbol]-pe.OPTIONAL_HEADER.ImageBase,6)
        assert thunk[:2]==b'\xff\x25' and imports[struct.unpack_from('<I',thunk,2)[0]]==symbol[1:]
    candidate=bytearray(built);checked=[]
    for site,(symbol,kind) in relocs.items():
        target=TARGETS[symbol];assert kind==(6 if symbol in ['___security_cookie','__imp__GetCommandLineW@0'] else 20)
        assert built[site:site+4]==b'\0'*4
        value=target-(ENTRY+site+4 if kind==20 else 0);struct.pack_into('<I',candidate,site,value&0xffffffff)
        checked.append(dict(offset=site,symbol=symbol,target=f'{target:08x}'))
    scenarios=[]
    for text in INPUTS:
        for alignment in [0,4]:
            for initial in [0,0xa5a5a5a5]:
                native=execute(retail,text,alignment,initial);recovered=execute(candidate,text,alignment,initial)
                assert native==recovered,(text,alignment,initial,native[0],recovered[0])
                scenarios.append(dict(command=text,alignment=alignment,initial=initial,events=native[0]))
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    alone=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert alone.returncode!=0
    linked=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert linked.returncode==0,linked.stdout+linked.stderr
    tested=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert tested.returncode==0 and 'APPLICATION_NAME_BEHAVIOR_PASS scenarios=24' in tested.stdout,tested.stdout+tested.stderr
    grade='RELOCATION_MATCH' if candidate==retail else 'NEAR_MATCH'
    report=dict(address='009a5340',grade=grade,retail_bytes=274,built_bytes=len(built),similarity=score.to_dict(),relocations=checked,
                behavior=tested.stdout.strip(),comparative_scenarios=scenarios,
                source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('APPLICATION_NAME_PASS grade='+grade+' compiled=24 comparative='+str(len(scenarios)))

if __name__=='__main__':main()
