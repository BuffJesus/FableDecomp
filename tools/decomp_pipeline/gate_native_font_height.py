"""Typed static/streaming font-height leaves with native and integer rounding oracles."""
from pathlib import Path
from datetime import datetime, timezone
import argparse, hashlib, json, re, struct, subprocess, sys
R = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(R/'tools/decomp_pipeline'))
import check_one as V
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE, read_virtual_data

PROFILES = [('CStaticFontBank', 0xab7a50, 0x129ead4, '00/ab/CStaticFontBank_GetFontMaxHeight_00ab7a50.cpp'),
            ('CStreamingFontBank', 0x9a0ea0, 0x129ab54, '00/9a/CStreamingFontBank_GetFontMaxHeight_009a0ea0.cpp')]
HELPERS = ['00/99/CBaseClass_CBaseClass_0099a2d0.cpp', '00/99/CCharString_DefaultConstructor_0099e4b0.cpp',
           '00/99/CCharString_Destructor_0099eae0.cpp', '00/99/CCharString_UnassignString_0099e9b0.cpp',
           '00/9a/CFontBank_Preload_009a0b20.cpp']
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--staging', type=Path)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    staging = args.staging.resolve() if args.staging else None
    b = args.output.resolve() if args.output else R/'rebuild/build/native-font-height'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    b.mkdir(parents=True)
    inc = b/'include'; inc.mkdir()
    overlay = staging/'include' if staging else R/'rebuild/include'
    fixture = staging/'Fixture.cpp' if staging else R/'rebuild/tests/integration/NativeFontHeight_test.cpp'
    roots = [(cls, entry, table, staging/(cls+'.cpp') if staging else R/'rebuild/src/compiled'/relative)
             for cls, entry, table, relative in PROFILES]
    helpers = [R/'rebuild/src/compiled'/p for p in HELPERS]
    inputs = {Path(__file__), EXE}; seen = set()
    def closure(p):
        p = p.resolve()
        if p in seen: return
        seen.add(p); inputs.add(p)
        for name in re.findall(r'^\s*#include\s*"([^"\n]+)"', p.read_text(), re.M):
            closure(next(q for q in (p.parent/name, overlay/name, R/'rebuild/include'/name) if q.is_file()))
    for p in helpers+[p for _,_,_,p in roots]+[fixture]: closure(p)
    for p in seen:
        if p.is_relative_to(overlay): target = inc/p.relative_to(overlay)
        elif p.is_relative_to(R/'rebuild/include'): target = inc/p.relative_to(R/'rebuild/include')
        else: continue
        target.parent.mkdir(parents=True, exist_ok=True); target.write_bytes(p.read_bytes())
    inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
    inputs.update(p for d in (V.VC/'bin', V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
    for module in tuple(sys.modules.values()):
        name = getattr(module, '__file__', None)
        if name:
            p = Path(name).resolve()
            if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'): inputs.add(p)
    pins = {str(p):sha(p) for p in inputs}; (b/'inputs.json').write_text(json.dumps(pins, indent=2))
    env = V.env(); env['INCLUDE'] = str(V.VC/'include')+';'+str(inc)
    objects = {}; records = []; controls = []
    def compile(source, label, flags=(), text=None):
        snapshot = b/(label+'.cpp'); snapshot.write_text(text if text is not None else source.read_text())
        obj = snapshot.with_suffix('.obj')
        result = V.cl(['/nologo','/c','/O2','/MT','/Gy',*flags,'/I'+str(inc),'/Fo'+str(obj),str(snapshot)], env, b)
        (b/(label+'.compile.json')).write_text(json.dumps(dict(code=result.returncode,stdout=result.stdout,stderr=result.stderr,source=str(source))))
        assert not result.returncode, result.stdout+result.stderr
        objects[str(obj)] = sha(obj)
        return obj
    def link(items, label):
        exe = b/(label+'.exe')
        result = subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'/map:'+str(b/(label+'.map')),*map(str,items)],env=env,cwd=b,capture_output=True,text=True)
        (b/(label+'.link.json')).write_text(json.dumps(dict(code=result.returncode,stdout=result.stdout,stderr=result.stderr)))
        return result, exe
    def execute(exe, args=()):
        result = subprocess.run([str(exe),*map(str,args)],cwd=b,capture_output=True,text=True,timeout=20)
        return dict(code=result.returncode,stdout=result.stdout,stderr=result.stderr)
    support = [compile(p,'helper'+str(i)) for i,p in enumerate(helpers)]
    retail = EXE.read_bytes()
    assert read_virtual_data(retail,0x122dcb4,4)==struct.pack('<f',4294967296.0)
    for cls, entry, table, source in roots:
        symbol = '?GetFontMaxHeight@'+cls+'@@UBEMXZ'
        obj = compile(source,cls); coff = Coff(obj); section,offset = coff.symbols[symbol]
        raw, rels = coff.sections[section]
        assert offset==0 and len(raw)==19 and rels=={13:('__real@4f800000',6)}
        fixed = bytearray(raw); struct.pack_into('<I',fixed,13,0x122dcb4)
        original = read_virtual_data(retail,entry,19); assert bytes(fixed)==original
        assert struct.unpack('<I',read_virtual_data(retail,table+8,4))[0]==entry
        locator = struct.unpack('<I',read_virtual_data(retail,table-4,4))[0]
        type_desc = struct.unpack('<I',read_virtual_data(retail,locator+12,4))[0]
        assert read_virtual_data(retail,type_desc+8,80).split(b'\0',1)[0].decode()=='.?AV'+cls+'@@'
        binary = b/(cls+'.bin'); binary.write_bytes(original)
        host = compile(fixture,'host-'+cls,['/DSTREAMING_FONT'] if cls=='CStreamingFontBank' else [])
        result,exe = link(support+[obj,host],cls); assert not result.returncode,result.stdout+result.stderr
        runs = [execute(exe),execute(exe,[binary])]
        assert all(r['code']==0 and 'NATIVE_FONT_HEIGHT_PASS cases=3072' in r['stdout'] for r in runs),runs
        assert 'original=3072' in runs[1]['stdout']
        repeat = execute(exe,[binary]); assert repeat==runs[1]
        mapped = {}
        for line in (b/(cls+'.map')).read_text().splitlines():
            match = re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
            if match: mapped[match[1]]=int(match[2],16)
        pe = pefile.PE(str(exe)); linked = pe.get_data(mapped[symbol]-pe.OPTIONAL_HEADER.ImageBase,19)
        assert linked[:13]==original[:13] and linked[17:]==original[17:]
        constant = struct.unpack_from('<I',linked,13)[0]
        assert pe.get_data(constant-pe.OPTIONAL_HEADER.ImageBase,4)==struct.pack('<f',4294967296.0)
        text = source.read_text()
        for name,replacement in [('signed','static_cast<float>(static_cast<long>(FontHeight))'),('wrong-field','static_cast<float>(MaxHeight)'),('zero','0.0f')]:
            assert text.count('static_cast<float>(FontHeight)')==1
            fault = compile(source,cls+'-'+name,text=text.replace('static_cast<float>(FontHeight)',replacement))
            result,fault_exe = link(support+[fault,host],cls+'-'+name); assert not result.returncode,result.stdout+result.stderr
            rejected = execute(fault_exe); assert rejected['code']==80 and 'FONT_HEIGHT_FAILURE' in rejected['stderr'],rejected
            controls.append(dict(class_name=cls,name=name,**rejected))
        omitted,_ = link(support+[host],cls+'-missing')
        assert omitted.returncode and 'GetFontMaxHeight' in omitted.stdout+omitted.stderr
        controls.append(dict(class_name=cls,name='missing-provider',link_rejected=True))
        fault = compile(source,cls+'-global',text='#include "fable_font_instances.h"\nfloat __fastcall GetFontMaxHeight('+cls+'* self){return static_cast<float>(self->FontHeight);}\n')
        assert symbol not in Coff(fault).symbols
        controls.append(dict(class_name=cls,name='historical-global-symbol',typed_provider_absent=True))
        mutant = bytearray(original); assert mutant[4:7]==bytes.fromhex('db4104'); mutant[6]=0x10
        mutant_file = b/(cls+'-original-wrong-field.bin'); mutant_file.write_bytes(mutant)
        rejected = execute(exe,[mutant_file]); assert rejected['code']==80 and 'FONT_HEIGHT_FAILURE' in rejected['stderr']
        controls.append(dict(class_name=cls,name='original-wrong-field',**rejected))
        records.append(dict(class_name=cls,address=hex(entry),symbol=symbol,vtable=hex(table),slot=2,grade='RELOCATION_MATCH',bytes=19,relocations={str(k):v for k,v in rels.items()},object=str(obj),runs=runs,repeat=True,final_pe_verified=True))
        print('NATIVE_FONT_HEIGHT_CLASS_PASS',cls,flush=True)
    assert len(controls)==12
    for p,h in pins.items(): assert sha(Path(p))==h,('input drift',p)
    for p,h in objects.items(): assert sha(Path(p))==h,('object drift',p)
    report = dict(status='PASS',build=str(b),records=records,controls=controls,cases=6144,original_cases=6144,input_sha256=pins,objects=objects,
                  boundary='Native height leaves and typed/direct/base-virtual/ABI calls agree with independent integer IEEE754 unsigned conversion across12 x87 modes. Concrete constructor/destructor and glyph services are fixture boundaries; actual base/empty Name helpers execute. No payload acquisition, complete derived lifetime or rendering certification.')
    (b/'receipt.json').write_text(json.dumps(report,indent=2))
    if args.output is None and staging is None:
        (R/'rebuild/compile-gate/native-font-height.json').write_text(json.dumps(report,indent=2))
    print('NATIVE_FONT_HEIGHT_GATE_PASS cases=6144 original=6144 classes=2 rounds=4 precisions=3 controls=12 bytes=38',b,flush=True)
if __name__=='__main__':main()
