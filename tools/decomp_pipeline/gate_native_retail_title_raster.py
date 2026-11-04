"""Real retail title through recovered renderer; GPU/retail-file dependent, no game boot claim."""
import hashlib,json,re,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
sys.path.insert(0,str(ROOT/'tools'))
import check_one as V
import texture_build as tb
from PIL import Image

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

work=ROOT/'work/codex-native-startup/native-retail-title-raster-20261010'
work.mkdir(parents=True,exist_ok=True)
canonical_gate=ROOT/'tools/decomp_pipeline/gate_native_indexed_raster.py'
# Mandatory fresh compilation/parity/omission/GPU checks. No scratch artifacts used.
fresh=subprocess.run([sys.executable,str(canonical_gate)],cwd=ROOT,capture_output=True,text=True,timeout=120)
print(fresh.stdout,flush=True)
assert not fresh.returncode,(fresh.returncode,fresh.stdout,fresh.stderr)
assert 'NATIVE_INDEXED_RASTER_PASS' in fresh.stdout
canonical=ROOT/'work/codex-native-startup/native-indexed-raster-20261009'
canonical_report=ROOT/'rebuild/compile-gate/native-indexed-raster.json'
canonical_evidence=json.loads(canonical_report.read_text())
assert canonical_evidence['omission_checks']==17 and canonical_evidence['pixels']==114688
env=V.env()
fixture=ROOT/'rebuild/tests/integration/NativeRetailTitleRaster_test.cpp'
decoder=ROOT/'rebuild/integration/fable_texture_decode.c'
objects=[]
for index,source in enumerate((fixture,decoder)):
    obj=work/f'{index}.obj'
    r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
    assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
providers=[canonical/f'{i}.obj' for i in range(17)]
exe=work/'fixture.exe'
user32=Path('C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x86/user32.lib')
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib',str(user32)]
omissions=[]

def require_missing_symbol(label,expected_symbol,linked):
    r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True)
    diagnostic=r.stdout+r.stderr
    missing_lines=[line for line in diagnostic.splitlines() if re.search(r'\bLNK(?:2001|2019)\b',line)]
    assert r.returncode and any(expected_symbol in line for line in missing_lines),(label,expected_symbol,r.returncode,diagnostic)
    omissions.append(dict(omitted=label,expected_missing_symbol=expected_symbol,returncode=r.returncode,diagnostic=diagnostic.strip()))

required_symbols=[
    '?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z',
    '?RealiseRenderState@CRenderStateManager@@QAEXXZ',
    '?BeginRender@CDisplayManager@@QAE_NXZ',
    '?BeginRender@CRenderManagerCore@@QAE_NXZ',
    '?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ',
    '?SetVertexFormat@CRenderManagerCore@@QAEXK@Z',
    '?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z',
    '?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z',
    '?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXKK@Z',
    '?DrawCurrentBuffersAsIndexedTris@CRenderManagerCore@@QAEXKKKK@Z',
    '?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKK@Z',
    '?DrawCurrentBuffersAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKKKK@Z',
    '?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z',
    '?GetLevelByteLength@CTexture@@QBEJJ@Z',
    '?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',
    '?GetColourDepth@CPixelFormat@@QBEKXZ',
    '?GD3DFormats@@3QBVCD3DFormatInfo@@B']
for omit in range(17):
    require_missing_symbol(str(providers[omit].relative_to(ROOT)),required_symbols[omit],objects+[p for i,p in enumerate(providers) if i!=omit])
require_missing_symbol(str(decoder.relative_to(ROOT)),'_FableDecodeBankTexture',[objects[0]]+providers)
r=subprocess.run(cmd+list(map(str,objects+providers)),env=env,capture_output=True,text=True)
assert not r.returncode,r.stdout+r.stderr
bank=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/data/graphics/pc/frontend.big')
assert bank.is_file(),bank
frame=work/'retail-title.ppm'
controls=[work/f'control-{i}.ppm' for i in range(7)]
r=subprocess.run([str(exe),str(bank),str(frame)]+list(map(str,controls)),env=env,capture_output=True,text=True,timeout=45)
print(r.stdout,flush=True)
assert not r.returncode,(r.returncode,r.stdout,r.stderr)
assert 'NATIVE_REAL_ASSET_PASS' in r.stdout
# Python is independent of the C integration decoder and GPU sampling/rendering.
buf,parsed=tb.load_big(str(bank))
sub,entry=tb.find_entry(parsed,'FRONTEND_TITLE_01_SPRITE')[0]
info,mips=tb.decode_entry(entry['info'],buf[entry['offset']:entry['offset']+entry['size']],max_mips=1)
assert entry['id']==3 and info['frames']==1
expected=Image.new('RGB',(288,160),(16,32,48))
expected.paste(Image.fromarray(mips[0]).convert('RGB'),(16,16))
actual=Image.open(frame).convert('RGB')
assert actual.size==expected.size and actual.tobytes()==expected.tobytes(),'independent retail/Python/GPU fullimage mismatch'
png=frame.with_suffix('.png');actual.save(png)
assert all(p.read_bytes()==controls[2].read_bytes() for p in controls[3:]),'canonical textured controls disagree'
hash_sources=(Path(__file__),fixture,decoder,ROOT/'rebuild/integration/fable_texture_decode.h',ROOT/'rebuild/integration/fable_bank_reader.h',ROOT/'rebuild/tests/integration/NativeIndexedRaster_test.cpp',canonical_gate,ROOT/'tools/texture_build.py',ROOT/'tools/parse_bigb.py',ROOT/'tools/parse_texture.py',ROOT/'tools/lionhead_lz.py')
report=dict(status='PASS',entry={k:v for k,v in entry.items() if k not in ('info','deps')},bank=str(bank),bank_sha256=sha(bank),info=info,frame=str(frame),png=str(png),frame_sha256=sha(frame),png_sha256=sha(png),omission_checks=18,production_omission_checks=17,decoder_omission_checks=1,omissions=omissions,canonical_report=str(canonical_report),canonical_report_sha256=sha(canonical_report),canonical_output=fresh.stdout.strip(),provider_objects_sha256={str(p.relative_to(ROOT)):sha(p) for p in providers},fixture_objects_sha256={str(p.relative_to(ROOT)):sha(p) for p in objects},source_sha256={str(p.relative_to(ROOT)):sha(p) for p in hash_sources},pixels=46080,asset_pixels=32768,background_pixels=13312,output=r.stdout.strip(),boundary='Real retail id3 runtime bank reader+bounded C decoder, independently compared against Python decoder and every GPU RGB output pixel. Recovered display/core lifecycle,FVF,texture attachment,state realization,indexed textured drawing,dimension and byte-length helpers. Canonical seven controls execute all17 providers and mandatory fresh canonical gate rechecks parity/omissions/GPU. Container/decompression/color conversion are existing integration ports NOT retail-parity reconstruction. OS/window/device/geometry/upload/readback/clock boundaries. No native LoadGraphicFrame9FED30,original frontend boot,presentation loop or playable game claim. No asset bytes committed; no additional function/data coverage.')
(ROOT/'rebuild/compile-gate/native-retail-title-raster.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_RETAIL_TITLE_RASTER_GATE_PASS',frame,flush=True)
