local MY_SCRIPT_NAME = "GhostFisherman"

local haveTalked = false
local ghostGoing = false

function Init(quest, me)
    haveTalked = false
    ghostGoing = false
    quest:EntitySetAsAbleToWalkThroughSolidObjects(me, true)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsThingForcePushable(me, false)
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetTargetingType(me, 2) -- TARGETING_TALKABLE = 2
    quest:EntitySetAsOpinionSourceByString(me, "OPINION_SOURCE_INANIMATE_EVIL_HIGH")
    quest:SetThingHasInformation(me, false, true, false)

    if not quest:GetStateBool("Helping") then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_VIGNETTE")
    end
end

function OnPersist(quest, me, context)
    haveTalked = quest:PersistTransferBool(context, "HaveTalked", haveTalked)
end

function Main(quest, me)
    quest:Log("GhostFisherman: Main() started.")

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: GhostFisherman Main - could not get Hero.")
        return
    end

    local bootyMarker = quest:GetThingWithScriptName("HiddenBooty")

    me:AcquireControl()
    quest:EntitySetTargetingType(me, 2)
    quest:SetThingHasInformation(me, false, true, false)

    local ghostTimer = quest:RegisterTimer()
    quest:SetTimer(ghostTimer, 0)

    while not quest:GetStateBool("WifeAttacked") do
        if ghostGoing then
            break
        end

        -- Periodic ambient cry to Hero (does not trigger cinematic movie mode)
        if not haveTalked then
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(ghostTimer) <= 0 then
                local convoID = quest:StartAmbientConversation(me, hero, false, false)
                quest:AddLineToConversation(convoID, "TEXT_QST_032_GHOST_FISHERMAN_HELP", me, hero, false)
                quest:SetTimer(ghostTimer, 15)
            end
        end

        -- Check if booty was already dug up before agreeing to help
        if not quest:GetStateBool("Helping") and (bootyMarker and not quest:IsDiggingSpotEnabled(bootyMarker)) then
            quest:SetStateBool("MissionAborted", true)
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
            quest:DeregisterTimer(ghostTimer)
            me:ReleaseControl()
            return
        end

        -- Talked to by Hero
        if me:IsTalkedToByHero() then
            if not haveTalked then
                haveTalked = true
                -- Release control and begin continuous cutscene session
                me:ReleaseControl()
                quest:StartCutscene({ HERO = hero, GHOST = me })
                quest:RunCutscene("CS_GHOSTFISH_FISH_INTRO_SETUP", false, false)
                quest:RunCutscene("CS_GHOSTFISH_FISH_INTRO_1", true, false)

                -- Yes/No prompt appears inside cutscene with camera holding
                local ans = quest:GiveHeroYesNoQuestion("TEXT_QST_032_GHOST_FISHERMAN_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                if ans == 1 then
                    quest:SetStateBool("Helping", true)
                    quest:RunCutscene("CS_GHOSTFISH_FISH_INTRO_2", true, false)

                    local activeQuestName = quest:GetActiveQuestName()
                    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_HEROS_OLD_HOUSE", activeQuestName, false)
                    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_01", "", "OakBay")

                    quest:MiniMapRemoveMarker(me)

                    local wife = quest:GetThingWithScriptName("FishermansWife")
                    if wife then
                        quest:MiniMapAddMarker(wife, "HUD_ORB_QUEST_VIGNETTE")
                    end
                else
                    quest:RunCutscene("CS_GHOSTFISH_FISH_INTRO_REFUSAL", true, false)
                end

                quest:EndCutscene()
                me:AcquireControl()
                quest:EntitySetTargetingType(me, 2)
                quest:SetThingHasInformation(me, false, true, false)
            else
                if quest:GetStateBool("Helping") then
                    if bootyMarker and quest:IsDiggingSpotEnabled(bootyMarker) then
                        me:SpeakAndWait("TEXT_QST_032_GHOST_FISHERMAN_WHERE")
                        quest:EntitySetTargetingType(me, 2)
                    elseif quest:GetStateBool("Helped") then
                        me:ReleaseControl()
                        quest:StartCutscene({ HERO = hero, GHOST = me })
                        quest:RunCutscene("CS_GHOSTFISH_FISH_INTRO_SETUP", false, false)
                        quest:RunCutscene("CS_GHOSTFISH_FISH_SUCCESS", true, false)
                        quest:EndCutscene()
                        me:AcquireControl()
                        local activeQuestName = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_04", "", "OakBay")
                        ghostGoing = true
                    else
                        me:SpeakAndWait("TEXT_QST_032_GHOST_FISHERMAN_COMPLAINT")
                        quest:EntitySetTargetingType(me, 2)
                    end
                else
                    me:ReleaseControl()
                    quest:StartCutscene({ HERO = hero, GHOST = me })
                    quest:RunCutscene("CS_GHOSTFISH_FISH_INTRO_SETUP", false, false)

                    local ans = quest:GiveHeroYesNoQuestion("TEXT_QST_032_GHOST_FISHERMAN_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                    if ans == 1 then
                        quest:SetStateBool("Helping", true)
                        quest:RunCutscene("CS_GHOSTFISH_FISH_INTRO_2", true, false)
                        quest:EndCutscene()
                        me:AcquireControl()

                        local activeQuestName = quest:GetActiveQuestName()
                        quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_HEROS_OLD_HOUSE", activeQuestName, false)
                        quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_01", "", "OakBay")

                        quest:MiniMapRemoveMarker(me)

                        local wife = quest:GetThingWithScriptName("FishermansWife")
                        if wife then
                            quest:MiniMapAddMarker(wife, "HUD_ORB_QUEST_VIGNETTE")
                        end
                    else
                        quest:EndCutscene()
                        me:AcquireControl()
                        me:SpeakAndWait("TEXT_QST_032_GHOST_FISHERMAN_REFUSAL")
                    end
                    quest:EntitySetTargetingType(me, 2)
                    quest:SetThingHasInformation(me, false, true, false)
                end
            end
        end

        if not quest:NewScriptFrame(me) then
            quest:DeregisterTimer(ghostTimer)
            me:ReleaseControl()
            return
        end
    end

    -- Clean up upon leaving / being banished
    if not quest:GetStateBool("WifeAttacked") then
        quest:RemoveThing(me)
    else
        quest:CreateEffectAtPos("Ghost_Appear_01", me:GetPos())
        quest:FadeOutAndKillEntity(me, true, 1.0, true)
    end

    quest:DeregisterTimer(ghostTimer)
    me:ReleaseControl()
    quest:Log("GhostFisherman: Main() terminated.")
end
