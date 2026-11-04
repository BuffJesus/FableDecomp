"""Erase SingingStone Main's C++ temporary pointer release after the frame loop."""
import hashlib

BLOCK = '''    r1 = nil
    __native_condition_1 = r1 == nil
    if not __native_condition_1 then
        -- TODO(native): *xStack_24 = *xStack_24 + -1;
        -- TODO(native): __native_condition_1 = *r1 ~= 0
        __native_condition_1 = nil --[[unresolved native value]]
    end
    if __native_condition_1 then goto LAB_00ed3906 end
    r1:GetName()
    -- LAB_00ed38fe: (native jump target)
'''
DIAGNOSTICS = (
    'unlifted: *xStack_24 = *xStack_24 + -1;',
    'unlifted: __native_condition_1 = *r1 ~= 0',
    'label LAB_00ed38fe',
)


def restore(source, todo, calls, fn, row, rdata):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('SingingStone temporary cleanup: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00ED3370 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require('auStack_24._0_4_ = &PTR__scalar_deleting_destructor__01238c8c;' in native and
            'if ((uStack_1c == (int *)0x0) || (*uStack_1c = *uStack_1c + -1, *uStack_1c != 0))' in native and
            'operator_delete(uStack_1c);\r\nLAB_00ed3906:' in native and
            'uStack_1c = (int *)0x0;' in native and
            'goto LAB_00ed38fe;' in native,
            'native temporary release changed')
    require(source.count(BLOCK) == 1 and source.count('::LAB_00ed3906::') == 1 and
            source.count('goto LAB_00ed3906') >= 1,
            'lifted release shape changed')
    for marker in DIAGNOSTICS:
        require(todo.count(marker) == 1, 'diagnostic changed: ' + marker)
        todo.remove(marker)
    return source.replace(BLOCK, ''), todo, calls
