-- Native DBACA1 onward, entered only after the hit-message predicate is true.
local function handleTheresaHit(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetVillagerHeroAllies(me)
    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
    return playTheresaHitResponse(quest, me, resources, control)
end

return handleTheresaHit
