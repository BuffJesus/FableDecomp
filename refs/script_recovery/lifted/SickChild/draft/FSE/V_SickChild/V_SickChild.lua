-- Generated native draft: V_SickChild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar7, cVar1, pCVar5
    local alive = true
    quest:AddEntityBinding("SickChildsMother", "V_SickChild/Entities/SickChildsMother")
    quest:AddEntityBinding("SickChild", "V_SickChild/Entities/SickChild")
    quest:AddEntityBinding("SickChildsSister", "V_SickChild/Entities/SickChildsSister")
    quest:AddEntityBinding("Witch", "V_SickChild/Entities/Witch")
    quest:AddEntityBinding("ManInLove", "V_SickChild/Entities/ManInLove")
    quest:AddEntityBinding("MansLover", "V_SickChild/Entities/MansLover")
    quest:AddEntityBinding("IngredientOwner", "V_SickChild/Entities/IngredientOwner")
    quest:AddEntityBinding("TalkingTrader1", "V_SickChild/Entities/TalkingTrader1")
    quest:AddEntityBinding("TalkingTrader2", "V_SickChild/Entities/TalkingTrader2")
    quest:AddEntityBinding("WomanToAttract", "V_SickChild/Entities/WomanToAttract")
    quest:AddEntityBinding("SickChildFishingSpot", "V_SickChild/Entities/SickChildFishingSpot")
    quest:FinalizeEntityBindings()
    bVar7 = quest:IsQuestCompleted("QS_GuardianSisterInfo")
    while not bVar7 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if bVar7 then
            return
        end
        bVar7 = quest:IsQuestCompleted("QS_GuardianSisterInfo")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar7 = not alive
    if not bVar7 then
        quest:ActivateQuest("V_SickChild_Activate")
        quest:ActivateQuest("V_SickChildBarrowFields")
        cVar1 = quest:GetStateBool("FinishedQuest")
        while not cVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar7 = not alive
            if bVar7 then
                return
            end
            cVar1 = quest:GetStateBool("FinishedQuest")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if not bVar7 then
            bVar7 = false
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar7, false, false)
            bVar7 = quest:IsRegionLoaded("BowerstoneSlums")
            if bVar7 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar7 = not alive
                    if bVar7 then
                        return
                    end
                    bVar7 = quest:IsRegionLoaded("BowerstoneSlums")
                until not (bVar7)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar7 = not alive
            if not bVar7 then
                quest:DeactivateQuestLater("V_SickChildBarrowFields", 0)
                quest:DeactivateQuestLater("V_SickChild_Activate", 0)
                pCVar5 = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(pCVar5, 0)
            end
        end
    end
end

function Init(quest)
    quest:SetStateBool("FinishedQuest", false)
    quest:SetStateBool("GotFishingSpotMushroom", false)
    quest:SetStateBool("MotherIntroDone", false)
    quest:SetStateInt("MansLoverState", 0)
    quest:SetStateBool("OwnerAlive", true)
    quest:SetStateBool("LaughingWomanKilled", false)
end

function OnPersist(quest, context)
    local finishedQuest = quest:GetStateBool("FinishedQuest") or false
    finishedQuest = quest:PersistTransferBool(context, "FinishedQuest", finishedQuest)
    quest:SetStateBool("FinishedQuest", finishedQuest)
    local gotFishingSpotMushroom = quest:GetStateBool("GotFishingSpotMushroom") or false
    gotFishingSpotMushroom = quest:PersistTransferBool(context, "GotFishingSpotMushroom", gotFishingSpotMushroom)
    quest:SetStateBool("GotFishingSpotMushroom", gotFishingSpotMushroom)
    local motherIntroDone = quest:GetStateBool("MotherIntroDone") or false
    motherIntroDone = quest:PersistTransferBool(context, "MotherIntroDone", motherIntroDone)
    quest:SetStateBool("MotherIntroDone", motherIntroDone)
    local mansLoverState = quest:GetStateInt("MansLoverState") or 0
    mansLoverState = quest:PersistTransferInt(context, "MansLoverState", mansLoverState)
    quest:SetStateInt("MansLoverState", mansLoverState)
    local ownerAlive = quest:GetStateBool("OwnerAlive") or false
    ownerAlive = quest:PersistTransferBool(context, "OwnerAlive", ownerAlive)
    quest:SetStateBool("OwnerAlive", ownerAlive)
    local laughingWomanKilled = quest:GetStateBool("LaughingWomanKilled") or false
    laughingWomanKilled = quest:PersistTransferBool(context, "LaughingWomanKilled", laughingWomanKilled)
    quest:SetStateBool("LaughingWomanKilled", laughingWomanKilled)
end

function helper_ECE460(quest, native_arg_macroName, native_arg_argument)
    local resources = quest:RetailResources()
    local pOther
    local xStack_1c = resources:NewStringMap()
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local piVar2 = ("NULL" or "")
    local piVar5 = (native_arg_argument or "")
    local iVar3 = ((string.lower(piVar5) == string.lower(piVar2)) and 0 or 1)
    if iVar3 ~= 0 then
        pOther = (native_arg_argument or "")
        resources:SetString(xStack_1c, "$ARG1", pOther)
    end
    local xStack_28 = resources:NewActorMap()
    resources:SetActor(xStack_28, "WITCH", resources:MemberResource("seh_Witch"))
    resources:SetActor(xStack_28, "MUM", resources:MemberResource("seh_Mother"))
    resources:SetActor(xStack_28, "HERO", xStack_10)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(native_arg_macroName, xStack_28, xStack_1c, false, true)
    quest:FixMovieSequenceCamera(false)
    resources:DestroyActorMap(xStack_28)
    resources:ReleaseResource(xStack_10)
    resources:DestroyStringMap(xStack_1c)
end

