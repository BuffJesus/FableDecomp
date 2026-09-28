"""Thing methods on a resource handle go through resources:ScriptThing (retail GetScriptThing)."""
from tools.script_recovery.lift_native_lua import route_resource_thing_calls

BODY = '''    r = resources:NewResource()
    r = r
    resources:PrepareResource(r)
    if not (r ~= nil and not r:IsNull()) then
        pos = {x = 0, y = 0, z = 0}
    else
        pos = r:GetPos()
    end
    -- TODO(native): r:Keep()'''.splitlines()


def test_thing_methods_on_a_resource_handle_use_its_acquired_thing():
    out = route_resource_thing_calls(BODY)
    assert '    if not (r ~= nil and not resources:ScriptThing(r):IsNull()) then' in out
    assert '        pos = resources:ScriptThing(r):GetPos()' in out
    assert '    resources:PrepareResource(r)' in out           # resource-table calls untouched
    assert out[-1] == BODY[-1]                              # comments untouched


def test_a_name_that_also_holds_something_else_is_left_alone():
    body = BODY + ['    r = quest:GetHero()']
    assert route_resource_thing_calls(body) == body
