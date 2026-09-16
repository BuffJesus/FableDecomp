"""Readable first Guild entity, checked against its original x86 control flow."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.lift_native_lua import RData

SOURCE = '''-- Retail Guild race marker. ReachedPlatform is parent quest state.
function Init(quest, me)
end

function Main(quest, me)
    while not quest:IsActiveThreadTerminating() do
        local hero = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(hero, me, 3.0) then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("ReachedPlatform", true)
            quest:MiniMapRemoveMarker(me)
        end
        quest:NewScriptFrame(me)
    end
end
'''


def verify(data=None):
    data = data or RData()
    witness = json.loads(Path(__file__).with_name('guild_race_marker_witness.json').read_text())
    for region in witness['regions']:
        if hashlib.sha256(data.bytes_at(region['address'], region['size'])).hexdigest() != region['sha256']:
            raise ValueError('Guild race marker native evidence changed')
    return witness


def build():
    verify()
    root = Path(__file__).resolve().parents[2] / 'refs/script_recovery/lifted/GuildTraining/readable'
    entity = root / 'FSE/GuildTraining/Entities/RaceMarker.lua'
    entity.parent.mkdir(parents=True, exist_ok=True)
    entity.write_text(SOURCE, encoding='utf-8')
    (root / 'FSE/quests.lua').write_text('-- Partial offline recovery; no quest registration.\nQuests = {}\n', encoding='utf-8')
    return entity


if __name__ == '__main__':
    print(build())
