import sys,tempfile,subprocess,json,hashlib,struct,collections
import capstone
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
w=Path(tempfile.mkdtemp(prefix='native-frontend-draw-'));objects=[];env=check_one.env()
sources=[ROOT/'rebuild/src/compiled/00/42/CNewFrontendGameComponent_Draw_0042df9e.cpp',ROOT/'rebuild/tests/integration/NativeFrontendDraw_test.cpp']
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w)
    assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);sec,offset=c.symbols['?Draw@CNewFrontendGameComponent@@QAEXABVCEngineInterpolationInfo@@@Z'];body,rels=c.sections[sec];assert offset==0
with pefile.PE(str(EXE))as pe:retail=pe.get_data(0x42df9e-0x400000,478)
score=compare(body,retail,tuple(rels),retail_relocations('0042df9e',478));assert score.near_match,score
# Similarity masks relocations; independently require the original call order
# and data identities. _controlfp replaces the original inline x87 control-word
# operation and is the sole additional call, exercised by the behavioral test.
targets={
 '??0CInterpolationInfo@@QAE@XZ':0x415a60,
 '?GFGetTime@@YINXZ':0x9e1bc0,
 '?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z':0xa0b560,
 '?GetVVisibleTelevisionScreenArea@CDisplayManager@@QBE?AVC2DBoxF@@XZ':0x9bece0,
 '?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z':0x9d8cf0,
 '?BeginRender@CDisplayManager@@QAE_NXZ':0x9bef20,
 '?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ':0x595582,
 '?Draw@CFrontEndManager@NUISystem@@QAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@@Z':0x595222,
 '?GetInstance@CManager@NUISystem@@SIPAV12@XZ':0x41e5f2,
 '?RenderErrorMessage@CManager@NUISystem@@QAEXAAVCIEngine@@@Z':0x41d03c,
 '?DrawBufferedPolys@CRenderManager2D@@QAEXXZ':0x9d9c80,
 '?Render2DDrawList@CRenderManager2D@@QAEX_N@Z':0x9da9f0,
 '?Get@CIME@NUISystem@@SIAAV12@XZ':0x404a80,
 '?Render@CIME@NUISystem@@QAEXXZ':0x404c00,
 '?EndRender@CDisplayManager@@QAEXXZ':0x9bef50,
 '?SwapScreens@CDisplayManager@@QAE_NXZ':0x9beeb0,
 '?g_FableVirtualWidth_013961E8@@3NA':0x13961e8,
 '?g_FableVirtualHeight_013961F0@@3NA':0x13961f0,
 '?g_FableRenderManager2D_013B8384@@3PAVCRenderManager2D@@A':0x13b8384,
 '?g_FableDisplayManager_013B8390@@3PAVCDisplayManager@@A':0x13b8390,
 '?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA':0x1396e64,
}
native_calls=[];native_data=[];rounding_calls=0
for p,(symbol,kind) in sorted(rels.items()):
    addend=struct.unpack_from('<I',body,p)[0]
    if symbol=='__controlfp':
        assert kind==20 and addend==0
        rounding_calls+=1
        continue
    assert symbol in targets and kind in (6,20),(symbol,kind)
    (native_calls if kind==20 else native_data).append(targets[symbol]+addend)
decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
retail_calls=[];retail_data=[]
for instruction in decoder.disasm(retail,0x42df9e):
    if instruction.mnemonic=='call' and instruction.operands[0].type==capstone.x86.X86_OP_IMM:
        retail_calls.append(instruction.operands[0].imm)
    for operand in instruction.operands:
        if operand.type==capstone.x86.X86_OP_MEM and not operand.mem.base and not operand.mem.index:
            retail_data.append(operand.mem.disp)
assert native_calls==retail_calls,(native_calls,retail_calls)
assert collections.Counter(native_data)==collections.Counter(retail_data),(native_data,retail_data)
assert rounding_calls==1
exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'NATIVE_FRONTEND_DRAW_PASS cases=512'in r.stdout,(r.returncode,r.stdout,r.stderr,w)
report=dict(address='0042df9e',built_bytes=len(body),retail_bytes=478,grade='NEAR_MATCH',**score.to_dict(),cases=512,omission_checks=1,relocations=[dict(offset=p,symbol=s,kind=k)for p,(s,k)in rels.items()],sources=[dict(source=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest())for p in sources],boundary='Native C++ frame coordinator, actual x87 rounding via CRT _controlfp. Renderer/UI/engine/IME presentation calls recorded; full render-state images and callback replacements checked. No displayed frame.')
(ROOT/'rebuild/compile-gate/native-frontend-draw.json').write_text(json.dumps(report,indent=2)+'\n');print(score);print(r.stdout.strip())
