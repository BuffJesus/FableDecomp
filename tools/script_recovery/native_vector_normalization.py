"""Recover a proven native float-triplet normalization call from stack stores."""
import hashlib
import re
import struct

CALLEE = 0xA14440
CALLEE_SHA256 = "920be4a11b60c8d6269864dce7816181e214da1ba45510ebf0ab317b7d63ad7a"
# fld y; lea receiver; fsub y; fld x; fsub x; zero z; fstp x; fstp y.
PLANAR_STORES = bytes.fromhex("d94424708d4c2454d86004d944246cd820c744245c00000000d95c2454d95c2458")
CALL = re.compile(r"C3DVector::GetScaled\(\(C3DVector \*\)([^;]+)\);")

def recover(function, source, rdata):
    if not function or "C3DVector::GetScaled" not in source:
        return source, []
    calls = [c for c in function.get("calls", []) if c.get("currentName") == "C3DVector::GetScaled"]
    matches = list(CALL.finditer(source))
    if len(calls) != len(matches):
        return source, []
    raw = rdata.bytes_at(CALLEE, 0x3d)
    if (raw is None or hashlib.sha256(raw).hexdigest() != CALLEE_SHA256
            or rdata.bytes_at(0x122DED8, 4) != struct.pack("<f", 1.0)):
        return source, []
    edits = []; evidence = []
    for index, (call, match) in enumerate(zip(calls, matches)):
        try:
            site = int(call["site"], 16); target = int(call["target"], 16)
        except (KeyError, TypeError, ValueError):
            continue
        # This reviewed instruction shape independently materializes exactly
        # three adjacent float words and ECX points at the first one.
        if (target != CALLEE or call.get("ecxStack") != -36
                or rdata.bytes_at(site-33, 33) != PLANAR_STORES
                or rdata.bytes_at(site, 5) != b"\xe8" + struct.pack("<i", CALLEE-site-5)):
            continue
        x = re.fullmatch(r"&((?:fStack_|f_stk_)24)", match[1])
        if not x:
            continue
        prefix = source[:match.start()]
        store = re.search(r"(?P<y>(?:fStack_|f_stk_)20) = (?P<ye>[^;]+);\s*"
                          + re.escape(x[1]) + r" = (?P<xe>[^;]+);\s*"
                          r"(?P<z>(?:fStack_|f_stk_)1c) = 0\.0;\s*$", prefix)
        if not store:
            continue
        yexpr = re.fullmatch(r'\(float\)(\w+)\._4_4_ - (\w+)\.y', store['ye'])
        xexpr = re.fullmatch(r'\(float\)(\w+)\._0_4_ - (\w+)\.x', store['xe'])
        if not yexpr or not xexpr or yexpr.groups() != xexpr.groups():
            continue
        token = "ENGINE_NormalizeFloat3_" + str(index)
        fields = [x[1], store["y"], store["z"]]
        edits.append((match.start(), match.end(), token + "(" + ", ".join(fields) + ");"))
        evidence.append({"token":token,"fields":fields,"site":hex(site),"target":hex(target),
                         "calleeSha256":CALLEE_SHA256,"receiverStack":-36,
                         "precision":"default native startup53bit, round-to-nearest; not live observed"})
    for start, end, new in reversed(edits):
        source = source[:start] + new + source[end:]
    return source, evidence
