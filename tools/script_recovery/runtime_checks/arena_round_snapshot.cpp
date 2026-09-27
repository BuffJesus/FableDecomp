#include "../runtime_bindings/arena_round_snapshot.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>

struct Memory {
    std::vector<unsigned char> bytes = std::vector<unsigned char>(0x10000);
    std::map<std::uint32_t, std::string> strings;
    std::uint32_t Word(std::uint32_t address) {
        if (address > bytes.size() - 4) throw std::runtime_error("fixture read outside memory");
        std::uint32_t value;
        std::memcpy(&value, &bytes[address], 4);
        return value;
    }
    std::int32_t Signed(std::uint32_t address) {
        auto bits = Word(address);
        std::int32_t value;
        std::memcpy(&value, &bits, 4);
        return value;
    }
    unsigned char Byte(std::uint32_t address) { return bytes.at(address); }
    std::string String(std::uint32_t address) { return strings.at(address); }
    void Put(std::uint32_t address, std::uint32_t value) { std::memcpy(&bytes.at(address), &value, 4); }
    void Vector(std::uint32_t at, std::uint32_t first, std::uint32_t size, std::uint32_t stride) {
        Put(at, first); Put(at+4, first+size*stride); Put(at+8, first+size*stride);
    }
};
struct State {
    std::map<std::string, int> ints;
    std::map<std::string, bool> bools;
    std::map<std::string, std::string> strings;
    void SetStateInt(const std::string& key, int value) { ints[key]=value; }
    void SetStateBool(const std::string& key, bool value) { bools[key]=value; }
    void SetStateString(const std::string& key, const std::string& value) { strings[key]=value; }
};
template<class Action> void MustFail(Action action) {
    bool failed=false;
    try { action(); } catch (const std::exception&) { failed=true; }
    assert(failed);
}
int main() {
    Memory memory;
    memory.Vector(0x2044, 0x3000, 2, 0x38);
    memory.Put(0x3028, 7); // declared count must not be replaced by vector length
    memory.Vector(0x302c, 0x4000, 2, 0x3c);
    memory.Put(0x4028, 99);
    memory.Vector(0x402c, 0x5000, 2, 0x38);
    memory.bytes[0x4038]=1;
    memory.strings[0x5028]="CREATURE_HOBBE";
    memory.Put(0x502c, 5); memory.strings[0x5030]="TEXT_HOBBES"; memory.Put(0x5034, 8);
    memory.strings[0x5060]="CREATURE_BANDIT";
    memory.Put(0x5064, 2); memory.strings[0x5068]="TEXT_BANDITS"; memory.Put(0x506c, static_cast<unsigned>(-3));
    auto snapshot=arena_rounds::Read(memory,0x1000);
    assert(snapshot.size()==2 && snapshot[0].waves.size()==2 && snapshot[1].waves.empty());
    assert(snapshot[0].waves[0].creatures.size()==2 && snapshot[0].waves[1].creatures.empty());
    memory.strings[0x5028]="CHANGED_AFTER_COPY";
    memory.Put(0x502c, 100);
    State quest;
    arena_rounds::Store(quest,snapshot);
    const std::string prefix="Rounds_0_Waves_0_Creatures_";
    assert(quest.ints.at("Rounds_Count")==2);
    assert(quest.ints.at("Rounds_0_NumWaves")==7 && quest.ints.at("Rounds_0_Waves_Count")==2);
    assert(quest.ints.at("Rounds_0_Waves_0_NumWaveCreatures")==99);
    assert(quest.ints.at("Rounds_0_Waves_0_Creatures_Count")==2);
    assert(quest.bools.at("Rounds_0_Waves_0_ShortWave"));
    assert(!quest.bools.at("Rounds_0_Waves_1_ShortWave"));
    assert(quest.strings.at(prefix+"0_CreatureType")=="CREATURE_HOBBE");
    assert(quest.strings.at(prefix+"1_HUDType")=="TEXT_BANDITS");
    assert(quest.ints.at(prefix+"0_NumCreatures")==5 && quest.ints.at(prefix+"1_DeathScore")==-3);
    Memory empty;
    assert(arena_rounds::Read(empty,0x1000).empty());
    // Empty allocated vectors are also valid: no element may be dereferenced.
    empty.Vector(0x2044,0x9000,0,0x38); empty.Put(0x204c,0x9070);
    assert(arena_rounds::Read(empty,0x1000).empty());
    MustFail([&] { arena_rounds::Read(memory,0); });
    MustFail([&] { arena_rounds::Read(memory,0xfffffff0); });
    for (int scenario=0; scenario!=4; ++scenario) {
        Memory bad;
        bad.Vector(0x2044,0x3000,1,0x38);
        if (scenario==0) bad.Put(0x2044,0);
        if (scenario==1) bad.Put(0x2048,0x2fff);
        if (scenario==2) bad.Put(0x2048,0x3037);
        if (scenario==3) bad.Put(0x204c,0x3000);
        MustFail([&] { arena_rounds::Read(bad,0x1000); });
    }
    std::cout << "Arena snapshot: nested counts, owned strings, empty vectors, signed values and invalid extents passed\n";
}
