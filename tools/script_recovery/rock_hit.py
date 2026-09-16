"""Hit helper: ordered native float comparison and nested-filter short circuit."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

BODY='''    quest:WithCopiedThreadThing(troll, function(actor)
        while not actor:MsgIsHitOrAggressiveAbilityFrom("SCRIPT_NAME_HERO") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        if actor:GetHealth() > 0.00009999999747378752 then
            if quest:IsActiveThreadTerminating() then return end
            actor:AddHealthBarToState("RockTrollHealthBarID", {r=255,g=0,b=0,a=255}, "HUD_QUEST_ICON_ROCK_TROLL", 1)
            quest:DisplayQuestInfo(true)
        end
    end)
'''


def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('rock_hit_witness.json').read_text())
    for region in w['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:raise ValueError('Rock hit native operand changed')
    for address,value in w['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('Rock hit literal changed')
    return BODY,w
