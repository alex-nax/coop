# Storage and saves

The new checkout centralizes source and documentation. It does not relocate live player data or game installations. Existing desktop shortcuts remain valid.

| Data | Location / policy |
| --- | --- |
| Source and docs | `~/coop` on Mac; the configured `coop` folder in the Windows user's home |
| Build outputs / upload packages | `build/`, `dist/`; ignored, reproducible |
| Pinned dependency cache | `storage/assets/nwn2-ee-gog/`; ignored |
| Original installers and raw investigations | Existing workspaces, reachable through `storage/legacy/` links |
| Private addresses and exact paths | `local/machines.json`, `local/MACHINE.md`; ignored |
| Nucleus runtime | Existing Nucleus installation, outside this repo |
| SS2 player data | `<Nucleus>\players\SS2RemasterGOG\Player1` and `Player2` |
| NWN2 player data | `%USERPROFILE%\nwn2-coop\players\Player1` and `Player2` |
| Saved Nucleus layouts | `<Nucleus>\game profiles\<GUID>`; private, hardware-specific |

Keep NWN2's old player-home folder name: the tested handler uses it and existing saves live there. Folder aesthetics are not a reason to risk saves. A future migration should back up both players, move data only while the session is closed, and preserve the old path with an explicitly reviewed junction or a fallback in the handler. The same principle applies to SS2's Nucleus-relative save directories.

Before changes, close the affected game normally and copy its two player folders, game profiles, owned handler/assets and relevant Nucleus registration JSON to a timestamped private backup. Nucleus's symlinked instance tree is not the authoritative save backup. Avoid recursive tools that follow legacy links and duplicate the entire archive.

Controller layout JSON contains device identifiers and monitor geometry. Keep it private. Public instructions should explain assigning pads and selecting halves of a screen; handler dimensions come from `Context.Width` and `Context.Height`.

The mirror script updates public files without deleting unrelated files. Deleted source may remain in an existing Windows mirror until reviewed cleanup; it never performs destructive `/MIR` synchronization. Runtime installation is an explicit later operation.
