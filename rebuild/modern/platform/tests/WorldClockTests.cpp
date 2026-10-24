#include <fable/platform/WorldClock.hpp>

#include <cmath>
#include <iostream>
#include <map>

namespace {

struct Run {
    std::uint64_t world_frames = 0;
    std::map<int, int> renders_per_world_frame;   // render count -> how many world frames had it
};

// Simulates the retail loop at a fixed present rate: each render-loop pass runs at most one world
// frame if due, then renders once at the next vblank.
Run simulate(double refresh_hz, double seconds)
{
    fable::platform::WorldClock clock;
    Run run;
    const double period = 1.0 / refresh_hz;
    int renders_this_frame = 0;
    const auto passes = static_cast<long>(seconds * refresh_hz);
    for (long i = 0; i < passes; ++i) {
        // No accumulated float drift, and a phase offset: real vblanks do not land exactly on the
        // 15 Hz grid (with exact alignment, rounding at the boundary decides 3 vs 5 renders).
        const double t = (static_cast<double>(i) + 0.3) / refresh_hz;
        const double present = fable::platform::predicted_present(t, t - period, period, 0);
        if (clock.due(present)) {
            clock.advance();
            ++run.world_frames;
            if (run.world_frames > 1) {
                ++run.renders_per_world_frame[renders_this_frame];
            }
            renders_this_frame = 0;
        }
        ++renders_this_frame;
    }
    return run;
}

} // namespace

int main()
{
    int failures = 0;
    auto expect = [&](bool ok, const char* what) {
        if (!ok) {
            std::cerr << "FAIL: " << what << '\n';
            ++failures;
        }
    };
    // The live retail sample (2026-10-17): 60 Hz presents, 225 world frames in 15 s, 4 renders each.
    const Run at60 = simulate(60.0, 15.0);
    expect(at60.world_frames >= 224 && at60.world_frames <= 226, "15 Hz world at 60 Hz presents");
    for (const auto& [renders, frames] : at60.renders_per_world_frame) {
        std::cout << "60 Hz: " << frames << " world frames with " << renders << " renders\n";
    }
    expect(at60.renders_per_world_frame.size() == 1 && at60.renders_per_world_frame.count(4) == 1,
           "exactly 4 renders per world frame at 60 Hz (live sample)");
    // High refresh rates change the render count, never the world rate.
    for (double hz : {144.0, 165.0, 240.0, 1000.0}) {
        const Run run = simulate(hz, 15.0);
        expect(run.world_frames >= 224 && run.world_frames <= 226, "world stays at 15 Hz at high refresh");
    }
    // A late world frame catches up one frame per loop pass, not in a burst.
    fable::platform::WorldClock clock;
    int ran = 0;
    for (int pass = 0; pass < 3 && clock.due(1.0); ++pass) {   // one second late: 14 frames due
        clock.advance();
        ++ran;
    }
    expect(ran == 3 && clock.due(1.0), "catch-up is one frame per Update");
    fable::platform::WorldClock fresh;
    expect(std::abs(fresh.interpolation(0.05) - 0.75) < 1e-9, "render fraction = present*15 - frame");
    if (failures == 0) {
        std::cout << "fable_platform.WorldClock: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
