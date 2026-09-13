# Progress log

## Session 1 — 2026-09-13
**Agent**: OpenAI Codex, initialization and consolidation
**Summary**: Created the dedicated coop repository from the two successful GOG handlers. Added agent orientation, a 23-item feature tracker, idempotent bootstrap, detailed game/SSH/storage/controller/networking/ABI docs, AI disclosure, templates and two Hub archives. Mirrored public files and packages to the new Windows home folder, preserving legacy archives and live save paths. The owner supplied `git@github.com:alex-nax/coop.git` for the source repository.
**Features completed**: #1–20. Local bootstrap passed twice; both two-player handler simulations passed; native sources compiled with warnings as errors; source sidecars validated; archives and internal hashes/links validated. Windows PowerShell syntax, isolated packaged asset setup (including repeat run), and Galaxy auth/ticket/achievement probe passed. Handler runtime logic differs from the tested versions only in user-facing descriptions. Installed handlers and game executable were preserved.
**Known issues**: #21–22: extended campaign/save-reload tests remain open. #23: Hub upload/review is an owner action and has not occurred. NWN2 requires its one-time pinned dependency setup. Freshly compiled adapter packages are labelled as needing Windows validation; initial archives use the preserved tested adapter. No GitHub Release or Hub publication is created by this source push.
**Next suggested task**: Upload the prepared archives using docs/hub-submission.md, or add the next requested game using docs/adding-games.md. Do not interrupt an existing game session to complete optional tests.

---
