#pragma once

// The world-tick scheduler, modelled on retail CMainGameComponent (docs/modern/FRAME_TIMING.md):
//
//   GetCurrentGameTime 0x416231            = GFGetTime() - start (+0x60)
//   GetGameTimeOfNextPresentCompletion 0x4166E2
//                                          = the next vblank after the frames already queued
//                                            (CVsyncCallbackTimer), never earlier than now, - start
//   IsTimeForServerUpdate(frame) 0x41674A  = predicted_present * 15 - frame > 1.0
//   ticks per second                       = 15 (global 0x1375550)
//
// Each Update runs at most one world frame (a late frame catches up one per loop pass, not in a
// burst), and the renderer draws the world `predicted_present * 15 - frame` of the way into the
// next frame. Nothing here depends on the render rate: that is why retail's world runs at exactly
// 15 Hz with 4 renders per world frame at 60 Hz (live sample, FRAME_TIMING "Live results").

#include <cstdint>

namespace fable::platform {

class WorldClock {
public:
    explicit WorldClock(double ticks_per_second = 15.0) noexcept : rate_(ticks_per_second) {}

    // IsTimeForServerUpdate: is the next world frame due for a present completing at `present`
    // (seconds of game time)?
    [[nodiscard]] bool due(double present) const noexcept { return present * rate_ - static_cast<double>(frame_) > 1.0; }

    // Runs the bookkeeping of one world frame (the game advances its state alongside).
    void advance() noexcept { ++frame_; }

    // The render fraction: how far past `frame` the predicted present is, in world frames.
    [[nodiscard]] double interpolation(double present) const noexcept
    {
        return present * rate_ - static_cast<double>(frame_);
    }

    [[nodiscard]] std::uint64_t frame() const noexcept { return frame_; }
    [[nodiscard]] double ticks_per_second() const noexcept { return rate_; }

private:
    double rate_;
    std::uint64_t frame_ = 0;
};

// The present-time prediction: the vblank after `queued` already-submitted frames, or `now` if
// that would be in the past (0x4166E2's comparison against GFGetTime()).
[[nodiscard]] inline double predicted_present(double now, double last_vblank, double vblank_period, unsigned queued) noexcept
{
    const double predicted = last_vblank + static_cast<double>(queued + 1) * vblank_period;
    return predicted > now ? predicted : now;
}

} // namespace fable::platform
