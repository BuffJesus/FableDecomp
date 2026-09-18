-- Generated native draft: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local bVar4, cVar5, fVar3, fret_0, iVar10, iVar8, p1, p4, p5, pCVar6, pThing, pppuVar9, r1, xStack_10, xStack_20, xStack_30
    local alive = true
    quest:FadeScreenOut(0.5, 0.0)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(false)
    bVar4 = false
    pCVar6 = quest:GetThingWithScriptName("M_DepartureTeacherStand")
    quest:EntityTeleportToThing(me, pCVar6, bVar4)
    pCVar6 = nil
    xStack_30 = resources:NewResource()
    bVar4 = false
    if bVar4 ~= 0 then
    end
    bVar4 = resources:TryAcquire(xStack_30, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            resources:ReleaseResource(xStack_30)
            return
        end
        bVar4 = resources:TryAcquire(xStack_30, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        bVar4 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
        if not bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d5134c end
            xStack_20 = resources:NewResource()
            bVar4 = false
            if bVar4 ~= 0 then
            end
            iVar10 = 4
            pppuVar9 = xStack_20
            pCVar6 = quest:GetHero()
            bVar4 = resources:TryAcquire(pppuVar9, pCVar6, iVar10)
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    resources:ReleaseResource(xStack_20)
                    resources:ReleaseResource(xStack_30)
                    return
                end
                iVar10 = 4
                pppuVar9 = xStack_20
                pCVar6 = quest:GetHero()
                bVar4 = resources:TryAcquire(pppuVar9, pCVar6, iVar10)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                -- LAB_00d50ebd: (native jump target)
                resources:ReleaseResource(xStack_20)
                resources:ReleaseResource(xStack_30)
                return
            end
            pCVar6 = resources:NewActorMap()
            resources:SetActor(pCVar6, "GM", xStack_30)
            resources:SetActor(pCVar6, "HERO", xStack_20)
            xStack_10 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_DEPARTURE_GM_DONE", pCVar6, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(pCVar6)
            resources:ReleaseResource(xStack_20)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_09", "GuildWoods", "")
            quest:ActivateQuest("Q_GuildTrainingWoodsDeparture")
            quest:SetQuestAsPersistent("Q_GuildTrainingWoodsDeparture", false)
            quest:SetMasterGameState("HeroTakingGuildTest", true)
        end
        bVar4 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
        if bVar4 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d5134c end
                cVar5 = me:IsTalkedToByHero()
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d5134c end
                    xStack_20 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar6 = resources:ScriptThing(xStack_30)
                    pCVar6 = pCVar6
                    fret_0 = quest:GetHealth(pCVar6)
                    fVar3 = 0.0
                    pCVar6 = nil
                    if fVar3 < fret_0 then
                        p5 = 0
                        p4 = 1
                        iVar10 = 0
                        iVar8 = 0
                        p1 = "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_MOAN"
                        pCVar6 = quest:GetHero()
                        r1 = me:Speak(pCVar6, p1, iVar8, (iVar10 ~= 0), (p4 ~= 0), (p5 ~= 0))
                        iVar8 = me:IsPerformingScriptTask()
                        cVar5 = iVar8
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20)
                                resources:ReleaseResource(xStack_30)
                                return
                            end
                            iVar8 = me:IsPerformingScriptTask()
                            cVar5 = iVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20)
                            resources:ReleaseResource(xStack_30)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20)
                end
                bVar4 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
            until not (bVar4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            quest:ClearThingHasInformation(me)
        end
    end
    ::LAB_00d5134c::
    resources:ReleaseResource(xStack_30)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HeroSpokenToMe", false)
    __native_entity_state:SetStateBool("TeleportToWoods", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

