-- Generated native draft: V_BeggarAndChild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local amount, bVar6, cVar1, delay, iVar8, pCVar4, pQuestName, pppuVar7, r1, r2, string, xStack_10, xStack_20, xStack_30, xStack_40, xStack_64
    local alive = true
    local function __cleanup_LAB_00e58639()
        quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_SNEER_DUMMY", 3, 10)
        resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", xStack_64, false, true)
        amount = quest:ReadGlobalGameDataFloat(0xe5c)
        quest:GiveHeroMorality(amount)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:DestroyActorMap(xStack_64)
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        resources:ReleaseResource(xStack_20)
        delay = 0
        pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, delay)
    end
    local function __cleanup_LAB_00e58666()
        resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", xStack_64, false, true)
        amount = quest:ReadGlobalGameDataFloat(0xe5c)
        quest:GiveHeroMorality(amount)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:DestroyActorMap(xStack_64)
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        resources:ReleaseResource(xStack_20)
        delay = 0
        pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, delay)
    end
    local function __cleanup_LAB_00e58a75()
        quest:SetStateBool("ExpressionTutorialShown", true)
        resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", xStack_64, false, true)
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xe5c))
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:DestroyActorMap(xStack_64)
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        resources:ReleaseResource(xStack_20)
        delay = 0
        pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, delay)
    end
    local function __cleanup_LAB_00e58b8b()
        resources:RunMacro("CS_BANDB_BULLYLEAVES_2", xStack_64, false, true)
        amount = quest:ReadGlobalGameDataFloat(0xe60)
        quest:GiveHeroMorality(amount)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:DestroyActorMap(xStack_64)
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        resources:ReleaseResource(xStack_20)
        delay = 0
        pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, delay)
    end
    local function __cleanup_LAB_00e58bc1()
        quest:GiveHeroMorality(amount)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:DestroyActorMap(xStack_64)
        resources:ReleaseResource(xStack_40)
        resources:ReleaseResource(xStack_30)
        resources:ReleaseResource(xStack_20)
        delay = 0
        pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, delay)
    end
    quest:AddEntityBinding("LookoutPointBeggar", "V_BeggarAndChild/Entities/LookoutPointBeggar", 1)
    quest:AddEntityBinding("BeggarBully", "V_BeggarAndChild/Entities/BeggarBully", 1)
    quest:FinalizeEntityBindings()
    cVar1 = quest:GetStateBool("BeggarHit")
    while (((not cVar1 and (not quest:GetStateBool("BullyHit"))) and (not quest:GetStateBool("BeggarLeft"))) and (not quest:GetStateBool("BullyLeft"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        cVar1 = quest:GetStateBool("BeggarHit")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then
        return
    end
    r1 = quest:GetThingWithScriptName("BeggarBully")
    r2 = quest:GetThingWithScriptName("LookoutPointBeggar")
    xStack_20 = resources:NewResource()
    xStack_30 = resources:NewResource()
    xStack_40 = resources:NewResource()
    resources:PrepareResource(xStack_20)
    iVar8 = 4
    pppuVar7 = xStack_20
    pCVar4 = quest:GetHero()
    bVar6 = resources:TryAcquire(pppuVar7, pCVar4, iVar8)
    while not bVar6 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then goto LAB_00e58586 end
        iVar8 = 4
        pppuVar7 = xStack_20
        pCVar4 = quest:GetHero()
        bVar6 = resources:TryAcquire(pppuVar7, pCVar4, iVar8)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then goto LAB_00e58586 end
    resources:PrepareResource(xStack_30)
    bVar6 = resources:TryAcquire(xStack_30, r1, 4)
    while not bVar6 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then goto LAB_00e58586 end
        bVar6 = resources:TryAcquire(xStack_30, r1, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then goto LAB_00e58586 end
    resources:PrepareResource(xStack_40)
    bVar6 = resources:TryAcquire(xStack_40, r2, 4)
    while not bVar6 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then goto LAB_00e58586 end
        bVar6 = resources:TryAcquire(xStack_40, r2, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then goto LAB_00e58586 end
    xStack_64 = resources:NewActorMap()
    resources:SetActor(xStack_64, "HERO", xStack_20)
    resources:SetActor(xStack_64, "BULLY", xStack_30)
    resources:SetActor(xStack_64, "BEGGAR", xStack_40)
    xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_BANDB_SETUP", xStack_64, false, true)
    if not quest:GetStateBool("BeggarHit") then
        if quest:GetStateBool("BullyHit") then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00e587cd end
            resources:RunMacro("CS_BANDB_BULLYHIT", xStack_64, false, true)
            if (not quest:GetStateBool("TaughtBelch")) and (not quest:GetStateBool("TaughtBattleCry")) then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e58565 end
                resources:RunMacro("CS_BANDB_BATTLE_TEACH", xStack_64, false, true)
                if not quest:GetStateBool("ExpressionTutorialShown") then
                    bVar6 = quest:IsXbox()
                    if bVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                            bVar6 = quest:MsgIsGameInfoClickedPast()
                            while not bVar6 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e58565 end
                                bVar6 = quest:MsgIsGameInfoClickedPast()
                            end
                            goto LAB_00e587be
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                            bVar6 = quest:MsgIsGameInfoClickedPast()
                            while not bVar6 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e58565 end
                                bVar6 = quest:MsgIsGameInfoClickedPast()
                            end
                            goto LAB_00e587be
                        end
                    end
                    goto FLOW_past_lab_00e587be
                    ::LAB_00e587be::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e587cd end
                    quest:SetStateBool("ExpressionTutorialShown", true)
                    goto LAB_00e58851
                    ::FLOW_past_lab_00e587be::
                    goto LAB_00e58565
                end
                ::LAB_00e58851::
                quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
            end
            if not quest:GetStateBool("TaughtThanks") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e58565 end
                resources:RunMacro("CS_BANDB_THANKS_TEACH", xStack_64, false, true)
            end
            __cleanup_LAB_00e58b8b()
            return
        end
        if not quest:GetStateBool("BeggarLeft") then
            if not quest:GetStateBool("BullyLeft") then
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_10)
                resources:DestroyActorMap(xStack_64)
                resources:ReleaseResource(xStack_40)
                resources:ReleaseResource(xStack_30)
                resources:ReleaseResource(xStack_20)
                delay = 0
                pQuestName = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(pQuestName, delay)
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                if not quest:GetStateBool("BelchedAtBully") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e58565 end
                    string = "CS_BANDB_BULLYLEAVES_CRY"
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e587cd end
                    string = "CS_BANDB_BULLYLEAVES_BELCH"
                end
                resources:RunMacro(string, xStack_64, false, true)
                resources:RunMacro("CS_BANDB_BULLYLEAVES_1", xStack_64, false, true)
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then __cleanup_LAB_00e58b8b(); return end
                end
            end
            goto LAB_00e58565
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then goto LAB_00e587cd end
        resources:RunMacro("CS_BANDB_BEGGARLEAVES_1", xStack_64, false, true)
        if quest:GetStateBool("TaughtSneer") then
            -- LAB_00e58a79: (native jump target)
            resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", xStack_64, false, true)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xe5c))
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(xStack_64)
            resources:ReleaseResource(xStack_40)
            resources:ReleaseResource(xStack_30)
            resources:ReleaseResource(xStack_20)
            delay = 0
            pQuestName = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pQuestName, delay)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then goto LAB_00e58565 end
        resources:RunMacro("CS_BANDB_SNEER_TEACH", xStack_64, false, true)
        if quest:GetStateBool("ExpressionTutorialShown") then
            resources:RunMacro("CS_BANDB_BEGGARLEAVES_2", xStack_64, false, true)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xe5c))
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(xStack_64)
            resources:ReleaseResource(xStack_40)
            resources:ReleaseResource(xStack_30)
            resources:ReleaseResource(xStack_20)
            delay = 0
            pQuestName = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pQuestName, delay)
            return
        end
        bVar6 = quest:IsXbox()
        if bVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                bVar6 = quest:MsgIsGameInfoClickedPast()
                while not bVar6 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e58565 end
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then __cleanup_LAB_00e58a75(); return end
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00e58570
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                bVar6 = quest:MsgIsGameInfoClickedPast()
                while not bVar6 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e58565 end
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e587cd end
                __cleanup_LAB_00e58a75()
                return
            end
        end
        goto LAB_00e58565
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then goto LAB_00e58565 end
        resources:RunMacro("CS_BANDB_BEGGARHIT", xStack_64, false, true)
        if (not quest:GetStateBool("TaughtBelch")) and (not quest:GetStateBool("TaughtBattleCry")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00e587cd end
            resources:RunMacro("CS_BANDB_BELCH_TEACH", xStack_64, false, true)
            if not quest:GetStateBool("ExpressionTutorialShown") then
                goto LAB_00e58459
            end
            goto FLOW_past_lab_00e58459
            ::LAB_00e58459::
            quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10)
            goto LAB_00e58486
            ::FLOW_past_lab_00e58459::
            bVar6 = quest:IsXbox()
            if bVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                    while not bVar6 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e587cd end
                        bVar6 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    goto LAB_00e5844f
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                    while not bVar6 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e587cd end
                        bVar6 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    goto LAB_00e5844f
                end
            end
            goto FLOW_past_lab_00e5844f
            ::LAB_00e5844f::
            if bVar6 then goto LAB_00e58565 end
            quest:SetStateBool("ExpressionTutorialShown", true)
            goto LAB_00e58459
            ::FLOW_past_lab_00e5844f::
        else
            goto LAB_00e58486
        end
        goto FLOW_past_lab_00e58486
        ::LAB_00e58486::
        if quest:GetStateBool("TaughtSneer") then
            __cleanup_LAB_00e58666()
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            resources:RunMacro("CS_BANDB_SNEER_TEACH", xStack_64, false, true)
            if quest:GetStateBool("ExpressionTutorialShown") then
                __cleanup_LAB_00e58639()
                return
            end
            bVar6 = quest:IsXbox()
            if bVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                    while not bVar6 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e587cd end
                        bVar6 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    goto LAB_00e5862f
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                    bVar6 = quest:MsgIsGameInfoClickedPast()
                    while not bVar6 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e587cd end
                        bVar6 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    goto LAB_00e5862f
                end
            end
            goto FLOW_past_lab_00e5862f
            ::LAB_00e5862f::
            if bVar6 then goto LAB_00e58565 end
            quest:SetStateBool("ExpressionTutorialShown", true)
            do __cleanup_LAB_00e58639(); return end
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
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_64)
    ::LAB_00e58586::
    resources:ReleaseResource(xStack_40)
    resources:ReleaseResource(xStack_30)
    resources:ReleaseResource(xStack_20)
