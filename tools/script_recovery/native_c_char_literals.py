"""Lower unambiguous low-byte C character constants without editing strings."""
import re


QUOTED = re.compile(r'''"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*' ''', re.VERBOSE)
HEX_CHAR = re.compile(r"'\\x([0-9a-fA-F]{1,2})'")


def lower_c_byte_literals(text):
    def replace(match):
        token = match[0]
        if token.startswith('"') or (match.start() and
                (text[match.start()-1].isalnum() or text[match.start()-1] == '_')):
            return token
        if token == r"'\0'":
            return '0'
        value = HEX_CHAR.fullmatch(token)
        # High bytes depend on signed-char / cast context; do not guess that
        # context. Ghidra's low-byte constants are integer values in C.
        if value and int(value[1], 16) <= 0x7f:
            return str(int(value[1], 16))
        return token
    return QUOTED.sub(replace, text)
