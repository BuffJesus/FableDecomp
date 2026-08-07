"""verify_land_jumptable.py — byte-exact verify/land for functions with an
EMBEDDED JUMP TABLE (dense `switch` -> `movzx idx,[bytetab]; jmp [eax*4+jmptab]`
with the tables laid out inline in .text after the code).

`verify_and_land.py` cannot handle these: its `obj_text` parses `objdump -d`,
which (1) splits the body at every internal `$Lxxx` local label — so the
leaf-named block is only the pre-`jmp` head — and (2) renders the relocated-to-
zero jump-table dwords as `...`, dropping 4*N trailing bytes. Symptom is a
`DIFFER(NvM)` with N much smaller than the oracle length even though the source
is correct.

This wrapper imports verify_and_land unchanged and swaps ONLY `obj_text` for a
raw-COFF section extractor (reads the .text section bytes directly from the
object's symbol table, start -> next non-`$` symbol, trimming trailing CC/90).
Everything else — the pragma x flag sweep, relocation masking, behavior test,
catalog + oracle + src/tests landing — is the shared harness, byte-for-byte.

Usage is identical to verify_and_land.py:
    python tools/decomp_pipeline/verify_land_jumptable.py <land.json> <oracle.tsv> [--land]

Proven on CKeyRedefiner::GetSubTypeForAction 0x557CA0 (RELOCATION_MATCH 120/120)
and CKeyRedefiner::AreAllowedToCoexist 0x5578A0 (147/147). See CLAUDE.md gotcha
and docs/REDEFINE_INPUT_SYSTEM.md. Assumes the function is at .text offset 0
(true for the single-function objects this pipeline compiles); raises otherwise.
"""
import struct
import importlib.util
from pathlib import Path

VAL_PATH = Path(__file__).with_name("verify_and_land.py")
spec = importlib.util.spec_from_file_location("verify_and_land", str(VAL_PATH))
val = importlib.util.module_from_spec(spec)
spec.loader.exec_module(val)


def raw_obj_text(path, leaf):
    data = open(str(path), "rb").read()
    nsec = struct.unpack_from("<H", data, 2)[0]
    symptr = struct.unpack_from("<I", data, 8)[0]
    nsym = struct.unpack_from("<I", data, 12)[0]
    strtab = symptr + nsym * 18
    secs = []
    for i in range(nsec):
        o = 20 + i * 40
        name = data[o:o + 8].rstrip(b"\0").decode("latin1")
        rawsize = struct.unpack_from("<I", data, o + 16)[0]
        rawptr = struct.unpack_from("<I", data, o + 20)[0]
        secs.append((name, rawsize, rawptr))
    # ordinal among sections named exactly ".text" (matches obj_relocs' -h regex)
    text_ord = {}
    k = 0
    for i, (name, rs, rp) in enumerate(secs):
        if name == ".text":
            text_ord[i + 1] = k
            k += 1

    def symname(o):
        if struct.unpack_from("<I", data, o)[0] == 0:
            off = struct.unpack_from("<I", data, o + 4)[0]
            end = data.index(b"\0", strtab + off)
            return data[strtab + off:end].decode("latin1")
        return data[o:o + 8].rstrip(b"\0").decode("latin1")

    syms = []
    i = 0
    while i < nsym:
        o = symptr + i * 18
        nm = symname(o)
        v = struct.unpack_from("<I", data, o + 8)[0]
        sec = struct.unpack_from("<h", data, o + 12)[0]
        naux = data[o + 17]
        syms.append((nm, v, sec))
        i += 1 + naux

    target = None
    for nm, v, sec in syms:
        if leaf and leaf in nm and sec > 0 and secs[sec - 1][0] == ".text":
            target = (nm, v, sec)
            break
    if target is None:
        # non-.text / edge cases: defer to the original objdump path
        return val.__dict__["_orig_obj_text"](path, leaf)

    nm, start, secnum = target
    secname, rawsize, rawptr = secs[secnum - 1]
    ends = [v for (n2, v, s2) in syms
            if s2 == secnum and v > start and not n2.startswith("$") and n2 != nm]
    end = min(ends) if ends else rawsize
    b = bytearray(data[rawptr + start:rawptr + end])
    while len(b) > 0 and b[-1] in (0xCC, 0x90):
        b = b[:-1]
    if start != 0:
        # obj_relocs returns section-relative offsets; the harness applies them to
        # bytes starting at the function, so the function must sit at section 0.
        raise RuntimeError(f"function not at .text offset 0 (start={start:#x})")
    return bytes(b), text_ord.get(secnum, 0), nm


val.__dict__["_orig_obj_text"] = val.obj_text
val.obj_text = raw_obj_text

if __name__ == "__main__":
    val.main()
