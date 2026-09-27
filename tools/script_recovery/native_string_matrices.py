"""Typed two-dimensional string addresses become named state values."""
import re


def lower_string_matrix(text, array, base, tag):
    """Preserve static addresses and reject unsupported pointer expressions.

    Both scales come from the reviewed descriptor: four-byte CCharString values
    inside rows whose stride is four times the declared number of columns.
    """
    dimensions = array.get('dimensions', [])
    if len(dimensions) != 2 or array.get('element') not in ('CCharString', 'CWideString'):
        return text
    rows, columns = dimensions
    if array['count'] != rows or array['stride'] != columns * 4:
        return text
    if array['members'] != {column * 4: (str(column), 'String') for column in range(columns)}:
        return text
    number = lambda value: '(?:' + hex(value) + '|' + str(value) + ')'
    # Class/parent fields can be selectors before ordinary field lifting runs.
    scalar = r'(?:\*\(int \*\)\((?:this|\*\(int \*\)\(this \+ 0x14\)) \+ 0x[0-9a-f]+\)|(?:QUEST|ENTITY)STATE_GetInt\("[^"]*"\)|[A-Za-z_]\w*)'
    key = lambda row, col: f'{tag}STATE_GetString(__key("{array["name"]}_" .. {row} .. "_" .. {col}))'
    cast = r'\(' + array['element'] + r' \*\)\('
    row = r'(?P<row>' + scalar + ')'
    col = r'(?P<col>' + scalar + ')'
    # Independently indexed rows/columns, as in Arena.CrowdChecker.
    expression = (base + r' \+ ' + col + r' \* (?:4|0x4) \+ ' + row + r' \* ' +
                  number(array['stride']) + r' \+ ' + number(array['base']))
    text = re.sub(cast + expression + r'\)', lambda m: key(m['row'], m['col']), text)
    # A fixed column selected from a dynamic row, e.g. reaction 3 (cheer).
    for column in range(columns):
        expression = (base + r' \+ ' + number(array['base'] + 4 * column) + r' \+ ' +
                      row + r' \* ' + number(array['stride']))
        text = re.sub(cast + expression + r'\)', lambda m, c=column: key(m['row'], str(c)), text)
    return text
