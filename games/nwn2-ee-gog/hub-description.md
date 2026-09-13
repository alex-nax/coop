# Neverwinter Nights 2 Enhanced Edition — GOG / 2 gamepads

[Source code, updates and full documentation on GitHub](https://github.com/alex-nax/coop/tree/main/games/nwn2-ee-gog)

Initial community handler for GOG 521755.27 (92708), Nucleus Co-op 2.4.2. Two XInput gamepads, recommended left/right split. Human testing confirmed independent input, the multiplayer menu, host discovery and joining on one PC.

ONE-TIME SETUP: after import, close Nucleus and run Prepare-Assets.ps1 from the imported handler's asset folder. It needs 7-Zip, retrieves pinned upstream Steam/EOS libraries and copies the exact Nucleus 2.4.2 Galaxy backend. See the included README. The package includes the original Galaxy ABI adapter and source; third-party DLLs are obtained separately. Do not press Play until setup succeeds.

Launch directly through Nucleus, not Galaxy. Assign one pad to each side, then open Multiplayer in both windows. Host on one side and find/join it from the other. Saves use %USERPROFILE%\nwn2-coop\players\Player1 and Player2. Exit normally so Nucleus restores its temporary Documents environment.

The handler uses a Galaxy IUser/IStats compatibility adapter and local EOS initialization; it does not patch nwn2.exe or bypass IsLoggedOn. Exact build/backend pairing is required. Extended campaign/save-reload, complete achievement coverage, other builds, internet play and cross-PC sessions remain unverified.

Made with AI assistance using OpenAI Codex, with human testing. Unofficial community work, unaffiliated with SplitScreen.Me, GOG or the publisher; no claim of official Hub verification. Requires your own game. Upstream libraries retain their own authorship and terms.

Release note: v0.1.0 — two-player GOG handler, independent controls/saves, pinned Galaxy ABI adapter and confirmed local host/join. AI-assisted (OpenAI Codex), human-tested. Source: https://github.com/alex-nax/coop
