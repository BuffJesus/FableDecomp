#!/usr/bin/env python3
"""Read-only bank opening and async ownership chain compared against retail."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
from check_ui_strings import SOURCES as STRINGS
from check_ui_bank_stream import SOURCES as STREAMS
from check_ui_bank_stream_read import SOURCES as READS
from check_ui_bank_decode import SOURCES as DECODE
from check_ui_bank_registry import SOURCES as REGISTRY
from check_ui_bank_path import SOURCES as PATH
from check_ui_wide_strings import SOURCES as WIDE
FUNCTIONS=[
 ('00/9d/CBankFile_OpenReadOnly_009d06f0',0x9D06F0,385,'FableUiOpenBankReadOnly'),
 ('00/9d/CBankFileAsync_OpenReadOnly_009d56c0',0x9D56C0,348,'FableUiOpenAsyncBankReadOnly'),
 ('00/9c/CBankFile_GetPath_009cbf10',0x9CBF10,36,'FableUiGetBankPath'),
 ('00/9a/CCountedPointer_ResetThreadedFile_009a9c80',0x9A9C80,103,'FableUiResetThreadedFile'),
 ('00/9a/CCountedPointer_DeleteThreadedFile_009a9040',0x9A9040,11,'FableUiDeleteThreadedFile'),
 ('00/9a/CCountedPointer_ReleaseThreadedFile_009a9c40',0x9A9C40,53,'FableUiReleaseThreadedFile'),
 ('00/9a/CCountedPointer_ReleaseDiskFile_009a9bb0',0x9A9BB0,53,'FableUiReleaseDiskFile'),
 ('00/9a/CCountedPointer_ReleaseRegisteredBank_009a9d60',0x9A9D60,53,'FableUiReleaseRegisteredBank'),
 ('00/9a/CCountedPointer_AssignDiskFile_009a9bf0',0x9A9BF0,72,'FableUiAssignDiskFile'),
 ('00/9d/CCountedPointer_AssignThreadedFile_009d6fd0',0x9D6FD0,72,'FableUiAssignThreadedFile'),
 ('00/98/CThreadedFile_ConstructForBank_0098dfd0',0x98DFD0,41,'FableUiConstructThreadedFile'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
DEPENDENCIES=STRINGS+STREAMS+READS+DECODE+REGISTRY+PATH+WIDE+['rebuild/src/compiled/00/41/CCountedPointer_ReleaseBankReference_00419108.cpp','rebuild/src/compiled/00/41/CCountedPointer_ShareBankReference_00419134.cpp','rebuild/src/compiled/00/99/CBase_ConstructUiResourceBase_0099a2f0.cpp','rebuild/src/compiled/00/99/CWideString_Constructor_0099aed0.cpp','rebuild/src/compiled/00/9e/Global_StartBankProgress_009e9f40.cpp']
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_open_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiBankOpen_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=2048: raise RuntimeError('Incomplete bank-open traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x30000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,bank,refs,files,thread,registered=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000
    input_,input_data,input_text,progress_data,progress_buffer,path_data=0x20015000,0x20015100,0x20015200,0x20016000,0x20016100,0x20017000
    bank_vt,file_vt,progress,progress_vt=0x20018000,0x20018100,0x20018200,0x20018300
    alias_head,bank_head,bank_entry,registry_head,registry_node=0x20022000,0x20022100,0x20022200,0x20022300,0x20022400
    path_head,path_entry,path_text=0x20022500,0x20022600,0x20022700
    stream_buffer=0x20024000
    archive_temporary,archive_name=0x20022900,0x20022a00
    sample,file_read=0x20022800,0x20021040
    file_length,file_position,file_seekable,file_setpos=0x20021000,0x20021010,0x20021020,0x20021030
    open_path,get_type,finish,get_path,destroy,delete,announce=[0x20020000+i*16 for i in range(7)]
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p):return struct.unpack('<i',uc.mem_read(p,4))[0]
    def refid(p):return (p-refs)//12+1 if p else 0
    def fileid(p):return 0 if not p else 5 if p==thread else (p-files)//8+1
    def text(p):return bytes(uc.mem_read(word(word(p)),64)).split(b'\0')[0].decode()
    services={0xBFEA0E:0,0xBFEA14:0,0x9CFBC0:12,0x98E1E0:8,0xBFEA1A:0,0xBFE9BC:0,0xBFEB22:0,0xBFEB1C:0,open_path:8,get_type:0,finish:0,get_path:4,destroy:0,delete:4,announce:16,file_length:0,file_position:0,file_seekable:0,file_setpos:4,file_read:12}
    for p,n in services.items():uc.mem_write(p,b'\xc2'+struct.pack('<H',n) if n else b'\xc3')
    # The entry-decoder boundary logs its arguments, then invokes the real
    # slow reader for four representative bytes. It does not decode entries.
    thunk=b'\x8b\x4c\x24\x04\x6a\x04\x68'+struct.pack('<I',sample)+b'\xb8'+struct.pack('<I',0x993CA0)+b'\xff\xd0\xc2\x0c\x00'
    thunk=thunk[:-3]
    sample_done=0x9CFBC0+len(thunk)
    thunk+=b'\x8b\x4c\x24\x04\x68'+struct.pack('<I',archive_name)+b'\xb8'+struct.pack('<I',0x996390)+b'\xff\xd0'
    name_done=0x9CFBC0+len(thunk)
    thunk+=b'\xb9'+struct.pack('<I',archive_name)+b'\xb8'+struct.pack('<I',0x99EAE0)+b'\xff\xd0\xc2\x0c\x00'
    uc.mem_write(0x9CFBC0,thunk)
    for offset,address in ((32,0x994360),(36,0x9943B0),(40,0x9943D0)):put(0x129A728+offset,address)
    put(file_vt+12,file_read)
    put(bank_vt+12,open_path);put(bank_vt+32,get_type);put(bank_vt+44,finish);put(file_vt+44,get_path)
    put(file_vt+20,file_setpos);put(file_vt+28,file_position);put(file_vt+36,file_length);put(file_vt+40,file_seekable)
    put(0x129A158,delete);put(progress,progress_vt);put(progress_vt+12,announce);put(0x13CAA38,progress)
    for i in range(4):put(files+i*8,file_vt);put(files+i*8+4,i+1)
    put(input_,input_data);put(input_data,input_text);uc.mem_write(input_text,b'frontend\0');put(input_data+4,8);put(input_data+8,16);uc.mem_write(input_data+12,b'\0')
    events=[];seed=lookups=0
    def hook(uc,address,size,data):
        nonlocal lookups
        if address==sample_done:events.append(f'SAMPLE{word(sample):08x}');return
        if address==name_done:events.append('SNAME:'+(text(archive_name) if word(archive_name) else ''));return
        if address not in services:return
        esp=uc.reg_read(UC_X86_REG_ESP);receiver=uc.reg_read(UC_X86_REG_ECX);result=0
        if address==0xBFEA0E:events.append('ARALLOC'+str(word(esp+4)));result=archive_temporary
        elif address==0xBFEA14:events.append('ARFREE')
        elif address==open_path:
            events.append(f'OPENPATH{int(word(word(esp+4))==path_data)}:{word(esp+8)}');uc.mem_write(bank+0x8C,b'\0');result=int(bool(seed&16))
        elif address==get_type:events.append('TYPE');result=43 if ((seed>>2)&3)==3 else 42
        elif address==finish:events.append('FINISH');uc.mem_write(bank+0x94,b'\1')
        elif address==get_path:events.append('GETPATH'+str(fileid(receiver)));put(word(esp+4),path_data);put(path_data+12,word(path_data+12)+1);put(0x13BCA20,word(0x13BCA20)+1);result=word(esp+4)
        elif address==file_read:
            out,n=word(esp+4),word(esp+8);events.append(f'FILEREAD{fileid(receiver)}:{n}:{word(esp+12)&255}');uc.mem_write(out,bytes((seed+i*13)&255 for i in range(n)));put(out+4,seed%16)
            if seed%16:uc.mem_write(out+8,bytes(65+j for j in range(seed%16)))
        elif address==file_length:events.append('LENGTH'+str(fileid(receiver)));result=0x200000
        elif address==file_position:events.append('POSITION'+str(fileid(receiver)));result=seed%5
        elif address==file_seekable:events.append('CANSEEK'+str(fileid(receiver)));result=int(bool(seed&32))
        elif address==file_setpos:events.append(f'SETPOS{fileid(receiver)}:{word(esp+4)}')
        elif address==0x9CFBC0:events.append(f'READ{int(receiver==bank)}:{fileid(word(word(esp+4)+24))}:{word(esp+8)}:{word(esp+12)}')
        elif address==0x98E1E0:events.append(f'THREADOPEN{fileid(receiver)}:{int(word(word(esp+4))==path_data)}:{word(esp+8)&255}');result=int(bool(seed&32))
        elif address==destroy:events.append('DEST'+str(fileid(receiver)))
        elif address==delete:events.append(f'DELETE{fileid(receiver)}:{word(esp+4)}')
        elif address==announce:
            amount=struct.unpack('<f',struct.pack('<I',word(esp+8)))[0]
            events.append(f'PROGRESS:{text(word(esp+4))}:{amount:g}:{word(esp+12)&255}:{word(esp+16)&255}')
            if seed&64:uc.mem_write(bank+0x8C,bytes([1-uc.mem_read(bank+0x8C,1)[0]]))
            if ((seed>>2)&3)==2:put(bank_head+4,0)
        elif address==0xBFEA1A:
            n=word(esp+4)
            if n==17:events.append('STRALLOC17');result=progress_data
            else:events.append('ALLOC'+str(n));result=thread if n==28 else 0 if seed&512 else refs+60
        elif address==0xBFE9BC:
            p=word(esp+4);events.append('STRFREE' if p==progress_data else 'FREE'+str(refid(p)))
        elif address==0xBFEB22:events.append('BUFALLOC'+str(word(esp+4)));result=stream_buffer if word(esp+4)==16384 else progress_buffer
        elif address==0xBFEB1C:events.append('STREAMFREE' if word(esp+4)==stream_buffer else 'BUFFREE')
        uc.reg_write(UC_X86_REG_EAX,result)
    uc.hook_add(UC_HOOK_CODE,hook)
    def normalize(v):
        symbols={bank_vt:0x70000200,0x129A158:0x70000201,input_data:0x64000000,0x9A9040:0x70000202,destroy:0x70000203,thread:0x62000000}
        if v in symbols:return symbols[v]
        if files<=v<files+32:return 0x61000000+v-files
        if refs<=v<refs+72:return 0x63000000+v-refs
        return v
    def dump(p,n):return ''.join(f':{normalize(word(p+i)):08x}' for i in range(0,n,4))
    def execute(address,receiver,args=()):
        put(stack,stop)
        for i,a in enumerate(args):put(stack+4+i*4,a)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,receiver);uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4:raise RuntimeError('Bank-open stack mismatch')
        return uc.reg_read(UC_X86_REG_EAX)
    errors=[]
    for index,actual in enumerate(lines):
        mode,seed=divmod(index,1024);events.clear();lookups=0
        uc.mem_write(bank,b'\xA5'*0x164);uc.mem_write(thread,b'\xA5'*28);uc.mem_write(progress_data,b'\xA5'*17);uc.mem_write(progress_buffer,b'\xCD'*64)
        put(bank,bank_vt);put(bank+0x88,0);uc.mem_write(bank+0x8C,b'\0');uc.mem_write(bank+0x148,bytes([int(bool(seed&32))]))
        for i in range(6):put(refs+i*12,3 if i==4 else 1);put(refs+i*12+4,destroy);put(refs+i*12+8,files+(i%4)*8)
        put(bank+0x7C,files);put(bank+0x80,refs if seed&256 else 0);put(bank+0x110,files+8);put(bank+0x114,refs+12 if seed&256 else 0)
        put(registered+4,files+16);put(registered+8,refs+24 if seed&128 else 0);put(registered+12,files+24);put(registered+16,refs+36 if seed&128 else 0);uc.mem_write(registered+20,bytes([int(bool(seed&32))]))
        put(input_data+13,1);put(0x13BD800,100);put(0x13BCA20,50);uc.mem_write(0x13CA7B0,bytes([seed&1]))
        uc.mem_write(alias_head,b'\0'*16);uc.mem_write(bank_head,b'\0'*16);uc.mem_write(bank_entry,b'\0'*40)
        put(0x13CA79C+24,alias_head);put(0x13CA79C+16,registry_head);put(registry_head,registry_node);put(registry_head+4,registry_node)
        put(registry_node,registry_head);put(registry_node+4,registry_head);put(registry_node+8,registered);put(registry_node+12,refs+48)
        uc.mem_write(path_head,b'\0'*16);uc.mem_write(path_entry,b'\0'*24);put(path_head+4,path_entry);put(path_entry+16,input_data);put(path_entry+20,path_data)
        put(0x13CA79C+4,path_head);put(0x13CA79C+36,0);put(path_data,path_text);put(path_data+4,path_text+24);put(path_data+8,path_text+26);put(path_data+12,10);uc.mem_write(path_text,'frontend.big\0'.encode('utf-16le'))
        put(bank_entry+16,input_data)
        for i,v in enumerate((42,17 if seed%7 else 0xFFFFFFFF,123+seed,0x7654,64+seed)):put(bank_entry+20+i*4,v)
        put(bank_head+4,0 if ((seed>>2)&3)==1 else bank_entry);put(registered+24,bank_head)
        result=execute(0x9D56C0 if mode else 0x9D06F0,bank,(input_,0xD8|(4 if seed&2 else 0)))
        events.append('RESULT'+str(result&255));events.append('BANK'+dump(bank,0x164));events.append('REFS'+dump(refs,72));events.append('THREAD'+dump(thread,28));events.append(f'COUNTS{word(0x13BD800)}:{word(0x13BCA20)}:{word(input_data+13)}:{word(path_data+12)}')
        execute(0x9A9C40,bank+0x110);execute(0x9A9BB0,bank+0x7C);execute(0x9A9040,0)
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=index,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section);offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address)
        matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=2048,errors=errors,comparisons=comparisons,scope='complete base/async read-only open, reference lifetime/reset, threaded ctor and filename forwarding; real buffered stream construction/seek/read/cleanup; entry boundary reads four representative bytes and a length-prefixed string through the real decode/refill chain; real registry/map/comparator/path lookup and wide lifetime; file virtual methods/read/finalize/threaded OS open controlled; registered path has null base and shared filename, allocating concatenation covered separately'),indent=2)+'\n')
    print(f"UI_BANK_OPEN {'FAIL' if errors else 'PASS'} cases=2048 failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
