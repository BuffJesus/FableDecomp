"""Fresh VC71 parity gate: streaming font module, first roots — CStreamingFontBank::SetAlphaMaskUVs /
SetUseFixedFunction (static UV and fixed-function switches) and the glyph-cache LRU helpers
InitializeStreamingFontCacheEntries, UpdateUseStreamingFontCacheEntry, InsertNewStreamingFontCacheEntry
(anonymous namespace; Ego_r names, ghidra_out/labels_streaming_font.tsv)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?AlphaMaskUVMul@CStreamingFontBank@@2VC2DVector@@A':0x13ca2f8,  # derived from retail site; not in symbol_index
 '?AlphaMaskUVAdd@CStreamingFontBank@@2VC2DVector@@A':0x13ca300,  # derived from retail site; not in symbol_index
 '?UseFixedFunction@CStreamingFontBank@@2_NA':0x13c5a7c,  # derived from retail site; not in symbol_index
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 # Module file statics (C-style static names in the object) and the teardown callees.
 '_streamingFontBankList':0x13bda50,'_glyphLookup':0x13bda70,'_textureCacheMem':0x13bda24,
 '_glyphTextureCacheGlyphAdjustment':0x13bda40,'_glyphTextureCacheGlyphDataIndices':0x13bda34,'_glyphTextureCacheGlyphData':0x13bda38,'_glyphTextureCacheGlyphDataCache':0x13bda3c,
 '??_7CStreamingFontBank@@6B@':0x129ab54,'??_7CFontBank@@6B@':0x129ab38,'??1CStreamingFontBank@@UAE@XZ':0x9a1090,
 '??3@YAXPAX@Z':0xbfe9bc,'??_V@YAXPAX@Z':0xbfeb1c,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,'??1CCharString@@QAE@XZ':0x99eae0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '?EnterSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2000,'?LeaveSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2010,'?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z':0x9f2920,
 '?GetNoAllocations@CMemoryAllocator@@QAEKXZ':0x9f1f10,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,
 '??1CMemoryAllocator@@IAE@XZ':0x9f2310,'??3CMemoryAllocator@@KIXPAX@Z':0x9f1f90,
 '?g_StreamingFontCacheBufferData@?A0x38f3e2b1@@3U?$StreamingFontCacheBufferData_static@$0CA@$0CA@@1@A':0x13c5a98,
 '?g_StreamingFontCacheBufferData2@?A0x38f3e2b1@@3U?$StreamingFontCacheBufferData_static@$0EA@$0EA@@1@A':0x13c7ac8,
 '?ToWideString@CCharString@@QBE?AVCWideString@@XZ':0x99e480,'??1CWideString@@QAE@XZ':0x99b510,
 '?ReadTextFileIntoArray@?A0x38f3e2b1@@YIKABVCWideString@@AAV?$CArray@G@@@Z':0x9a3460,
 # Static construction/destruction of the unit's other globals (derived from retail sites; atexit is the
 # executable's CRT registration routine, called by every unit's dynamic initialisers).
 '_atexit':0x4012bc,'_free':0xbfea14,'_GFontTextureCache':0x13c5a80,'_GFontPixelLookupTable':0x13c5a88,
 '?g_PendingLoadedGlyphs@NStreamingFontBankGlobals@@3PAUPendingLoadedGlyph@@A':0x13c82f8,
}
F=R/'rebuild/src/compiled/00/9a/CStreamingFontBank_Module_009a0ec0.cpp'
ANON=r'\?%s@\?A0x[0-9a-f]{8}@@%s'
ROOTS=[
 dict(name='set_alpha_mask_uvs',source=F,symbol='?SetAlphaMaskUVs@CStreamingFontBank@@SIXABVC2DVector@@0@Z',address=0x9a0ec0,size=33,instructions=9,relocations=4),
 dict(name='set_use_fixed_function',source=F,symbol='?SetUseFixedFunction@CStreamingFontBank@@SIX_N@Z',address=0x9a0ef0,size=7,instructions=2,relocations=1),
 dict(name='init_cache_entries',source=F,symbol_pattern=ANON%('InitializeStreamingFontCacheEntries',r'YIXAAUStreamingFontCacheBufferData@1@@Z'),address=0x9a0f00,size=137,instructions=45,relocations=1),
 dict(name='update_use_cache_entry',source=F,symbol_pattern=ANON%('UpdateUseStreamingFontCacheEntry',r'YIXAAUStreamingFontCacheBufferData@1@G@Z'),address=0x9a0f90,size=143,instructions=51,relocations=1),
 dict(name='dtor',source=F,symbol='??1CStreamingFontBank@@UAE@XZ',address=0x9a1090,size=170,instructions=54,relocations=11),
 dict(name='uninitialise_texture_cache',source=F,symbol='?UninitializeStreamingFontTextureCache@@YIXXZ',address=0x9a1660,size=188,instructions=51,relocations=26),
 dict(name='scalar_deleting_dtor',source=F,symbol='??_GCStreamingFontBank@@UAEPAXI@Z',address=0x9a4e40,size=30,instructions=11,relocations=2),
 # Dynamic initialisers of the two cache buffers (the template constructor inlined); VC7.1 names them $E<n>.
 dict(name='small_cache_static_init',source=F,symbol='_$E11',address=0x1223f00,size=88,instructions=14,relocations=13),
 dict(name='big_cache_static_init',source=F,symbol='_$E13',address=0x1223f60,size=93,instructions=14,relocations=13),
 dict(name='read_text_file_narrow',source=F,symbol_pattern=ANON%('ReadTextFileIntoArray',r'YIKABVCCharString@@AAV\?\$CArray@G@@@Z'),address=0x9a3610,size=39,instructions=16,relocations=3),
 # The remaining dynamic initialisers ($E<n>, resolved by content below): the pending-glyph ring's
 # construction (counted pointers zeroed) and atexit destruction (release each pointer, last owner frees the
 # info), and the atexit destructors of the font texture cache (tail jump to CTexture::Uninitialise) and the
 # pixel lookup table (CArray storage freed).
 dict(name='pending_glyphs_static_init',source=F,symbol='_$E5',address=0x1223fc0,size=39,instructions=13,relocations=3),
 dict(name='pending_glyphs_static_destroy',source=F,symbol='_$E6',address=0x122a610,size=71,instructions=26,relocations=2),
 dict(name='texture_cache_static_init',source=F,symbol='_$E8',address=0x1223ee0,size=12,instructions=4,relocations=2),
 dict(name='texture_cache_static_destroy',source=F,symbol='_$E9',address=0x122a600,size=10,instructions=2,relocations=2),
 dict(name='pixel_lookup_static_init',source=F,symbol='_$E11',address=0x1223ef0,size=12,instructions=4,relocations=2),
 dict(name='pixel_lookup_static_destroy',source=F,symbol='_$E12',address=0x122a660,size=17,instructions=7,relocations=2),
 dict(name='insert_new_cache_entry',source=F,symbol_pattern=ANON%('InsertNewStreamingFontCacheEntry',r'YIGAAUStreamingFontCacheBufferData@1@@Z'),address=0x9a1020,size=105,instructions=37,relocations=1),
]
def resolve_static_initialisers():
 # VC7.1 numbers dynamic initialisers $E<n> in unit order, so the number moves when the unit grows: pick each
 # cache buffer's initialiser by what it initialises (the $E function relocating to that buffer object).
 import tempfile,check_one as V
 from gate_game_component_abi import Coff
 obj=Path(tempfile.mkdtemp(prefix='streaming-einit-'))/'module.obj'
 r=V.cl(native_exact_roots.FLAGS+['/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(F)],V.env(),obj.parent);assert r.returncode==0,r.stdout
 c=Coff(obj);found={}
 for name,(sec,off) in c.symbols.items():
  if not name.startswith('_$E'):continue
  built,rels=c.sections[sec]
  targets={(t[0] if isinstance(t,tuple) else t) for t in rels.values()} if isinstance(rels,dict) else set()
  for root,buffer in (('small_cache_static_init','?g_StreamingFontCacheBufferData@'),('big_cache_static_init','?g_StreamingFontCacheBufferData2@')):
   if any(str(t).startswith(buffer) for t in targets) and len(built)==next(x['size'] for x in ROOTS if x['name']==root) and not (root=='small_cache_static_init' and len(built)==93):
    found.setdefault(root,[]).append(name)
 for x in ROOTS:
  if x['name'] in ('small_cache_static_init','big_cache_static_init'):
   assert len(found.get(x['name'],[]))==1,(x['name'],found);x['symbol']=found[x['name']][0]
 # Destroyers by the object they tear down and their size, then each registering initialiser by the destroyer
 # it hands to atexit; the destroyer's retail address joins the ADDRESS map under its built name.
 def refs(name):
  sec=c.symbols[name][0];built,rels=c.sections[sec]
  return len(built),{str(t[0] if isinstance(t,tuple) else t) for t in rels.values()} if isinstance(rels,dict) else set()
 einit=[n for n in c.symbols if n.startswith('_$E')]
 for destroy,init,target in (('pending_glyphs_static_destroy','pending_glyphs_static_init','?g_PendingLoadedGlyphs@'),
   ('texture_cache_static_destroy','texture_cache_static_init','_GFontTextureCache'),('pixel_lookup_static_destroy','pixel_lookup_static_init','_GFontPixelLookupTable')):
  d=next(x for x in ROOTS if x['name']==destroy);i=next(x for x in ROOTS if x['name']==init)
  ds=[n for n in einit if refs(n)[0]==d['size'] and any(t.startswith(target) for t in refs(n)[1]) and '_atexit' not in refs(n)[1]]
  assert len(ds)==1,(destroy,ds);d['symbol']=ds[0];ADDRESS[ds[0]]=d['address']
  inits=[n for n in einit if refs(n)[0]==i['size'] and ds[0] in refs(n)[1] and '_atexit' in refs(n)[1]]
  assert len(inits)==1,(init,inits);i['symbol']=inits[0]
def main():
 resolve_static_initialisers()
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-streaming-font-cache.json',[],
  'CStreamingFontBank alpha-mask/fixed-function statics and the streaming glyph-cache LRU entry helpers')
 print('NATIVE_STREAMING_FONT_CACHE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
