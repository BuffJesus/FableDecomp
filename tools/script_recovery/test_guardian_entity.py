"""Complete reference Init/Main versus native instructions and instrumented APIs."""
import hashlib,json
from pathlib import Path
import unittest
from lupa import LuaRuntime
from tools.script_recovery.test_guardian_encounter import HARNESS as ENCOUNTER
ROOT=Path(__file__).resolve().parents[2]
PORT=ROOT/'refs/script_recovery/guardian_trophy_dealer_info'

EXTEND='''function(h,polls,health,busy,angle)
    local function emit(...) h.events[#h.events+1]={...} end
    h.resources={};h.pi=0;h.row={};h.hi=0;h.bi=0
    function h:angle() emit('angle',angle);return angle end
    function h:register_alive_condition() emit('register_alive_condition') end
    function h:information(...) emit('information',...) end
    function h:killable(...) emit('killable',...) end
    function h:damageable(...) emit('damageable',...) end
    function h:pushable(...) emit('pushable',...) end
    function h:start_screen(...) emit('start_screen',...) end
    function h:add_marker(...) emit('add_marker',...) end
    function h:hit()
        self.pi=self.pi+1;self.row=polls[math.min(self.pi,#polls)] or {}
        local v=self.row.hit or false;emit('hit',v);return v
    end
    function h:special() local v=self.row.special or false;emit('special',v);return v end
    function h:heal(id) assert(id==14);local v=self.row.heal or false;emit('heal',v);return v end
    function h:used() local v=self.row.use or false;emit('use',v);return v end
    function h:face_hero(v) emit('face_hero',v) end
    function h:resource_thing(r)
        assert(r==self.resources.maze and not self.thing);self.thing={};emit('resource_thing');return self.thing
    end
    function h:health(t)
        assert(t==self.thing);self.hi=self.hi+1;local v=health[math.min(self.hi,#health)]
        assert(v~=nil);emit('health',v);return v
    end
    function h:release_thing(t) assert(t==self.thing);self.thing=nil;emit('release_thing') end
    function h:speak(r,...) assert(r==self.resources.maze and r.owned);emit('speak',...) end
    function h:busy(r)
        assert(r==self.resources.maze);self.bi=self.bi+1;local v=busy[math.min(self.bi,#busy)]
        assert(v~=nil);emit('busy',v);return v
    end
    return h
end'''


class GuardianEntityTests(unittest.TestCase):
    def test_complete_native_cases(self):
        evidence=json.loads((PORT/'runtime_evidence/native-entity-cases-20260912.json').read_text())
        replay=ROOT/'work/runtime-20260912-video-comparison/prove_guardian_entity.py'
        self.assertEqual(hashlib.sha256(replay.read_bytes()).hexdigest(),evidence['replay_sha256'])
        self.assertEqual(len(evidence['cases']),28)
        for case in evidence['cases']:
            with self.subTest(case=case['name']):
                lua=LuaRuntime(unpack_returned_tuples=True)
                q=lua.execute((PORT/'reference/quest.lua').read_text())
                h=lua.eval(ENCOUNTER)(lua.table_from(case['acquisitions']),case['stop_query'],case['cancel_macro'])
                lua.eval(EXTEND)(h,lua.table_from([lua.table_from(row) for row in case['polls']]),
                                 lua.table_from(case['health']),lua.table_from(case['busy']),case['angle'])
                q.entity_main(h,q.entity_init(h))
                self.assertEqual([list(e.values()) for e in h.events.values()],case['events'])
                self.assertEqual(h.piece,case['piece_over'])
                self.assertFalse(any(r.owned for r in h.resources.values()))
                self.assertIsNone(h.movie);self.assertIsNone(h.map);self.assertIsNone(h.thing)


if __name__=='__main__':unittest.main()
