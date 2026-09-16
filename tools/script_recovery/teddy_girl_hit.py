"""Native hit consequence order and retained control/movie continuation."""
from tools.script_recovery.teddy_girl_health import prove
SOURCE='''function TeddyGirlHit(quest, resources, me, control, state, addBadDeed)
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    state:SetStateBool("HeroHitMe", true)
    addBadDeed(2)
    if not TeddyGirlAcquire(quest, resources, me, control) then return false end
    return TeddyGirlWithMovie(quest, resources, function()
        return TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_DONT_HIT")
    end)
end
'''

def recover(data=None):
    w=prove(data)
    return SOURCE,{'nativeMainSha256':w['mainSha256'],'start':0xdb0273,'end':0xdb042b,
        'movieOffset':184,'badDeed':2,'limit':'Outer callback-error cleanup and native AddBadDeed implementation remain separate boundaries.'}
