-- Generated native draft: Q_WhiteBalverineKnotholeGlade. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local bVar3, fVar14, iVar1, iVar13, native_arg_sequence_1, pCVar4, pCVar6, pCVar7, pCVar8, r1, r2, r3, r4, r5, r6, r7, xStack_10, xStack_20, xStack_30, xStack_3c
    local alive = true
    bVar3 = quest:IsLevelLoaded("KnotholeGlade")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsLevelLoaded("KnotholeGlade")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    if quest:GetStateInt("BalverineState") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        quest:ActivateQuest("V_KnotholeGladeGates")
    end
    pCVar4 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pCVar4, "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_01", "KnotholeGlade", "KnotholeGlade")
    quest:AddEntityBinding("WB_Villager", "WhiteBalverineKnotholeGlade/Entities/WB_Villager", 1)
    quest:AddEntityBinding("WB_WhiteBalverine", "WhiteBalverineKnotholeGlade/Entities/WB_WhiteBalverine", 1)
    quest:AddEntityBinding("WB_ScaredVillager", "WhiteBalverineKnotholeGlade/Entities/WB_ScaredVillager", 1)
    quest:AddEntityBinding("WB_Guard1", "WhiteBalverineKnotholeGlade/Entities/WB_Guard1", 1)
    quest:AddEntityBinding("WB_Guard2", "WhiteBalverineKnotholeGlade/Entities/WB_Guard2", 1)
    quest:AddEntityBinding("KG_Chief", "WhiteBalverineKnotholeGlade/Entities/KG_Chief", 1)
    quest:FinalizeEntityBindings()
    bVar3 = quest:IsLevelLoaded("KnotholeGlade")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsLevelLoaded("KnotholeGlade")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    r1 = quest:GetThingWithScriptName("KG_Chief")
    r2 = quest:GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE")
    quest:SetVillageLimbo(r2, true)
    if not quest:GetStateBool("QuestStartScreenShown") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        quest:SetStateBool("QuestStartScreenShown", true)
        quest:KickOffQuestStartScreen("Q_WhiteBalverineKnotholeGlade", true, false)
    end
    quest:CreateThread("WatchForTermination")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleWhiteBalverineQuestObjective: lift it as function WatchForTermination(quest)
    if not bVar3 then
    end
    if (quest:GetStateInt("BalverineState") == 0) or (quest:GetStateInt("BalverineState") == 1) then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        quest:SetStateInt("BalverineState", 1)
        bVar3 = quest:IsQuestActive("V_KnotholeGladeGates")
        if bVar3 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e154cb end
                bVar3 = quest:IsQuestActive("V_KnotholeGladeGates")
            until not (bVar3)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        quest:SetStateInt("BalverineState", 2)
    end
    if quest:GetStateInt("BalverineState") == 2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        pCVar6 = quest:GetThingWithScriptName("WB_WhiteBalverine")
        bVar3 = (pCVar6 ~= nil and pCVar6:IsAlive())
        if not bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            pCVar6 = quest:GetThingWithScriptName("WB_WhiteBalverineSpawnMarker")
            bVar3 = false
            pCVar7 = pCVar6:GetPos()
            r3 = quest:CreateCreature("CREATURE_FULLMOON_BALVERINE", pCVar7, "WB_WhiteBalverine")
        end
        pCVar4 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar4, "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_02", "KnotholeGlade", "KnotholeGlade")
        iVar1 = quest:GetStateInt("BalverineState")
        while iVar1 ~= 3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            iVar1 = quest:GetStateInt("BalverineState")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        quest:StopOverrideMusic(false)
    end
    if quest:GetStateInt("BalverineState") == 3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        bVar3 = quest:IsHeroControlledByPlayer()
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            bVar3 = quest:IsHeroControlledByPlayer()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        xStack_20 = resources:NewResource()
        xStack_30 = resources:NewResource()
        resources:TryAcquire(xStack_20, r1, 4)
        iVar13 = 4
        pCVar6 = xStack_30
        pCVar8 = quest:GetHero()
        resources:TryAcquire(pCVar6, pCVar8, iVar13)
        xStack_3c = resources:NewActorMap()
        resources:SetActor(xStack_3c, "HERO", xStack_30)
        resources:SetActor(xStack_3c, "CHIEF", xStack_20)
        xStack_10 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_WBK_CHIEF1", xStack_3c, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:SetStateBool("GreenWife", true)
        resources:PrepareResource(xStack_20)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:DestroyActorMap(xStack_3c)
        resources:ReleaseResource(xStack_30)
        resources:ReleaseResource(xStack_20)
        pCVar4 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar4, "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_03", "KnotholeGlade", "KnotholeGlade")
        fVar14 = 10.0
        pCVar6 = quest:GetHero()
        pCVar8 = quest:GetThingWithScriptName("WhiteBalverineAmbushMarker")
        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar8, pCVar6, fVar14)
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            fVar14 = 10.0
            pCVar6 = quest:GetHero()
            pCVar8 = quest:GetThingWithScriptName("WhiteBalverineAmbushMarker")
            bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar8, pCVar6, fVar14)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        pCVar4 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar4, "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_04", "KnotholeGlade", "KnotholeGlade")
        quest:SetStateInt("BalverineState", 4)
    end
    if quest:GetStateInt("BalverineState") == 4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        pCVar6 = quest:GetThingWithScriptName("WB_WhiteBalverine")
        bVar3 = (pCVar6 ~= nil and pCVar6:IsAlive())
        if not bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            pCVar6 = quest:GetThingWithScriptName("WB_EscapePoint2")
            pCVar7 = pCVar6:GetPos()
            pCVar8 = {x = pCVar7.x, y = pCVar7.y, z = pCVar7.z + 9.0}
            r4 = quest:CreateCreature("CREATURE_FULLMOON_BALVERINE", pCVar8, "WB_WhiteBalverine")
        end
        iVar1 = quest:GetStateInt("BalverineState")
        while iVar1 ~= 5 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            iVar1 = quest:GetStateInt("BalverineState")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        quest:StopOverrideMusic(false)
    end
    if quest:GetStateInt("BalverineState") == 5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        bVar3 = quest:IsHeroControlledByPlayer()
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            bVar3 = quest:IsHeroControlledByPlayer()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        quest:SetStateBool("WifeCutsceneStart", true)
        xStack_20 = resources:NewResource()
        xStack_30 = resources:NewResource()
        resources:TryAcquire(xStack_20, r1, 4)
        iVar13 = 4
        pCVar6 = xStack_30
        pCVar8 = quest:GetHero()
        resources:TryAcquire(pCVar6, pCVar8, iVar13)
        xStack_3c = resources:NewActorMap()
        resources:SetActor(xStack_3c, "HERO", xStack_30)
        resources:SetActor(xStack_3c, "CHIEF", xStack_20)
        xStack_10 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_WBK_CHIEF2", xStack_3c, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:SetStateBool("WifeCutsceneStop", true)
        resources:PrepareResource(xStack_20)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:DestroyActorMap(xStack_3c)
        resources:ReleaseResource(xStack_30)
        resources:ReleaseResource(xStack_20)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        pCVar4 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar4, "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_05", "KnotholeGlade", "KnotholeGlade")
        quest:GiveHeroExperience(quest:ReadGlobalGameData(0x3c))
        iVar13 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xfa0)))
        quest:GiveHeroRenownPoints(iVar13)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        quest:Pause(2.0)
        bVar3 = quest:DisplayTutorial(0x28)
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            bVar3 = quest:MsgIsTutorialClickedPast()
            while not bVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e154cb end
                bVar3 = quest:MsgIsTutorialClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
        end
        quest:SetStateInt("BalverineState", 6)
    end
    if quest:GetStateInt("BalverineState") == 6 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        pCVar6 = quest:GetThingWithScriptName("WB_WhiteBalverine")
        bVar3 = (pCVar6 ~= nil and pCVar6:IsAlive())
        if not bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            pCVar6 = quest:GetThingWithScriptName("WB_BalvEscape2")
            bVar3 = false
            pCVar7 = pCVar6:GetPos()
            r5 = quest:CreateCreature("CREATURE_FULLMOON_BALVERINE", pCVar7, "WB_WhiteBalverine")
        end
        pCVar6 = quest:GetThingWithScriptName("WB_Guard1Pos")
        bVar3 = false
        pCVar7 = pCVar6:GetPos()
        r6 = quest:CreateCreature("CREATURE_KN_GUARD", pCVar7, "WB_Guard1")
        pCVar6 = quest:GetThingWithScriptName("WB_Guard2Pos")
        bVar3 = false
        pCVar7 = pCVar6:GetPos()
        r7 = quest:CreateCreature("CREATURE_KN_GUARD", pCVar7, "WB_Guard2")
        iVar1 = quest:GetStateInt("BalverineState")
        while iVar1 ~= 7 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            iVar1 = quest:GetStateInt("BalverineState")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e154cb end
        quest:StopOverrideMusic(false)
    end
    native_arg_sequence_1 = false
    if quest:GetStateInt("BalverineState") == 7 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if native_arg_sequence_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
    end
    if native_arg_sequence_1 then
        bVar3 = quest:IsHeroControlledByPlayer()
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e154cb end
            bVar3 = quest:IsHeroControlledByPlayer()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_20 = resources:NewResource()
            xStack_30 = resources:NewResource()
            resources:TryAcquire(xStack_20, r1, 4)
            iVar13 = 4
            pCVar6 = xStack_30
            pCVar8 = quest:GetHero()
            resources:TryAcquire(pCVar6, pCVar8, iVar13)
            xStack_3c = resources:NewActorMap()
            resources:SetActor(xStack_3c, "HERO", xStack_30)
            resources:SetActor(xStack_3c, "CHIEF", xStack_20)
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_WBK_CHIEF3", xStack_3c, false, true)
            quest:FixMovieSequenceCamera(false)
            resources:PrepareResource(xStack_20)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(xStack_3c)
            resources:ReleaseResource(xStack_30)
            resources:ReleaseResource(xStack_20)
            quest:SetStateBool("MissionSucceeded", true)
        end
    end
    ::LAB_00e154cb::
