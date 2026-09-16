#include <cassert>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

struct Scope {
    enum class Kind { Resource, Thing, Movie };
    struct Entry {
        explicit Entry(Kind value) : kind(value) {}
        Kind kind;
        unsigned id = 0;
        bool live = false;
    };
    bool m_closed = false;
    unsigned m_nextId = 0;
    std::map<unsigned, std::unique_ptr<Entry>> m_entries;
    std::vector<unsigned> destroyed;
    void CheckOpen() { if (m_closed) throw std::runtime_error("closed"); }
    void Destroy(Entry& entry) {
        if (!entry.live) return;
        destroyed.push_back(entry.id);
        entry.live = false;
    }
#include "retail_resource_storage.inc"
};
template<class F> void rejects(F operation) {
    bool rejected = false;
    try { operation(); } catch (const std::runtime_error&) { rejected = true; }
    assert(rejected);
}
int main() {
    Scope scope;
    auto& actor = scope.Add(Scope::Kind::Resource);
    actor.live = true;
    const unsigned actorId = actor.id;
    unsigned previous = actorId;
    for (unsigned i = 0; i < 100000; ++i) {
        const auto kind = i % 2 ? Scope::Kind::Thing : Scope::Kind::Movie;
        auto& entry = scope.Add(kind);
        entry.live = true;
        const unsigned id = entry.id;
        assert(id > previous);
        assert(scope.m_entries.size() == 2);
        assert(&scope.Get(actorId, Scope::Kind::Resource) == &actor);
        if (previous != actorId) rejects([&] { scope.Get(previous, kind); });
        rejects([&] { scope.Get(id, Scope::Kind::Resource); });
        scope.Release(id, kind);
        assert(scope.m_entries.size() == 1);
        rejects([&] { scope.Release(id, kind); });
        previous = id;
    }
    auto& movie = scope.Add(Scope::Kind::Movie); movie.live = true;
    auto& thing = scope.Add(Scope::Kind::Thing); thing.live = true;
    const unsigned movieId = movie.id, thingId = thing.id;
    for (auto it = scope.m_entries.rbegin(); it != scope.m_entries.rend(); ++it) scope.Destroy(*it->second);
    scope.m_entries.clear();
    assert(scope.destroyed[scope.destroyed.size()-3] == thingId);
    assert(scope.destroyed[scope.destroyed.size()-2] == movieId);
    assert(scope.destroyed.back() == actorId);
    scope.m_nextId = (std::numeric_limits<unsigned>::max)();
    rejects([&] { scope.Add(Scope::Kind::Thing); });
    assert(scope.m_entries.empty());
    scope.m_closed = true;
    rejects([&] { scope.Get(actorId, Scope::Kind::Resource); });
    rejects([&] { scope.Add(Scope::Kind::Thing); });
}
