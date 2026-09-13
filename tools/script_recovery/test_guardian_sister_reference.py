"""Guardian Sister reference/adapter versus native root and callback executions."""
import json
from pathlib import Path
import unittest
from lupa import LuaRuntime
ROOT=Path(__file__).resolve().parents[2]
PORT=ROOT/'refs/script_recovery/guardian_sister_info'
FACADE='''function(case)
    local q={events={},piece=case.initial,queries=0,polls=0,storage=case.stored}
    local function emit(...) q.events[#q.events+1]={...} end
    function q:WithRetailResources(f) f({Frame=function() emit('frame') end}) end
    function q:GetStateBool(key) assert(key=='GuardianSpokeToHero');return self.piece end
    function q:SetStateBool(key,v)
        assert(key=='GuardianSpokeToHero');self.piece=v
        if case.mode=='root' then emit('state',v) end
    end
    function q:AddEntityBinding(name,path,flags)
        assert(path=='GuardianSisterInfo/Entities/MazeAtTavern');emit('bind',name,flags)
    end
    function q:FinalizeEntityBindings() emit('finalize') end
    function q:GetActiveQuestName() emit('active');return 'QS_GuardianSisterInfo' end
    function q:SetQuestCardObjective(...) emit('objective',...) end
    function q:IsRegionLoaded(name)
        self.polls=self.polls+1;local v=case.regions[self.polls];assert(v~=nil)
        emit('region',name,v~=0);return v~=0
    end
    function q:IsActiveThreadTerminating()
        self.queries=self.queries+1;local v=case.stop_query>0 and self.queries>=case.stop_query
        emit('query',v);return v
    end
    function q:SetTimeOfDay(value) emit('time',value) end
    function q:DeactivateQuestLater(...) emit('deactivate',...) end
    function q:PersistTransferBool(context,key,before,default)
        assert(context==123)
        local after=before
        if case.reading then
            after=self.storage[key];if after==nil then after=default end
        else self.storage[key]=before end
        emit('persist',key,before,default,after);return after
    end
    return q
end'''

class GuardianSisterReferenceTests(unittest.TestCase):
    def test_native_lifecycle(self):
        cases=json.loads((PORT/'runtime_evidence/native-root-lifecycle-20260912.json').read_text())['cases']
        self.assertEqual(len(cases),19)
        for case in cases:
            with self.subTest(case=case['name']):
                lua=LuaRuntime(unpack_returned_tuples=True)
                Q=lua.execute((PORT/'reference/quest.lua').read_text())
                A=lua.execute((PORT/'reference/runtime_adapter.lua').read_text())
                q=lua.eval(FACADE)(lua.table_from(case,recursive=True));h=A.new(q)
                if case['mode']=='root':Q.init(h);A.run(Q,'main',q)
                elif case['mode']=='predicate':Q.predicate_fail(h)
                elif case['mode']=='persist':Q.persist(h,123)
                else:self.assertEqual(dict(Q.entity_init().items()),case['entity_fields'])
                self.assertEqual([list(row.values()) for row in q.events.values()],case['adapter_events'])
                self.assertEqual(q.piece,case['final'])
                self.assertEqual(dict(q.storage.items()),case['storage'])

if __name__=='__main__':unittest.main()
