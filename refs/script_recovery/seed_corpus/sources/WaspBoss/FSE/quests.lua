Quests = {

    FSE_Master = {
        name = "FSE_Master",
        file = "Master/FSE_Master",
        id = 1000,
        entity_scripts = {}
    },

    WaspBoss = {
        name = "WaspBoss",
        file = "WaspBoss/WaspBoss",
        id = 1008,
        entity_scripts = {
            { name = "GratefulVillagerSpawn", file = "WaspBoss/Entities/GratefulVillagerSpawn", id = 74 },
            { name = "WaspChaser",            file = "WaspBoss/Entities/WaspChaser",            id = 75 },
            { name = "WaspChaseWoman",        file = "WaspBoss/Entities/WaspChaseWoman",        id = 76 },
            { name = "WaspAttacker",          file = "WaspBoss/Entities/WaspAttacker",          id = 77 },
            { name = "WaspVictim",            file = "WaspBoss/Entities/WaspVictim",            id = 78 },
            { name = "FleeingWoman",          file = "WaspBoss/Entities/FleeingWoman",          id = 79 },
            { name = "WaspHelper",            file = "WaspBoss/Entities/WaspHelper",            id = 80 },
            { name = "QueenHornet",           file = "WaspBoss/Entities/QueenHornet",           id = 81 },
            { name = "HornetDrone",           file = "WaspBoss/Entities/HornetDrone",           id = 82 },
        }
    }
}