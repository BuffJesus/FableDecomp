#pragma once
// Retail Arena definitions: vector headers are 12 bytes, without the PDB's
// iterator-debugging pointer. Values are copied before any quest state is written.
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace arena_rounds {
struct Creature {
    std::string type, hud;
    std::int32_t count, deathScore;
};
struct Wave {
    std::int32_t declaredGroups;
    bool shortWave;
    std::vector<Creature> creatures;
};
struct Round {
    std::int32_t declaredWaves;
    std::vector<Wave> waves;
};
using Snapshot = std::vector<Round>;

inline std::uint32_t Add(std::uint32_t base, std::uint32_t offset) {
    if (base > std::numeric_limits<std::uint32_t>::max() - offset)
        throw std::runtime_error("Arena definition address overflow");
    return base + offset;
}

template<class Reader, class Visit>
void VisitVector(Reader& memory, std::uint32_t header, std::uint32_t stride, Visit visit) {
    const auto first = memory.Word(header);
    const auto last = memory.Word(Add(header, 4));
    const auto capacity = memory.Word(Add(header, 8));
    if ((!first && (last || capacity)) || last < first || capacity < last ||
        (last - first) % stride || (capacity - first) % stride)
        throw std::runtime_error("Invalid Arena definition vector extent");
    for (auto element = first; element != last; element = Add(element, stride))
        visit(element);
}

template<class Reader>
Snapshot Read(Reader& memory, std::uint32_t definitions) {
    if (!definitions) throw std::runtime_error("Arena global definitions unavailable");
    Snapshot result;
    VisitVector(memory, Add(definitions, 0x1044), 0x38, [&](std::uint32_t roundAddress) {
        Round round;
        round.declaredWaves = memory.Signed(Add(roundAddress, 0x28));
        VisitVector(memory, Add(roundAddress, 0x2c), 0x3c, [&](std::uint32_t waveAddress) {
            Wave wave;
            wave.declaredGroups = memory.Signed(Add(waveAddress, 0x28));
            wave.shortWave = memory.Byte(Add(waveAddress, 0x38)) != 0;
            VisitVector(memory, Add(waveAddress, 0x2c), 0x38, [&](std::uint32_t creatureAddress) {
                Creature creature;
                creature.type = memory.String(Add(creatureAddress, 0x28));
                creature.count = memory.Signed(Add(creatureAddress, 0x2c));
                creature.hud = memory.String(Add(creatureAddress, 0x30));
                creature.deathScore = memory.Signed(Add(creatureAddress, 0x34));
                wave.creatures.push_back(std::move(creature));
            });
            round.waves.push_back(std::move(wave));
        });
        result.push_back(std::move(round));
    });
    return result;
}

template<class State>
void Store(State& state, const Snapshot& rounds) {
    state.SetStateInt("Rounds_Count", static_cast<int>(rounds.size()));
    for (std::size_t r = 0; r != rounds.size(); ++r) {
        const auto row = "Rounds_" + std::to_string(r);
        const auto& round = rounds[r];
        state.SetStateInt(row + "_NumWaves", round.declaredWaves);
        state.SetStateInt(row + "_Waves_Count", static_cast<int>(round.waves.size()));
        for (std::size_t w = 0; w != round.waves.size(); ++w) {
            const auto waveKey = row + "_Waves_" + std::to_string(w);
            const auto& wave = round.waves[w];
            state.SetStateInt(waveKey + "_NumWaveCreatures", wave.declaredGroups);
            state.SetStateBool(waveKey + "_ShortWave", wave.shortWave);
            state.SetStateInt(waveKey + "_Creatures_Count", static_cast<int>(wave.creatures.size()));
            for (std::size_t c = 0; c != wave.creatures.size(); ++c) {
                const auto creatureKey = waveKey + "_Creatures_" + std::to_string(c);
                const auto& creature = wave.creatures[c];
                state.SetStateString(creatureKey + "_CreatureType", creature.type);
                state.SetStateString(creatureKey + "_HUDType", creature.hud);
                state.SetStateInt(creatureKey + "_NumCreatures", creature.count);
                state.SetStateInt(creatureKey + "_DeathScore", creature.deathScore);
            }
        }
    }
}
} // namespace arena_rounds
