"""VC71 gate: C3DClothPrimitive::C3DClothPrimitive(const C3DMeshFileClothChunk&) (retail 0x00AC3E00, NEAR_MATCH) and
CCountedPointer<CParticleProgram>::Reset (retail 0x00AC3FF0, MATCH) and ::DeleteData (retail 0x00AC3FD0, MATCH).

Genuine C++ (no asm). Each root: the compiled body is compared with retail - an `exact` root must equal retail with
relocation fields masked, a `near` root must pass near_match.compare (same instruction count, opcode similarity >= 0.90);
every relocation must resolve, in instruction order, to the retail target at the same position (pins in ADDRESS; retail
absolute operands are found by pinned value, rel32 branches by target). Then the behaviour fixture must pass and must
not link without the provider. Writes rebuild/compile-gate/native-cloth-primitive-construct.json.
Pre-landing check: --source X.cpp --test Y_test.cpp --include DIR (repeatable).
"""
import hashlib, json, subprocess, sys, tempfile
from pathlib import Path
R = Path(__file__).resolve().parents[2]
if not (R / 'rebuild').is_dir():
    R = Path(r'D:\Documents\FableTLC')
sys.path.insert(0, str(R / 'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE, read_virtual_data
from near_match import compare, retail_relocations
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ADDRESS = {
    '??0CBaseClassNonCopyable@@QAE@XZ': 0x99a2f0,
    '??_7C3DClothPrimitive@@6B@': 0x129df74,
    '??2@YAPAXI@Z': 0xbfea1a,
    '??3@YAXPAX@Z': 0xbfe9bc,
    '_free': 0xbfea14,
    '??0CParticleProgram@@QAE@XZ': 0xae3550,
    '?Reset@?$CCountedPointer@VCParticleProgram@@@@QAEXPAVCParticleProgram@@@Z': 0xac3ff0,
    '??0CMemoryDataInputStream@@QAE@PBXK@Z': 0x994930,
    '??1CMemoryDataInputStream@@UAE@XZ': 0x993b10,
    '?ClothMemoryPool@N3DMeshAllocators@@3PAVCMovableResourceMemoryPool@@A': 0x13d2d70,
    '?ReadFromFile@CParticleProgram@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z': 0xae3c30,
    '??_C@_04FNMOPFIH@VGRP?$AA@': 0x129eaf4,
    '?CountSubChunksOfType@CChunkedFileSuperChunk@@QBEKPBD@Z': 0xaa7ea0,
    '?ReserveGroups@CParticleProgram@@QAEXK@Z': 0xae3330,
    '?FindFirstGroup@C3DMeshFileClothChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileGroupChunk@@@@XZ': 0xac6490,
    '??0C3DGroup2@@QAE@ABVC3DMeshFileGroupChunk@@@Z': 0xac4150,
    '?AddGroup@CParticleProgram@@QAEXABVC3DGroup2@@@Z': 0xae37d0,
    '?FindNextGroup@C3DMeshFileClothChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileGroupChunk@@@@ABV2@@Z': 0xac6520,
    '?DeleteData@?$CCountedPointer@VCParticleProgram@@@@CIXPAX@Z': 0xac3fd0,
}
SOURCE = R / 'rebuild/src/compiled/00/ac/C3DClothPrimitive_Construct_00ac3e00.cpp'
ROOTS = [
    dict(name='cloth_primitive_ctor', source=SOURCE, symbol='??0C3DClothPrimitive@@QAE@ABVC3DMeshFileClothChunk@@@Z',
         address=0xac3e00, size=0x1b9, instructions=149, relocations=20, grade='near'),
    dict(name='reset_cparticleprogram', source=SOURCE,
         symbol='?Reset@?$CCountedPointer@VCParticleProgram@@@@QAEXPAVCParticleProgram@@@Z',
         address=0xac3ff0, size=0x67, instructions=None, relocations=3, grade='exact'),
    dict(name='deletedata_cparticleprogram', source=SOURCE, symbol='?DeleteData@?$CCountedPointer@VCParticleProgram@@@@CIXPAX@Z',
         address=0xac3fd0, size=0xb, instructions=None, relocations=0, grade='exact'),
]
TEST = R / 'rebuild/tests/00/ac/C3DClothPrimitive_Construct_00ac3e00_test.cpp'
PATTERN = 'CLOTH_PRIMITIVE_CONSTRUCT_OK'


def arg(name, default):
    a = sys.argv[1:]
    vals = [a[i + 1] for i in range(len(a) - 1) if a[i] == name]
    return vals or default


def ordered_fields(code, base, sites, md, retail):
    out = []
    for ins in md.disasm(code, base):
        for s in sorted(sites):
            if ins.address - base <= s < ins.address - base + ins.size:
                v = int.from_bytes(code[s:s + 4], 'little')
                if retail and ins.mnemonic in ('call', 'jmp') and ins.bytes[0] in (0xe8, 0xe9):
                    v = (base + s + 4 + v) & 0xffffffff
                out.append((s, v))
    return out


def check_root(root, coff, image, md):
    sec, off = coff.symbols[root['symbol']]; body, rels = coff.sections[sec]
    assert off == 0
    size = len(body) if root['grade'] == 'exact' else root['size']   # declared sizes feed registration
    retail = read_virtual_data(image, root['address'], size)
    rel32, absolute = set(), set()
    pinned = set(v for v in ADDRESS.values() if v)
    for ins in md.disasm(retail, root['address']):
        o = ins.address - root['address']
        if ins.mnemonic in ('call', 'jmp') and ins.bytes[0] in (0xe8, 0xe9):
            t = int(ins.op_str, 16)
            if not root['address'] <= t < root['address'] + size:
                rel32.add(o + 1)
            continue
        for k in range(1, ins.size - 3):
            if int.from_bytes(retail[o + k:o + k + 4], 'little') in pinned:
                absolute.add(o + k)
    built = [(s, rels[s][0]) for s, _ in ordered_fields(bytes(body), 0, rels.keys(), md, False)]
    ret = ordered_fields(retail, root['address'], rel32 | absolute, md, True)
    assert len(built) == len(ret), (root['name'], built, ret)
    proof = []
    for (bs, name), (rs, target) in zip(built, ret):
        assert name in ADDRESS, (root['name'], name)
        if ADDRESS[name] is not None:
            assert ADDRESS[name] == target, (root['name'], name, hex(target))
        proof.append(dict(built_site=bs, retail_site=rs, symbol=name, target=hex(target)))
    if root['grade'] == 'exact':
        b, r = bytearray(body), bytearray(retail)
        for s, _ in built: b[s:s + 4] = b'\0\0\0\0'
        for s, _ in ret: r[s:s + 4] = b'\0\0\0\0'
        assert len(body) == len(retail) and b == r, (root['name'], 'bytes differ')
        return dict(name=root['name'], grade='MATCH', bytes=len(body), relocations=proof), 'MATCH'
    sim = compare(bytes(body), retail, list(rels), retail_relocations(hex(root['address']), size))
    assert sim.near_match and sim.complete_decode, (root['name'], sim)
    return dict(name=root['name'], grade='NEAR_MATCH', bytes=len(body), retail_bytes=size, similarity=sim.__dict__,
                relocations=proof), 'NEAR_MATCH'


def main():
    src = Path(arg('--source', [str(SOURCE)])[0]).resolve()
    test = Path(arg('--test', [str(TEST)])[0]).resolve()
    incs = [str(Path(p).resolve()) for p in arg('--include', [])] + [str(R / 'rebuild/include')]
    text = src.read_text(encoding='utf-8')
    assert '__asm' not in text and 'naked' not in text, 'asm is not reconstruction'
    env = V.env(); b = Path(tempfile.mkdtemp(prefix='cloth-construct-'))
    r = V.cl(['/nologo', '/c', '/O2', '/MT', '/W3'] + ['/I' + i for i in incs] + ['/Fo' + str(b / 'm.obj'), str(src)], env, b)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(b / 'm.obj'); image = EXE.read_bytes(); md = Cs(CS_ARCH_X86, CS_MODE_32)
    results, grades = [], []
    for root in ROOTS:
        res, grade = check_root(root, coff, image, md); results.append(res); grades.append('%s=%s' % (root['name'], grade))
    for flags, s, o in ((['/O2', '/Oy'], src, 'p.obj'), (['/Od'], test, 't.obj')):
        r = V.cl(['/nologo', '/c', '/W3', '/EHsc'] + flags + ['/I' + i for i in incs] + ['/Fo' + str(b / o), str(s)], env, b)
        assert not r.returncode, r.stdout + r.stderr
    link = [str(V.VC / 'bin/link.exe'), '/nologo', '/out:' + str(b / 't.exe')]
    alone = subprocess.run(link + [str(b / 't.obj')], env=env, capture_output=True, text=True)
    assert alone.returncode, 'fixture links without the provider'
    r = subprocess.run(link + [str(b / 'p.obj'), str(b / 't.obj')], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    run = subprocess.run([str(b / 't.exe')], env=env, capture_output=True, text=True, timeout=60)
    assert run.returncode == 0 and PATTERN in run.stdout, (run.returncode, run.stdout, run.stderr)
    sha = lambda p: hashlib.sha256(Path(p).read_bytes()).hexdigest()
    receipt = dict(status='PASS', roots=results, fixture=run.stdout.strip(), input_sha256={str(src): sha(src), str(test): sha(test)})
    if '--source' not in sys.argv:
        (R / 'rebuild/compile-gate/native-cloth-primitive-construct.json').write_text(json.dumps(receipt, indent=2))
    print('NATIVE_CLOTH_PRIMITIVE_CONSTRUCT_PASS', ' '.join(grades))


if __name__ == '__main__':
    main()
