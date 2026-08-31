local MY_SCRIPT_NAME = "EmptyGrave"
local EGP_JACK_BOSS_FIGHT = 1700 -- 0x6A4
function Init(quest, me)
end
function Main(quest, me)
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: EmptyGrave Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end
    if quest:GetStateBool("BookRead") then
        quest:MiniMapAddMarker(myThing, "HUD_ORB_QUEST_VIGNETTE")
    end
    local sword = quest:GetThingWithScriptName("GoodSword")
    if sword then
        quest:EntitySetInLimbo(sword, true)
    else
        quest:Log("!!! WARNING: EmptyGrave Main - could not find 'GoodSword'.")
    end
    while true do
        if me:MsgIsUsedByHero() then
            local bookRead = quest:GetStateBool("BookRead")
            local swordTaken = quest:GetStateBool("SwordTaken")
            if bookRead and not swordTaken then
                local postSavePos = quest:GetMasterGameState("PostSavePosition") or 0
                local jackResult = quest:GetMasterGameState("JackBossBattleResult") or 0
                if postSavePos > EGP_JACK_BOSS_FIGHT and jackResult == 2 then
                    local hero = quest:GetHero()
                    if not hero then
                        quest:Log("!!! ERROR: EmptyGrave - could not get Hero.")
                        return
                    end
                    if sword then
                        quest:EntitySetInLimbo(sword, false)
                        quest:EntitySetAlpha(sword, 0.0)
                    end
                    local isHeroGood = quest:GetMasterGameState("JackBossBattleHeroGoodAtEnd")
                    local cutsceneName = isHeroGood and "CS_GET_SWORD_OF_AEONS_SAINT" or "CS_GET_SWORD_OF_AEONS_REDEEM"
					quest:Log("EmptyGrave: Triggering cutscene -> " .. cutsceneName)
					quest:PlayCutscene(cutsceneName, { HERO = hero }, { UNLIMBO = false })
                    quest:MiniMapRemoveMarker(myThing)
                    quest:SetStateBool("SwordTaken", true)
					local activeQuestName = quest:GetActiveQuestName()
                    quest:DeactivateQuestLater(activeQuestName, 0)
                    return
                end
            end
        end
        if not quest:NewScriptFrame(me) then
            return
        end
    end
end