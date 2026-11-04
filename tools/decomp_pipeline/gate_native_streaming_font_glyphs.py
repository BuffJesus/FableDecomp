"""Streaming font glyph gate: actual CStreamingFontBank::FindGlyph / GlyphLookup / GetStringExtents (streaming
font module unit) against transplanted retail 0x009A4160 / 0x009A1140 / 0x009A14E0 from identical states over
generated glyph data (two fonts, raw and LZO banks, LZO glyph-data chunks, adjustments); retail
lzo1x_decompress_safe 0x00C08170 is a both-modes library boundary. Results and the full glyph-cache state
(lookup table, chunk cache and index, bank caches, cache-buffer entries) must be equal for every operation.
Mutated module copies must be rejected. Reports near_match grades of the three roots."""
from pathlib import Path
from datetime import datetime,timezone
import sys,re,struct,subprocess,json,hashlib,argparse
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
import native_exact_roots as N
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import native_streaming_glyph_fixture as G
ap=argparse.ArgumentParser();ap.add_argument('--output',type=Path);a=ap.parse_args()
b=a.output.resolve() if a.output else R/'rebuild/build/native-streaming-font-glyphs'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
MODULE=R/'rebuild/src/compiled/00/9a/CStreamingFontBank_Module_009a0ec0.cpp'
FIXTURE=R/'rebuild/tests/integration/NativeStreamingFontGlyphs_test.cpp'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
inputs=[MODULE,FIXTURE,Path(__file__),Path(G.__file__),EXE,R/'rebuild/include/fable_streaming_font_cache.h',R/'rebuild/include/fable_font_instances.h']
pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
env=V.env()
# Real providers linked beside the fixture unit: the engine pixel-format table and CPixelFormat::Initialise.
PROVIDERS=[R/'rebuild/src/compiled'/p for p in ('01/29/CPixelFormat_Table_0129ba40.cpp','00/9e/CPixelFormat_Initialise_009e3830.cpp',
 # Construction: base class, name string, memory input stream, and the remaining real CStreamingFontBank / CFontBank virtuals.
 '00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/CCharString_DefaultConstructor_0099e4b0.cpp','00/99/CCharString_Destructor_0099eae0.cpp',
 '00/99/CCharString_CopyAssignment_0099efb0.cpp','00/99/CDataInputStream_CDataInputStream_00993b20.cpp','00/99/CMemoryDataInputStream_DestructorTyped_00993b10.cpp',
 '00/99/CMemoryDataInputStream_ReadNullTerminatedString_00995d30.cpp','00/99/DataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp','00/99/CInitBaseClass_IsValid_0099a330.cpp',
 '00/9a/CStreamingFontBank_GetFontMaxHeight_009a0ea0.cpp','01/39/CRenderStateManager_StaticData_01396da8.cpp','01/39/CRenderStateManager_D3DAlphaFuncModeTable_01396dd8.cpp','01/39/CRenderStateManager_D3DTextureOperationModeTable_01396e70.cpp','00/9a/CFontBank_Preload_009a0b20.cpp',
 '00/99/CCharString_UnassignString_0099e9b0.cpp','00/99/CCharString_CCharString_0099ec30.cpp','00/99/CCharString_Constructor_0099ebf0.cpp',
 '00/99/CCharString_AllocStringData_0099ea60.cpp',
 '00/99/CWideString_LiteralConstructor_0099b6b0.cpp','00/99/CWideString_Destructor_0099b510.cpp','00/99/CWideString_UnassignString_0099b4d0.cpp',
 '00/99/CWideString_AllocStringData_0099b3c0.cpp','00/99/CWideString_Constructor_0099aed0.cpp','00/99/CWideString_CopyConstructor_0099b720.cpp',
 '00/99/CCharString_operatorcharconstptr2_0099af10.cpp','00/99/STL_Mthrowlengtherror_0099c550.cpp','00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp','00/9a/CCharString_InitFromCStr_009a0590.cpp')]
inputs+=PROVIDERS;pins.update({str(p):sha(p) for p in PROVIDERS});(b/'inputs.json').write_text(json.dumps(pins,indent=2))
provider_objects=[]
for k,p in enumerate(PROVIDERS):
    o=b/('provider%d.obj'%k);x=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(o),str(p)],env,b);assert not x.returncode,(p.name,x.stdout[-2000:]);provider_objects.append(o)
