-- NewOakValeIntro/deeds.lua
--
-- Retail CQ_NewOakValeIntroScript::AddGoodDeed @0x00DB0660 and AddBadDeed(EBadDeeds) @0x00DAEA70.
-- Both are quest methods that entity scripts call synchronously on their own thread (the caller waits
-- for the "did a deed" info box). They are therefore shared code, not a quest thread.
--
-- Morality amount: retail reads a float from the game-data block (`DAT_0143e90c + 0xd64`) and passes
-- +value / -value to GiveHeroMorality. That block is not reachable from Lua; the value is declared as
-- an unknown constant and every trace shows the gap instead of a guessed number.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")

local Deeds = {}

-- EBadDeeds indices observed at retail call sites. Enum member names are not in the PDB tables.
Deeds.BAD_DEED_BARREL_BROKEN = 0   -- WatchBarrels @0x00DBE890 (first barrel smashed)
Deeds.BAD_DEED_1 = 1
Deeds.BAD_DEED_2 = 2
Deeds.BAD_DEED_TEDDY_TO_BULLY = 3 -- CNOVI_Bully::GivenTeddy @0x00DBCD00
Deeds.BAD_DEED_4 = 4

Deeds.TEXT_FIRST_GOOD  = "TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED"
Deeds.TEXT_GOOD        = "TEXT_QST_048_SCRMSG_DID_GOOD_DEED"
Deeds.TEXT_FIRST_BAD   = "TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED"
Deeds.TEXT_BAD         = "TEXT_QST_048_SCRMSG_DID_BAD_DEED"
Deeds.TEXT_LOG_BASICS  = "TEXT_QST_LOG_BASICS_MAP"
Deeds.TEXT_OBJECTIVE_02 = "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02"

-- Gold threshold shared with WatchForGotGold / ManageQuestCoreMarkers (GetHeroGold() < 3).
Deeds.GOLD_FOR_SWEETS = 3

-- The first deed of either kind also writes the "basics" logbook page. Retail calls the sibling of the
-- AddLogbookStoryEntry helper (0x00CBE9EE, string overload) — labelled inference, see doc.
local function log_basics(quest)
    quest:AddLogbookStoryEntryString(Deeds.TEXT_LOG_BASICS)
end

local function morality_amount(quest)
    -- retail: *(float*)(DAT_0143e90c + 0xd64)
    return NOVI.unsupported_value("UNKNOWN_FLOAT", "GameData+0xd64 (deed morality)")
end

function Deeds.add_good(quest, me)
    local good = F.increment(quest, F.GoodDeedsPerformed)
    local amount = morality_amount(quest)
    if amount ~= nil then
        quest:GiveHeroMorality(amount)
    else
        NOVI.unsupported(quest, "GiveHeroMorality(+deed)", {})
    end

    if good == 1 and F.get_int(quest, F.BadDeedsPerformed) == 0 then
        if not NOVI.game_info_blocking(quest, me, Deeds.TEXT_FIRST_GOOD) then return false end
        log_basics(quest)
    else
        if not NOVI.game_info_blocking(quest, me, Deeds.TEXT_GOOD) then return false end
    end

    -- retail: still short of sweets money and the trader has not been paid -> point at objective 2
    if quest:GetHeroGold() < Deeds.GOLD_FOR_SWEETS and not F.get(quest, F.GivenSweets) then
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), Deeds.TEXT_OBJECTIVE_02, "", "")
    end
    quest:UpdateQuestInfoCounter(F.get_int(quest, F.GUIGoodDeedCounter), good, -1)
    return true
end

function Deeds.add_bad(quest, me, which)
    local bad = F.increment(quest, F.BadDeedsPerformed)
    local amount = morality_amount(quest)
    if amount ~= nil then
        quest:GiveHeroMorality(-amount)
    else
        NOVI.unsupported(quest, "GiveHeroMorality(-deed)", {})
    end
    local flag = F.WhichBadDeedsPerformed[which]

    if bad == 1 and F.get_int(quest, F.GoodDeedsPerformed) == 0 then
        if not NOVI.game_info_blocking(quest, me, Deeds.TEXT_FIRST_BAD) then return false end
        log_basics(quest)
        F.set(quest, flag, true)
        return true
    end

    -- repeat offences of the same kind are silent; a new kind shows the generic message once
    if not F.get(quest, flag) then
        if not NOVI.game_info_blocking(quest, me, Deeds.TEXT_BAD) then return false end
    end
    F.set(quest, flag, true)
    return true
end

return Deeds
