# Neverwinter Nights 2 Enhanced Edition — GOG

[Source code, updates and full documentation on GitHub](https://github.com/alex-nax/coop/tree/main/games/nwn2-ee-gog)

Two gamepads, left/right split, local host/join on one Windows PC. Tested on GOG 521755.27 (build 92708), Nucleus Co-op 2.4.2 and a 3840×2160 display. Human testing confirmed independent controllers, host discovery and joining. See [validation](VALIDATION.md).

**Made with AI assistance using OpenAI Codex, with human testing.** This is an unofficial community handler and original compatibility adapter. It is not an official or Hub-verified release. Third-party libraries retain their own authorship and licenses. Use your own GOG installation.

## Install from a Hub package

1. Install Nucleus Co-op **2.4.2**, your GOG game, and 7-Zip (needed to extract the upstream Steam library).
2. Import `nwn2-ee-gog-0.1.0.nc` through Nucleus and add `nwn2.exe`. Close Nucleus after importing, before the one-time asset setup.
3. Open PowerShell and run the script in the newly imported handler asset folder, for example:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File 'C:\NucleusCo-op\handlers\Neverwinter Nights 2 Enhanced Edition (GOG, 2 gamepads)\Prepare-Assets.ps1' -NucleusDirectory 'C:\NucleusCo-op'
```

Nucleus names the imported folder after `Game.GameName`. If your folder differs, use the actual imported asset folder. The script downloads the exact upstream `gbe_fork` and EOS LAN releases, checks their hashes, and copies the pinned Nemirtingas backend from your Nucleus installation. The package contains this project's tested adapter and source, but does not redistribute the three third-party DLLs. If offline, provide `-CacheDirectory` pointing at a private folder containing all four exact DLLs. Re-running the script with the correct assets is safe.

The script fails clearly if Nucleus's backend differs. Do not substitute a newer DLL: the adapter forwards exports by ordinals bound to that exact backend. See [dependency manifest](dependencies.json). No game installation files are overwritten by setup.

## Play

1. Connect both pads and launch Nucleus normally. Do not launch through Galaxy.
2. Select the NWN2 GOG handler, divide the screen left/right and assign one pad to each half. Press Play.
3. Wait for both intro/menu sequences. Confirm each pad affects only its own side.
4. Open Multiplayer on both sides. Host on the left; find and join that host on the right.
5. Exit normally so Nucleus can restore its temporary Documents environment.

On the original PC the already-installed profile and shortcut remain usable; dependency setup has already been completed there. Each half was 1920×2160 on a 3840×2160 monitor.

## Saves and compatibility

The stable native `-home` paths remain `%USERPROFILE%\nwn2-coop\players\Player1` and `Player2`, including on the reorganized workstation. Do not delete that old-looking folder: it holds player data. Nucleus's isolated environment also avoids the tested startup failure involving legacy ANSI APIs and a non-ASCII OneDrive Documents path. Normal exit should restore the user's original Documents setting.

The tested combination includes modern Steam exports, the original Galaxy IUser/IStats adapter, the pinned Nemirtingas backend and Splitux EOS LAN v2.1.0. All replacement DLLs are private copies inside each Nucleus instance. The base GOG executable remains unpatched. No IsLoggedOn bypass is part of this handler. [The complete diagnosis](../../docs/nwn2-galaxy-abi.md) explains why selecting only the API version was insufficient.

Only the two-player same-PC path is confirmed. Internet matchmaking, cross-PC LAN, other game builds, additional players, complete achievement coverage and long campaign/save-load behavior are not verified. Continuous EOS logs are disabled for ordinary play.
