"""Readable equivalent of retail GFCharStringToInt, 0x0099E7F0.

The retail loop scans every byte up to the first period, collects decimal digits,
records any minus sign, and returns a signed 32-bit result. It is not strtol or
Lua tonumber: empty/non-numeric text is zero, and intervening letters are skipped.
"""
import re


PARSER = '''local function {name}(text)
    local value, negative = 0, false
    for position = 1, #text do
        local character = text:sub(position, position)
        if character == "." then break end
        if character == "-" then
            negative = true
        elseif character >= "0" and character <= "9" then
            value = (value * 10 + tonumber(character)) % 4294967296
        end
    end
    if negative then value = (-value) % 4294967296 end
    -- Match the game's signed 32-bit result, including overflow.
    if value >= 2147483648 then value = value - 4294967296 end
    return value
end
'''


def recover_integer_parser(source):
    pattern = re.compile(r'\bGFCharStringToInt\(')
    if not pattern.search(source):
        return source
    name = 'parseGameInteger'
    while re.search(r'\b' + name + r'\b', source):
        name += 'Value'
    source = pattern.sub(name + '(', source)
    helper = PARSER.format(name=name)
    if re.match(r'^(?:local )?function \w+\([^\n]*\)\n', source):
        head, _, body = source.partition('\n')
        return head + '\n' + '\n'.join('    ' + line for line in helper.splitlines()) + '\n' + body
    return helper + '\n' + source
