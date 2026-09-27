-- Readable native conversion: V_BeggarAndChild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BC_EvilMoralityLoss = 3676,  -- -0.009999999776482582
    BC_GoodMoralityGain = 3680,  -- 0.009999999776482582
}

-- V_BeggarAndChild.Main (retail 0x00e57e60)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local amount, predicateResult, predicateResult14, pQuestName, string, movie, resource, resource2
    local resource3, actorMap
    local function ReleaseEverything()
        quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_SNEER_DUMMY", 3, 10)
        resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", actorMap, false, true)
        local amount = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BC_EvilMoralityLoss)
        quest:GiveHeroMorality(amount)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource3)
        resources:ReleaseResource(resource2)
        resources:ReleaseResource(resource)
        local pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, 0)
    end
    local function ReleaseEverything2()
        resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", actorMap, false, true)
        local amount = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BC_EvilMoralityLoss)
        quest:GiveHeroMorality(amount)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource3)
        resources:ReleaseResource(resource2)
        resources:ReleaseResource(resource)
        local pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, 0)
    end
    local function ReleaseEverything3()
        quest:SetStateBool("ExpressionTutorialShown", true)
        resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", actorMap, false, true)
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BC_EvilMoralityLoss))
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource3)
        resources:ReleaseResource(resource2)
        resources:ReleaseResource(resource)
        local pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, 0)
    end
    local function ReleaseEverything4()
        resources:RunMacro("CS_BANDB_BULLYLEAVES_2", actorMap, false, true)
        local amount = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BC_GoodMoralityGain)
        quest:GiveHeroMorality(amount)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource3)
        resources:ReleaseResource(resource2)
        resources:ReleaseResource(resource)
        local pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, 0)
    end
    quest:AddEntityBinding("LookoutPointBeggar", "V_BeggarAndChild/Entities/LookoutPointBeggar", 1)
    quest:AddEntityBinding("BeggarBully", "V_BeggarAndChild/Entities/BeggarBully", 1)
    quest:FinalizeEntityBindings()
    while ((not quest:GetStateBool("BeggarHit") and (not quest:GetStateBool("BullyHit"))) and (not quest:GetStateBool("BeggarLeft"))) and not quest:GetStateBool("BullyLeft") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local beggarBully = quest:GetThingWithScriptName("BeggarBully")
    local lookoutPointBeggar = quest:GetThingWithScriptName("LookoutPointBeggar")
    resource = resources:NewResource()
    resource2 = resources:NewResource()
    resource3 = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00e58586 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e58586 end
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, beggarBully, 4) do
        if not quest:NewScriptFrame() then goto LAB_00e58586 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e58586 end
    resources:PrepareResource(resource3)
    while not resources:TryAcquire(resource3, lookoutPointBeggar, 4) do
        if not quest:NewScriptFrame() then goto LAB_00e58586 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e58586 end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "BULLY", resource2)
    resources:SetActor(actorMap, "BEGGAR", resource3)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_BANDB_SETUP", actorMap, false, true)
    if not quest:GetStateBool("BeggarHit") then
        if quest:GetStateBool("BullyHit") then
            if quest:IsActiveThreadTerminating() then goto LAB_00e587cd end
            resources:RunMacro("CS_BANDB_BULLYHIT", actorMap, false, true)
            if not quest:GetStateBool("TaughtBelch") and not quest:GetStateBool("TaughtBattleCry") then
                if quest:IsActiveThreadTerminating() then goto LAB_00e58565 end
                resources:RunMacro("CS_BANDB_BATTLE_TEACH", actorMap, false, true)
                if not quest:GetStateBool("ExpressionTutorialShown") then
                    if quest:IsXbox() then
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame() then goto LAB_00e58565 end
                            end
                            goto LAB_00e587be
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame() then goto LAB_00e58565 end
                        end
                        goto LAB_00e587be
                    end
                    goto FLOW_past_lab_00e587be
                    ::LAB_00e587be::
                    if quest:IsActiveThreadTerminating() then goto LAB_00e587cd end
                    quest:SetStateBool("ExpressionTutorialShown", true)
                    goto LAB_00e58851
                    ::FLOW_past_lab_00e587be::
                    goto LAB_00e58565
                end
                ::LAB_00e58851::
                quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
            end
            if not quest:GetStateBool("TaughtThanks") then
                if quest:IsActiveThreadTerminating() then goto LAB_00e58565 end
                resources:RunMacro("CS_BANDB_THANKS_TEACH", actorMap, false, true)
            end
            ReleaseEverything4()
            return
        end
        if not quest:GetStateBool("BeggarLeft") then
            if not quest:GetStateBool("BullyLeft") then
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource3)
                resources:ReleaseResource(resource2)
                resources:ReleaseResource(resource)
                quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                return
            end
            if not quest:IsActiveThreadTerminating() then
                if not quest:GetStateBool("BelchedAtBully") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e58565 end
                    string = "CS_BANDB_BULLYLEAVES_CRY"
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e587cd end
                    string = "CS_BANDB_BULLYLEAVES_BELCH"
                end
                resources:RunMacro(string, actorMap, false, true)
                resources:RunMacro("CS_BANDB_BULLYLEAVES_1", actorMap, false, true)
                quest:NewScriptFrame()
                if not quest:IsActiveThreadTerminating() then
                    quest:NewScriptFrame()
                    if not quest:IsActiveThreadTerminating() then ReleaseEverything4(); return end
                end
            end
            goto LAB_00e58565
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e587cd end
        resources:RunMacro("CS_BANDB_BEGGARLEAVES_1", actorMap, false, true)
        if quest:GetStateBool("TaughtSneer") then
            resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", actorMap, false, true)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BC_EvilMoralityLoss))
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            return
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e58565 end
        resources:RunMacro("CS_BANDB_SNEER_TEACH", actorMap, false, true)
        if quest:GetStateBool("ExpressionTutorialShown") then
            resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", actorMap, false, true)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BC_EvilMoralityLoss))
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            return
        end
        if quest:IsXbox() then
            if not quest:IsActiveThreadTerminating() then
                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                while not quest:MsgIsGameInfoClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00e58565 end
                end
                if not quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00e58570
            end
        elseif not quest:IsActiveThreadTerminating() then
            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame() then goto LAB_00e58565 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e587cd end
            ReleaseEverything3()
            return
        end
        goto LAB_00e58565
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00e58565 end
        resources:RunMacro("CS_BANDB_BEGGARHIT", actorMap, false, true)
        if not quest:GetStateBool("TaughtBelch") and not quest:GetStateBool("TaughtBattleCry") then
            if quest:IsActiveThreadTerminating() then goto LAB_00e587cd end
            resources:RunMacro("CS_BANDB_BELCH_TEACH", actorMap, false, true)
            if not quest:GetStateBool("ExpressionTutorialShown") then
                goto LAB_00e58459
            end
            goto FLOW_past_lab_00e58459
            ::LAB_00e58459::
            quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10)
            goto LAB_00e58486
            ::FLOW_past_lab_00e58459::
            if quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame() then goto LAB_00e587cd end
                    end
                    predicateResult = quest:IsActiveThreadTerminating()
                    goto LAB_00e5844f
                end
            elseif not quest:IsActiveThreadTerminating() then
                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                while not quest:MsgIsGameInfoClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00e587cd end
                end
                predicateResult = quest:IsActiveThreadTerminating()
                goto LAB_00e5844f
            end
            goto FLOW_past_lab_00e5844f
            ::LAB_00e5844f::
            if predicateResult then goto LAB_00e58565 end
            quest:SetStateBool("ExpressionTutorialShown", true)
            goto LAB_00e58459
            ::FLOW_past_lab_00e5844f::
        else
            goto LAB_00e58486
        end
        goto FLOW_past_lab_00e58486
        ::LAB_00e58486::
        if quest:GetStateBool("TaughtSneer") then
            ReleaseEverything2()
            return
        end
        if not quest:IsActiveThreadTerminating() then
            resources:RunMacro("CS_BANDB_SNEER_TEACH", actorMap, false, true)
            if quest:GetStateBool("ExpressionTutorialShown") then
                ReleaseEverything()
                return
            end
            if quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame() then goto LAB_00e587cd end
                    end
                    predicateResult14 = quest:IsActiveThreadTerminating()
                    goto LAB_00e5862f
                end
            elseif not quest:IsActiveThreadTerminating() then
                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                while not quest:MsgIsGameInfoClickedPast() do
                    if not quest:NewScriptFrame() then goto LAB_00e587cd end
                end
                predicateResult14 = quest:IsActiveThreadTerminating()
                goto LAB_00e5862f
            end
            goto FLOW_past_lab_00e5862f
            ::LAB_00e5862f::
            if predicateResult14 then goto LAB_00e58565 end
            quest:SetStateBool("ExpressionTutorialShown", true)
            do ReleaseEverything(); return end
            ::FLOW_past_lab_00e5862f::
        end
        ::FLOW_past_lab_00e58486::
        goto LAB_00e587cd
    end
    goto FLOW_past_lab_00e58565
    ::LAB_00e58565::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00e58565::
    goto FLOW_past_lab_00e587cd
    ::LAB_00e587cd::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00e587cd::
    ::LAB_00e58570::
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    ::LAB_00e58586::
    resources:ReleaseResource(resource3)
    resources:ReleaseResource(resource2)
    resources:ReleaseResource(resource)
