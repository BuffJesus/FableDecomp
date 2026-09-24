#!/usr/bin/env python3
"""Remaining word/string vector and string-tree storage services vs retail."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
FUNCTIONS=[
 ('00/46/UIntVector_ResizeBankChecksums_00464931',0x464931,62,'FableUiResizeBankChecksums'),
 ('00/9d/UIntVector_ResizeBankUpdates_009d3df0',0x9D3DF0,87,'FableUiResizeBankUpdates'),
 ('00/49/StringVector_ResizeBankSymbols_0049b760',0x49B760,101,'FableUiResizeBankSymbols'),
 ('00/57/StringTree_DestroyBankSubtree_00579435',0x579435,53,'FableUiDestroyBankStringTree'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
DEPENDENCIES=['rebuild/src/compiled/00/99/'+n+'.cpp' for n in ('CCharString_ConstructEmpty_0099e4b0','CCharString_Copy_0099ec30','CCharString_Assign_0099efb0','CCharString_Unassign_0099e9b0','CCharString_Destroy_0099eae0')]+['rebuild/src/compiled/00/41/CCharString_DestroyRange_00414e00.cpp','rebuild/src/compiled/00/43/StringVector_EraseRange_0043336a.cpp']
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_storage_helpers_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiBankStorageHelpers_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=3328:raise RuntimeError('Incomplete storage traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,array,old,new,records,nodes,fill,tree=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000,0x20015000,0x20016000
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xffffffff))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    def normalize(v):
        for i,(base,size) in enumerate(((old,128),(new,128),(records,136),(nodes,168))):
            if base<=v<base+size:return 0x61000000+i*0x1000000+v-base
        return v
    def dump(p,n):return ''.join(f':{normalize(word(p+i)):08x}' for i in range(0,n,4))
    services=(0xBFEA0E,0xBFEA14,0xBFE9BC,0xBFEAE6)
    for a in services:uc.mem_write(a,b'\xc3')
    events=[]
    def hook(uc,address,size,data):
        if address not in services:return
        esp=uc.reg_read(UC_X86_REG_ESP);arg=word(esp+4)
        if address==0xBFEA0E:events.append(f'ALLOC{arg}:{word(0x13BD800)}');uc.reg_write(UC_X86_REG_EAX,new)
        elif address==0xBFEA14:events.append(f'FREE{normalize(arg):08x}:{word(0x13BD800)}')
        elif address==0xBFE9BC:events.append('RF'+str((arg-records)//17))
        elif address==0xBFEAE6:
            n=word(esp+12)
            if n:uc.mem_write(arg,bytes(uc.mem_read(word(esp+8),n)))
            uc.reg_write(UC_X86_REG_EAX,arg)
    uc.hook_add(UC_HOOK_CODE,hook);errors=[]
    for index,actual in enumerate(lines):
        mode,seed=divmod(index,1024);events.clear();uc.mem_write(new,b'\xcd'*128);uc.mem_write(records,b'\0'*136);uc.mem_write(nodes,b'\xa5'*168);put(0x13BD800,100)
        for i in range(32):put(old+i*4,seed*131+i*17)
        put(fill,0xa5b6c7d8);length=seed%8;spare=(seed//8)%4;count=(seed//32)%16
        put(array,old);put(array+4,old+length*4);put(array+8,old+(length+spare)*4)
        if mode==2:
            for i in range(length):
                rec=records+(seed+i)%5*17;put(old+i*4,rec);put(rec+13,word(rec+13)+1)
        if mode==3:
            count=seed%8
            for i in range(count):
                left=i+1 if seed&8 else i*2+1;right=count if seed&8 else i*2+2
                left=nodes+left*24 if left<count else 0;right=nodes+right*24 if right<count else 0
                if seed&16:left,right=right,left
                put(nodes+i*24+8,left);put(nodes+i*24+12,right);rec=0 if seed&32 else records+(i+seed)%5*17;put(nodes+i*24+16,rec)
                if rec:put(rec+13,word(rec+13)+1)
        value=old+(length-1)*4 if seed&512 and length else fill
        address=FUNCTIONS[mode][1];receiver=tree if mode==3 else array;args=[nodes if count else 0] if mode==3 else [count] if mode==2 else [count,value]
        put(stack,stop)
        for i,v in enumerate(args):put(stack+4+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,receiver);uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4:raise RuntimeError('Storage helper stack mismatch')
        events.extend(('ARRAY'+dump(array,12),'OLD'+dump(old,128),'NEW'+dump(new,128),'NODES'+dump(nodes,168),'RECORDS'+bytes(uc.mem_read(records,136)).hex(),'COUNT'+str(word(0x13BD800))))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=index,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section);off=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address);matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[off:off+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()));(directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    helper,section,_=parity.obj_text(objects[0],'?FableUiResizeWordVector');helper_relocs=parity.obj_relocs(objects[0],section)
    shared_helper=dict(symbol='FableUiResizeWordVector',compiled_bytes=len(helper),masked_sha256=hashlib.sha256(parity.mask(helper,helper_relocs)).hexdigest())
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(lines),errors=errors,comparisons=comparisons,shared_helper=shared_helper,scope='Real word/string resizing and string-tree deletion; allocator/free and CRT copy boundaries. Valid capacities, alias fill, full storage, string ownership, tree shapes/order. Word wrappers share an inline helper; delegated retail growth logic is inlined into reconstructed code.'),indent=2)+'\n')
    print(f"UI_BANK_STORAGE_HELPERS {'FAIL' if errors else 'PASS'} cases={len(lines)} failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
