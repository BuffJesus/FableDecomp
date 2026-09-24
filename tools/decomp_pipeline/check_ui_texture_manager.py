#!/usr/bin/env python3
"""All texture-manager bytes and sixteen pool self-links against retail."""
import hashlib
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run
FUNCTIONS=[
 ('00/a6/CTextureManager_Construct_00a6a360',0xA6A360,219,'FableUiConstructTextureManager'),
 ('00/9f/CResourceList_Construct_009fc570',0x9FC570,77,'FableUiConstructResourceList'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,storage=0x20000000,0x20008000,0x20010000; manager=storage+8
    directory=ROOT/'work/ui_texture_manager_check'; directory.mkdir(parents=True,exist_ok=True)
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+['rebuild/src/compiled/00/99/CBase_ConstructUiResourceBase_0099a2f0.cpp','rebuild/tests/integration/UiTextureManager_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=256: raise RuntimeError('Incomplete texture-manager traces')
    errors=[]; symbols={0x129DC5C:0x70000109,0x129C808:0x70000103,0x12354A4:0x70000104,0x129DC50:0x7000010A}
    for seed,actual in enumerate(lines):
        original=bytes((seed+i*17)&255 for i in range(0x5E4)); uc.mem_write(storage,original); uc.mem_write(stack,struct.pack('<I',stop))
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,manager); uc.emu_start(0xA6A360,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4 or uc.reg_read(UC_X86_REG_EAX)!=manager: raise RuntimeError('Texture-manager return/stack mismatch')
        state=bytes(uc.mem_read(storage,0x5E4))
        if state[:8]!=original[:8] or state[-8:]!=original[-8:]: raise RuntimeError('Retail exceeded manager extent')
        words=[]
        for word in struct.unpack('<373I',state[8:-8]):
            if word in symbols: word=symbols[word]
            elif manager<=word<manager+0x5D4: word=0x60000000+word-manager
            words.append(word)
        expected='CASE'+str(seed)+''.join(f':{word:08x}' for word in words)
        if actual!=expected: errors.append(dict(case=seed,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section)
        offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address); matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER'))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=256,errors=errors,comparisons=comparisons,scope='full texture-manager/resource-list/base construction, all 1492 bytes with guards; no external calls doubled'),indent=2)+'\n')
    print(f"UI_TEXTURE_MANAGER {'FAIL' if errors else 'PASS'} cases=256 failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))
if __name__=='__main__': raise SystemExit(main())