end

-- V_BeggarAndChild.Init (retail 0x00e57c00)
function Init(quest)
    quest:SetStateInt("TauntTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("BeggarReplyTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateBool("BeggarHit", false)
    quest:SetStateBool("BeggarLeft", false)
    quest:SetStateBool("BullyLeft", false)
    quest:SetStateBool("BullyHit", false)
    quest:SetStateBool("TaughtBelch", false)
    quest:SetStateBool("TaughtSneer", false)
    quest:SetStateBool("TaughtThanks", false)
    quest:SetStateBool("TaughtBattleCry", false)
    quest:SetStateBool("BullyHasTaunted", false)
    quest:SetStateInt("BelchedAtBeggar", 0)
    quest:SetStateBool("BelchedAtBully", false)
    quest:SetStateBool("QuestCardGiven", false)
    quest:SetStateBool("ExpressionTutorialShown", false)
    quest:SetTimer(quest:GetStateInt("TauntTimer"), 5)
    quest:SetTimer(quest:GetStateInt("BeggarReplyTimer"), 5)
end

-- V_BeggarAndChild.OnPersist (retail 0x00e57d20)
function OnPersist(quest, context)
    quest:SetStateBool("BeggarHit", quest:PersistTransferBool(context, "BeggarHit", quest:GetStateBool("BeggarHit")))
    quest:SetStateBool("BeggarLeft", quest:PersistTransferBool(context, "BeggarLeft", quest:GetStateBool("BeggarLeft")))
    quest:SetStateBool("BullyLeft", quest:PersistTransferBool(context, "BullyLeft", quest:GetStateBool("BullyLeft")))
    quest:SetStateBool("BullyHit", quest:PersistTransferBool(context, "BullyHit", quest:GetStateBool("BullyHit")))
    quest:SetStateBool("TaughtBelch", quest:PersistTransferBool(context, "TaughtBelch", quest:GetStateBool("TaughtBelch")))
    quest:SetStateBool("TaughtSneer", quest:PersistTransferBool(context, "TaughtSneer", quest:GetStateBool("TaughtSneer")))
    quest:SetStateBool("TaughtThanks", quest:PersistTransferBool(context, "TaughtThanks", quest:GetStateBool("TaughtThanks")))
    quest:SetStateBool("TaughtBattleCry", quest:PersistTransferBool(context, "TaughtBattleCry", quest:GetStateBool("TaughtBattleCry")))
    quest:SetStateBool("BullyHasTaunted", quest:PersistTransferBool(context, "BullyHasTaunted", quest:GetStateBool("BullyHasTaunted")))
    quest:SetStateInt("BelchedAtBeggar", quest:PersistTransferInt(context, "BelchedAtBeggar", quest:GetStateInt("BelchedAtBeggar") or 0))
    quest:SetStateBool("BelchedAtBully", quest:PersistTransferBool(context, "BelchedAtBully", quest:GetStateBool("BelchedAtBully")))
    quest:SetStateBool("QuestCardGiven", quest:PersistTransferBool(context, "QuestCardGiven", quest:GetStateBool("QuestCardGiven")))
end

