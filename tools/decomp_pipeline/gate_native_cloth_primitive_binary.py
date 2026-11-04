"""VC71 gate: C3DClothPrimitive::SaveBinary (retail 0x00AC3B00, exact) and LoadBinary (retail 0x00AC3BD0, NEAR_MATCH:
opcode similarity 0.995, 199/199 instructions, 549 bytes; the counted pointer release clears Data before Info where
retail clears Info first).

Genuine C++ (no asm). The compiled body must pass near_match.compare against retail (opcode similarity >= 0.90, within
10%% of retail's instruction count), every relocation must resolve in instruction order to the retail target at the
same position (pins in ADDRESS), and the behaviour fixture must pass and must not link without the provider.
Writes rebuild/compile-gate/native-cloth-primitive-binary.json. Pre-landing: --source/--test/--include.
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
    '?WriteULONG@CDataOutputStream@@QAEXK@Z': 0x993ea0,
    '??0CMemoryDataOutputStream@@QAE@XZ': 0x996af0,
    '?WriteToFile@CParticleProgram@@QBEXAAVCDataOutputStream@@@Z': 0xae2230,
    '?GetLength@CMemoryDataOutputStream@@QBEKXZ': 0x994b40,
    '?PeekData@CMemoryDataOutputStream@@QBEPBXXZ': 0x995e70,
    '?WriteCompressed@CDataOutputStream@@QAEXPBXJ@Z': 0x996610,
    '??1CMemoryDataOutputStream@@UAE@XZ': 0x996b30,
    '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z': 0x993ca0,
    '?Allocate@CHandleBasedMemoryPool@@QAE_NAAVCScopedMemoryHandle@@KK@Z': 0xa032f0,
    '?Free@CHandleBasedMemoryPool@@QAEXJ@Z': 0xa02b30,
    '?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z': 0xa026b0,
    '??0CMemoryDataInputStream@@QAE@PBXK@Z': 0x994930,
    '??2@YAPAXI@Z': 0xbfea1a,
    '??0CParticleProgram@@QAE@XZ': 0xae3550,
    '?Reset@?$CCountedPointer@VCParticleProgram@@@@QAEXPAVCParticleProgram@@@Z': 0xac3ff0,
    '?ClothMemoryPool@N3DMeshAllocators@@3PAVCMovableResourceMemoryPool@@A': 0x13d2d70,
    '?ReadFromFile@CParticleProgram@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z': 0xae3c30,
    '??3@YAXPAX@Z': 0xbfe9bc,
    '??1CMemoryDataInputStream@@UAE@XZ': 0x993b10,
}
SOURCE = R / 'rebuild/src/compiled/00/ac/C3DClothPrimitive_Binary_00ac3b00.cpp'
ROOTS = [
    dict(name='cloth_save_binary', source=SOURCE,
         symbol='?SaveBinary@C3DClothPrimitive@@QBEXAAVCDataOutputStream@@@Z',
         address=0xac3b00, size=116, instructions=40, relocations=10, grade='exact'),
    dict(name='cloth_load_binary', source=SOURCE,
         symbol='?LoadBinary@C3DClothPrimitive@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@@Z',
         address=0xac3bd0, size=549, instructions=199, relocations=17, grade='near'),
]
TEST = R / 'rebuild/tests/00/ac/C3DClothPrimitive_Binary_00ac3b00_test.cpp'
PATTERN = 'CLOTH_BINARY_OK'


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
    env = V.env(); b = Path(tempfile.mkdtemp(prefix='cloth-primitive-binary-'))
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
        (R / 'rebuild/compile-gate/native-cloth-primitive-binary.json').write_text(json.dumps(receipt, indent=2))
    print('NATIVE_CLOTH_PRIMITIVE_BINARY_PASS', ' '.join(grades))


if __name__ == '__main__':
    main()
