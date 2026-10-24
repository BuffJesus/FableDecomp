#include <fable/platform/KeyboardAccumulator.hpp>

#include <iostream>

int main()
{
    int failures = 0;
    auto expect = [&](bool ok, const char* what) {
        if (!ok) {
            std::cerr << "FAIL: " << what << '\n';
            ++failures;
        }
    };
    fable::platform::KeyboardAccumulator kb;
    kb.key('W', true);
    kb.key('W', true);    // auto-repeat
    kb.key('E', true);
    kb.key('E', false);   // a tap inside one tick
    auto f = kb.take();
    expect(f.held.test('W') && !f.held.test('E'), "held snapshot at the end of the tick");
    expect(f.pressed.test('W') && f.pressed.test('E'), "a tap between ticks is still pressed");
    expect(f.events.size() == 3, "auto-repeat is not an event; transitions are kept in order");
    f = kb.take();
    expect(f.held.test('W') && f.pressed.none() && f.events.empty(), "held keys carry into the next tick");
    kb.release_all();
    f = kb.take();
    expect(f.held.none() && f.events.size() == 1 && !f.events[0].down, "focus loss releases held keys");
    if (failures == 0) {
        std::cout << "fable_platform.Keyboard: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
