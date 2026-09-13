"""Reviewed reference watcher versus native callback cases; no entity port claim."""
import json
from pathlib import Path
import unittest
from lupa import LuaRuntime
ROOT=Path(__file__).resolve().parents[2]
PORT=ROOT/'refs/script_recovery/guardian_trophy_dealer_info'


class GuardianTrophyWaiterTests(unittest.TestCase):
    def test_native_waiter_cases(self):
        native=json.loads((PORT/'runtime_evidence/native-waiter-cases-20260912.json').read_text())
        for case in native['cases']:
            with self.subTest(case=case['name']):
                lua=LuaRuntime(unpack_returned_tuples=True)
                q=lua.execute((PORT/'reference/quest.lua').read_text())
                h=lua.eval('''function(piece,set_frame,alive,stop_query,missing)
                    local h={events={},piece=piece,frames=0,queries=0,polls=0}
                    local function emit(e) h.events[#h.events+1]=e end
                    function h:piece_over() return self.piece end
                    function h:frame()
                        emit('frame');self.frames=self.frames+1
                        if set_frame>0 and self.frames>=set_frame then self.piece=true end
                    end
                    function h:terminating()
                        emit('query');self.queries=self.queries+1
                        return stop_query>0 and self.queries>=stop_query
                    end
                    function h:lookup(name) assert(name=='GTDI_Maze');emit('lookup');return {missing=missing} end
                    function h:alive(thing)
                        if thing.missing then return false end
                        self.polls=self.polls+1;local value=alive[self.polls]
                        assert(value~=nil);emit('alive_'..(value and '1' or '0'));return value
                    end
                    function h:active() emit('active');return 'QS_GuardianTrophyDealerInfo' end
                    function h:deactivate(name,delay) assert(name=='QS_GuardianTrophyDealerInfo' and delay==0);emit('deactivate') end
                    function h:release_lookup() emit('release_lookup') end
                    return h
                end''')(case['piece'],case['set_frame'],lua.table_from(case['alive']),case['stop_query'],case['missing'])
                q.waiter(h)
                self.assertEqual(list(h.events.values()),case['events'])


if __name__=='__main__':unittest.main()
