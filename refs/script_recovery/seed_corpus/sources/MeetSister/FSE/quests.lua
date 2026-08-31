Quests = {

    FSE_Master = {
        name = "FSE_Master",
        file = "Master/FSE_Master",
        id = 1000,
        entity_scripts = {}
    },
	
	QS_MeetSister = {
		name = "MeetSister",
		file = "MeetSister/MeetSister",
		id = 1003,
		entity_scripts = {
			{ name = "MeetSisterMessenger", file = "MeetSister/Entities/MeetSisterMessenger", id = 66 },
			{ name = "MeetSisterSister", file = "MeetSister/Entities/MeetSisterSister", id = 67 },
		}
	}
}