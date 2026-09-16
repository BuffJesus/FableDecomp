"""Exhumation map/movie continuation using existing explicit resource APIs."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE='''-- Uses the outer self resource; Hero phase releases Hero after movie/map cleanup.
function WithRockTrollExhumationPhase(quest, me, resources, seh_me, continuation)
    if not quest:GetStateBool("PlayedExhumeCutScene") then
        local completed = false
        WithRockTrollHeroPhase(quest, resources, function(seh_hero)
            local actors = resources:NewActorMap()
            resources:SetActor(actors, "HERO", seh_hero)
            resources:SetActor(actors, "TROLL", seh_me)
            local movie = resources:StartMovie("")
            resources:Pause(true)
            resources:RunMacro("CS_ROCKTROLL_EXHUME", actors, false, true)
            quest:CreateRetainedThingThread("WatchForRockTrollHit", me, false, "")
            quest:SetStateBool("PlayedExhumeCutScene", true)
            resources:Pause(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actors)
            completed = true
        end)
        if not completed then return end
    end
    continuation()
end
'''


def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('rock_exhumation_witness.json').read_text())
    for region in w['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:raise ValueError('Rock exhumation native operand changed')
    for address,value in w['strings'].items():
        actual='' if data.bytes_at(int(address,16),1)==b'\0' else data.string_at(int(address,16))
        if actual!=value:raise ValueError('Rock exhumation literal changed')
    return SOURCE,w
