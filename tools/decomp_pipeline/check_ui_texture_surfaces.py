#!/usr/bin/env python3
"""Recovered texture/surface code and connected bank initialization vs retail D3D calls."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run
FUNCTIONS=[
 ('00/9f/CSurface_Release_009f2e20',0x9F2E20,32,'FableUiReleaseSurface'),
 ('00/9f/CSurface_Attach_009f2f10',0x9F2F10,66,'FableUiAttachSurface'),
 ('00/9f/CSurface_Copy_009f2d60',0x9F2D60,55,'FableUiCopySurface'),
 ('00/9f/CTexture_GetSurface_009f9e00',0x9F9E00,96,'FableUiGetTextureSurface'),
 ('00/9f/CSurface_Lock_009f33e0',0x9F33E0,141,'FableUiLockSurface'),
 ('00/9e/CPixelFormat_GetBits_009e3820',0x9E3820,13,'FableUiPixelFormatBits'),
 ('00/9f/CSurface_Clear_009f40a0',0x9F40A0,120,'FableUiClearSurface'),
 ('00/9f/CTexture_Release_009f9f70',0x9F9F70,34,'FableUiReleaseTexture'),
 ('00/9f/CTexture_UpdateByteLength_009f9ee0',0x9F9EE0,143,'FableUiUpdateTextureByteLength'),
 ('00/9f/CTexture_CreateBlank_009fa280',0x9FA280,181,'FableUiCreateBlankTexture'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
DEPENDENCIES=[
 'rebuild/src/compiled/00/9e/CPixelFormat_SetD3DFormat_009e3830.cpp',
 'rebuild/src/compiled/00/9a/CSystemManager_GetGlobal_009a4ec0.cpp',
 'rebuild/src/compiled/00/9f/CGraphicDataBank_Initialise_009fd4e0.cpp',
 'rebuild/src/compiled/00/9d/CBankFile_SetPreloadPolicy_009d5230.cpp',
]
def cases_to_run():
    cases=[]
    for mode in range(4):
        for seed,status,mutation,null,source in itertools.product(range(8),(-2147467259,0,1),range(3),range(2),(0,2,3)):
            if mode in (1,2) and (null or source!=0): continue
            width=(0,1,7,8,16,31,32,65)[seed]; height=(1,0,8,7,16,32,63,9)[seed]
            if mode==2: width=min(width,16); height=min(height,16)
            cases.append((mode,seed,status,width,height,(-1,0,1,4)[seed%4],seed%4,(0,1,2,3,0xDEADBEEF)[seed%5],mutation,null,source))
    # Wraparound of 32-bit byte products, signed level counts, unusual mip dimensions.
    for seed in range(32):
        cases.append((3,seed,0,0x80000000+seed,0xFFFFFFFF-seed,-1,seed%4,seed,seed%3,seed%2,2))
    for mask in range(128):
        cases.append((4,mask,0,8+mask%9,8+mask%7,-1,0,1,0,0,0))
    return cases

def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_texture_surfaces_check'; directory.mkdir(parents=True,exist_ok=True)
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiTextureSurfaces_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    cases=cases_to_run(); inputs=directory/'cases.txt'; inputs.write_text(''.join(' '.join(map(str,c))+'\n' for c in cases))
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete texture/surface traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x40000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,surface,copy,texture,bank=0x20000000,0x20008000,0x20010000,0x20010100,0x20010200,0x20011000
    surfaces,textures,device,sv,tv,dv,display=0x20012000,0x20012100,0x20012200,0x20013000,0x20013100,0x20013200,0x20014000
    pixels,lock,dim,fmt,init=0x20016000,0x20019000,0x20019100,0x20019200,0x20019300
    callbacks=[0x20020000+i*16 for i in range(10)]
    sa,sr,sd,sl,su,tr,tc,td,ts,dc=callbacks
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    def sid(p): return (p-surfaces)//4+1 if p else 0
    def tid(p): return (p-textures)//4+1 if p else 0
    for p,clean in zip(callbacks,(4,4,8,16,4,4,4,12,12,36)): uc.mem_write(p,b'\xc2'+struct.pack('<H',clean))
    for offset,cb in ((4,sa),(8,sr),(0x30,sd),(0x34,sl),(0x38,su)): put(sv+offset,cb)
    for offset,cb in ((8,tr),(0x34,tc),(0x44,td),(0x48,ts)): put(tv+offset,cb)
    put(dv+0x5C,dc); put(device,dv)
    for i in range(2): put(surfaces+i*4,sv); put(textures+i*4,tv)
    put(0x13CA618+0x60,display); put(display+0x58,device)
    formats=[21,22,0x31545844,34]
    for i,bits in enumerate((32,24,4,16)): put(0x129BA40+i*36,formats[i]); put(0x129BA48+i*36,bits)
    put(0x129BA48+4*36,-1)
    events=[]; case=None; descriptions=level_descriptions=0
    def hook(uc,address,size,data):
        nonlocal descriptions,level_descriptions
        if address not in callbacks: return
        mode,seed,status,width,height,levels,format_,pool,mutation,null,source=case
        esp=uc.reg_read(UC_X86_REG_ESP); p=word(esp+4)
        if address==sa: events.append('SA'+str(sid(p))); result=9
        elif address==sr:
            events.append('SR'+str(sid(p))); result=8
            if mutation==2: put(surface+4,surfaces+4); put(surface+8,0x98765432); put(surface+12,0x76543210)
        elif address==sd:
            n=descriptions; descriptions+=1; out=word(esp+8)
            w=(width+n%3)&0xFFFFFFFF; h=(height+n%2)&0xFFFFFFFF; f=formats[(seed+n)%4]
            uc.mem_write(out,b'\x63'*32); put(out,f); put(out+24,w); put(out+28,h)
            events.append(f'SD{sid(p)}:{n}:{w}:{h}:{f}'); result=status
            if mutation==1: put(surface+4,surfaces+4)
        elif address==sl:
            out,rect,flags=word(esp+8),word(esp+12),word(esp+16)
            events.append(f'SL{sid(p)}:'+':'.join(map(str,[signed(rect+i*4) for i in range(4)]+[flags])))
            put(out,width*7+seed); put(out+4,pixels+16); result=status if mode==1 else 0
        elif address==su: events.append('SU'+str(sid(p))); result=status
        elif address==tr:
            events.append('TR'+str(tid(p))); result=3
            if mutation==2: put(texture+4,0xCAFEBABE); put(texture,textures+4)
        elif address==tc:
            n=seed%5; n=0xFFFFFFFF if n==4 else n; events.append(f'TC{tid(p)}:{n}'); result=n
        elif address==td:
            level,out=word(esp+8),word(esp+12); n=level_descriptions; level_descriptions+=1
            shift=min(level,5); w=((width>>shift)+1)&0xFFFFFFFF; h=((height>>shift)+1)&0xFFFFFFFF; f=formats[(seed+n)%4]
            uc.mem_write(out,b'\x47'*32); put(out,f); put(out+24,w); put(out+28,h)
            events.append(f'TD{tid(p)}:{level}:{w}:{h}:{f}'); result=status
            if mutation==1: put(texture,textures+4)
        elif address==ts:
            level,out=word(esp+8),word(esp+12); events.append(f'TS{tid(p)}:{level}')
            put(out,0 if mode==0 and null else surfaces+(seed%2)*4); result=status
        elif address==dc:
            args=[word(esp+i*4) for i in range(2,10)]
            events.append('DC'+':'.join(map(str,args[:6]+[int(args[7]!=0)])))
            put(args[6],0 if mode==3 and status<0 and null else textures+4); result=0 if mode==4 else status
        uc.reg_write(UC_X86_REG_EAX,result&0xFFFFFFFF)
    uc.hook_add(UC_HOOK_CODE,hook)
    def execute(address,receiver,args=(),result=None):
        put(stack,stop)
        for i,arg in enumerate(args): put(stack+4+i*4,arg)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.emu_start(address,stop,count=200000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4: raise RuntimeError('Texture/surface return/stack mismatch')
        value=uc.reg_read(UC_X86_REG_EAX)
        if result is not None and value!=result: raise RuntimeError('Wrong output return')
        return value
    def dump_surface(p):
        v=word(p); v=0x70000108 if v==0x122F84C else v
        events.append(f'S{v:08x}:{sid(word(p+4))}:{word(p+8):08x}:{word(p+12):08x}')
    def dump_texture(p): events.append(f'T{tid(word(p))}:{word(p+4):08x}')
    def dump_pixels():
        h=2166136261
        for v in bytes(uc.mem_read(pixels,8192)): h=((h^v)*16777619)&0xFFFFFFFF
        events.append(f'P{h:08x}')
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        mode,seed,status,width,height,levels,format_,pool,mutation,null,source=case
        events.clear(); descriptions=level_descriptions=0
        uc.mem_write(pixels,bytes([seed&255])*8192); uc.mem_write(surface,b'\xA5'*16); uc.mem_write(copy,b'\x5A'*16)
        put(surface+4,0 if null else surfaces); put(surface+8,source); put(texture,0 if null else textures); put(texture+4,0xD7654321)
        if mode==0:
            execute(0x9F2D60,copy,(surface,),copy); dump_surface(copy)
            execute(0x9F2D60,surface,(surface,),surface); dump_surface(surface)
            execute(0x9F2E20,surface); dump_surface(surface); execute(0x9F2E20,surface); dump_surface(surface)
            put(surface+4,surfaces); execute(0x9F2F10,surface,(0 if null else surfaces+4,)); dump_surface(surface)
            put(texture,textures); execute(0x9F9E00,texture,(copy,seed),copy); dump_surface(copy)
            execute(0x9F2E20,copy); dump_surface(copy)
        elif mode==1:
            put(surface+4,surfaces); uc.mem_write(lock,b'\xA5'*16); execute(0x9F33E0,surface,(lock,pool),lock)
            events.append(f'L{word(lock)}:{word(lock+4)}:{signed(lock+8)}:{16 if word(lock+12) else 0}'); dump_surface(surface)
        elif mode==2:
            put(surface+4,surfaces); execute(0x9F40A0,surface); dump_surface(surface); dump_pixels()
        elif mode==3:
            execute(0x9F9EE0,texture); dump_texture(texture)
            put(dim,width); put(dim+4,height); put(fmt,format_)
            r=execute(0x9FA280,texture,(dim,levels,fmt,seed,pool,seed&1,source))
            events.append('R'+str(r&255)); dump_texture(texture); execute(0x9F9F70,texture); dump_texture(texture)
        else:
            uc.mem_write(bank,b'\xA5'*0x30C); uc.mem_write(bank+0x2B4,b'\0'*88); uc.mem_write(init,b'\0'*44)
            for i in range(7): put(init+i*4,i%4 if seed&(1<<i) else -1)
            execute(0x9FD4E0,bank,(init,))
            for i in range(11): dump_texture(bank+0x2B4+i*8)
            values=[bytes(uc.mem_read(bank+0x1F8,1))[0]]+[word(bank+o) for o in (0x268,0x26C,0x270,0x274,0x278)]+[bytes(uc.mem_read(bank+0x27C,1))[0],word(bank+0x260),word(bank+0x264)]
            events.append('B'+':'.join(map(str,values))); dump_pixels()
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section)
        offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address); matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,comparisons=comparisons,scope='real surface lifetime/lock/clear, texture creation/size/release, connected bank initialization; only D3D COM calls controlled'),indent=2)+'\n')
    print(f"UI_TEXTURE_SURFACES {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))
if __name__=='__main__': raise SystemExit(main())
