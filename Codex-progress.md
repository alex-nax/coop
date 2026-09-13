# Progress log

## Session 2 — 2026-09-13
**Agent**: OpenAI Codex, Hub publication preparation
**Summary**: Identified the owner-created SS2 Hub entry, added GitHub source links alongside AI disclosure in both handler descriptions, public game/release documentation and package manifests. The owner authorized SS2 publication and asked to review it before NWN2 EE proceeds.
**Features completed**: #24. Both archives rebuilt and validated; every packaged Markdown document, handler description, Hub description/release note and manifest contains the repository link and AI disclosure.
**Known issues**: Browser integration was selected and sign-in is complete in the regular browser, but the available browser runtime still reports no backend. No authenticated Hub changes were made. The public SS2 record last showed version 0, private=true and publicAuthorized=false. Its initial metadata incorrectly enables keyboard/mouse support; set it to None for this two-gamepad handler. First upload must precede requesting public availability; administrator authorization is separate.
**Next suggested task**: Resume SS2 Edit & Manage when browser control is available, or use the prepared review document for the owner to submit. Do not proceed to NWN2 publication until the owner reviews SS2.

---

## Session 1 — 2026-09-13
**Agent**: OpenAI Codex, initialization and consolidation
**Summary**: Created the dedicated coop repository from the two successful GOG handlers. Added agent orientation, a 23-item feature tracker, idempotent bootstrap, detailed game/SSH/storage/controller/networking/ABI docs, AI disclosure, templates and two Hub archives. Mirrored public files and packages to the new Windows home folder, preserving legacy archives and live save paths. The owner supplied `git@github.com:alex-nax/coop.git` for the source repository.
**Features completed**: #1–20. Local bootstrap passed twice; both two-player handler simulations passed; native sources compiled with warnings as errors; source sidecars validated; archives and internal hashes/links validated. Windows PowerShell syntax, isolated packaged asset setup (including repeat run), and Galaxy auth/ticket/achievement probe passed. Handler runtime logic differs from the tested versions only in user-facing descriptions. Installed handlers and game executable were preserved.
**Known issues**: #21–22: extended campaign/save-reload tests remain open. #23: Hub upload/review is an owner action and has not occurred. NWN2 requires its one-time pinned dependency setup. Freshly compiled adapter packages are labelled as needing Windows validation; initial archives use the preserved tested adapter. No GitHub Release or Hub publication is created by this source push.
**Next suggested task**: Upload the prepared archives using docs/hub-submission.md, or add the next requested game using docs/adding-games.md. Do not interrupt an existing game session to complete optional tests.

---
