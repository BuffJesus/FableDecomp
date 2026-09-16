"""Analyze BookTrader temporaries without treating its checked cleanup as opaque."""
from tools.script_recovery.readable_lua import readable_source


HELPER = '''    local function release_book()
        assert(book_movie == nil and book_thing == nil, "BookTrader cleanup order changed")
        resources:ReleaseResource(book_resource)
        book_resource = nil
    end
'''
MARKER = '    -- reviewed BookTrader cleanup helper restored after local analysis\n'


def readable_book_source(source):
    # This exact helper captures only stable, explicitly named resource handles.
    # It cannot observe any of the generated locals we split or inline. Preserve
    # its declaration position, body and source line count across analysis.
    if source.count(HELPER) != 1 or MARKER in source:
        raise ValueError('BookTrader cleanup helper capture review changed')
    placeholder = MARKER + '\n' * (HELPER.count('\n')-1)
    staged = source.replace(HELPER, placeholder)
    output, mapping = readable_source(staged, inline_literals=True)
    if output.count(placeholder) != 1:
        raise ValueError('BookTrader cleanup helper analysis anchor changed')
    output = output.replace(placeholder, HELPER)
    return output, mapping
