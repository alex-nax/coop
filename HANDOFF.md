# Handoff — NWN2 automatic setup and Hub publication

Updated 2026-09-13. Source: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human gameplay testing and untested development work are distinguished below. This is unofficial community work; no claim of official Hub verification.

## Resume here

The owner asked to stop development, save this handoff and push it to `main`. **NWN2 upload is deferred; the owner will upload later.** The next development task is to automate dependency preparation through Nucleus so players do not need to run PowerShell manually.

The tested `0.1.0` handler remains the active source. The unfinished changes are preserved in [the automatic-setup draft patch](docs/drafts/nwn2-automatic-setup.patch). They were not deployed to Windows, built into a package, or tested in Nucleus. Apply the draft only when resuming development; it is not a release artifact.

```sh
cd ~/coop
cat AGENTS.md Codex-progress.md HANDOFF.md
./init.sh
# When ready to resume implementation:
git apply --check docs/drafts/nwn2-automatic-setup.patch
git apply docs/drafts/nwn2-automatic-setup.patch
```

Review the relevant source sidecars before applying. After changes, update their notes/anchors and stamp them using the llm-sidecar tool. The patch deliberately contains source changes only; version bumps, tests, sidecar maintenance and revised installation docs still need doing.

## Hub state verified in Chrome

