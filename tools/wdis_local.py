import struct
import sys

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


def va_to_offset(data: bytes, va: int) -> int:
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    sections = struct.unpack_from("<H", data, pe + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe + 20)[0]
    optional = pe + 24
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    table = optional + optional_size
    rva = va - image_base
    for i in range(sections):
        entry = table + i * 40
        virtual_size, virtual_address, raw_size, raw_offset = struct.unpack_from(
            "<IIII", data, entry + 8
        )
        if virtual_address <= rva < virtual_address + max(virtual_size, raw_size):
            delta = rva - virtual_address
            if delta >= raw_size:
                raise ValueError(f"VA 0x{va:x} is in an unbacked section range")
            return raw_offset + delta
    raise ValueError(f"VA 0x{va:x} is outside all sections")


def main() -> None:
    if len(sys.argv) != 4:
        raise SystemExit("usage: wdis_local.py <exe> <va> <byte-count>")
    path, va_text, size_text = sys.argv[1:]
    va = int(va_text, 0)
    size = int(size_text, 0)
    data = open(path, "rb").read()
    offset = va_to_offset(data, va)
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    for instruction in md.disasm(data[offset : offset + size], va):
        print(
            f"{instruction.address:08x}  {instruction.bytes.hex():<20} "
            f"{instruction.mnemonic:<8} {instruction.op_str}"
        )


if __name__ == "__main__":
    main()
