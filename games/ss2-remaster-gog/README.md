# System Shock 2: 25th Anniversary Remaster — GOG

Two gamepads, top/bottom split, native LAN. Tested on GOG 1.3 (build 89551), Nucleus Co-op 2.4.2 and a 3840×2160 screen. The user confirmed independent controllers and the connection test. See [validation](VALIDATION.md) for limits.

**Made with AI assistance using OpenAI Codex, with human testing.** This is an unofficial community handler, not an official or Hub-verified release. It requires your own game installation.

## Install and play

1. Install the GOG game and Nucleus. Import `ss2-remaster-gog-0.1.0.nc` in Nucleus (drag/drop into the supported import interface) and add `SystemShock2Remastered.exe` from your game folder.
2. Connect two XInput pads. Choose the handler, split the monitor into top and bottom, and assign one pad to each half.
3. Press Play. The handler creates separate player configuration/save folders and starts both windows.
4. In both windows choose **Multiplayer > LAN**. Host with player 1, then join `127.0.0.1` with player 2.
5. Exit the session normally when finished. Existing desktop shortcuts on the original test PC still work.

No additional multiplayer emulator or Galaxy client is required by this handler. If Nucleus displays its general OneDrive notice, this game stores player data using its explicit `-userpath` below. The handler does not redirect the Documents registry setting.

## Player data and implementation

Player data is in `<Nucleus>\players\SS2RemasterGOG\Player1` and `Player2`. Back up both directories before changing the installation. Stable names preserve saves across controller reconnects and layout changes.

The handler preserves unrelated configuration entries, writes the assigned window rectangle, permits background input and uses Proto Input OpenXinput. SDL hints force the controller path through XInput; without OpenXinput the second window previously ignored the second controller. It disables voice chat/microphone in its local configuration. See [input lessons](../../docs/input-and-layout.md).

On the original display each player had a 3840×1080 view. Other display sizes are derived from Nucleus's assigned rectangle; they have not been separately tested. Saved hardware-specific layout JSON is not included in public packages.

## Maintenance

Keep GUID `SS2RemasterGOG` and player folders stable. Runtime logic in this initial repository release matches the successful local handler; its description now includes disclosure and the user's connection confirmation. The handler needs no bundled DLL assets. Build the Hub package with `python3 scripts/package.py` from the repository root.
