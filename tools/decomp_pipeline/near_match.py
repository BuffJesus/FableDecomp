"""VC7.1 instruction similarity; a score is not evidence of equal behavior.

Keep opcode, operand width/kind, constants and non-stack member offsets. Mask
register allocation, branch destinations, stack displacements and relocations.
SequenceMatcher is ordered (autojunk disabled); no instruction sorting is done.
Incomplete disassembly fails closed. Embedded data needs a separate code boundary.
"""
from dataclasses import asdict, dataclass
from difflib import SequenceMatcher
from functools import lru_cache

import capstone as cs
from capstone import x86_const as x86


@lru_cache(maxsize=4)
def image_relocations(path):
    """Absolute relocation VAs from the local retail PE; no offset guessing."""
    import pefile
    with pefile.PE(str(path), fast_load=True) as pe:
        pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_BASERELOC']])
        return frozenset(pe.OPTIONAL_HEADER.ImageBase + entry.rva
                         for block in getattr(pe, 'DIRECTORY_ENTRY_BASERELOC', ())
                         for entry in block.entries if entry.type == 3)


def retail_relocations(address, size):
    from pe_oracle import EXE
    if not EXE.exists():
        return ()  # Conservative: unmatched pointer immediates lower the score.
    start = int(address, 16)
    return tuple(va - start for va in image_relocations(str(EXE)) if start <= va < start + size)


@dataclass(frozen=True)
class Similarity:
    retail_instructions: int
    built_instructions: int
    instruction_delta: float
    opcode_similarity: float
    complete_decode: bool
    near_match: bool

    def to_dict(self):
        return asdict(self)


def normalized_instructions(code, relocations=()):
    decoder = cs.Cs(cs.CS_ARCH_X86, cs.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, 0))
    complete = bool(code) and sum(i.size for i in instructions) == len(code)
    # /Oy permits EBP to hold an ordinary object pointer. Only mask its
    # displacements when an actual frame setup establishes that meaning.
    frame_setup = [i for i in instructions if i.mnemonic == 'mov' and
                   len(i.operands) == 2 and
                   i.operands[0].type == x86.X86_OP_REG and
                   i.operands[0].reg == x86.X86_REG_EBP and
                   i.operands[1].type == x86.X86_OP_REG and
                   i.operands[1].reg == x86.X86_REG_ESP]
    ebp_frame = bool(frame_setup) and all(
        i in frame_setup or i.mnemonic in ('pop', 'leave') or
        x86.X86_REG_EBP not in i.regs_access()[1] for i in instructions)
    if ebp_frame:
        ebp_frame = all(i.address >= frame_setup[0].address for i in instructions
                        if any(op.type == x86.X86_OP_MEM and op.mem.base == x86.X86_REG_EBP
                               for op in i.operands))
    relocs = set(relocations)
    tokens = []
    for ins in instructions:
        operands = []
        control = ins.group(cs.CS_GRP_JUMP) or ins.group(cs.CS_GRP_CALL)
        for op in ins.operands:
            if op.type == x86.X86_OP_REG:
                name = ins.reg_name(op.reg)
                # Keep register families (x87/SIMD/segment vs general purpose).
                family = ('x87' if name.startswith('st') else
                          'xmm' if name.startswith('xmm') else
                          'mm' if name.startswith('mm') else
                          name if name in ('cs', 'ds', 'es', 'fs', 'gs', 'ss') else 'gpr')
                operands.append(('reg', op.size, family))
            elif op.type == x86.X86_OP_IMM:
                relocated = ins.address + ins.imm_offset in relocs
                operands.append(('imm', op.size, '*' if control or relocated else op.imm))
            elif op.type == x86.X86_OP_MEM:
                mem = op.mem
                stack = mem.base == x86.X86_REG_ESP or (ebp_frame and mem.base == x86.X86_REG_EBP)
                relocated = ins.address + ins.disp_offset in relocs
                # Absolute memory references are linker-selected globals/imports.
                absolute = not mem.base and not mem.index
                operands.append(('mem', op.size, ins.reg_name(mem.segment),
                                 'stack' if stack else bool(mem.base), bool(mem.index),
                                 mem.scale, '*' if stack or relocated or absolute else mem.disp))
        tokens.append((ins.mnemonic, tuple(operands)))
    return tokens, complete


def compare(built, retail, built_relocations=(), retail_relocations=(),
            max_instruction_delta=0.10, min_similarity=0.90):
    b, bc = normalized_instructions(built, built_relocations)
    r, rc = normalized_instructions(retail, retail_relocations)
    delta = abs(len(b) - len(r)) / len(r) if r else float('inf')
    score = SequenceMatcher(None, r, b, autojunk=False).ratio() if r and b else 0.0
    complete = bc and rc
    return Similarity(len(r), len(b), delta, score, complete,
                      complete and delta <= max_instruction_delta and score >= min_similarity)
