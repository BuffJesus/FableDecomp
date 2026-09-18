import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_cleanup_regions import hoist_cleanup_regions


def calls_of(source, *args):
    calls = []
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.execute(source)
    lua.globals().Main(lambda s: calls.append(s), *args)
    return calls


class CleanupRegionTests(unittest.TestCase):
    def test_region_that_falls_out_of_an_if_block_is_hoisted(self):
        # the label sits inside an `if`; the epilogue continues after that block's `end`
        source = '''function Main(log, stop)
    local held
    held = 1
    if stop then
        if true then return end  -- TODO(native): goto LAB_00d00001
        log("work")
    else
        -- LAB_00d00001: (native jump target)
        log("release " .. held)
    end
    return
end
'''
        out, report = hoist_cleanup_regions(source)
        self.assertIn('LAB_00d00001', report)
        self.assertIn('__cleanup_LAB_00d00001(); return', out)
        self.assertEqual(calls_of(out, True), ['release 1'])

    def test_terminal_jump_to_an_emitted_label_takes_that_label_s_tail(self):
        source = '''function Main(log, stop)
    if stop then
        if true then return end  -- TODO(native): goto LAB_00d00002
    end
    if true then
        -- LAB_00d00002: (native jump target)
        log("timer")
        goto FLOW_done
    end
    ::FLOW_done::
    log("resource")
end
'''
        out, report = hoist_cleanup_regions(source)
        self.assertEqual(report['LAB_00d00002'], ['log("timer")', 'log("resource")'])
        self.assertEqual(calls_of(out, True), ['timer', 'resource'])

    def test_a_region_that_delegates_does_not_repeat_the_shared_tail(self):
        source = '''function Main(log, stop)
    if stop then
        if true then return end  -- TODO(native): goto LAB_00d00003
    end
    if true then
        -- LAB_00d00003: (native jump target)
        log("outer")
        -- LAB_00d00004: (native jump target)
        log("inner")
        goto FLOW_done
    end
    ::FLOW_done::
    log("shared")
end
'''
        out, report = hoist_cleanup_regions(source)
        self.assertEqual(report['LAB_00d00003'], ['log("outer")', 'log("inner")', 'log("shared")'])
        self.assertEqual(calls_of(out, True), ['outer', 'inner', 'shared'])

    def test_a_label_inside_a_loop_body_is_not_a_region(self):
        # falling out of a loop body is the next iteration, not the epilogue
        source = '''function Main(log, stop)
    while stop do
        -- LAB_00d00005: (native jump target)
        log("body")
    end
    log("after")
end
'''
        out, report = hoist_cleanup_regions(source)
        self.assertEqual(report, {})
        self.assertEqual(out, source)


if __name__ == '__main__':
    unittest.main()
