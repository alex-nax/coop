# Third-party components

This repository's MIT license applies to its original handlers, compatibility adapter, tooling and documentation. It does not relicense any game, SDK, emulator or Nucleus component. AI assistance does not change upstream authorship.

| Component | Use and provenance | Distribution by this project |
| --- | --- | --- |
| Nucleus Co-op / Proto Input | User installs [Nucleus 2.4.2](https://github.com/SplitScreen-Me/splitscreenme-nucleus/releases/tag/v2.4.2); see its source and notices | Not included |
| gbe_fork Steam replacement | [release-2026_08_23](https://github.com/Detanup01/gbe_fork/releases/tag/release-2026_08_23); upstream [LICENSE](https://github.com/Detanup01/gbe_fork/blob/release-2026_08_23/LICENSE) is LGPL v3 | DLL not included; setup retrieves release directly from upstream |
| Splitux EOS LAN | [v2.1.0](https://github.com/splitux-gg/eos_sdk_emu-splitux/releases/tag/v2.1.0) | DLL not included; setup retrieves upstream archive. No top-level license file was present in the inspected source tree; no redistribution permission is inferred. |
| Nemirtingas Galaxy Emu | Nucleus 2.4.2 `utils/NemirtingasGalaxyEmu/x64/Galaxy64.dll`, exact 0.8.5 build pinned in manifest | Not included; copied from user's installed Nucleus after hash verification |
| 7-Zip | Archive extraction on user's PC; [official site](https://www.7-zip.org/) | Not included |
| GOG game installers, executable code and original platform libraries | User's purchased game installations | Never included in source or upload archives |

NWN2 packages contain only this project's original adapter DLL/source, setup helper, manifests and docs. Pinned third-party DLLs remain in ignored private storage on the development machines. If future releases redistribute a third-party binary, establish the applicable license/source/notice requirements first and update this file and the package allowlist.

The original adapter is a bounded interoperability layer for the tested Galaxy interface; it does not embed the original GOG SDK binary or the Nemirtingas backend. Hashes, version strings and diagnostic offsets document compatibility, not ownership of those upstream works.

Project source: [alex-nax/coop](https://github.com/alex-nax/coop). Made with AI assistance using OpenAI Codex; third-party authorship remains with upstream contributors.
