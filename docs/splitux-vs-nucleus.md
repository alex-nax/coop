# Splitux and Nucleus Co-op

For this Windows PC and the two tested games, keep Nucleus as the launcher. Splitux's EOS LAN library is already useful as a separate NWN2 dependency; adopting that library does not require adopting the Splitux launcher.

| Area | Splitux | Nucleus in this project |
| --- | --- | --- |
| Host platform | Linux; Windows games through Proton/UMU | Windows, already tested with both GOG games |
| Isolation | Gamescope, Bubblewrap device masking, profile/overlay separation | Windows input hooks and handler-specific instance/player environments |
| Automation | CLI/TUI and GUI documented by upstream | JavaScript handler API; GUI work over SSH needs the interactive Windows session |
| Remote play | Upstream includes browser-streaming components | These handlers provide local split-screen; streaming would be separate work |
| Configuration | YAML game definitions and selectable networking backends | Game-specific JavaScript and extensive Windows hooks |
| Cost of switching here | Linux installation, Proton compatibility checks and new handlers | Preserve the working deployment and accumulated tests |

Splitux's process/device isolation and command-line interface are attractive for a future Linux machine. It adds a different operating environment and compatibility layer to validate. No FPS, latency, memory or reliability benchmark comparing the two was run on this PC, so this project makes no performance ranking. Catalog counts change and are not a meaningful guarantee that a particular store/build works.

The initial EOS LAN trial exported the 68 functions imported by NWN2, including one absent from the older bundled EOS emulator. Export coverage alone did not solve the GOG identity prerequisite. The later Galaxy ABI correction completed the working path, and human host/join testing passed. Earlier private comparison notes saying no connection had succeeded are superseded by that result.

Sources checked during the 2026-09-13 investigation: [Splitux source and README](https://github.com/splitux-gg/splitux), [handler registry](https://github.com/splitux-gg/splitux-handlers/blob/main/index.json), [EOS LAN v2.1.0](https://github.com/splitux-gg/eos_sdk_emu-splitux/releases/tag/v2.1.0), [Nucleus FAQ](https://www.splitscreen.me/docs/faq/). Recheck upstream before planning a platform migration.
