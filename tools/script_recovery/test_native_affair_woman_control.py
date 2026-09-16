import unittest
from pathlib import Path
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.native_affair_woman_control import recover


class AffairWomanControlTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.before = (ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairWoman.lua').read_text()
        cls.after, cls.report = recover(cls.before)

    def test_termination_booleans_replace_unresolved_al(self):
        self.assertNotIn('extraout_AL_02', self.after)
        self.assertNotIn('extraout_AL_20', self.after)
        self.assertEqual(self.after.count('cVar6 = not alive'), 2)
        self.assertIn('while not cVar6 do', self.after)

    def test_departure_uses_current_self_position_and_removes_self_after_camera_exit(self):
        # Exercise the recovered operand region; resource reset and Thing cleanup
        # remain explicit gaps outside this test's scope.
        body = self.after[self.after.index('    ::LAB_00db28de::'):self.after.index('    ::LAB_00db2974::')]
        for visible_queries in range(3):
            for cancel_at in range(1, 7):
                lua = LuaRuntime()
                lua.globals().visible_queries = visible_queries
                lua.globals().cancel_at = cancel_at
                lua.execute('''
events = {}; local function record(s) events[#events+1]=s end
local checks,queries=0,0
me = {}; quest = {}; local lastPosition
function me:GetPos()
    queries=queries+1; record('position'..queries)
    lastPosition={x=queries,y=2,z=3}; return lastPosition
end
function quest:IsCameraPosOnScreen(position)
    assert(position==lastPosition); record('camera'..queries)
    return queries<=visible_queries
end
function quest:IsActiveThreadTerminating()
    checks=checks+1; record('check'); return checks==cancel_at
end
function quest:NewScriptFrame(actor) assert(actor==me); record('frame'); return true end
function quest:RemoveThing(actor,a,b)
    assert(actor==me and a==false and b==true); record('remove')
end
''')
                lua.execute('function Departure(quest,me)\nlocal alive,cVar6,bVar5\n'+body+'\nend')
                lua.execute('Departure(quest,me)')
                expected = ['check']
                checks = 1
                if cancel_at != checks:
                    for query in range(1, visible_queries+2):
                        expected += ['position'+str(query),'camera'+str(query)]
                        if query <= visible_queries:
                            expected += ['frame','check']; checks += 1
                            if checks == cancel_at:
                                break
                        else:
                            expected.append('check'); checks += 1
                            if checks != cancel_at:
                                expected.append('remove')
                with self.subTest(visible=visible_queries,cancel=cancel_at):
                    self.assertEqual(list(lua.globals().events.values()), expected)

    def test_changed_loop_camera_removal_or_binding_rejects(self):
        data = RData()
        for site in (0xDB203F,0xDB26ED,0xDB290D,0xDB2962,0x1260F0C+0x694):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with self.subTest(site=site),self.assertRaises(ValueError):
                recover(self.before,SimpleNamespace(bytes_at=read))
        with self.assertRaisesRegex(ValueError,'draft changed'):
            recover(self.before.replace('quest:RemoveThing(r6)','quest:RemoveThing(me)'))
