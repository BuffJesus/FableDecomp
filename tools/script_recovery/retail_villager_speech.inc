    void SpeakVillagerAttacked(unsigned control,bool male) {
        CheckOpen();
        if(!GetHero_API)throw std::runtime_error("Retail Villager hero lookup unavailable");
        auto* hero=GetHero_API(m_game);
        Speak(control,hero,male?"TEXT_QST_048_VILLAGER_ATTACKED_MALE":
            "TEXT_QST_048_VILLAGER_ATTACKED_FEMALE",1,false,true,false);
    }
