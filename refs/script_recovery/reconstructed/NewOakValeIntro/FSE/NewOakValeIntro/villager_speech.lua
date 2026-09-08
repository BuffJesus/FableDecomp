-- NewOakValeIntro/villager_speech.lua
--
-- The eight villager reaction text-key lists that retail CQ_NewOakValeIntroScript::Init @0x00DAADD0
-- pushes into its VillagerSpeech{Male,Female}{Good,Bad,Both,Neither} vectors (PDB names). Retail keeps
-- them on the quest object; NOVI_Villager picks one at random through GetVillagerSpeechIndex, avoiding
-- the previous index stored in `lastVillagerSpeechIdx`. ForgeFSE entity scripts cannot read a quest
-- vector, so the lists live here as shared data (inference: data placement only, contents are exact).
--
-- Evidence: native-decompile (Init string pushes, retail vector offsets 0x9c..0xf0).

local function keys(prefix, suffixes)
    local list = {}
    for _, suffix in ipairs(suffixes) do
        list[#list + 1] = prefix .. suffix
    end
    return list
end

local SIX  = { "_10", "_20", "_30", "_40", "_50", "_60" }
local FOUR = { "_10", "_20", "_30", "_40" }
local FIVE = { "_10", "_20", "_30", "_40", "_50" }

local Speech = {
    MaleGood      = keys("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE",   SIX),   -- retail vector @0x9c
    MaleBad       = keys("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE",    SIX),   -- @0xa8
    MaleBoth      = keys("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE",   FOUR),  -- @0xb4
    MaleNeither   = keys("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE",     FIVE),  -- @0xc0
    FemaleGood    = keys("TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE", SIX),   -- @0xcc
    FemaleBad     = keys("TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE",  SIX),   -- @0xd8
    FemaleBoth    = keys("TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE", FOUR),  -- @0xe4
    FemaleNeither = keys("TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE",   FIVE),  -- @0xf0
}

-- Retail Init push order (for trace comparison): FemaleBad, MaleBad, FemaleBoth, MaleBoth,
-- FemaleGood, MaleGood, FemaleNeither, MaleNeither.
Speech.INIT_ORDER = {
    "FemaleBad", "MaleBad", "FemaleBoth", "MaleBoth", "FemaleGood", "MaleGood", "FemaleNeither", "MaleNeither",
}

return Speech
