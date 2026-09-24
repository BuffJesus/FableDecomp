#!/usr/bin/env python3
"""Real bank registry/map/string comparator vs retail, including callback ownership."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
FUNCTIONS=[
 ('00/42/CCharStringData_Less_00429950',0x429950,88,'FableUiStringDataLess'),
 ('00/9a/CStringMap_FindContainedBank_009ab4f0',0x9AB4F0,110,'FableUiFindContainedBankNode'),
 ('00/9a/CStringMap_FindBankPath_009ab560',0x9AB560,110,'FableUiFindBankPathNode'),
 ('00/9a/CStringMap_FindBankAlias_009ab5d0',0x9AB5D0,110,'FableUiFindBankAliasNode'),
 ('00/9a/CBankRegistry_FindRegisteredBank_009a7f80',0x9A7F80,270,'FableUiFindRegisteredBank'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
DEPENDENCIES=['rebuild/src/compiled/00/99/'+n+'.cpp' for n in ['CCharString_Copy_0099ec30','CCharString_Destroy_0099eae0','CCharString_Unassign_0099e9b0']]+['rebuild/src/compiled/00/41/'+n+'.cpp' for n in ['CCountedPointer_ReleaseBankReference_00419108','CCountedPointer_ShareBankReference_00419134']]
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_registry_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiBankRegistry_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=1657:raise RuntimeError('Incomplete registry traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x30000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,heads,nodes,banks,list_,refs,records,texts,tree,query,registry,out,header,destroy= [0x20000000,0x20008000,0x20010000,0x20011000,0x20013000,0x20014000,0x20015000,0x20016000,0x20017000,0x20018000,0x20018100,0x20019000,0x2001A000,0x2001B000,0x2001C000]
    values=[None,b'\x80',b'\xff',b'',b'a',b'aa',b'ab',b'b',b'bank',b'z',b'a',b'ab\0tail']
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p):return struct.unpack('<i',uc.mem_read(p,4))[0]
    def bid(p):return 1 if p==banks else 2 if p==banks+36 else 9
    def rid(p):return (p-refs)//12+1 if p else 0
    events=[];seed=0
    uc.mem_write(destroy,b'\xc3');uc.mem_write(0xBFE9BC,b'\xc3')
    def hook(uc,address,size,data):
        if address==destroy:
            id_=bid(uc.reg_read(UC_X86_REG_ECX));events.append('D'+str(id_))
            if seed&16 and id_<=2:put(list_+id_*16+12,refs+36)
        elif address==0xBFE9BC:events.append('F'+str(rid(word(uc.reg_read(UC_X86_REG_ESP)+4))))
    uc.hook_add(UC_HOOK_CODE,hook)
    def initialize():
        uc.mem_write(records,b'\xA5'*(12*17))
        for i in range(1,12):
            p=records+i*17;put(p,texts+i*32);uc.mem_write(texts+i*32,values[i]+b'\0');put(p+4,len(values[i]));put(p+13,10)
        put(0x13BD800,200)
    def build(which,mask):
        head=heads+which*16;base=nodes+which*400;uc.mem_write(head,b'\xA5'*16);put(head+4,0);uc.mem_write(base,b'\xA5'*400)
        for j in range(10):
            key=(j+seed)%10
            if not mask&(1<<key):continue
            node=base+key*40;put(node+16,records+key*17 if key else 0);put(node+8,0);put(node+12,0);link=head+4
            while word(link):
                index=(word(link)-base)//40;link=word(link)+(8 if key<index else 12)
            put(link,node)
            for i,v in enumerate((which*100+key,0xFFFFFF00+key,which*4096+key*16,0x12345678,1<<(key%8))):put(node+20+i*4,v)
    def execute(address,receiver,args):
        put(stack,stop)
        for i,a in enumerate(args):put(stack+4+i*4,a)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,receiver);uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4:raise RuntimeError('Registry stack mismatch')
        return uc.reg_read(UC_X86_REG_EAX)
    expected=[];initialize()
    for a in range(1,12):
        for b in range(1,12):expected.append(f'CMP{a}:{b}:{execute(0x429950,records+a*17,[records+b*17])&255}')
    for seed in range(64):
        for q in range(12):
            initialize();build(0,((seed*73)^0x2AD)&1023);put(tree,heads);put(query,records+q*17 if q else 0);found=[]
            for address in (0x9AB4F0,0x9AB560,0x9AB5D0):
                result=execute(address,tree,[query]);found.append(99 if result==heads else (result-nodes)//40)
            expected.append('FIND'+':'.join(map(str,found)))
    for seed in range(64):
        for q in range(12):
            initialize();events.clear();uc.mem_write(banks,b'\xA5'*72);uc.mem_write(list_,b'\0'*48)
            for i in range(4):put(refs+i*12,0 if i<2 and seed&16 else 1);put(refs+i*12+4,destroy);put(refs+i*12+8,banks+i*36 if i<2 else 0x12345678)
            mask=(1<<4 if seed&1 else 0)|(1 if seed&2 else 0)|(1<<8 if seed&4 else 0);build(0,mask)
            for key,target in ((0,3),(4,7),(8,9)):put(nodes+key*40+20,records+target*17)
            put(registry+24,heads);count=seed%3;put(list_,list_+16 if count else list_);put(list_+4,list_+count*16 if count else list_);put(registry+16,list_)
            for i in range(2):
                build(i+1,(((seed+17*i)*73)^0x2AD)&1023);put(banks+i*36+24,heads+(i+1)*16)
                node=list_+(i+1)*16;put(node,list_+(i+2)*16 if i+1<count else list_);put(node+4,list_+i*16);put(node+8,banks+i*36);put(node+12,0 if seed&8 else refs+i*12)
            put(out,0x12345678);put(out+4,refs if seed&32 else refs+24);uc.mem_write(header,b'\xA5'*20);put(query,records+q*17 if q else 0)
            result=execute(0x9A7F80,registry,[query,header,out]);events.append(f'RESULT{result&255}:{bid(word(out))}:{rid(word(out+4))}')
            events.append('H'+''.join(f':{word(header+i*4):08x}' for i in range(5)));events.append('REFS'+''.join(':'+str(signed(refs+i*12)) for i in range(4)))
            events.append(f'LIST:{rid(word(list_+28))}:{rid(word(list_+44))}');events.append('STR:'+str(word(0x13BD800))+''.join(':'+str(signed(records+i*17+13)) for i in range(1,12)))
            expected.append('TRACE'+''.join(' '+e for e in events)+' END')
    errors=[dict(case=i,actual=a,retail=b) for i,(a,b) in enumerate(zip(lines,expected)) if a!=b];comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section);off=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address);matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[off:off+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()));(directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    helper,section,_=parity.obj_text(objects[1],'?FableUiFindStringMapNode'); helper_relocs=parity.obj_relocs(objects[1],section)
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(lines),errors=errors,comparisons=comparisons,shared_tree_body=dict(compiled_bytes=len(helper),masked_sha256=hashlib.sha256(parity.mask(helper,helper_relocs)).hexdigest()),scope='real registry, three tree lookups, signed string comparator, string lifetime and counted ownership; destruction callbacks controlled'),indent=2)+'\n')
    print(f"UI_BANK_REGISTRY {'FAIL' if errors else 'PASS'} cases={len(lines)} failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
