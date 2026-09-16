"""Exact Init stores and GivenTeddy ordering; host integration remains staged."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE='''function TeddyGirlInitialize(quest, resources, me, state)
    state:SetStateBool("DoneIntro", false)
    state:SetStateBool("FoundTeddy", false)
    state:SetStateBool("SpokeAboutFindingTeddy", false)
    state:SetStateBool("HeroHitMe", false)
    resources:InitializeTeddyGirlActor(me)
end

function TeddyGirlGiven(quest, resources, me, state, addGoodDeed)
    resources:TakeTeddyFromHero()
    state:SetStateBool("FoundTeddy", true)
    addGoodDeed()
    resources:ClearRawInformation(me)
    quest:SetMasterGameState("TeddySolution", "B")
end
'''

def prove(data=None):
    data=data or RData()
    w=json.loads(Path(__file__).with_name('teddy_girl_lifecycle_witness.json').read_text())
    for row in w['functions']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:
            raise ValueError('TeddyGirl lifecycle bytes changed')
    for address,value in w['literals'].items():
        if data.string_at(int(address))!=value:raise ValueError('TeddyGirl lifecycle literal changed')
    return w
