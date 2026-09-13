# Troubleshooting

| Symptom | Evidence / next action |
| --- | --- |
| Only the first gamepad controls a window | Confirm both pads are awake in Windows, then check OpenXinput and SDL backend hints. Reassign pads after reconnecting. |
| Both pads affect one window | Check per-instance assignment and competing SDL/input hooks; test one pad at a time. |
| A process exists but no window appears | Check whether SSH started it on a service desktop. Use the interactive Windows session and inspect startup logs. |
| `SteamInternal_SteamAPI_Init` missing in NWN2 | Restore the pinned modern Steam API library with Prepare-Assets; the older Nucleus copy lacks the export. |
| NWN2 service unavailable | Confirm all four pinned libraries and Galaxy API version; follow the staged ABI diagnosis. An IsLoggedOn bypass is not the release fix. |
| NWN2 crash reading address `0xF` | New SignInGalaxy timeout interpreted as the old listener pointer; check adapter/backend pairing. |
| NWN2 callback crash with ASCII achievement bytes | Check the three-slot IStats shift; an unadapted old method treated a string as a listener. |
| NWN2 early startup with non-ASCII Documents path | Verify Nucleus player environments and original Documents backup/restoration; do not permanently relocate OneDrive. |
| Nucleus stops starting after settings edit | Check file encoding; write Settings.ini as UTF-8 without BOM and restore the last working backup. |
| Logs grow continuously | Restore `EOSLAN_LOG_PATH=NUL` after bounded diagnosis. |
| Existing library entry disappears after registration | Merge only the owned GUID. Avoid reusing `$library` for an asset loop variable; a historical script had that collision. |
| Game files changed after library replacement | Check symlinks: delete the instance link before copying a DLL. Compare base install hashes to backup. |

The original SS2 installer logged success and then crashed in cleanup; subsequent game launches and user connection testing succeeded. Inspect the install log and actual executable before assuming the installation failed. Conversely, an installer process exiting does not alone prove that every download part or game file is valid.

Old scripts in `storage/legacy/` include force-stop and experimental rollback actions. They are evidence, not the current operational interface. A script named “restore baseline” can restore a pre-fix library combination; use the dependency manifest and current handler as the release baseline.

---

Source and updates: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human testing is documented per game. Unofficial community project; no claim of official Hub verification.
