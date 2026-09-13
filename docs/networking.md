# Networking lessons

SS2 exposes native LAN multiplayer. Open Multiplayer > LAN in both windows; host on one and join `127.0.0.1` from the other. UDP port 5069 was observed for the host. The user subsequently confirmed the connection test. No extra multiplayer emulator is part of this handler.

NWN2's GOG build uses several platform layers even for the desired local session. The working combination is modern `gbe_fork` Steam exports, a Galaxy IUser/IStats compatibility adapter, the exact bundled Nemirtingas Galaxy backend and Splitux EOS LAN. Do not infer that a GOG build has no Steam API dependency from its storefront name.

| NWN2 setting | Player 1 | Player 2 |
| --- | --- | --- |
| Player name | Player1 | Player2 |
| Galaxy ID | 145496244628982941 | 145496244628982942 |
| Steam listen port | 47584 | 47585 |
| Game client port | 5120 | 5122 |
| Game host port | 5121 | 5123 |
| EOS username | NWN2Player1 | NWN2Player2 |

Steam IDs come from the Nucleus player context. Galaxy IDs are written as literal decimal strings inside JSON to preserve all digits. EOS localhost mode is enabled and Steam broadcast discovery includes loopback. These settings were tested for two instances on the same PC; cross-PC LAN and internet matchmaking are outside the verified scope.

## Diagnose the first failing stage

1. Does the game start and expose the expected DLL exports?
2. Does the multiplayer menu open?
3. Does the local platform identity report success and provide its emulated ticket?
4. Does the game's middleware initialize and complete login?
5. Does a host appear in the second instance's server list?
6. Does the second instance join, then remain playable?

The same NWN2 dialog appeared both at a false IsLoggedOn result and at a later authentication timeout. Replacing the EOS library alone or forcing the login boolean did not establish a working platform identity. The [ABI diagnosis](nwn2-galaxy-abi.md) explains the durable fix.

Use bounded diagnostic logging and turn it off for normal play. EOS LAN's tested binary can produce substantial continuous logs; the release handler sets `EOSLAN_LOG_PATH=NUL`. If a new failure needs tracing, set a private per-player log path for a controlled run, restore the release setting afterwards, and avoid logging credential or ticket contents. Do not change firewall rules until port/process evidence supports that diagnosis.

---

Source and updates: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human testing is documented per game. Unofficial community project; no claim of official Hub verification.
