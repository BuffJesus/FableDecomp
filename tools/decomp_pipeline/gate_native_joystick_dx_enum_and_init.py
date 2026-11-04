"""Near-match VC71 gate: CJoystickDX::EnumAndInit (retail 0x00AB6B10, 506 bytes), the IDirectInput8 EnumDevices callback.

Genuine C++ (no asm). Passes when (1) near_match.compare grades the compiled body near_match against retail (same
instruction count, opcode similarity >= 0.90); (2) every relocation in the compiled body resolves, in instruction
order, to the retail target at the corresponding retail relocation (pins in ADDRESS); (3) the behaviour fixture
passes, and does not link without the provider. Writes rebuild/compile-gate/native-joystick-dx-enum-and-init.json.
Override paths for a pre-landing check: --source X.cpp --test Y_test.cpp --include DIR (--include may repeat).
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
    '?GFGetSystemManager@@YIPAVCSystemManager@@XZ': 0x9a4ec0,
    '_c_dfDIJoystick': 0x12ae20c,
}
ROOTS = [
    dict(name='enum_and_init', source=R / 'rebuild/src/compiled/00/ab/CJoystickDX_EnumAndInit_00ab6b10.cpp',
         symbol='?EnumAndInit@CJoystickDX@@KGHPBUDIDEVICEINSTANCEW@@PAX@Z', address=0xab6b10, size=506,
         instructions=174, relocations=2),
]
TEST = R / 'rebuild/tests/00/ab/CJoystickDX_EnumAndInit_00ab6b10_test.cpp'
PATTERN = 'JOYSTICK_ENUM_AND_INIT_OK'


def arg(name, default):
    a = sys.argv[1:]
    vals = [a[i + 1] for i in range(len(a) - 1) if a[i] == name]
    return vals or default


def reloc_targets(code, base, sites, md, retail):
    """Ordered (site, value) for each 4-byte relocation field: the absolute operand, or the rel32 branch target."""
    out = []
    for ins in md.disasm(code, base):
        for s in sorted(sites):
            if ins.address - base <= s < ins.address - base + ins.size:
                field = int.from_bytes(code[s:s + 4], 'little')
                if retail and ins.mnemonic in ('call', 'jmp') and ins.bytes[0] in (0xe8, 0xe9):
                    field = (base + s + 4 + field) & 0xffffffff
                out.append((s, field))
    return out


def main():
    root = ROOTS[0]
    src = Path(arg('--source', [str(root['source'])])[0]).resolve()
    test = Path(arg('--test', [str(TEST)])[0]).resolve()
    incs = [str(Path(p).resolve()) for p in arg('--include', [])] + [str(R / 'rebuild/include')]
    env = V.env(); b = Path(tempfile.mkdtemp(prefix='enum-and-init-'))
    text = src.read_text(encoding='utf-8')
    assert '__asm' not in text and 'naked' not in text, 'asm is not reconstruction'
    r = V.cl(['/nologo', '/c', '/O2', '/MT', '/W3'] + ['/I' + i for i in incs] + ['/Fo' + str(b / 'm.obj'), str(src)], env, b)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(b / 'm.obj'); sec, off = coff.symbols[root['symbol']]; body, rels = coff.sections[sec]
    assert off == 0
    image = EXE.read_bytes(); retail = read_virtual_data(image, root['address'], root['size'])
    retail_sites = retail_relocations(hex(root['address']), root['size'])
    sim = compare(bytes(body), retail, list(rels), retail_sites)
    assert sim.near_match and sim.complete_decode, sim
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    # Built relocation fields name their symbol; retail ones hold the absolute target or a rel32 displacement.
    built = [(s, rels[s][0]) for s, _ in reloc_targets(bytes(body), 0, rels.keys(), md, False)]
    rel32_sites = set()
    for ins in md.disasm(retail, root['address']):
        if ins.mnemonic in ('call', 'jmp') and ins.bytes[0] in (0xe8, 0xe9):
            t = int(ins.op_str, 16)
            if not root['address'] <= t < root['address'] + root['size']:
                rel32_sites.add(ins.address - root['address'] + 1)
    # Absolute operands: the base-relocation sites inside the body, plus any 4-byte instruction field holding a pinned
    # absolute target (this image's base-relocation table does not cover every such field).
    pinned = set(ADDRESS.values())
    for ins in md.disasm(retail, root['address']):
        o = ins.address - root['address']
        for k in range(1, ins.size - 3):
            if int.from_bytes(retail[o + k:o + k + 4], 'little') in pinned and o + k not in rel32_sites:
                rel32_sites.discard(o + k); retail_sites = tuple(retail_sites) + (o + k,)
    rsites = set(retail_sites) | rel32_sites
    ret = reloc_targets(retail, root['address'], rsites, md, True)
    assert len(built) == len(ret) == root['relocations'], (built, ret)
    proof = []
    for (bs, name), (rs, target) in zip(built, ret):
        assert ADDRESS.get(name) == target, (name, hex(target))
        proof.append(dict(built_site=bs, retail_site=rs, symbol=name, target=hex(target)))
    # Behaviour fixture: provider /O2 /Oy, fixture /Od; the fixture must not link alone.
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
    receipt = dict(status='PASS', grade='NEAR_MATCH', symbol=root['symbol'], address=hex(root['address']),
                   built_bytes=len(body), retail_bytes=root['size'], similarity=sim.__dict__, relocations=proof,
                   fixture=run.stdout.strip(), input_sha256={str(src): sha(src), str(test): sha(test)})
    if '--source' not in sys.argv:
        (R / 'rebuild/compile-gate/native-joystick-dx-enum-and-init.json').write_text(json.dumps(receipt, indent=2))
    print('NATIVE_JOYSTICK_DX_ENUM_AND_INIT_PASS', 'enum_and_init=NEAR_MATCH similarity=%.3f instructions=%d/%d'
          % (sim.opcode_similarity, sim.built_instructions, sim.retail_instructions))


if __name__ == '__main__':
    main()
