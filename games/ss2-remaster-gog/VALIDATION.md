# SS2 validation — 2026-09-13

| Check | Result / evidence |
| --- | --- |
| GOG version | 1.3, build 89551 |
| Nucleus | 2.4.2 |
| Visible instances/layout | Two windows; 3840×1080 each, top/bottom on 3840×2160 |
| Controllers | Two physical XInput pads; independent input confirmed by user after OpenXinput fix |
| LAN connection | User confirmed: “I've tested SS2 connection” during repository preparation |
| Earlier network evidence | Host UDP 5069 and second-instance client socket observed |
| Separate player storage | Native `-userpath` uses stable Player1/Player2 folders |
| Long campaign / save-reload | Not separately verified |
| Other builds/platforms/layouts | Not separately verified |
| Hub review | Not submitted or officially verified |

The last installed handler before documentation-only release changes had SHA-256 `0df7eab6e90293732ab764d76e43d67f7da65ea4740fe04114139534ea08cc2a`. The package manifest records the release handler's hash. The behavioral code is preserved; no game was relaunched for repository consolidation.

Installer provenance retained privately: GOG setup EXE SHA-256 `967eaaebf04cf11dad9046b86b612a2d87bd716170a0ec4798dbe61a68825315`, first BIN `66f99faba415fa1c02e764a70a2c690f00b0e478eeea9136e2e230a8d1dbb4b0`. The installer recorded success before a cleanup crash; game launches succeeded. Installers are not distributed here.

---

Source and updates: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human testing is documented per game. Unofficial community project; no claim of official Hub verification.
