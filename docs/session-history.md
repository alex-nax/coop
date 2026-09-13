# Development history and handoff

## 2026-09-12: System Shock 2 Remaster

Prepared the GOG installer and Nucleus on macOS, copied them to the Windows user's home and installed the game. The user requested two gamepads and top/bottom split. The first launch routed only the first pad. Proto Input OpenXinput plus SDL XInput-only configuration resolved this, and the user confirmed the updated session and began playing. Further invasive checks stopped at the user's request. The user confirmed SS2's connection test on 2026-09-13 during repository preparation.

## 2026-09-13: Neverwinter Nights 2 Enhanced Edition

Waited for all `dnd_` GOG download parts on Windows. Prepared an independent workspace. The user requested direct executable launch through Nucleus, not Galaxy, and changed the preferred layout to left/right. Both controllers were confirmed independent.

The bundled Steam replacement lacked a required initialization export. The modern pinned replacement solved that stage. Multiplayer still reported service unavailable. Runtime tracing identified a false native GOG IsLoggedOn result; a one-time bypass merely moved failure to a platform-authentication timeout. The correct Galaxy API version did not resolve C++ ABI mismatches by itself.

An isolated probe exposed the SignInGalaxy timeout/listener signature mismatch and inserted IUser slot. A custom adapter fixed them. A second crash exposed three extra IStats methods in the actual SDK binary; its public header was older. Correcting the IStats map allowed natural local login. The user confirmed the multiplayer menu, host visibility and successful joining. Final runtime configuration suppressed continuous EOS logs and preserved the game executable and SS2 registration.

## Repository consolidation

The requested `~/coop` repository centralizes public source, human docs, agent orientation and repeatable package tooling. The Windows home has a corresponding workspace. Old installers, raw research and player paths remain preserved and reachable through private legacy links. Hub preparation includes AI disclosure, validation scope, pinned dependency setup and two `.nc` archives. Actual Hub publication has not been performed.

See each game's `VALIDATION.md` for the authoritative supported scope. Future work should start from the current handler and dependency manifest, not one of the earlier diagnostic snapshots.

---

Source and updates: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human testing is documented per game. Unofficial community project; no claim of official Hub verification.
