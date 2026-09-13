"""Recover bounded integer byte slices and explicit Ghidra unsigned narrowing."""
import re


_UINT3 = re.compile(r'(?P<protected>"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'|/\*[\s\S]*?\*/|//[^\n]*)|'
                    r'\(uint3\)\s*(?P<value>[A-Za-z_]\w*|-?0x[0-9a-fA-F]+|-?\d+)(?![\w.\[]|\s*(?:\(|->|::|\+\+|--))')


def fold_unsigned_three_byte_casts(source):
    """uint3 retains exactly 24 low bits; accept only scalar atoms, not calls."""
    return _UINT3.sub(lambda m: m[0] if m['protected'] is not None else
                     '(' + m['value'] + ' & 0xffffff)', source)

PATTERN = re.compile(r'(?P<quoted>"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\')|'
                     r'(?<![\w.])(?P<value>-?(?:0x[0-9a-fA-F]+|\d+))\._(?P<offset>\d+)_(?P<size>\d+)_')


def fold_literal_slices(source):
    def replace(match):
        if match['quoted']:
            return match[0]
        offset, size = int(match['offset']), int(match['size'])
        if size < 1 or offset + size > 8:
            return match[0]
        value = int(match['value'], 16 if '0x' in match['value'] else 10)
        if not -(1 << 63) <= value < (1 << 64):
            return match[0]
        return str((value >> (offset * 8)) & ((1 << (size * 8)) - 1))
    return PATTERN.sub(replace, source)
