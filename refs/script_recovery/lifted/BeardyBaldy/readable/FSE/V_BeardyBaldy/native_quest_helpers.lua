-- Generated from the same native helper bodies as the quest draft.
local IsHeroWearingAnyOddHairdo, IsHeroWearingAnyTash, ResetRandomSpeechTime
function IsHeroWearingAnyOddHairdo(quest, me)
    return CONCAT31(int3(extraout_EAX >> 8),1)
end

function IsHeroWearingAnyTash(quest, me)
    return CONCAT31(int3(extraout_EAX >> 8),1)
end

-- E53C70: bsim names this body NScript::CV_AmbushScamScript::ResetRandomSpeechTime (a homologous script member); no PDB name
function ResetRandomSpeechTime(quest, me)
    local scratchValue = math.random(0, 32767)
    quest:SetTimer(quest:GetStateInt("RandomSpeechTimer"), quest:ReadGlobalGameData(1172) - scratchValue % quest:ReadGlobalGameData(1176))
end

return {IsHeroWearingAnyOddHairdo = IsHeroWearingAnyOddHairdo, IsHeroWearingAnyTash = IsHeroWearingAnyTash, ResetRandomSpeechTime = ResetRandomSpeechTime}
