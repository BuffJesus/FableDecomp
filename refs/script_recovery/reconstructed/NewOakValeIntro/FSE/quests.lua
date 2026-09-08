-- ForgeFSE package registry for the reconstructed retail prologue quest.
--
-- Retail identity: Q_NewOakValeIntro (native NScript::CQ_NewOakValeIntroScript, section S_QNOVI).
-- The numeric ids below are ForgeFSE-local package metadata chosen to avoid the ids used by the
-- reconstructed seed corpus (quests 1001-1029, entities 61-108). They are NOT retail evidence.
-- Entity ids must stay below ForgeFSE's MAX_ENTITY_SCRIPTS (300).
--
-- Evidence for the entity list: CQ_NewOakValeIntroScript::Main @ 0x00DABAC0 registers exactly these
-- sixteen AddEntityScriptBinding names in this order (native-decompile).
Quests = {

    FSE_Master = {
        name = "FSE_Master",
        file = "Master/FSE_Master",
        id = 1000,
        entity_scripts = {}
    },

    Q_NewOakValeIntro = {
        name = "NewOakValeIntro",
        file = "NewOakValeIntro/NewOakValeIntro",
        id = 1100,
        entity_scripts = {
            { name = "NOVI_LiveFather",    file = "NewOakValeIntro/Entities/NOVI_LiveFather",    id = 200 },
            { name = "NOVI_Theresa",       file = "NewOakValeIntro/Entities/NOVI_Theresa",       id = 201 },
            { name = "NOVI_Guard",         file = "NewOakValeIntro/Entities/NOVI_Guard",         id = 202 },
            { name = "NOVI_Villager",      file = "NewOakValeIntro/Entities/NOVI_Villager",      id = 203 },
            { name = "NOVI_Bully",         file = "NewOakValeIntro/Entities/NOVI_Bully",         id = 204 },
            { name = "NOVI_Victim",        file = "NewOakValeIntro/Entities/NOVI_Victim",        id = 205 },
            { name = "NOVI_TeddyGirl",     file = "NewOakValeIntro/Entities/NOVI_TeddyGirl",     id = 206 },
            { name = "NOVI_AffairMan",     file = "NewOakValeIntro/Entities/NOVI_AffairMan",     id = 207 },
            { name = "NOVI_AffairWoman",   file = "NewOakValeIntro/Entities/NOVI_AffairWoman",   id = 208 },
            { name = "NOVI_AffairWife",    file = "NewOakValeIntro/Entities/NOVI_AffairWife",    id = 209 },
            { name = "NOVI_BookTrader",    file = "NewOakValeIntro/Entities/NOVI_BookTrader",    id = 210 },
            { name = "NOVI_BarrelMan",     file = "NewOakValeIntro/Entities/NOVI_BarrelMan",     id = 211 },
            { name = "NOVI_BarrelThug",    file = "NewOakValeIntro/Entities/NOVI_BarrelThug",    id = 212 },
            { name = "NOVI_Barrel",        file = "NewOakValeIntro/Entities/NOVI_Barrel",        id = 213 },
            { name = "NOVI_CreatedBeetle", file = "NewOakValeIntro/Entities/NOVI_CreatedBeetle", id = 214 },
            { name = "OVI_DeadFather",     file = "NewOakValeIntro/Entities/OVI_DeadFather",     id = 215 },
        }
    }
}
