# Mid-function fragment catalog entries (unverifiable)

Found 2026-08-20 while closing the `ORACLE_MISSING` gap (`tools/decomp_pipeline/backfill_oracles.py`).

**125 catalog entries are neither a manifest function start nor covered by an oracle row.**
Every one of them falls INSIDE a known manifest function, i.e. they are mid-function
fragments (the `misbounded-manifest-fragments` hazard), and their sources are trivial stubs
(`return <constant>;`, `return this;`, empty body). Nothing about them is verified: no
authoritative start, so no oracle can honestly be extracted, so the comparer has never
compared them -- yet they count toward the landed total.

Recommendation: prune them. `verify_and_land.py --prune-outside-manifest` exists for exactly
this, or remove the catalog blocks plus their src/tests files. Not done here: they belong to
an earlier lane and removing 125 landed entries is the owner's call.

| address | containing function starts at | source |
|---|---|---|
| 00401480 | 00401400 | `global_sub00401480_00401480.cpp` |
| 004039f0 | 004038c0 | `global_sub004039f0_004039f0.cpp` |
| 0040d8f0 | 0040d8d0 | `global_sub0040d8f0_0040d8f0.cpp` |
| 0040d900 | 0040d8d0 | `global_sub0040d900_0040d900.cpp` |
| 0040d910 | 0040d8d0 | `global_sub0040d910_0040d910.cpp` |
| 0040d950 | 0040d8d0 | `global_sub0040d950_0040d950.cpp` |
| 0040db30 | 0040db10 | `global_sub0040db30_0040db30.cpp` |
| 0040db40 | 0040db10 | `global_sub0040db40_0040db40.cpp` |
| 0040db50 | 0040db10 | `global_sub0040db50_0040db50.cpp` |
| 0040dbf0 | 0040db10 | `global_sub0040dbf0_0040dbf0.cpp` |
| 0040dc00 | 0040db10 | `global_sub0040dc00_0040dc00.cpp` |
| 0040dc10 | 0040db10 | `global_sub0040dc10_0040dc10.cpp` |
| 0040dc20 | 0040db10 | `global_sub0040dc20_0040dc20.cpp` |
| 0040dc60 | 0040db10 | `global_sub0040dc60_0040dc60.cpp` |
| 0040dc70 | 0040db10 | `global_sub0040dc70_0040dc70.cpp` |
| 0040dc90 | 0040db10 | `global_sub0040dc90_0040dc90.cpp` |
| 0040fd80 | 0040fd40 | `global_sub0040fd80_0040fd80.cpp` |
| 00411600 | 004115a0 | `global_sub00411600_00411600.cpp` |
| 00414d40 | 00414ce0 | `global_sub00414d40_00414d40.cpp` |
| 00414db0 | 00414ce0 | `global_sub00414db0_00414db0.cpp` |
| 00415755 | 00415640 | `global_sub00415755_00415755.cpp` |
| 004197f0 | 004197b3 | `global_sub004197f0_004197f0.cpp` |
| 0041ca1c | 0041c9c0 | `global_sub0041ca1c_0041ca1c.cpp` |
| 0041ca22 | 0041c9c0 | `global_sub0041ca22_0041ca22.cpp` |
| 0041ca6d | 0041ca52 | `global_sub0041ca6d_0041ca6d.cpp` |
| 0041ccd7 | 0041cc70 | `global_sub0041ccd7_0041ccd7.cpp` |
| 0041cd01 | 0041cce4 | `global_sub0041cd01_0041cd01.cpp` |
| 0041cd23 | 0041cd13 | `global_sub0041cd23_0041cd23.cpp` |
| 0041cd6f | 0041cd13 | `global_sub0041cd6f_0041cd6f.cpp` |
| 0041cd83 | 0041cd13 | `global_sub0041cd83_0041cd83.cpp` |
| 00428e0a | 00428d80 | `global_sub00428e0a_00428e0a.cpp` |
| 00428e8b | 00428d80 | `global_sub00428e8b_00428e8b.cpp` |
| 00428eb2 | 00428d80 | `global_sub00428eb2_00428eb2.cpp` |
| 00428eb7 | 00428d80 | `global_sub00428eb7_00428eb7.cpp` |
| 004292ec | 004292c0 | `global_sub004292ec_004292ec.cpp` |
| 004292f1 | 004292c0 | `global_sub004292f1_004292f1.cpp` |
| 004292f6 | 004292c0 | `global_sub004292f6_004292f6.cpp` |
| 004292fb | 004292c0 | `global_sub004292fb_004292fb.cpp` |
| 00429317 | 004292c0 | `global_sub00429317_00429317.cpp` |
| 0042931c | 004292c0 | `global_sub0042931c_0042931c.cpp` |
| 00429321 | 004292c0 | `global_sub00429321_00429321.cpp` |
| 0042f939 | 0042f8a4 | `global_sub0042f939_0042f939.cpp` |
| 0042f93e | 0042f8a4 | `global_sub0042f93e_0042f93e.cpp` |
| 0042f943 | 0042f8a4 | `global_sub0042f943_0042f943.cpp` |
| 00434830 | 004347d0 | `global_sub00434830_00434830.cpp` |
| 00434b00 | 00434ad0 | `global_sub00434b00_00434b00.cpp` |
| 004367f0 | 004367c0 | `global_sub004367f0_004367f0.cpp` |
| 00436800 | 004367c0 | `global_sub00436800_00436800.cpp` |
| 0043cd20 | 0043cd00 | `global_sub0043cd20_0043cd20.cpp` |
| 0043cd30 | 0043cd00 | `global_sub0043cd30_0043cd30.cpp` |
| 0043cd40 | 0043cd00 | `global_sub0043cd40_0043cd40.cpp` |
| 0043cf50 | 0043ce90 | `global_sub0043cf50_0043cf50.cpp` |
| 0043cf60 | 0043ce90 | `global_sub0043cf60_0043cf60.cpp` |
| 0043cf70 | 0043ce90 | `global_sub0043cf70_0043cf70.cpp` |
| 0043cf80 | 0043ce90 | `global_sub0043cf80_0043cf80.cpp` |
| 0043cf90 | 0043ce90 | `global_sub0043cf90_0043cf90.cpp` |
| 0043cff0 | 0043cfd0 | `global_sub0043cff0_0043cff0.cpp` |
| 0043dff0 | 0043dfc0 | `global_sub0043dff0_0043dff0.cpp` |
| 0043e000 | 0043dfc0 | `global_sub0043e000_0043e000.cpp` |
| 0043f0b0 | 0043f050 | `global_sub0043f0b0_0043f0b0.cpp` |
| 0043f0c0 | 0043f050 | `global_sub0043f0c0_0043f0c0.cpp` |
| 0043f0d0 | 0043f050 | `global_sub0043f0d0_0043f0d0.cpp` |
| 0043f0e0 | 0043f050 | `global_sub0043f0e0_0043f0e0.cpp` |
| 0043f0f0 | 0043f050 | `global_sub0043f0f0_0043f0f0.cpp` |
| 0043f100 | 0043f050 | `global_sub0043f100_0043f100.cpp` |
| 0043f190 | 0043f160 | `global_sub0043f190_0043f190.cpp` |
| 0043f1a0 | 0043f160 | `global_sub0043f1a0_0043f1a0.cpp` |
| 0043f1b0 | 0043f160 | `global_sub0043f1b0_0043f1b0.cpp` |
| 0043f1c0 | 0043f160 | `global_sub0043f1c0_0043f1c0.cpp` |
| 0043f1d0 | 0043f160 | `global_sub0043f1d0_0043f1d0.cpp` |
| 0043f1e0 | 0043f160 | `global_sub0043f1e0_0043f1e0.cpp` |
| 0043f1f0 | 0043f160 | `global_sub0043f1f0_0043f1f0.cpp` |
| 0043f200 | 0043f160 | `global_sub0043f200_0043f200.cpp` |
| 0043f210 | 0043f160 | `global_sub0043f210_0043f210.cpp` |
| 0043f220 | 0043f160 | `global_sub0043f220_0043f220.cpp` |
| 0044aab0 | 0044aa60 | `global_sub0044aab0_0044aab0.cpp` |
| 0044aaf0 | 0044aa60 | `global_sub0044aaf0_0044aaf0.cpp` |
| 0044ab00 | 0044aa60 | `global_sub0044ab00_0044ab00.cpp` |
| 0044eebd | 0044ebe4 | `global_sub0044eebd_0044eebd.cpp` |
| 0044eec2 | 0044ebe4 | `global_sub0044eec2_0044eec2.cpp` |
| 0044eec7 | 0044ebe4 | `global_sub0044eec7_0044eec7.cpp` |
| 0044eecc | 0044ebe4 | `global_sub0044eecc_0044eecc.cpp` |
| 0044eed1 | 0044ebe4 | `global_sub0044eed1_0044eed1.cpp` |
| 0044eed6 | 0044ebe4 | `global_sub0044eed6_0044eed6.cpp` |
| 0044eedb | 0044ebe4 | `global_sub0044eedb_0044eedb.cpp` |
| 0044eee0 | 0044ebe4 | `global_sub0044eee0_0044eee0.cpp` |
| 0044f03c | 0044ebe4 | `global_sub0044f03c_0044f03c.cpp` |
| 0044f08e | 0044ebe4 | `global_sub0044f08e_0044f08e.cpp` |
| 0044f1bb | 0044ebe4 | `global_sub0044f1bb_0044f1bb.cpp` |
| 0044f24f | 0044ebe4 | `global_sub0044f24f_0044f24f.cpp` |
| 0044f254 | 0044ebe4 | `global_sub0044f254_0044f254.cpp` |
| 0044f40b | 0044f3de | `global_sub0044f40b_0044f40b.cpp` |
| 0044f410 | 0044f3de | `global_sub0044f410_0044f410.cpp` |
| 0044f415 | 0044f3de | `global_sub0044f415_0044f415.cpp` |
| 0044f41a | 0044f3de | `global_sub0044f41a_0044f41a.cpp` |
| 0044f41f | 0044f3de | `global_sub0044f41f_0044f41f.cpp` |
| 0044f424 | 0044f3de | `global_sub0044f424_0044f424.cpp` |
| 0044f429 | 0044f3de | `global_sub0044f429_0044f429.cpp` |
| 0044f42e | 0044f3de | `global_sub0044f42e_0044f42e.cpp` |
| 0044f433 | 0044f3de | `global_sub0044f433_0044f433.cpp` |
| 0044f438 | 0044f3de | `global_sub0044f438_0044f438.cpp` |
| 0044f43d | 0044f3de | `global_sub0044f43d_0044f43d.cpp` |
| 0044f442 | 0044f3de | `global_sub0044f442_0044f442.cpp` |
| 0044f447 | 0044f3de | `global_sub0044f447_0044f447.cpp` |
| 0044f44c | 0044f3de | `global_sub0044f44c_0044f44c.cpp` |
| 0044f451 | 0044f3de | `global_sub0044f451_0044f451.cpp` |
| 0044f456 | 0044f3de | `global_sub0044f456_0044f456.cpp` |
| 00451bc4 | 00451b82 | `global_sub00451bc4_00451bc4.cpp` |
| 00451bc5 | 00451b82 | `global_sub00451bc5_00451bc5.cpp` |
| 00451bc6 | 00451b82 | `global_sub00451bc6_00451bc6.cpp` |
| 00451bfc | 00451b82 | `global_sub00451bfc_00451bfc.cpp` |
| 00451c25 | 00451b82 | `global_sub00451c25_00451c25.cpp` |
| 00451c26 | 00451b82 | `global_sub00451c26_00451c26.cpp` |
| 00451c27 | 00451b82 | `global_sub00451c27_00451c27.cpp` |
| 00451c28 | 00451b82 | `global_sub00451c28_00451c28.cpp` |
| 00451c29 | 00451b82 | `global_sub00451c29_00451c29.cpp` |
| 00451c2a | 00451b82 | `global_sub00451c2a_00451c2a.cpp` |
| 00451c39 | 00451b82 | `global_sub00451c39_00451c39.cpp` |
| 00451c3a | 00451b82 | `global_sub00451c3a_00451c3a.cpp` |
| 00451c3b | 00451b82 | `global_sub00451c3b_00451c3b.cpp` |
| 00451c3c | 00451b82 | `global_sub00451c3c_00451c3c.cpp` |
| 00451c3d | 00451b82 | `global_sub00451c3d_00451c3d.cpp` |
| 0045948d | 00459471 | `global_sub0045948d_0045948d.cpp` |
| 0045cdf0 | 0045cde2 | `global_sub0045cdf0_0045cdf0.cpp` |
| 00461b82 | 00461b5c | `global_sub00461b82_00461b82.cpp` |
