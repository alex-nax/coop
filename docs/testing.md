# Validation

## Local checks

Run `./init.sh` after source changes. It validates catalog/feature metadata and documentation links, checks handler JavaScript syntax and simulates two player contexts. The simulation checks distinct player storage/identities/ports, dimensions and private DLL replacement. This catches configuration regressions but cannot prove controller hooks or real networking.

Build original native code with `./scripts/build-native.sh`; warnings are errors. Run `python3 scripts/package.py` and `python3 scripts/validate.py --packages` to verify root `handler.js`, explicit asset directory entries, archive paths, allowed files, disclosure and artifact hashes. Third-party game files must never enter the package.

## Windows checks

Before changing runtime files, inspect running processes. If a user is playing, complete source/doc/package work and defer runtime replacement. Back up the affected handler, dependencies and player data when the session is closed.

For native adapter changes, first run the isolated [Galaxy probe](../tools/diagnostics/README.md) in a private temporary directory with the exact pinned backend and copied settings. This can run without launching or attaching to the game. It must report auth success, logged-on state, ticket callback success and achievement set/get/clear. A successful probe does not replace the two-window game test.

For each game, record:

1. Exact GOG build, Nucleus version and relevant library hashes.
2. Two visible windows with the requested split and separate player paths.
3. Pad 1 affects only player 1; pad 2 affects only player 2, including background input.
4. Multiplayer menus open, host starts, client discovers/connects and both remain responsive.
5. Both players enter gameplay; save, close normally, restart and load the intended player data.
6. Nucleus exits cleanly, the original Documents setting is restored where applicable, and the other game's files/registration remain intact.

The initial validation records distinguish completed human observations from open extended/save-load tests. Record the user's exact observation when possible. Do not upgrade “both windows launched” into “co-op works,” or a menu joining test into a long campaign validation.

Store raw PIDs, device IDs, screenshots, traces and command lines privately. Public validation should include the tested versions, relevant hashes, method and result, without machine identifiers or proprietary binary contents.

The reusable Windows packaging check is `scripts/windows/Test-Preparation.ps1 -CacheDirectory <pinned-DLL-folder> -ProbeExecutable <galaxy_probe.exe>`. It syntax-checks PowerShell, extracts the NWN2 package into private storage, runs setup twice, verifies hashes and optionally runs the isolated probe. The initial consolidation used the cache branch; it did not re-download the upstream releases.
