# Original diagnostic tools

These sources were made with AI assistance and are preserved for maintenance. They are not launched by normal play or bootstrap and are not substitutes for a real game test.

`galaxy_probe.c` is an isolated emulator test. `compat` exercises the corrected Galaxy signature/slots, auth callback, logged-on state, ticket retrieval and achievement set/get/clear. `old` and `new` are historical ABI reproduction modes; the mismatched `new` mode can deliberately crash the old backend. Use a private directory and never the game install or a player's save folder.

Build with `./scripts/build-native.sh`. In a fresh Windows directory, place `galaxy_probe.exe`, the adapter as `Galaxy64.dll`, and the pinned backend as `NemirtingasGalaxy64.dll`. Add `ngalaxye_settings/NemirtingasGalaxyEmu.json`:

```json
{"api_version":"1.152.10.0","disable_online_networking":true,"enable_lan":true,"log_level":"warn","enable_overlay":false,"galaxyid":145496244628982949,"language":"en","productid":1993442013,"username":"CoopAbiProbe"}
```

Run `./galaxy_probe.exe compat` with that directory as the working directory. Expect OnAuthSuccess, IsLoggedOn=1, STATS_RESULT unlocked=1, ticket callback success, ticket length 107 and exit 0. Do not print ticket contents. Probe state stays in that directory.

`service_trace.c` is a version-specific Windows debugger. It attaches to one PID and uses one-shot breakpoints at known RVAs for the tested GOG executable. Read the source and [diagnosis](../../docs/nwn2-galaxy-abi.md) before use. Verify the executable build/hash and selected PID first. The historical `--bypass-once` option changes a login result in memory for diagnosis; it is not a normal launch fix. Only attach during an agreed diagnostic test, never during ongoing play or to unrelated processes. After exit independently verify debugger detachment and original breakpoint bytes.

The initial isolated probe was repeated during repository consolidation without opening a game window. The source build and preserved tested adapter are distinct artifacts; use their recorded hashes when comparing results.
