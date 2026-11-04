"""Restore the destination string of `CCharString::AssignFromWide` when Ghidra drops ECX.

Some functions print the thiscall as `CCharString__AssignFromWide(&DAT_x);` -- the
UTF-16 literal survives but the stack string it initialises does not (Arena
ExitGuard Main 0x00F1B9C0: the empty fail message later passed to SetQuestAsFailed
by address, so the lifter put that address in the bool slot and guessed the
string from a stale temporary). Runs on the lowered source after the reviewed
restores, so their hash gates still see the text they reviewed.

Every printed call is paired with its native site through `callOrder`; each site
must be exactly `push <wide literal>; lea ecx,[esp+d]; call`, the printed literal
must equal the UTF-16 bytes, and the recovered slot must already be named in the
text. Any disagreement leaves the function unchanged. The result is the string
constructor the wide-literal lowering emits for the two-operand spelling.
"""
import re

ASSIGN_FROM_WIDE = 0x99B6B0
RE_DROPPED = re.compile(r'^([ \t]*)CCharString(?:::|__)AssignFromWide\(("(?:[^"\\]|\\.)*")\);', re.M)
RE_ANY = re.compile(r'\bCCharString(?:::|__)AssignFromWide\(')


def recover_wide_assign_receivers(source, fn, rdata):
    import json
    printed = list(RE_DROPPED.finditer(source))
    if not printed or len(printed) != len(RE_ANY.findall(source)):
        return source
    calls = {c['site'].lower(): c for c in fn.get('calls', [])
             if int(c.get('target', '0'), 16) == ASSIGN_FROM_WIDE}
    sites = [s.lower() for s in fn.get('callOrder', []) if s.lower() in calls]
    if len(sites) != len(calls) or len(sites) != len(printed):
        return source
    import capstone
    from capstone import x86
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    changes = []
    for match, site in zip(printed, sites):
        call = calls[site]
        slot, depth = call.get('ecxStack'), call.get('depth')
        if not isinstance(slot, int) or not isinstance(depth, int) or slot >= 0:
            return source
        address = int(site, 16)
        # push imm32 (5) + lea ecx,[esp+disp8|disp32] (4|7) immediately before the call
        for lea_size in (4, 7):
            ins = list(decoder.disasm(rdata.bytes_at(address - 5 - lea_size, 5 + lea_size + 5),
                                      address - 5 - lea_size))
            if len(ins) == 3 and ins[2].address == address:
                break
        else:
            return source
        push, lea, op = ins
        if not (push.mnemonic == 'push' and push.operands[0].type == x86.X86_OP_IMM):
            return source
        literal = rdata.wide_string_at(push.operands[0].imm)
        if literal is None or json.loads(match[2]) != literal:
            return source
        if not (lea.mnemonic == 'lea' and lea.operands[0].reg == x86.X86_REG_ECX
                and lea.operands[1].mem.base == x86.X86_REG_ESP and not lea.operands[1].mem.index
                and lea.operands[1].mem.disp - depth == slot):
            return source
        if not (op.mnemonic == 'call' and op.operands[0].type == x86.X86_OP_IMM
                and op.operands[0].imm == ASSIGN_FROM_WIDE):
            return source
        handles = set(re.findall(r'\b[a-zA-Z]+Stack_' + format(-slot, 'x') + r'\b', source))
        if len(handles) != 1:
            return source
        changes.append((match.start(), match.end(),
                        f'{match[1]}CCharString::CCharString((CCharString *)&{handles.pop()},{match[2]},-1);'))
    for a, b, text in reversed(changes):
        source = source[:a] + text + source[b:]
    return source