end

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

function OnPersist(quest, context)
    local beggarHit = quest:GetStateBool("BeggarHit") or false
    beggarHit = quest:PersistTransferBool(context, "BeggarHit", beggarHit)
    quest:SetStateBool("BeggarHit", beggarHit)
    local beggarLeft = quest:GetStateBool("BeggarLeft") or false
    beggarLeft = quest:PersistTransferBool(context, "BeggarLeft", beggarLeft)
    quest:SetStateBool("BeggarLeft", beggarLeft)
    local bullyLeft = quest:GetStateBool("BullyLeft") or false
    bullyLeft = quest:PersistTransferBool(context, "BullyLeft", bullyLeft)
    quest:SetStateBool("BullyLeft", bullyLeft)
    local bullyHit = quest:GetStateBool("BullyHit") or false
    bullyHit = quest:PersistTransferBool(context, "BullyHit", bullyHit)
    quest:SetStateBool("BullyHit", bullyHit)
    local taughtBelch = quest:GetStateBool("TaughtBelch") or false
    taughtBelch = quest:PersistTransferBool(context, "TaughtBelch", taughtBelch)
    quest:SetStateBool("TaughtBelch", taughtBelch)
    local taughtSneer = quest:GetStateBool("TaughtSneer") or false
    taughtSneer = quest:PersistTransferBool(context, "TaughtSneer", taughtSneer)
    quest:SetStateBool("TaughtSneer", taughtSneer)
    local taughtThanks = quest:GetStateBool("TaughtThanks") or false
    taughtThanks = quest:PersistTransferBool(context, "TaughtThanks", taughtThanks)
    quest:SetStateBool("TaughtThanks", taughtThanks)
    local taughtBattleCry = quest:GetStateBool("TaughtBattleCry") or false
    taughtBattleCry = quest:PersistTransferBool(context, "TaughtBattleCry", taughtBattleCry)
    quest:SetStateBool("TaughtBattleCry", taughtBattleCry)
    local bullyHasTaunted = quest:GetStateBool("BullyHasTaunted") or false
    bullyHasTaunted = quest:PersistTransferBool(context, "BullyHasTaunted", bullyHasTaunted)
    quest:SetStateBool("BullyHasTaunted", bullyHasTaunted)
    local belchedAtBeggar = quest:GetStateInt("BelchedAtBeggar") or 0
    belchedAtBeggar = quest:PersistTransferInt(context, "BelchedAtBeggar", belchedAtBeggar)
    quest:SetStateInt("BelchedAtBeggar", belchedAtBeggar)
    local belchedAtBully = quest:GetStateBool("BelchedAtBully") or false
    belchedAtBully = quest:PersistTransferBool(context, "BelchedAtBully", belchedAtBully)
    quest:SetStateBool("BelchedAtBully", belchedAtBully)
    local questCardGiven = quest:GetStateBool("QuestCardGiven") or false
    questCardGiven = quest:PersistTransferBool(context, "QuestCardGiven", questCardGiven)
    quest:SetStateBool("QuestCardGiven", questCardGiven)
end

