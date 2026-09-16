    void AssignVillagerSpeechText(unsigned text,RetailVillagerSpeechLists* lists,
            const std::string& category,bool male,int index) {
        auto& key=Get(text,Kind::Text).text;
        if(!lists)throw std::runtime_error("Villager speech lists are unavailable");
        lists->CopySelectedTo(&key,category,male,index);
    }
