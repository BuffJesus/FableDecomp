"""Compose the unattended-warehouse movie and its normal/cancellation exits."""
import hashlib
import json
from pathlib import Path

HELPER = '''    local function showWarehouseFailure()
        local movie = resources:StartMovie("")
        resources:Pause(true)
        quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED")
        local dismissed = quest:MsgIsGameInfoClickedPast()
        while not dismissed do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:Pause(false)
                resources:DestroyMovie(movie)
                return false
            end
            dismissed = quest:MsgIsGameInfoClickedPast()
        end
        if quest:IsActiveThreadTerminating() then
            resources:Pause(false)
            resources:DestroyMovie(movie)
            return false
        end
        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 1)
        resources:Pause(false)
        resources:DestroyMovie(movie)
        return true
    end
'''


def recover(source,data):
    w=json.loads(Path(__file__).with_name('native_barrel_failure_movie_witness.json').read_text())
    for r in w['regions']:
        raw=data.bytes_at(r['address'],r['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=r['sha256']:
            raise ValueError('Barrel failure movie native instructions changed')
    for address,value in w['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('Barrel failure movie text changed')
    if source.count(w['oldLua'])!=1:raise ValueError('Barrel failure movie source correspondence changed')
    replacement='''                            if not showWarehouseFailure() then goto LAB_00db6afd end
                            quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")
                            __native_entity_state:SetStateInt("MyPhase", 5)
                            break
'''
    return source.replace(w['oldLua'],replacement),dict(w,status='recovered',
        behavior='Dismissal awards one bad deed, unpauses, destroys movie, sets villager brain and phase5; cancellation skips deed/phase and cleans movie once.')
