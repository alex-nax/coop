# Agent instructions

## Start here

1. Confirm `pwd`, then read `Codex-progress.md` and `features.json`.
2. Read the relevant game README, validation record and source sidecar before editing.
3. Run `./init.sh`. Use `docs/architecture.md` for boundaries and `docs/adding-games.md` for the workflow.
4. Work on the user's requested scope; choose one coherent feature at a time. Preserve feature descriptions and acceptance criteria. Mark `passes` true only with evidence.
5. Run applicable local checks. Windows controller/network claims require Windows testing and, where needed, explicit human observation.
6. Update game validation, progress and feature status; leave reviewable changes and commit completed work. Never push or publish without authorization.

## Operating constraints

- macOS is the authoring machine; Windows is the execution target. Read `local/machines.json` and `local/MACHINE.md` for this installation; neither is public.
- Check running processes before deployment. Do not close games, restart Nucleus, change the registry, alter displays or disconnect pads during play. User permission to investigate is not permission to interrupt unrelated gameplay.
- Keep each game isolated. Preserve GUIDs, saved player folders, user settings, installed libraries and the other game's registration. Back up owned files before replacing them.
- Copy runtime libraries into instance directories only. Delete an instance symlink before writing its replacement; writing through it can corrupt the original game.
- Do not patch game executables or keep a login-check bypass as a normal launch requirement. NWN2 uses a bounded Galaxy ABI adapter; read its design before changing it.
- Never commit proprietary installers/game binaries, saves, private configuration, raw screenshots/traces, credentials or fetched dependencies. Use `storage/`, `local/`, `build/` and `dist/`.
- Scripts must be parameterized, fail clearly and leave Windows settings intact. PowerShell is Windows PowerShell 5.1 compatible; use UTF-8 without BOM for Nucleus settings.
- For GUI work use the Windows interactive session, not the SSH service desktop. Capture only the relevant application window and obtain human observation when remote visibility is insufficient.
- Maintain `._llm.json` beside moved/edited source; review notes, repair anchors, then stamp with the llm-sidecar tool if available. Put cross-file explanations in docs.
- Keep AI-assisted development disclosure in README, each game guide, handler description and Hub release notes. Never imply official Hub verification or tests that did not occur.
- Do not delegate to other agents unless requested. Avoid generic environment/security changes, broad process termination, automatic public uploads and account sign-ins.

## Commands and evidence

`./init.sh` checks metadata, links and handler behavior with a mocked Nucleus context. `./scripts/build-native.sh` builds the adapter and diagnostics. `python3 scripts/package.py` makes allowlisted Hub archives; `python3 scripts/validate.py --packages` checks their structure and content. `python3 scripts/sync_windows.py` mirrors public source plus packages using ignored machine configuration.

Local simulation is not an end-to-end game test. Follow `docs/testing.md` for Windows validation. Record exact versions/hashes, controller isolation, window layout, host/join results, save separation and cleanup. Long campaign/save-load tests are still open in `features.json`.
