import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import native_cleanup_regions as candidate
COMMENT='-- reviewed native interface-table staging: held = *pointer'
ASSIGN='held = nil --[[reviewed native interface-table staging]]'

def source(comment=COMMENT,assignment=ASSIGN):
    return '''function Main(log, stop)
    local held
    if stop then return end  -- TODO(native): goto LAB_00d00001
    log("continue")
    do return end
    -- LAB_00d00001: (native jump target)
    '''+comment+'\n    '+assignment+'''
    log("unpause")
    log("destroy")
    return
end
'''

@pytest.mark.parametrize('stop',[True,False])
def test_reviewed_comment_preserves_cleanup_order(stop):
    out,report=candidate.hoist_cleanup_regions(source())
    assert 'LAB_00d00001' in report
    events=[];vm=LuaRuntime();vm.execute(out);vm.globals().Main(events.append,stop)
    assert events==(['unpause','destroy'] if stop else ['continue'])

@pytest.mark.parametrize('comment',[
 '-- reviewed native interface-table staging: other = *pointer',
 '-- reviewed native interface-table staging: held = pointer',
 '-- reviewed native interface-table staging: held = *pointer extra',
 '-- reviewed native interface-pointer staging: held = *pointer',
 '-- arbitrary review comment',
 '--[[ reviewed native interface-table staging: held = *pointer ]]',
])
def test_unrecognized_or_mismatched_comment_does_not_hoist(comment):
    _,report=candidate.hoist_cleanup_regions(source(comment=comment))
    assert 'LAB_00d00001' not in report

@pytest.mark.parametrize('assignment',[
 'held = nil --[[unresolved native value]]',
 'other = nil --[[reviewed native interface-table staging]]',
 'held = false --[[reviewed native interface-table staging]]',
 'held = nil -- reviewed native interface-table staging',
 'held = nil --[[reviewed native interface-table staging]]; log("extra")',
 '-- held = nil --[[reviewed native interface-table staging]]',
])
def test_mismatched_assignment_does_not_hoist(assignment):
    _,report=candidate.hoist_cleanup_regions(source(assignment=assignment))
    assert 'LAB_00d00001' not in report

def test_opaque_longstring_cannot_supply_a_reviewed_comment():
    text=source().replace('    '+COMMENT,'    value = [=[\n    '+COMMENT).replace('    log("unpause")','    ]=]\n    log("unpause")')
    _,report=candidate.hoist_cleanup_regions(text)
    assert 'LAB_00d00001' not in report