def build(module,name):
    unit=b/(name+'.cpp');unit.write_text('#include "%s"\n#include "%s"\n'%(str(module).replace('\\','/'),str(FIXTURE).replace('\\','/')))
    obj=unit.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(unit)],env,b);assert not x.returncode,(name,x.stdout[-3000:])
    exe=unit.with_suffix('.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),str(obj),*map(str,provider_objects),'kernel32.lib'],cwd=b,env=env,capture_output=True,text=True);assert not x.returncode,(name,x.stdout[-3000:])
    return exe
def read_map(path):
    mapped={}
    for line in path.read_text().splitlines():
        m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
        if m:mapped.setdefault(m[1],int(m[2],16))
    return mapped
raw=EXE.read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
LZO_AT,FIND_AT,LOOKUP_AT,EXTENTS_AT,INIT_AT,CTOR_AT,DTOR_AT,ENTRIES_AT,PRELOAD_AT,INSERT_AT,UPDATE_AT,TEXTURE_UPDATE_AT,READ_TEXT_AT,PREPARE_AT,RENDER_AT,BLOCK=0,1024,1280,2304,4096,6144,7456,7648,8192,9184,9296,9472,10240,11264,13568,17408
FUNCS={0xc08170:(LZO_AT,0x3b3),0x9a4160:(FIND_AT,142),0x9a1140:(LOOKUP_AT,918),0x9a14e0:(EXTENTS_AT,380),0x9a3640:(INIT_AT,1490),
 0x9a3c20:(CTOR_AT,1298),0x9a1090:(DTOR_AT,170),0x9a0f00:(ENTRIES_AT,137),0x9a2000:(PRELOAD_AT,982),0x9a1020:(INSERT_AT,105),0x9a0f90:(UPDATE_AT,143),0x9a23e0:(TEXTURE_UPDATE_AT,495),0x9a3460:(READ_TEXT_AT,424),0x9a1720:(PREPARE_AT,2260),0x9a25d0:(RENDER_AT,3722)}
CALLS={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0xbfea70:'__ftol2',
 0x9f2870:'?Create@CMemoryAllocator@@KIXXZ',0x9f2000:'?EnterSynchronisation@CMemoryAllocator@@KIXXZ',0x9f2010:'?LeaveSynchronisation@CMemoryAllocator@@KIXXZ',
 0x9f2a40:'?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z',0x9e3830:'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',
 0x485f3a:'?_M_fill_insert@?$vector@EV?$allocator@E@_STL@@@_STL@@QAEXPAEIABE@Z',
 0x9fa280:'?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z',
 0x9be6c0:'?GetBestAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z',
 0xa5f010:'?StreamGlyph@CStreamingFontDataBank@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@PAXKK_N@Z',
 0xa5ef30:'?ReadData@CStreamingFontDataBank@@QAEXPAXKK@Z',0xbfeb22:'??_U@YAPAXI@Z',0xbfe9bc:'??3@YAXPAX@Z',
 0x9cc240:'?GetEntryDataSize@CBankFile@@QBEKK@Z',0x9cc2a0:'?ReadEntryData@CBankFile@@QAEXKKPAXK@Z',0xbfea0e:'_malloc',0xbfea14:'_free',0xbfea30:'__chkstk',
 0x994930:'??0CMemoryDataInputStream@@QAE@PBXK@Z',0x993b10:'??1CMemoryDataInputStream@@UAE@XZ',0xbfea1a:'??2@YAPAXI@Z',
 0x995d30:'?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ',0x99efb0:'??4CCharString@@QAEAAV0@ABV0@@Z',0x99eae0:'??1CCharString@@QAE@XZ',
 0x993ca0:'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z',0x99a2f0:'??0CBaseClassNonCopyable@@QAE@XZ',0x99e4b0:'??0CCharString@@QAE@XZ',
 0xbfeb1c:'??_V@YAXPAX@Z',0x9f9f70:'?Uninitialise@CTexture@@QAEXXZ',0x99a300:'??1CBaseClassNonCopyable@@UAE@XZ',
 0x9d54e0:'?IsFinished@CBankFileAsyncData@@QBE_NXZ',0x99af10:'?GetAsCharArray@CWideString@@QBEPBGXZ',
 0x9a4440:'??4?$CCountedPointer@VCBankFileAsyncData@@@@QAEAAV0@ABV0@@Z',0x9a4400:'??1?$CCountedPointer@VCBankFileAsyncData@@@@QAE@XZ',
 0x9fa450:'?Lock@CTexture@@QAE?AVCSurfaceLockedRect@@KK@Z',0x9f9de0:'?Unlock@CTexture@@QAEXK@Z',
 0x99aed0:'??0CWideString@@QAE@XZ',0x99a6a0:'?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z',
 0x4110d0:'?_M_fill_insert@?$vector@DV?$allocator@D@_STL@@@_STL@@QAEXPADIABD@Z',0x99a960:'?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z',
 0x99a920:'?Close@CDiskFileWin32@@UAEXXZ',0x517900:'?resize@?$vector@GV?$allocator@G@_STL@@@_STL@@QAEXIABG@Z',0x99b510:'??1CWideString@@QAE@XZ',
 0xa0aa30:'?SetVertexFormat@CRenderManagerCore@@QAEXK@Z',0xa0af40:'?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z',
 0x9bedc0:'?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ',0xbfeb8a:'_floor',0x99af00:'?GetAsCharArray@CWideString@@QBEPBGXZ'}
# Module file statics (fixture DataAddress index, patch kind 3) and fixture constants (map, kind 0).
STATIC=[(0x13bda70,0x8000),(0x13c5a98,0x202c),(0x13c7ac8,0x82c),(0x138e068,4),(0x138e06c,4),(0x13bda34,4),(0x13bda38,4),(0x13bda3c,4),(0x13bda40,4),
 (0x138e070,4),(0x13bda24,4),(0x13bda28,4),(0x13bda2c,4),(0x13bda30,4),(0x13bda44,4),(0x13bda48,4),(0x13bda4c,4),(0x13c5a80,8),(0x13c5a88,12),(0x13bda50,0x20),
 (0x13bd808,0x200),(0x13c5a70,1)]
CONSTANTS=[(0x1230a5c,'_GlyphHundredth',0.01),(0x122dedc,'_GlyphZero',0.0),(0x122f59c,'_GlyphHalf',0.5),(0x122dcb4,'_GlyphUnsignedBias',4294967296.0),(0x122ded8,'_GlyphOne',1.0),(0x129ab28,'_GlyphTexelU',1/1024),(0x129ab2c,'_GlyphTexelV',1/512)]
for address,name,value in CONSTANTS:assert read_virtual_data(raw,address,4)==struct.pack('<f',value),hex(address)
assert struct.unpack('<2I',read_virtual_data(raw,0x138e068,8))==(0x40,0xffffffff)
assert 'numTextureCacheGlyphDatasPerChunk = 0x40;' in MODULE.read_text() and 'glyphTextureCacheGlyphDataCacheChunk = 0xFFFFFFFF;' in MODULE.read_text()
data=[(a_,n,('static',k)) for k,(a_,n) in enumerate(STATIC)]+[(a_,4,('map',name)) for a_,name,v in CONSTANTS]+[
 (0x13caa90,4,('map','?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A')), (0x129ab38,28,('map','??_7CFontBank@@6B@')),(0x122d06c,64,('map','??_7CDiskFileWin32@@6B@')),
 (0x1396dd8,32,('map','?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA')),(0x1396e70,0x40,('map','?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA')),
 (0x1396e64,12,('map','?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA')),(0x1396f78,4,('map','?FableRenderDefaultValue_01396f78@@3KA')),(0x1396f7c,4,('map','?FableRenderDefaultValue_01396f7c@@3KA')),(0x1396fa4,4,('map','?FableRenderDefaultValue_01396fa4@@3KA')),(0x1396fb0,4,('map','?FableRenderDefaultValue_01396fb0@@3KA')),(0x1396e5c,4,('map','?FableRenderDefaultValue_01396e5c@@3KA')),(0x1396fcc,4,('map','?FableRenderDefaultValue_01396fcc@@3KA')),(0x1396eec,4,('map','?FableRenderDefaultValue_01396eec@@3KA')),(0x1396ef0,4,('map','?FableRenderDefaultValue_01396ef0@@3KA')),(0x13c82f8,0x2000,('map','?g_PendingLoadedGlyphs@NStreamingFontBankGlobals@@3PAUPendingLoadedGlyph@@A')),
 (0x13c5a74,4,('map','?g_HeadPendingLoadedGlyphs@NStreamingFontBankGlobals@@3KA')),(0x13c5a78,4,('map','?g_TailPendingLoadedGlyphs@NStreamingFontBankGlobals@@3KA')),(0x129ab54,28,('map','??_7CStreamingFontBank@@6B@')),(0x129ba40,47*36,('map','?GD3DFormats@@3QBVCD3DFormatInfo@@B')),
 (0x13bc470,0x400,('map','_FixtureShaderManager')),(0x138e0b5,1,('map','_RenderBuildIndicesOriginal')),(0x13ca308,768,('map','_RenderIndicesOriginal')),
 (0x13ca2f8,8,('map','?AlphaMaskUVMul@CStreamingFontBank@@2VC2DVector@@A')),(0x13ca300,8,('map','?AlphaMaskUVAdd@CStreamingFontBank@@2VC2DVector@@A')),
 (0x1396ee8,4,('map','?FableRenderDefaultValue_01396ee8@@3KA'))]
def absolute(i):
    out=[]
    for o in i.operands:
        if o.type==2 and i.mnemonic not in ('test','and','or','xor') and 0x400000<=(o.imm&0xffffffff)<0x1600000:out.append(o.imm&0xffffffff)
        if o.type==3 and 0x400000<=(o.mem.disp&0xffffffff)<0x1600000:out.append(o.mem.disp&0xffffffff)
    return out
block=bytearray(BLOCK);specs=[]
for address,(at,size) in FUNCS.items():
    code=read_virtual_data(raw,address,size);block[at:at+size]=code;assert read_virtual_data(raw,address+size,1)==b'\xcc',hex(address)
    for i in md.disasm(code,address):
        site=at+i.address-address
        if i.mnemonic=='call' and i.operands[0].type==2:
            t=i.operands[0].imm;specs.append((site+i.imm_offset,FUNCS[t][0],2) if t in FUNCS else (site+i.imm_offset,('map',CALLS[t]),1));continue
        if i.mnemonic.startswith('j'):
            t=i.operands[0].imm if i.operands[0].type==2 else None
            if t is not None and not address<=t<address+size:
                assert i.mnemonic=='jmp' and i.bytes[0]==0xe9 and t in CALLS,hex(i.address);specs.append((site+1,('map',CALLS[t]),1))
            continue
        for value in absolute(i):
            base=[d for d in data if d[0]<=value<d[0]+d[1]];assert len(base)==1,(hex(i.address),i.op_str)
            k=bytes(i.bytes).rfind(struct.pack('<I',value));assert k>=0
            specs.append((site+k,(base[0][2],value-base[0][0]),3 if base[0][2][0]=='static' else 0))
(b/'original.bin').write_bytes(block)
def patch_file(path,mapping):
    m=read_map(mapping);out=[]
    for site,target,kind in specs:
        if kind==2:out.append((site,target,2))
        elif kind==1:out.append((site,m[target[1]],1))
        elif kind==3:out.append((site,(target[0][1]<<24)|target[1],3))
        else:out.append((site,m[target[0][1]]+target[1],0))
    path.write_bytes(struct.pack('<I',len(out))+b''.join(struct.pack('<III',*p) for p in out));return out
glyphs=b/'glyphs.bin';glyphs.write_bytes(G.build())
def run(exe):
    patches=exe.with_suffix('.patches.bin');patch_file(patches,exe.with_suffix('.map'))
    return subprocess.run([str(exe),str(glyphs),str(b/'original.bin'),str(patches)],cwd=b,capture_output=True,text=True,timeout=600)
exe=build(MODULE,'positive');x=run(exe);positive=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr[-2000:]);(b/'run.json').write_text(json.dumps(positive,indent=2));print(positive,flush=True)
m=re.search(r'STREAMING_GLYPHS_PASS inits=(\d+) fonts=(?:\d+)/(?:\d+) preloads=(?:\d+)/(?:\d+) reads=(?:\d+)/(?:\d+) updates=(?:\d+) texts=(?:\d+) prepares=(?:\d+) renders=(?:\d+)/(?:\d+)/(?:\d+) ops=(\d+) lookups=(\d+) null=(\d+) extents=(\d+)',x.stdout);assert not x.returncode and m,positive
# Controls: each mutated module copy is compiled into its own fixture unit and must fail a check.
text=MODULE.read_text();records=[]
MUTANTS=[
 ('lookup-fallback-char','        if (ch == 0x25A1)\n            return 0;\n        ch = 0x25A1;','        if (ch == 0x25A0)\n            return 0;\n        ch = 0x25A0;'),
 ('lookup-font-ignored','if (ways[way].ch == ch && ways[way].font == fontIndex)','if (ways[way].ch == ch)'),
 ('lookup-stale-inverted','return GetCacheBuffer(bigChar).entries[cachePos].glyphIndex != glyphIndex;','return GetCacheBuffer(bigChar).entries[cachePos].glyphIndex == glyphIndex;'),
 ('lookup-lru-newest','                if (oldest <= age)','                if (oldest > age)'),
 ('lookup-chunk-not-recorded','        glyphTextureCacheGlyphDataCacheChunk = chunk;\n','\n'),
 ('lookup-cachepos-kept','    entry.cachePos = 0;\n','\n'),
 ('lookup-chunk-offset','    entry.data = glyphTextureCacheGlyphDataCache[offset];','    entry.data = glyphTextureCacheGlyphDataCache[offset ^ 1];'),
 ('find-nbsp-unmapped','    if (ch == 0xA0)\n        ch = 0x20;\n    GlyphBank* bank','    GlyphBank* bank'),
 ('find-bank-index-not-recorded','            glyphBankCacheIndex = bankIndex;\n','\n'),
 ('extents-round-down','            currentX += static_cast<signed char>(static_cast<long>(advance < 0.0f ? advance - 0.5f : advance + 0.5f));','            currentX += static_cast<signed char>(static_cast<long>(advance < 0.0f ? advance - 0.5f : advance + 0.25f));'),
 ('render-space-advance','            penX += static_cast<signed char>(static_cast<long>(advance < 0.0f ? advance - 0.5f : advance + 0.5f));\n            continue;','            penX += static_cast<signed char>(static_cast<long>(advance < 0.0f ? advance - 0.5f : advance + 1.5f));\n            continue;'),
 ('extents-newline-x-kept','            currentX = 0.0f;\n            currentY +=','            currentY +='),
 ('extents-first-line-ignored','            if (firstLineOnly)\n                return C2DExtentsF(maxX, maxY);\n',''),
 ('extents-scale-ignored','glyphTextureCacheGlyphAdjustment[lookup->indices.AdjustmentIndex].Scale * 0.01f','1.0f'),
 ('init-guard-dropped','    if (textureCacheMem)\n        return;\n    textureCacheSize','    textureCacheSize'),
 ('init-a8-without-mipmaps','format, 0, static_cast<ESurfacePool>(1), false, true))','format, 0, static_cast<ESurfacePool>(1), false, false))'),
 ('init-a4-lookup','<< 12) | 0x0FFF);','<< 12) | 0x0FFE);'),
 ('init-best-alpha-shift','lookup32[i] = ~best.GetAMask() | (i << best.GetAPosShift());','lookup32[i] = ~best.GetAMask() | (i << (best.GetAPosShift() - 1));'),
 ('init-index-count','glyphTextureCacheGlyphDataNumIndices = (numTextureCacheGlyphDatasPerChunk + textureCacheNumGlyphs - 1) / numTextureCacheGlyphDatasPerChunk;','glyphTextureCacheGlyphDataNumIndices = (textureCacheNumGlyphs + 1) / numTextureCacheGlyphDatasPerChunk;'),
 ('init-adjustments-short','textureCacheNumGlyphAdjustments * 2);','textureCacheNumGlyphAdjustments);'),
 ('ctor-italics-inverted','    Italics = stream.ReadEBOOL();','    Italics = !stream.ReadEBOOL();'),
 ('ctor-flags-unbased','static_cast<unsigned short>((b << 6) + g + glyphs->FirstGlyph);','static_cast<unsigned short>((b << 6) + g);'),
 ('ctor-compressed-bank-index-kept','            glyphs = glyphBankCache;\n            glyphBankCacheIndex = b;\n','            glyphs = glyphBankCache;\n'),
 ('ctor-big-cache-entries-skipped','    InitializeStreamingFontCacheEntries(g_StreamingFontCacheBufferData2);\n',''),
 ('dtor-lookups-kept','                entry.ch = 0;\n',''),
 ('dtor-slot-kept','        streamingFontBankList[fontIndex] = 0;\n','        ;\n'),
 ('preload-retire-unmarked','        GFontTextureCacheUpdateMask[pending.index] = 1;\n',''),
 ('preload-ring-half','                g_TailPendingLoadedGlyphs = (g_TailPendingLoadedGlyphs + 1) & 0x1FF;','                g_TailPendingLoadedGlyphs = (g_TailPendingLoadedGlyphs + 1) & 0x0FF;'),
 ('preload-space-loaded','        if (ch == 10 || ch == 0x20 || ch == 0xA0)','        if (ch == 10 || ch == 0xA0)'),
 ('preload-pixel-offset','(lookup->data.MemOffset << 10) + textureCacheMemOffset','(lookup->data.MemOffset << 9) + textureCacheMemOffset'),
 ('preload-bigger-kept','            bigger->entries[index].glyphIndex = 0;\n',''),
 ('preload-smaller-span-fixed','            span <<= 1;\n',''),
 ('preload-async-flag-kept','    fonts->LoadFontGlyphsAsynchronously = wasAsynchronous;\n',''),
 ('refresh-mask-kept','        GFontTextureCacheUpdateMask[cell] = 0;\n',''),
 ('refresh-column-unscaled','column * GFontTextureBytesPerPixel * 32;','column * 32;'),
 ('refresh-a4-lookup-bytes','                    *texel++ = lookup[*source++];','                    *texel++ = lookup[*source++ ^ 1];'),
 ('read-text-big-endian-swapped','            text[i] = (unsigned short)(((unsigned char)bytes[i * 2 + 2] << 8) + (unsigned char)bytes[i * 2 + 3]);','            text[i] = (unsigned short)(((unsigned char)bytes[i * 2 + 3] << 8) + (unsigned char)bytes[i * 2 + 2]);'),
 ('read-text-unterminated','    text[count] = 0;\n',''),
 ('render-index-order','            *index++ = first + 3;','            *index++ = first + 1;'),
 ('render-flush-early','        if (numVertices > 252)','        if (numVertices > 248)'),
 ('render-mask-v','const float maskTop = (1.0f - top * clipScaleY)','const float maskTop = (top * clipScaleY - 1.0f)'),
 ('prepare-select-arg2','D3DTextureOperationModeTable[TEXTURE_OP_SELECT_ARG1], render->DeviceState.TextureOperation[0]','D3DTextureOperationModeTable[TEXTURE_OP_SELECT_ARG2], render->DeviceState.TextureOperation[0]'),
 ('prepare-alpha-ref','    render->SetRenderState(8, render->DeviceState.AlphaRef);','    render->SetRenderState(1, render->DeviceState.AlphaRef);'),
 ('prepare-stages-always','    if (setupTextureStages)\n    {\n        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_SELECT_ARG1]','    if (true)\n    {\n        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_SELECT_ARG1]'),
 ('refresh-wide-texel-stride','                    texels += GFontTextureBytesPerPixel;','                    texels += 1;'),
]
for name,old,new in MUTANTS:
    assert text.count(old)==1,(name,text.count(old))
    path=b/('mutant-'+name+'.cpp');path.write_text(text.replace(old,new))
    x=run(build(path,'fault-'+name));record=dict(name=name,code=x.returncode,stderr=x.stderr[-500:])
    assert x.returncode==80 and 'GLYPH_FAILURE' in x.stderr,record
    records.append(record);print(name,'rejected',flush=True)
