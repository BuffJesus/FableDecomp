import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
w=Path(tempfile.mkdtemp(prefix='native-sprite-supported-'));env=V.env()
grow='?_M_insert_overflow@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXPAJABJABU__true_type@2@I_N@Z'
specs=[('00/bb/CEnginePrimitiveManagerSprites_GetSupportedTypes_00bb1600.cpp','?GetSupportedTypes@CEnginePrimitiveManagerSprites@@UAEXAAVCEnginePrimitiveTypeList@@@Z',0xbb1600,62,1,0),
       ('00/bb/CEnginePrimitiveManager2DSprites_GetSupportedTypes_00bb1640.cpp','?GetSupportedTypes@CEnginePrimitiveManager2DSprites@@UAEXAAVCEnginePrimitiveTypeList@@@Z',0xbb1640,122,2,64),
       ('00/44/LongVector_InsertOverflow_00440990.cpp',grow,0x440990,216,4,256)]
targets={grow:0x440990,'_malloc':0xbfea0e,'_free':0xbfea14,'_memmove':0xbfeae6}
(w/'sprite_supported_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in [('@SpriteTypes@12',specs[0][1]),('@TwoDTypes@12',specs[1][1])])+'\n')
raw=EXE.read_bytes();body=bytearray(472);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for _,symbol,address,size,n,offset in specs:
    span=read_virtual_data(raw,address,size);body[offset:offset+size]=span
    for ins in md.disasm(span,address):
        if ins.mnemonic=='call':
            target=ins.operands[0].imm;assert target in targets.values();fixups.append((offset+ins.address-address+1,target))
assert len(fixups)==7
(w/'sprite_supported_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%x for x in fixups)+'};\n')
objects=[];proofs=[]
fixture=ROOT/'rebuild/tests/integration/NativeSpriteSupportedTypes_test.cpp'
for i,path in enumerate([ROOT/'rebuild/src/compiled'/s[0] for s in specs]+[fixture]):
    obj=w/f'{i}.obj';flags=['/Dmalloc=Allocate','/Dfree=Free'] if i==2 else []
    r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj)]+flags+[str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
    if i<2:proofs.append(check(obj,*specs[i][1:5],targets))
    if i==2:
        plain=w/'plain.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(plain),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr
        proofs.append(check(plain,*specs[i][1:5],targets))
        a=Coff(plain);b=Coff(obj);sa,oa=a.symbols[grow];sb,ob=b.symbols[grow];ba,ra=a.sections[sa];bb,rb=b.sections[sb]
        assert oa==ob and ba==bb and {p:({'_malloc':'_Allocate','_free':'_Free'}.get(t,t),k) for p,(t,k) in ra.items()}==rb
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
    r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=45);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=576' in r.stdout,r.stdout
report=dict(cases=576,snapshots=int(r.stdout.split('snapshots=')[1].strip()),providers=proofs,omission_checks=3,source_sha256={str((ROOT/'rebuild/src/compiled'/s[0]).relative_to(ROOT)):hashlib.sha256((ROOT/'rebuild/src/compiled'/s[0]).read_bytes()).hexdigest() for s in specs},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual sprite type append routines plus actual signed-long vector growth. Complete4KiB native/compiled callback and final snapshots,empty/nonempty lists,spare/full capacity,two locations,allocation mutations. Independent prefix and appended-type expectations. Only malloc/free boundaries;real memmove.')
(ROOT/'rebuild/compile-gate/native-sprite-supported-types.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_SPRITE_SUPPORTED_TYPES_GATE_PASS')
