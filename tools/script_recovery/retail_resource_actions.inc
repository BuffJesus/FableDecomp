    // Explicit native resource actions: no acquisition, wait or termination policy.
    void MoveToPosition(unsigned id, sol::table position, float radius, int moveType,
                        bool avoidObstacles, bool ignorePathPreferability) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->MoveToPosition)
            throw std::runtime_error("Retail resource MoveToPosition is unavailable");
        // A missing coordinate is unresolved input, not a zero-vector fallback.
        C3DVector pos = { position["x"].get<float>(), position["y"].get<float>(), position["z"].get<float>() };
        table->MoveToPosition(expert, &pos, radius, static_cast<EScriptEntityMoveType>(moveType),
                              avoidObstacles, ignorePathPreferability);
    }
    void PlayAnimation(unsigned id, const std::string& name,
                       bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->PlayAnimation)
            throw std::runtime_error("Retail resource PlayAnimation is unavailable");
        FableString animation(name.c_str());
        table->PlayAnimation(expert, animation.get(), b1, b2, b3, b4, b5, b6, b7);
    }
    void ClearCommands(unsigned id) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->ClearCommands)
            throw std::runtime_error("Retail resource ClearCommands is unavailable");
        table->ClearCommands(expert);
    }
    void ClearAllActions(unsigned id) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->ClearAllActions)
            throw std::runtime_error("Retail resource ClearAllActions is unavailable");
        table->ClearAllActions(expert);
    }