again=run(exe);assert again.returncode==0 and again.stdout==positive['stdout'],again.stdout
# Grades of the three roots in the catalog unit (exact roots are gated by gate_native_streaming_font_cache.py).
grades={}
obj=b/'module.obj';x=V.cl(N.FLAGS+['/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(MODULE)],env,b);assert not x.returncode
c=Coff(obj)
for name,pattern,address,size in (('Render',r'\?Render@CStreamingFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z',0x9a25d0,3722),('PrepareToRender',r'\?PrepareToRender@CStreamingFontBank@@UBEX_N@Z',0x9a1720,2260),('ReadTextFileIntoArray',r'\?ReadTextFileIntoArray@\?A0x[0-9a-f]+@@YIKABVCWideString@@.*',0x9a3460,424),('GFUpdateFontTextureCache',r'\?GFUpdateFontTextureCache@\?A0x[0-9a-f]+@@YIXXZ',0x9a23e0,495),('Preload',r'\?Preload@CStreamingFontBank@@UBE_NABVCWideString@@_N@Z',0x9a2000,982),('CStreamingFontBank',r'\?\?0CStreamingFontBank@@QAE@PAVCStreamingFontDataBank@@K@Z',0x9a3c20,1298),('InitializeStreamingFontTextureCache',r'\?InitializeStreamingFontTextureCache@\?A0x[0-9a-f]+@@.*',0x9a3640,1490),('FindGlyph',r'\?FindGlyph@CStreamingFontBank@@.*',0x9a4160,142),('GlyphLookup',r'\?GlyphLookup@CStreamingFontBank@@.*',0x9a1140,918),('GetStringExtents',r'\?GetStringExtents@CStreamingFontBank@@.*',0x9a14e0,380)):
    symbol=[s for s in c.symbols if re.fullmatch(pattern,s)];assert len(symbol)==1
    sec,off=c.symbols[symbol[0]];built,rels=c.sections[sec]
    grades[name]=dict(compare(bytes(built),read_virtual_data(raw,address,size),list(rels.keys()),retail_relocations(hex(address),size)).to_dict(),built_bytes=len(built),retail_bytes=size)
assert all(sha(p)==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',initialisations=int(m[1]),operations=int(m[2]),lookups=int(m[3]),null_lookups=int(m[4]),extents=int(m[5]),repeat=True,controls=records,grades=grades,run=positive,build=str(b),input_sha256=pins,
 boundary='Recreated GFUpdateFontTextureCache (changed cells into a fixture-locked texel buffer for 1/2/4 byte texels), CStreamingFontBank::Preload (pending-read ring, cache-cell allocation and overlap invalidation, pixel reads through a fixture StreamGlyph that completes at once or leaves scripted requests in flight; full 512 KiB pixel cache compared), constructor and destructor (font records through fixture GetEntryDataSize/ReadEntryData boundaries; real memory input stream and strings; fonts compared byte for byte with owned allocations by content), recreated InitializeStreamingFontTextureCache (allocator, texture creation, display best format and streaming-bank reads are recording fixture boundaries; CPixelFormat::Initialise, GD3DFormats and the byte-vector fill insert are real and shared by both modes) and FindGlyph/GlyphLookup/GetStringExtents compiled in one unit with the fixture (to seed the module file statics) versus transplanted retail code from identical states; generated glyph data (not a retail font); retail lzo1x_decompress_safe 0x00C08170 transplanted for both modes; frame counter through a fixture system/display manager; fonts are raw storage seeded from the data (no constructor). GlyphLookup is behavior-equal but below the near_match threshold.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if a.output is None:(R/'rebuild/compile-gate/native-streaming-font-glyphs.json').write_text(json.dumps(report,indent=2))
print('NATIVE_STREAMING_FONT_GLYPHS_PASS inits=%s operations=%s controls=%d'%(m[1],m[2],len(records)),' '.join('%s=%.3f%s'%(k,v['opcode_similarity'],'(near)' if v['near_match'] else '') for k,v in grades.items()),b,flush=True)
