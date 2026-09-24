#!/usr/bin/env python3
"""Full CManager constructor connected to real singleton, containers and config."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run
from check_ui_bank_ownership import SOURCES as BANK_OWNERSHIP_SOURCES
from check_ui_strings import SOURCES as STRING_SOURCES
from check_ui_display_formats import SOURCES as DISPLAY_SOURCES, retail_table
from check_ui_bank_factory import SOURCES as FACTORY_SOURCES
from check_ui_bank_runtime import SOURCES as BANK_RUNTIME_SOURCES
from check_ui_texture_manager import SOURCES as TEXTURE_MANAGER_SOURCES
from check_ui_bank_file import SOURCES as BANK_FILE_SOURCES
from check_ui_bank_open import SOURCES as BANK_OPEN_SOURCES
from check_ui_bank_registry import SOURCES as BANK_REGISTRY_SOURCES
from check_ui_bank_stream import SOURCES as BANK_STREAM_SOURCES
from check_ui_bank_path import SOURCES as BANK_PATH_SOURCES
from check_ui_wide_strings import SOURCES as WIDE_STRING_SOURCES
from check_ui_texture_surfaces import SOURCES as TEXTURE_SURFACE_SOURCES


def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x60000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,pool,manager,system,mode_context=0x20000000,0x20008000,0x20010000,0x20013000,0x20014000,0x20015000
    bank_info,bank_destroy=0x20016000,0x20017000
    string_record,string_buffer=0x20018000,0x20019000
    display,devices,device_vtable,capability=0x2001A000,0x2001B000,0x2001C000,0x2001D000
    bank_manager,bank_head,bank_node,bank_storage,bank_vtable,bank_open=0x20020000,0x20020100,0x20020200,0x20021000,0x20022000,0x20023000
    texture_manager,texture_info,texture_vtable,texture_delete=0x20024000,0x20025000,0x20026000,0x20027000
    bank_critical=0x20028000
    open_missing_path=0x20029000
    native_textures,native_surfaces,native_device=0x20030000,0x20030100,0x20030200
    native_tv,native_sv,native_dv=0x20031000,0x20031100,0x20031200
    native_pixels=0x20040000
    callbacks=[0x20032000+i*16 for i in range(9)]
    sa,sr,sd,sl,su,tc,td,ts,dc=callbacks
    native_descriptions={}; native_levels={}; surface_references=[0]*11
    system=0x13CA618
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    def cstring(p): return bytes(uc.mem_read(p,64)).split(b'\0')[0].decode()
    formats=(0x9BE830,0x9BE870,0x9BE8B0,0x9BE6C0,0x9BE610,0x9BE590)
    for p,cleanup in [(0xBFEA0E,0),(0xBFEA1A,0),(bank_critical,4),(texture_delete,4),(bank_destroy,4),(open_missing_path,8),(0xBFE9BC,0),(0xBFEB22,0),(0xBFEB1C,0),(capability,28)]:
        uc.mem_write(p,b'\xc2'+struct.pack('<H',cleanup) if cleanup else b'\xc3')
    put(0x13CA79C+24,0x2002A000); put(0x2002A004,0)
    put(0x13CA79C+16,0x2002B000); put(0x2002B000,0x2002B000); put(0x2002B004,0x2002B000)
    put(0x13CA79C+4,0x2002C000); put(0x13CA79C+36,0); put(0x2002C004,0x2002C100)
    for i,t in enumerate((b'GBANK_FRONT_END',b'GBANK_MAIN')):
        node=0x2002C100+i*24; rec=0x2002D000+i*17; txt=0x2002D100+i*32
        uc.mem_write(node,b'\0'*24);put(node+16,rec);put(rec,txt);put(rec+13,10);uc.mem_write(txt,t+b'\0')
    put(0x2002C10C,0x2002C118)
    put(0x143FEE4,bank_critical)
    for p,cleanup in zip(callbacks,(4,4,8,16,4,4,12,12,36)): uc.mem_write(p,b'\xc2'+struct.pack('<H',cleanup))
    for offset,cb in ((4,sa),(8,sr),(0x30,sd),(0x34,sl),(0x38,su)): put(native_sv+offset,cb)
    for offset,cb in ((0x34,tc),(0x44,td),(0x48,ts)): put(native_tv+offset,cb)
    put(native_dv+0x5C,dc); put(native_device,native_dv)
    for i in range(11): put(native_textures+i*4,native_tv); put(native_surfaces+i*4,native_sv)
    events=[]; sizes=[]; case=None; string_count=buffer_count=reference_allocations=0
    def hook(uc,address,size,data):
        nonlocal string_count,buffer_count,reference_allocations
        esp=uc.reg_read(UC_X86_REG_ESP); receiver=uc.reg_read(UC_X86_REG_ECX)
        if address==0xBFEA0E:
            size=word(esp+4); i=len(sizes)
            if size==64: events.append('GN64'); uc.reg_write(UC_X86_REG_EAX,bank_node); return
            if i>=128 or size not in (12,16,20,24,28,52): raise RuntimeError('Unexpected manager allocation')
            events.append(f'A{i}:{size}'); sizes.append(size); uc.reg_write(UC_X86_REG_EAX,pool+i*64)
        elif address==0xBFEA1A:
            size=word(esp+4)
            if size==17:
                events.append('SR17'); uc.reg_write(UC_X86_REG_EAX,string_record+17*string_count); string_count+=1; return
            if size in (12,0x30C,0x5D4):
                events.append('GA'+str(size))
                if size==0x30C: p=bank_storage
                elif size==0x5D4: p=texture_manager
                else:
                    p=bank_info if reference_allocations else texture_info; reference_allocations+=1
                    if not case[6]: p=0
                uc.reg_write(UC_X86_REG_EAX,p); return
            events.append('J'+str(size))
            if size!=0xD0: raise RuntimeError('Wrong singleton allocation size')
            uc.reg_write(UC_X86_REG_EAX,manager)
        elif address==capability:
            values=[word(esp+i*4) for i in range(1,8)]; i=(values[0]-devices)//4
            if i not in (0,1): raise RuntimeError('Unexpected D3D receiver')
            events.append('CAP'+':'.join(map(str,[i]+values[1:])))
            put(0x13B8390,display+(1-i)*0x200); uc.reg_write(UC_X86_REG_EAX,0xFFFFFFFF if case[2]<0 else 0)
        elif address==0xBFEB22:
            size=word(esp+4); events.append('SB'+str(size))
            if size>64: raise RuntimeError('Unexpected bank-name allocation')
            uc.reg_write(UC_X86_REG_EAX,string_buffer+64*buffer_count); buffer_count+=1
        elif address==0xBFEB1C: events.append('ST'+str((word(esp+4)-string_buffer)//64))
        elif address==bank_critical:
            p=word(esp+4); events.append('CS'+str(p-bank_storage)); uc.mem_write(p,b'\x6C'*24)
        elif address==texture_delete: events.append('TDELETE'+str(word(esp+4)))
        elif address in callbacks:
            p=word(esp+4)
            if address in (sa,sr,sd,sl,su): i=(p-native_surfaces)//4
            elif address!=dc: i=(p-native_textures)//4
            if address==sa: events.append('SA'+str(i)); surface_references[i]+=1; result=surface_references[i]
            elif address==sr: events.append('SRF'+str(i)); surface_references[i]-=1; result=surface_references[i]
            elif address==sd:
                events.append('SD'+str(i)); uc.mem_write(word(esp+8),native_descriptions[i]); result=0
            elif address==sl:
                out,rect,flags=word(esp+8),word(esp+12),word(esp+16)
                events.append(f'SL{i}:'+':'.join(map(str,[signed(rect+j*4) for j in range(4)]+[flags])))
                w=struct.unpack_from('<I',native_descriptions[i],24)[0]; put(out,w*4+16); put(out+4,native_pixels+i*4112+8); result=0
            elif address==su: events.append('SU'+str(i)); result=0
            elif address==tc: events.append('TC'+str(i)); result=native_levels[i]
            elif address==td:
                level,out=word(esp+8),word(esp+12); events.append(f'TD{i}:{level}')
                uc.mem_write(out,native_descriptions[i]); put(out+24,max(1,word(out+24)>>level)); put(out+28,max(1,word(out+28)>>level)); result=0
            elif address==ts:
                level,out=word(esp+8),word(esp+12); events.append(f'TS{i}:{level}'); surface_references[i]+=1
                put(out,native_surfaces+i*4); result=0
            else:
                w,h,levels,usage,fmt,memory_pool,out,shared=[word(esp+j*4) for j in range(2,10)]
                i=(out-bank_storage-0x2B4)//8
                events.append('DC'+':'.join(map(str,[i,w,h,levels,usage,fmt,memory_pool,int(shared!=0)])))
                native_descriptions[i]=struct.pack('<8I',fmt,0,0,0,0,0,w,h); native_levels[i]=levels; surface_references[i]=1
                put(out,native_textures+i*4); result=0
            uc.reg_write(UC_X86_REG_EAX,result)
        elif address==0x9D56C0: events.append(f'GOPEN{int(receiver==bank_storage)}:{cstring(word(word(word(esp+4))))}:{word(esp+8)}')
        elif address==open_missing_path:
            events.append('OPEN_MISSING:'+str(word(esp+8))); uc.mem_write(receiver+0x8C,b'\0'); uc.reg_write(UC_X86_REG_EAX,0)
        elif address==bank_destroy: events.append('GDELETE'+str(word(esp+4)))
        elif address==0xBFE9BC:
            p=word(esp+4)
            events.append('SF'+str((p-string_record)//17) if string_record<=p<string_record+68 else f'FREEBANK:{int(p==bank_info)}')
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=[(mode,enabled,seed,*dims,pattern,bank) for mode,enabled,seed,dims,pattern,bank in itertools.product(range(3),(0,1,255),(-1,100),((640,480),(1920,1080),(2147483647,-2147483648)),(0,0xA5,255),range(2))]
    directory=ROOT/'work/ui_manager_construction_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    sources=['rebuild/tests/integration/UiManagerConstruction_test.cpp']
    sources+=BANK_OWNERSHIP_SOURCES
    sources+=STRING_SOURCES
    sources+=DISPLAY_SOURCES
    sources+=FACTORY_SOURCES
    sources+=BANK_RUNTIME_SOURCES
    sources+=TEXTURE_MANAGER_SOURCES
    sources+=BANK_FILE_SOURCES
    sources+=BANK_OPEN_SOURCES
    sources+=BANK_REGISTRY_SOURCES
    sources+=BANK_STREAM_SOURCES
    sources+=BANK_PATH_SOURCES
    sources+=WIDE_STRING_SOURCES
    sources+=TEXTURE_SURFACE_SOURCES
    sources+=['rebuild/src/compiled/00/41/'+s+'.cpp' for s in ('CFrontEndManager_GetInstance_0041e5f2','CManager_CManager_0041e3f6','CManager_SetInput_0041df10','CManager_SetMetaLayer_0041e1cd','CGraphicDataBankInit_CGraphicDataBankInit_00415b80')]
    sources+=['rebuild/src/compiled/00/42/'+s+'.cpp' for s in ('CObservable_CObservable_0042be7b','global_ConstructObserverList_0042ac0a','global_ConstructInputMap_0042bf67','global_ConstructLayerMap_0042bf85','global_ConstructComponentMap_0042d28b','global_LookupInputKey_0042d201','global_LookupLayer_0042d246','global_RotateEventTreeLeft_0042951b','global_RotateEventTreeRight_0042955b','global_BalanceEventTree_0042971d','global_SetRelativeCoordinates_004299a8')]
    env=parity.env(); objects=[]
    for i,s in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    table_file=directory/'formats.txt'; table_file.write_text('\n'.join(' '.join(map(str,r)) for r in retail_table(image))+'\n')
    lines=run([exe,inputs,table_file],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete construction traces')
    def normalize(value):
        if string_record<=value<string_record+68: return 0x64000000+value-string_record
        if value==0x1230134: return 0x70000001
        if value==bank_info: return 0x1A005678
        if value==bank_storage: return 0x1A001234
        symbols={0x129A7C4:0x70000100,0x129C814:0x70000101,0x129C938:0x70000102,0x129C808:0x70000103,0x12354A4:0x70000104,0x129C888:0x70000105,0x129C8CC:0x70000106,0x129DC5C:0x70000109,0x129DC50:0x7000010A}
        if value in symbols: return symbols[value]
        if native_textures<=value<native_textures+44: return 0x65000000+value-native_textures
        if value==texture_info: return 0x63000000
        if value==texture_manager: return 0x62000000
        if texture_manager<value<texture_manager+0x5D4: return 0x62000000+value-texture_manager
        if bank_storage<value<bank_storage+0x30C: return 0x61000000+value-bank_storage
        for i,size in enumerate(sizes):
            start=pool+i*64
            if start<=value<start+size: return 0x60000000+i*256+value-start
        return value
    def dump(p,size): return ''.join(':'+f'{normalize(word(p+i)):08x}' for i in range(0,size,4))
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        mode,enabled,seed,width,height,pattern,bank=case; events.clear(); sizes.clear(); string_count=buffer_count=reference_allocations=0
        uc.mem_write(manager,bytes([pattern])*0xD0); uc.mem_write(pool,b'\xCD'*8192); put(0x13B8710,0)
        uc.mem_write(bank_info,b'\0'*12); uc.mem_write(bank_node,b'\xA5'*64); put(bank_head,bank_head); put(bank_head+4,bank_head); put(bank_manager+4,bank_head)
        uc.mem_write(texture_info,b'\0'*12); uc.mem_write(bank_storage,b'\xA5'*0x30C); uc.mem_write(texture_manager,b'\xA5'*0x5D4); put(texture_vtable,texture_delete); put(0x129DC5C,texture_delete)
        put(bank_vtable,bank_destroy); put(bank_vtable+4,0x9D56C0); put(0x13CAA38,0); uc.mem_write(0x13CA7B0,bytes([int(mode==2)]))
        put(0x129C8CC,bank_destroy); put(0x129C8CC+4,0x9D56C0); put(0x129C8CC+12,open_missing_path)
        uc.mem_write(string_record,b'\xA5'*68); uc.mem_write(string_buffer,b'\xCD'*256); put(0x13BD800,100); put(0x13BCA20,50)
        uc.mem_write(native_pixels,b'\xA7'*(11*4112)); surface_references[:]=[0]*11; native_descriptions.clear(); native_levels.clear()
        uc.mem_write(0x13B8768,bytes([enabled])); uc.mem_write(0x13B876C,struct.pack('<ff',1600,900)); put(0x13B8390,display)
        for d in range(2):
            p=display+d*0x200; put(p+0x54,devices+d*4); put(p+0x58,native_device); put(devices+d*4,device_vtable); put(device_vtable+40,capability)
            put(p+0x60,7+d); put(p+0x5C,2+d); put(p+0x1C4,21+d); put(p+0x194,width); put(p+0x198,height)
        put(system+0x60,display); put(system+0x6C,bank_manager); put(0x13B871C,mode_context if mode else 0); uc.mem_write(mode_context+9,bytes([int(mode==2)]))
        for _ in range(2):
            put(stack,stop); uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_FPCW,0x37F); uc.emu_start(0x41E5F2,stop,count=200000)
            if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4 or uc.reg_read(UC_X86_REG_EAX)!=manager or word(0x13B8710)!=manager: raise RuntimeError('Singleton construction failed')
        events.append(f'MODE{uc.mem_read(0x13B8768,1)[0]}:{word(0x13B876C):08x}:{word(0x13B8770):08x}')
        events.append('M'+dump(manager,0xD0))
        for i,size in enumerate(sizes): events.append(f'H{i}'+dump(pool+i*64,size))
        events.append(f'BANK:{word(bank_info)}')
        state=f'STR:{signed(0x13BD800)}'
        for s in range(string_count):
            r=string_record+s*17; state+=f':{int(word(r)==0)}:{word(r+4)}:{word(r+8):08x}:{uc.mem_read(r+12,1)[0]}:{signed(r+13)}'
        events.append(state)
        events.append('CACHE:'+':'.join(str(int(x)) for x in (word(bank_head)==bank_node,word(bank_head+4)==bank_node,word(bank_node)==bank_head,word(bank_node+4)==bank_head,word(bank_node+52)==string_record,word(bank_node+60)==bank_info)))
        events.append('GDATA'+dump(bank_storage,0x30C)); events.append('TREF:'+str(signed(texture_info)))
        events.append('TDATA'+dump(texture_manager,0x5D4))
        events.append('WIDE:'+str(signed(0x13BCA20)))
        h=2166136261
        for v in bytes(uc.mem_read(native_pixels,11*4112)): h=((h^v)*16777619)&0xFFFFFFFF
        events.append(f'GPU:{h:08x}'+''.join(':'+str(r) for r in surface_references))
        expected='TRACE' +''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,scope='real singleton/CManager/observable/containers/config/maps/relative setup/graphic init, bank factory/ctor/init/resources/ownership, texture-manager/pools, strings and display/global services; bank-file/async/cache construction, texture/surface operations and actual base/async OpenReadOnly linked; real empty registered-file lookup, registered path lookup and wide lifetime; path-backed file open unavailable, D3D COM, Win32 critical section and allocators controlled; all manager/bank/texture-manager bytes, cache/reference/narrow-wide string counts, GPU memory hash and service traces'),indent=2)+'\n')
    print(f"UI_MANAGER_CONSTRUCTION {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