| Game | Entry | Last observed state |
| --- | --- | --- |
| SS2 Remaster GOG | [FFBhS68GK5T7u2CEP](https://hub.splitscreen.me/handler/FFBhS68GK5T7u2CEP) | Owner uploaded Hub version 1 with the v0.1.0 note. Download link and version history exist. Public sharing is checked; status is **pending validation**, and the page says **Unverified**. |
| NWN2 EE GOG | [TEWAqPYPA5KksWHYN](https://hub.splitscreen.me/handler/TEWAqPYPA5KksWHYN) | Owner created the entry. Title, full description, two-player controller support and no keyboard/mouse support were saved. **Version 0: no package uploaded.** |

The owner reviewed SS2 and approved proceeding with NWN2; that review gate is satisfied. Preserve the owner's final SS2 description. Both pages include GitHub source links and an explicit AI disclaimer, which must remain in future descriptions, packages and release notes.

The NWN2 page still describes manual `Prepare-Assets.ps1` setup, matching the current 0.1.0 package. Do not change it to promise automatic setup until the new implementation is validated. A short release note was filled in the browser but was not submitted; do not assume unsaved form state survives.

Chrome control now works with the updated bundled Chrome plugin. Attaching a local archive still returned `Not allowed`. The supported remedy is **ChatGPT browser extension > Details > Allow access to file URLs** at `chrome://extensions`, or owner-selected upload. The owner chose to upload later. Do not spend the next session troubleshooting uploads before completing the automatic setup work.

## What the draft does

- At the beginning of `Game.Play`, player 0 runs the packaged helper synchronously through Windows PowerShell. It passes `Context.NucleusFolder`, writes `setup.log` in the handler asset folder, and stops with an error if setup fails.
- PowerShell command text is encoded as UTF-16LE/Base64; literal path arguments escape apostrophes. No `cmd.exe` shell is used.
- The helper adds `-FromNucleus`, allowing preparation from Nucleus while retaining the standalone process guard. Already-correct assets are reused with no downloads. Missing or incorrect assets cannot be repaired while NWN2 is running.
- The helper tries Windows `tar.exe` for the upstream `.7z`, keeps the existing 7-Zip fallback/manual override, and adds a download timeout. ZIP extraction still uses `Expand-Archive`.
- The package builder gains `--game` so a NWN2 rebuild need not replace the already-uploaded SS2 archive. Its NWN2 installation text changes to automatic preparation.

This remains a draft. It has not passed the existing handler simulation: the current mock has no `System.Diagnostics.Process`, encoding or conversion APIs. No test has been added yet. In particular, confirm the actual Nucleus Jint runtime supports the .NET calls and numeric comparisons used in the draft. The handler must wait for successful preparation before copying any instance DLLs.

## Next implementation and validation steps

1. Apply/review the draft and add meaningful tests for first-player setup, waiting before file copies, failure stopping launch, second-player behavior, spaces/apostrophes in paths, and cached repeat runs. Extend the current mock without treating mocked success as Windows evidence.
2. Exercise the helper in an **isolated Windows folder** with the original adapter and dependency manifest. Test fresh upstream downloads, archive extraction, final hashes, cached/no-network reuse, mismatched backend, corrupted/missing dependency, failure logging and retry. Avoid installed handler or save directories for these checks.
3. Test the PowerShell launch path with Nucleus 2.4.2's actual Jint/.NET runtime. Consider a temporary harness loading its Jint DLL and executing the setup portion without launching games. Confirm that `Game.Play` exceptions stop startup and follow normal Nucleus cleanup; review the lack of an overall process timeout and the extraction failure fallback.
4. When the Windows PC is idle, back up and deploy only the NWN2 handler/assets, then test a clean import and normal Play. Confirm two windows, independent pads, Multiplayer, host discovery/join, stable saves and normal exit. Ask for human observation where needed; do not interrupt gameplay.
5. Use a new NWN2 package version, proposed `0.2.0`, since its setup behavior changes. Update `games/index.json`, `release.json`, README, VALIDATION, Hub text, package docs, THIRD_PARTY and the submission guide. Keep the exact tested runtime DLL hashes, GitHub links and AI disclaimer.
6. Repair/stamp source sidecars, run local checks and Windows validation, then build **only NWN2** with the draft's `python3 scripts/package.py --game nwn2-ee-gog`. Validate the archives and inspect the resulting installation instructions. Preserve SS2's already-uploaded bytes.
7. Update the NWN2 Hub description only after the new behavior is verified. Leave upload to the owner as requested. After they upload, verify actual version/public-sharing/moderation status and record it; do not equate upload with official verification.

## Technical findings to retain

The [Nucleus API reference](https://www.splitscreen.me/docs/handler-api/) documents handler execution and path access. Local Nucleus source inspection confirmed `GenericGameInfo.PrePlay` invokes `Game.Play` synchronously before launch in the normal path. `Context.ScriptFolder` resolves to the per-handler asset folder in the inspected source; the public path documentation is less precise. Use the tested context path rather than hardcoding an import folder.

The owner mentioned Skyrim bundling its mod. The current public [Skyrim Together Reborn handler](https://hub.splitscreen.me/handler/PPct546raDXfjRLGw) instead tells users to install the mod into the main game first. That does not prevent NWN2 from automatically provisioning its own dependencies. Our `.nc` layout already supports the original adapter, helper and manifests.

The last read-only Windows check reported **bsdtar 3.8.8 with liblzma 5.8.1**. No Nucleus, NWN2 or SS2 game process appeared in that check. Archive extraction was not actually exercised, and this does not prove the machine is idle in a later session.

Keep third-party dependencies fetched from their pinned upstream releases or copied from the exact installed Nucleus backend. The package currently redistributes only the original adapter. [THIRD_PARTY.md](THIRD_PARTY.md) explains provenance; no new redistribution decision was made. [dependencies.json](games/nwn2-ee-gog/dependencies.json) remains authoritative for hashes.

## Existing evidence and artifacts

The 0.1.0 gameplay path was human-tested: SS2 independent controllers and LAN connection; NWN2 independent controllers, Multiplayer menu, host discovery and joining. Extended campaign/save-reload testing remains open for both. No executable patch or IsLoggedOn bypass is required by the tested NWN2 solution. See [NWN2 validation](games/nwn2-ee-gog/VALIDATION.md) and [Galaxy ABI diagnosis](docs/nwn2-galaxy-abi.md).

Before the automation draft, `./init.sh` and `python3 scripts/validate.py --packages` passed. The active source is restored to that tested implementation for this handoff. Bootstrap, package validation, draft application check and source-sidecar validation passed again before the handoff commit. No new gameplay or dependency-download test occurred during the paused implementation.

Existing ignored archives remain version 0.1.0. **There is no 0.2.0 archive yet.** NWN2 0.1.0 requires manual setup and is not the requested final automatic-setup release.

| Local artifact | SHA-256 |
| --- | --- |
| `dist/ss2-remaster-gog-0.1.0.nc` | `bfe135c36dad73a58b80ac65ec1856faef99912012c6a6bbb7f293723cdf8779` |
| `dist/nwn2-ee-gog-0.1.0.nc` | `fcaf3f6a61ee514be8287ca9dac65d920e0272a84c971a1097915ac9c8ece05b` |

These hashes describe the local prepared archives, not an independently downloaded comparison against the owner's SS2 upload. Do not rebuild SS2 casually: the package builder includes shared docs, so even documentation changes alter archive bytes.

The canonical checkout is `~/coop`; legacy storage remains `~/ss2-coop` through ignored links. Private SSH/Windows paths are in `local/machines.json` and `local/MACHINE.md`; read [Windows/SSH operations](docs/windows-ssh.md) before execution. The Windows source mirror and installed runtime were not changed during this automation draft. Stable player save paths remain as documented in [storage and saves](docs/storage-and-saves.md).
