-- Readable native conversion: Q_OrchardFarmRaidGood. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    RansomVictimNoDamageBoastReward = 384,  -- 900
    RansomVictimNoWeaponsBoastCost = 388,  -- 200
    RansomVictimNoWeaponsBoastReward = 392,  -- 600
    TraderConflictEvilNakedBoastCost = 396,  -- 100
    TraderConflictEvilNakedBoastReward = 400,  -- 400
    TraderConflictEvilNoDamageBoastCost = 404,  -- 1000
    OFG_NoCratesStolenBoastCost = 3464,  -- 80
    OFG_NoCratesStolenBoastReward = 3468,  -- 180
    OFG_NoGuardsDieBoastCost = 3472,  -- 100
    OFG_NoGuardsDieBoastReward = 3476,  -- 250
}

-- Q_OrchardFarmRaidGood.Main (retail 0x00dd2390)
function Main(quest)
end

-- Q_OrchardFarmRaidGood.Init (retail 0x00dd23a0)
function Init(quest)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.RansomVictimNoDamageBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.RansomVictimNoWeaponsBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.RansomVictimNoWeaponsBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictEvilNakedBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictEvilNakedBoastReward), quest:ReadGlobalGameData(SCRIPT_DEF.TraderConflictEvilNoDamageBoastCost), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOCRATESSTOLEN", 17, quest:ReadGlobalGameData(SCRIPT_DEF.OFG_NoCratesStolenBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.OFG_NoCratesStolenBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTGUARDS", 19, quest:ReadGlobalGameData(SCRIPT_DEF.OFG_NoGuardsDieBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.OFG_NoGuardsDieBoastReward), false, "", 0)
    quest:ActivateQuest("Q_OrchardFarmRaid")
end

