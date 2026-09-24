#!/usr/bin/env python3
"""Graphics-bank factory/cache with actual strings, reference ownership and dispatch."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run
from check_ui_strings import SOURCES as STRING_SOURCES
from check_ui_bank_ownership import SOURCES as OWNERSHIP_SOURCES

FUNCTIONS=[
    ('00/9f/CGraphicsBankManager_Create_009f83d0',0x9F83D0,693,'FableUiCreateGraphicsBank'),
    ('00/41/Global_CompareSignedStringBytes_00411570',0x411570,40,'FableUiCompareStringBytes'),
    ('00/41/CCountedPointer_DeleteGraphicsBank_00419036',0x419036,11,'FableUiDestroyGraphicsBank'),
    ('00/9e/Global_StartBankProgress_009e9f40',0x9E9F40,35,'FableUiStartBankProgress'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]

def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x40000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,manager,head,nodes,strings,buffers,banks,refs=0x20000000,0x20008000,0x20010000,0x20010100,0x20011000,0x20012000,0x20013000,0x20014000,0x20017000
    names,results,init,texta,textb=0x20018000,0x20018100,0x20018200,0x20018300,0x20018400
    bv,pv,progress,delete_cb,open_cb,progress_cb=0x20019000,0x20019100,0x20019200,0x20019300,0x20019400,0x20019500
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    def ident(p,base,stride): return (p-base)//stride+1 if p else 0
    def nodeid(p): return 0 if p==head else ident(p,nodes,64)
    def text(p): return bytes(uc.mem_read(word(word(p)),100)).split(b'\0')[0].hex() if word(p) else ''
    for address,cleanup in ((0xBFEA1A,0),(0xBFEA0E,0),(0xBFE9BC,0),(0xBFEB22,0),(0xBFEB1C,0),(0x9FEA20,0),(0x9FD4E0,4),(delete_cb,4),(open_cb,8),(progress_cb,16)):
        uc.mem_write(address,b'\xc2'+struct.pack('<H',cleanup) if cleanup else b'\xc3')
    put(bv,delete_cb); put(bv+4,open_cb); put(pv+12,progress_cb); put(progress,pv)
    events=[]; ns=nb=ng=nr=nn=attempt=0; case=None
    def hook(uc,address,size,data):
        nonlocal ns,nb,ng,nr,nn,attempt
        esp=uc.reg_read(UC_X86_REG_ESP); receiver=uc.reg_read(UC_X86_REG_ECX)
        if address==0xBFEA1A:
            n=word(esp+4)
            if n==17: events.append('SR17'); p=strings+ns*32; ns+=1
            elif n==0x30C: events.append('A780'); p=banks+ng*0x30C; ng+=1
            elif n==12:
                events.append('A12'); attempt+=1
                if case[3]==2 or (case[3]==1 and attempt==1): events.append('NOINFO'); p=0
                else: p=refs+nr*12; nr+=1
            else: raise RuntimeError('Unknown scalar allocation')
            uc.reg_write(UC_X86_REG_EAX,p)
        elif address==0xBFEA0E:
            if word(esp+4)!=64: raise RuntimeError('Wrong cache node size')
            events.append('L64'); uc.reg_write(UC_X86_REG_EAX,nodes+nn*64); nn+=1
        elif address==0xBFEB22:
            n=word(esp+4)
            if n>128: raise RuntimeError('Buffer overflow')
            events.append('SB'+str(n)); uc.reg_write(UC_X86_REG_EAX,buffers+nb*128); nb+=1
        elif address==0xBFEB1C: events.append('ST'+str(ident(word(esp+4),buffers,128)))
        elif address==0xBFE9BC:
            p=word(esp+4); events.append(('SF'+str(ident(p,strings,32))) if strings<=p<strings+512 else 'RF'+str(ident(p,refs,12)))
        elif address==0x9FEA20:
            events.append('CTOR'+str(ident(receiver,banks,0x30C))); put(receiver,bv); uc.reg_write(UC_X86_REG_EAX,receiver)
        elif address==0x9FD4E0: events.append('INIT'+str(ident(receiver,banks,0x30C))+':'+bytes(uc.mem_read(word(esp+4),44)).hex())
        elif address==delete_cb: events.append(f'DELETE{ident(receiver,banks,0x30C)}:{word(esp+4)}')
        elif address==open_cb: events.append(f'OPEN{ident(receiver,banks,0x30C)}:{word(esp+8)}:'+text(word(esp+4)))
        elif address==progress_cb:
            events.append(f'PROGRESS{word(esp+8):08x}:{word(esp+12)&255}:{word(esp+16)&255}:'+text(word(esp+4)))
            uc.mem_write(0x13CA7B0,bytes([uc.mem_read(0x13CA7B0,1)[0]^1]))
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product(range(4),range(4),(0,1,255),range(3),range(2),(0,0xA5)))
    directory=ROOT/'work/ui_bank_factory_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+STRING_SOURCES+OWNERSHIP_SOURCES+['rebuild/tests/integration/UiBankFactory_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete factory traces')
    def execute(address,receiver,args=(),check_result=None):
        put(stack,stop)
        for i,arg in enumerate(args): put(stack+4+i*4,arg)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4: raise RuntimeError('Factory return/stack mismatch')
        if check_result is not None and uc.reg_read(UC_X86_REG_EAX)!=check_result: raise RuntimeError('Wrong factory result pointer')
    def snapshot(count):
        events.append(f'S{uc.mem_read(0x13CA7B0,1)[0]}:{signed(0x13BD800)}:{ng}')
        for i in range(count): events.append(f'O{ident(word(results+i*8),banks,0x30C)}:{ident(word(results+i*8+4),refs,12)}')
        for i in range(nr): events.append(f'R{i}:{signed(refs+i*12)}:{int(word(refs+i*12+4)==0x419036)}:{ident(word(refs+i*12+8),banks,0x30C)}')
        events.append(f'HEAD{nodeid(word(head))}:{nodeid(word(head+4))}')
        for i in range(nn):
            p=nodes+i*64; events.append(f'N{i}:{nodeid(word(p))}:{nodeid(word(p+4))}:{ident(word(p+52),strings,32)}:{ident(word(p+56),banks,0x30C)}:{ident(word(p+60),refs,12)}:'+bytes(uc.mem_read(p+8,44)).hex())
        for i in range(ns): events.append(f'T{i}:{signed(strings+i*32+13)}')
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        name_case,variant,open_mode,fail_info,with_progress,pattern=case; events.clear(); ns=nb=ng=nr=nn=attempt=0
        uc.mem_write(strings,b'\xA5'*512); uc.mem_write(buffers,b'\xCD'*2048); uc.mem_write(banks,b'\xA5'*(8*0x30C)); uc.mem_write(nodes,b'\xA5'*512); uc.mem_write(refs,b'\xA5'*96)
        put(head,head); put(head+4,head); put(manager+4,head); uc.mem_write(0x13CA7B0,bytes([open_mode])); put(0x13CAA38,progress if with_progress else 0); put(0x13BD800,500)
        a=bytearray(b'GBANK_MAIN\0'+b'\0'*21)
        if name_case==1: a[0]=0
        if name_case==2: a[:4]=b'X\x80a\0'
        if name_case==3: a[:3]=b'AB\0'
        b=bytearray(a)
        if variant==2: b[0]=ord('z') if b[0] else ord('Q')
        uc.mem_write(texta,bytes(a)); uc.mem_write(textb,bytes(b)); uc.mem_write(init,bytes([pattern])*44)
        execute(0x99EBF0,names,(texta,-1)); execute(0x99EBF0,names+4,(textb,len(bytes(b).split(b'\0')[0])+2 if variant==3 else -1))
        for call in range(3):
            name=names+4 if call==1 and variant else names
            execute(0x9F83D0,manager,(results+call*8,name,init,call&1,(call&2)!=0),results+call*8); snapshot(call+1); put(init+4,call+123)
        events.append('CLEAN')
        for i in range(3): execute(0x419108,results+i*8)
        for i in range(nn): execute(0x419108,nodes+i*64+56); execute(0x99EAE0,nodes+i*64+52)
        execute(0x99EAE0,names); execute(0x99EAE0,names+4); snapshot(3)
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,comparisons=comparisons,scope='complete factory/cache plus strings, ownership, progress/delete dispatch; bank ctor/init/open and allocators controlled; cache hits/misses, init differences, null control records and cleanup'),indent=2)+'\n')
    print(f"UI_BANK_FACTORY {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))

if __name__=='__main__': raise SystemExit(main())
