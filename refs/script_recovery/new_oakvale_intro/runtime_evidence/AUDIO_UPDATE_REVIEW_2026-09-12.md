# Full native audio position-update execution

The earlier relative-position experiment entered at C1C49D and stopped before
the loop tail. This pass executes the complete C1C460..C1C61C function, including
SEH setup/restoration, both entry gates, voice iteration, geometry and exit.
All 91 cases pass against the pinned retail EXE.

The two entry gates are proxy+19 and mixer+885. An eligible voice additionally
needs a non-null wrapper+4, nonzero voice+48, and nonzero wrapper+25. Only such
voices reach the position submission. Locks balance whenever the gated update
runs; C33690 is called once on exit even when the position path is skipped.
Field offsets are reported as observed gates, without inventing higher-level
names for them.

The cases cover all 64 combinations of those five gates and relative mode,
ten heading/relative geometry combinations, an empty list, a mixed eligible/
ineligible list, and 15 additional world positions/headings. Native matrix math
and branches execute. Lock operations, voice lookup, backend submission and
the final backend call are doubles. Return stack, callee-saved registers and
the SEH chain are checked.

## New geometry finding

For the default relative source (game coordinates 0,0.5,0), native code submits
the point 0.5 metres ahead of the listener under each tested heading.

For world-space sources, C1C37A compares listener-local depth with zero and
C1C387 multiplies negative depth by -1. The point is then transformed back.
This reflects a source behind the listener across the listener's perpendicular
plane, preserving its distance. The output is in front of, or on the plane of,
the listener. The experiment checks the reflected coordinates and preserved
distance, not merely that a backend call occurred.

Example: listener game position (10,20,30), facing +Y, world source (0,0.5,0).
Submitted backend coordinates are (0,0,39.5), with the game Y/Z axes exchanged.
Relative mode instead submits (10,30,20.5). The earlier world-space result
already contained the reflected point; its assertions checked distance and
did not describe this reflection. No game code or audio setting was changed.

## Limits and next capture

This does not identify the user's quiet line, prove which flags/gates were
active in that scene, or measure audible volume, attenuation, mixing, device
routing or headphones/speakers. There is no demonstrated audio fix here.
The existing external observer can correlate sample identities/flags and
listener poses with an identified live line. Backend gain remains a separate
investigation; do not label emitter geometry as a volume measurement.

Evidence: `native-audio-update-gates-20260912.json`.

```
python work/runtime-20260912-video-comparison/prove_audio_update_gates.py --output work/NEW_AUDIO_UPDATE.json
```
