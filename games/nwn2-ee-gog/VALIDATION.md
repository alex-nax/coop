# NWN2 validation — 2026-09-13

| Check | Result / evidence |
| --- | --- |
| GOG version | 521755.27, build 92708 |
| Game executable | MD5 AAADCD6079527807653DDEBF84D1B3C2; no persistent executable patch |
| Nucleus | 2.4.2 |
| Split/controllers | Two 1920×2160 views, left/right; user confirmed independent gamepads |
| Service initialization | IsLoggedOn naturally returned 1; AML setup/login-start succeeded; EOS local login succeeded |
| Multiplayer menu | User: “Multiplayer menu opens” |
| Host discovery and join | User: “It appears and joins” |
| Isolated adapter probe | Auth callback, logged-on state, 107-byte emulated ticket and achievement set/get/clear succeeded |
| Player storage | Separate stable native -home paths for Player1 and Player2 |
| Long campaign / save-reload | Not separately verified |
| Hub review | Not submitted or officially verified |

The four exact runtime library hashes are in [dependencies.json](dependencies.json). The last installed successful handler before its release-description update had SHA-256 `1e2407ca58e6a0240237e0eb28f34e74c95feba1f1f3fd146d22fd23c70e768d`. The package manifest records the updated source hash. Runtime logic and save paths are preserved.

Raw traces and screenshots remain private in the legacy research archive. The public [ABI diagnosis](../../docs/nwn2-galaxy-abi.md) records the key evidence and bounded conclusions without redistributing game binaries or authentication material.

Repository preparation verification: the packaged dependency helper passed two successive runs using the preserved private cache, then verified all four hashes. A freshly compiled isolated probe passed against the packaged tested adapter/backend without launching a game. The upstream download branch was inspected but was not exercised in this repeat test; upstream release filenames and final DLL hashes are pinned.
