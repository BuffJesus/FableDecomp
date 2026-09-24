#!/usr/bin/env python3
"""Retail display format selection, pixel lookup, dimensions and global getter."""
import hashlib
import itertools
import json
import random
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run

FUNCTIONS=[
    ('00/9b/CDisplayManager_ChooseNonAlphaFormat_009be830',0x9BE830,63,'FableUiChooseNonAlphaFormat'),
    ('00/9b/CDisplayManager_ChooseAlphaFormat_009be870',0x9BE870,63,'FableUiChooseAlphaFormat'),
    ('00/9b/CDisplayManager_ChooseBooleanAlphaFormat_009be8b0',0x9BE8B0,63,'FableUiChooseBooleanAlphaFormat'),
    ('00/9b/CDisplayManager_ChooseUncompressedAlphaFormat_009be6c0',0x9BE6C0,140,'FableUiChooseUncompressedAlphaFormat'),
    ('00/9b/CDisplayManager_ChooseUncompressedNonAlphaFormat_009be610',0x9BE610,171,'FableUiChooseUncompressedNonAlphaFormat'),
    ('00/9b/CDisplayManager_ChooseSignedFormat_009be590',0x9BE590,126,'FableUiChooseSignedFormat'),
    ('00/9e/CPixelFormat_SetD3DFormat_009e3830',0x9E3830,50,'FableUiSetPixelFormat'),
    ('00/9b/CDisplayManager_QueryRenderTargetExtent_009bedc0',0x9BEDC0,24,'FableUiQueryDisplayExtent'),
    ('00/9a/CSystemManager_GetGlobal_009a4ec0',0x9A4EC0,6,'FableUiGetSystemManager'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]

def retail_table(image):
    start=pe_oracle.va_to_off(pe_oracle.pe_sections(image),0x129BA40); rows=[]
    for i in range(64):
        row=struct.unpack_from('<I8i',image,start+i*36)
        if row[2]==-1: return rows
        rows.append(row)
    raise RuntimeError('Pixel format sentinel missing')

def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x30000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,display,d3d,vtable,callback,out,extent=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000,0x20015000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    uc.mem_write(callback,b'\xc2\x1c\x00'); put(d3d,vtable); put(vtable+40,callback)
    events=[]; case=None
    def hook(uc,address,size,data):
        if address!=callback: return
        esp=uc.reg_read(UC_X86_REG_ESP); receiver,adapter,device,mode,usage,resource,fmt=[word(esp+4+i*4) for i in range(7)]
        if receiver!=d3d: raise RuntimeError('Wrong D3D interface')
        policy=case[2]; choice=(fmt^policy)%3
        result=0 if policy==0 else -1 if policy==1 else -2147483648 if choice==0 else 1 if choice==1 else 0
        events.append(f'Q{adapter}:{device}:{mode}:{usage}:{resource}:{fmt}:{result}'); uc.reg_write(UC_X86_REG_EAX,result&0xFFFFFFFF)
        if policy&8:
            for p in (display+0x60,display+0x5C,display+0x1C4): put(p,word(p)+1)
    uc.hook_add(UC_HOOK_CODE,hook)
    tables=[retail_table(image),[]]
    rng=random.Random(0x9BE610)
    for n in (1,2,7,16,31,46):
        rows=[]
        for i in range(n): rows.append((rng.choice((20,21,22,23,24,60,61,0x31545844,0x33545844)),rng.choice((1,2,6)),rng.choice((8,16,24,32)),rng.choice((-1,0,1,2,8)),*(rng.choice((-1,0,1,2,8)) for _ in range(3)),0,0))
        tables.append(rows)
    # Deliberate ties, a lower-alpha later choice, and first-entry-only alpha.
    tables.append([(21,2,32,8,8,8,8,0,0),(22,2,32,2,8,8,8,0,0),(23,2,32,2,8,8,8,0,0),(24,2,32,0,8,8,8,0,0),(60,6,16,0,8,8,0,0,0),(61,6,16,0,5,5,6,0,0)])
    cases=[(mode,bits,policy,usage,640 if i%2 else -2147483648,480 if i%2 else 2147483647,i)
           for i,mode,bits,policy,usage in itertools.product(range(len(tables)),range(6),(-1,8,16,24,32,64),(0,1,2,11),(0,1))]
    cases += [(6,code,0,0,1920,1080,i) for i in range(len(tables)) for code in (0,20,21,22,60,61,102,0x31545844,0x33545844,0x12345678)]
    directory=ROOT/'work/ui_display_formats_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c[:6]+(len(tables[c[6]]),)))+'\n'+'\n'.join(' '.join(map(str,r)) for r in tables[c[6]]) for c in cases)+'\n')
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+['rebuild/tests/integration/UiDisplayFormats_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete display traces')
    def execute(address,receiver,args=()):
        put(stack,stop)
        for i,arg in enumerate(args): put(stack+4+i*4,arg)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4: raise RuntimeError('Display return/stack mismatch')
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        mode,bits,policy,usage,width,height,table_index=case; events.clear()
        blank=struct.pack('<I8i',0,0,-1,0,0,0,0,0,0); uc.mem_write(0x129BA40,blank*64)
        for i,row in enumerate(tables[table_index]): uc.mem_write(0x129BA40+i*36,struct.pack('<I8i',*row))
        uc.mem_write(display,b'\xA5'*0x1C8); put(display+0x54,d3d); put(display+0x60,7); put(display+0x5C,2); put(display+0x1C4,21); put(display+0x194,width); put(display+0x198,height); put(out,-1234567)
        args=(bits,out,usage) if mode==4 else (bits,out) if mode<6 else (bits,)
        execute(FUNCTIONS[mode][1],out if mode==6 else display,args)
        ok=uc.reg_read(UC_X86_REG_EAX)&255 if mode<6 else 0
        execute(0x9BEDC0,display,(extent,)); execute(0x9A4EC0,0)
        events.append(f'O{ok}:{signed(out)} D{signed(extent)}:{signed(extent+4)} G{int(uc.reg_read(UC_X86_REG_EAX)==0x13CA618)} C{word(display+0x60)}:{word(display+0x5C)}:{word(display+0x1C4)}')
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section); offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address); retail=image[offset:offset+size]
        matched=len(code)==size and parity.mask(code,relocs)==parity.mask(retail,relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,comparisons=comparisons,scope='six selectors/pixel lookup/dimensions/global getter; real and adversarial tables; controlled D3D capability HRESULT and mutation'),indent=2)+'\n')
    print(f"UI_DISPLAY_FORMATS {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))

if __name__=='__main__': raise SystemExit(main())
