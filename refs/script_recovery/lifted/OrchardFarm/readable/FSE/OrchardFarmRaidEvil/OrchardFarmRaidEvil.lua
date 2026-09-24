-- Readable native conversion: Q_OrchardFarmRaidEvil. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    OFEvilNakedBoastCost = 360,  -- 80
    OFEvilNakedBoastReward = 364,  -- 160
    OFEvilNoDamageBoastCost = 368,  -- 100
    OFEvilNoDamageBoastReward = 372,  -- 400
    OFEvilNoWeaponsBoastCost = 376,  -- 100
    OFEvilNoWeaponsBoastReward = 380,  -- 300
    OFE_NoHealthPotionBoastCost = 3448,  -- 80
    OFE_NoHealthPotionBoastReward = 3452,  -- 160
    OFE_NoBanditsDieBoastCost = 3456,  -- 100
    OFE_NoBanditsDieBoastReward = 3460,  -- 275
}

-- Q_OrchardFarmRaidEvil.Main (retail 0x00dd2010)
function Main(quest)
end

-- Q_OrchardFarmRaidEvil.Init (retail 0x00dd2020)
function Init(quest)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.OFEvilNakedBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.OFEvilNakedBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.OFEvilNoDamageBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.OFEvilNoDamageBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(SCRIPT_DEF.OFEvilNoWeaponsBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.OFEvilNoWeaponsBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_USENOHEALTHPOTIONS", 8, quest:ReadGlobalGameData(SCRIPT_DEF.OFE_NoHealthPotionBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.OFE_NoHealthPotionBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS", 18, quest:ReadGlobalGameData(SCRIPT_DEF.OFE_NoBanditsDieBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.OFE_NoBanditsDieBoastReward), true, "", 0)
    quest:ActivateQuest("Q_OrchardFarmRaid")
end

