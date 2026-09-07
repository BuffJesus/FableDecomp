Quests = {

    FSE_Master = {
        name = "FSE_Master",
        file = "Master/FSE_Master",
        id = 1000,
        entity_scripts = {}
    },

    TentacleKrakenBossFight = {
        name = "TentacleKrakenBossFight",
        file = "TentacleKrakenBossFight/TentacleKrakenBossFight",
        id = 1011,
        entity_scripts = {
            { name = "KrakenMinion", file = "TentacleKrakenBossFight/Entities/KrakenMinion", id = 90 },
            { name = "M_Experience", file = "TentacleKrakenBossFight/Entities/M_Experience", id = 91 },
            { name = "TheKraken",    file = "TentacleKrakenBossFight/Entities/TheKraken",    id = 92 },
        }
    }
}