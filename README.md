# Co-op handler workshop

A home for developing, testing and sharing Nucleus Co-op handlers. Source and documentation live on macOS; games run on a Windows test PC. Start here when adding a game or returning after a break. Current paused work: [NWN2 automatic-setup handoff](HANDOFF.md).

**AI disclosure:** These handlers, the NWN2 compatibility adapter, scripts and documentation were made with AI assistance using OpenAI Codex. A human tested the games and reported the results below. AI assistance is not a guarantee of correctness or official Hub verification. This is an unofficial community project, unaffiliated with SplitScreen.Me, GOG, or the game publishers. Third-party tools retain their own authorship and licenses.

## Game index

| Game / tested distribution | Players and layout | Confirmed by human testing | Guide | Hub package |
| --- | --- | --- | --- | --- |
| System Shock 2: 25th Anniversary Remaster — GOG 1.3, build 89551 | 2 XInput pads; top/bottom | Launch, independent controllers, LAN connection | [Play and maintain](games/ss2-remaster-gog/README.md) | `dist/ss2-remaster-gog-0.1.0.nc` |
| Neverwinter Nights 2 Enhanced Edition — GOG 521755.27, build 92708 | 2 XInput pads; left/right | Launch, independent controllers, multiplayer menu, host discovery and joining | [Play and maintain](games/nwn2-ee-gog/README.md) | `dist/nwn2-ee-gog-0.1.0.nc` |

Both were tested with Nucleus Co-op 2.4.2 on a 3840×2160 Windows display. Extended campaign progression, save/reload across sessions, other game builds, four players and internet matchmaking are not verified. These are initial community releases, not official Hub-verified handlers. The [SS2 Hub entry](https://hub.splitscreen.me/handler/FFBhS68GK5T7u2CEP) has Hub version 1 uploaded and public sharing requested; administrator validation is pending. The [NWN2 Hub entry](https://hub.splitscreen.me/handler/TEWAqPYPA5KksWHYN) has its description saved, with upload deferred while automatic setup is developed. The machine-readable index is [games/index.json](games/index.json).

## Play

On the existing Windows PC, the installed Nucleus handlers and old shortcuts continue to work. Connect both pads, open Nucleus and select the game. Assign each pad to its half of the screen and press Play. Read the game guide before creating a multiplayer session.

For a new installation, install your own GOG game and Nucleus, import the appropriate `.nc` package, and add the game's executable. NWN2 needs its included **Prepare-Assets.ps1** once before the first launch; [its guide](games/nwn2-ee-gog/README.md#install-from-a-hub-package) gives the command. Do not launch through Galaxy. Both handlers launch the game executable through Nucleus.

## Work on handlers

```sh
cd ~/coop
./init.sh
./scripts/build-native.sh
python3 scripts/package.py
python3 scripts/validate.py --packages
```

The bootstrap needs Python 3 and Node.js. It creates ignored working directories and runs local checks; it does not start a game, contact Windows or install dependencies. The native build also requires the MinGW x64 cross compiler. See [Mac setup](docs/local-setup.md).

`dist/` contains the two Hub archives, checksums and a submission checklist after packaging. Commit source changes to this dedicated Git repository. The source remote is [alex-nax/coop](https://github.com/alex-nax/coop). Use `git push` after reviewing the staged diff; Hub publication is a separate step.

## Find your way

| Need | Read |
| --- | --- |
| Agent orientation and session handoff | [AGENTS.md](AGENTS.md), [progress](Codex-progress.md), [feature tracker](features.json) |
| Add the next game | [Development recipe](docs/adding-games.md), [game template](templates/game/README.md) |
| Understand repository boundaries | [Architecture](docs/architecture.md) |
| SSH into Windows and run GUI work correctly | [Windows / SSH operations](docs/windows-ssh.md) |
| Locate or back up saves and installers | [Storage and saves](docs/storage-and-saves.md) |
| Solve controller and focus problems | [Input and layout](docs/input-and-layout.md) |
| Diagnose multiplayer | [Networking](docs/networking.md), [NWN2 Galaxy ABI diagnosis](docs/nwn2-galaxy-abi.md) |
| Reproduce test evidence | [Validation procedure](docs/testing.md), [diagnostic tools](tools/diagnostics/README.md) |
| Submit to the Hub | [Submission guide](docs/hub-submission.md) |
| Compare Splitux and Nucleus | [Comparison](docs/splitux-vs-nucleus.md) |
| Understand previous experiments | [Session history](docs/session-history.md), [troubleshooting](docs/troubleshooting.md) |

## Storage rules

Track handlers, original source, dependency metadata, scripts, tests and human-readable documentation. Keep GOG installers, game files, saves, controller IDs, machine addresses, screenshots and raw traces in ignored `storage/` or `local/`. Generated packages and DLLs belong in ignored `dist/` and `build/`. Existing research and installers remain reachable through `storage/legacy/` on each machine; relocation does not change live save paths.

Never copy a whole legacy workspace into a public repository. The [license](LICENSE) covers this project's original contributions only; [third-party notices](THIRD_PARTY.md) describe the runtime dependencies and how packages obtain them.
