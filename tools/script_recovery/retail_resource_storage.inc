    Entry& Add(Kind kind) {
        CheckOpen();
        if (m_nextId == (std::numeric_limits<unsigned>::max)())
            throw std::runtime_error("Retail resource handle space exhausted");
        auto entry = std::make_unique<Entry>(kind);
        entry->id = ++m_nextId;
        auto inserted = m_entries.emplace(entry->id, std::move(entry));
        return *inserted.first->second;
    }
    Entry& Get(unsigned id, Kind kind) {
        CheckOpen();
        auto found = m_entries.find(id);
        if (found == m_entries.end() || found->second->kind != kind || !found->second->live)
            throw std::runtime_error("Invalid or released retail resource");
        return *found->second;
    }
    void Release(unsigned id, Kind kind) {
        auto& entry = Get(id, kind);
        Destroy(entry);
        m_entries.erase(id);
    }