end

function Init(quest)
    quest:AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "KnotholeGlade")
    quest:AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "Witchwood4")
    quest:AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "DemonDoor_KnotholeGlade")
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(0x210), quest:ReadGlobalGameData(0x214), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(0x218), quest:ReadGlobalGameData(0x21c), false, "", 0)
    quest:SetStateInt("BalverineState", 0)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("WifeCutsceneStart", false)
    quest:SetStateBool("WifeCutsceneStop", false)
    quest:SetStateBool("GreenWife", false)
    quest:SetStateBool("QuestStartScreenShown", false)
end

function OnPersist(quest, context)
    local questStartScreenShown = quest:GetStateBool("QuestStartScreenShown") or false
    questStartScreenShown = quest:PersistTransferBool(context, "QuestStartScreenShown", questStartScreenShown)
    quest:SetStateBool("QuestStartScreenShown", questStartScreenShown)
end

function WatchForTermination(quest)
    local b3, bVar4, bVar7, cVar1, delay, pCVar5, pQuestName, r1
    local alive = true
    cVar1 = quest:GetStateBool("MissionFailed")
    while (not cVar1 and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        cVar1 = quest:GetStateBool("MissionFailed")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        if quest:GetStateBool("MissionFailed") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar7 = true
            bVar4 = false
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pCVar5, bVar4, "", bVar7)
        end
        quest:ActivateQuest("Q_WhiteBalverineWW")
        pQuestName = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_06", "Witchwood4", "KnotholeGlade")
        r1 = quest:GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE")
        quest:SetVillageLimbo(r1, false)
        cVar1 = quest:GetMasterGameState("WhiteBalverineFinished")
        while not cVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e15740 end
            cVar1 = quest:GetMasterGameState("WhiteBalverineFinished")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            b3 = false
            bVar7 = false
            bVar4 = true
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar4, bVar7, b3)
            quest:Pause(1.0)
            quest:FadeScreenIn()
            delay = 0
            pCVar5 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar5, delay)
        end
        ::LAB_00e15740::
    end
end

