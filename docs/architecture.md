# Architecture

The repository is a catalog of independent handlers and a small shared toolset. No package manager, database or web service is needed.

```text
games/<slug>/handler.js + metadata + README + dependency recipe
             |                       |
             |                 native adapter source
             |                       | build-native.sh
             +-----------------------+----> package.py -> dist/<slug>-<version>.nc
                                                        |
                                                 Nucleus on Windows
                                                        |
                                        isolated instances + stable player saves
```

`games/index.json` is the public catalog. Each game owns its handler, metadata, validation and Hub text. Game handlers may use the Nucleus API and their own asset folder; they must not depend on this Mac's checkout or another game's files. A runtime dependency downloaded during setup must have a pinned hash and source in that game's `dependencies.json`.

`scripts/` owns reproducible packaging and workstation operations. `tests/` simulates handler configuration, not a real game. `tools/diagnostics/` holds original research tools; they are never run by the bootstrap or included as game launch requirements. `templates/` starts the next entry.

`build/` stores original compiled outputs. `storage/assets/` stores fetched or previously tested third-party libraries. `storage/legacy/` links to previous workspaces. `local/` contains private machine paths and operational notes. All are ignored. Public archives are built from an explicit per-game allowlist, never by zipping the checkout.

## Runtime boundaries

The GOG installation remains the baseline. Nucleus makes per-instance game trees. Handlers write only instance configuration, instance libraries and stable player directories. Symlinked files must be unlinked before replacement. A GUID identifies a game across the Nucleus library, content tree and profiles; changing it can strand saves/configuration.

SS2 uses its native `-userpath` plus Proto Input and SDL hints; it needs no external multiplayer emulator. NWN2 combines modern Steam exports, a Galaxy ABI adapter and local EOS. The adapter forwards to one exact Nemirtingas backend by ordinal, so that DLL cannot be silently upgraded. See [the ABI design](nwn2-galaxy-abi.md).

## Naming and change rules

Use lowercase kebab-case catalog slugs and stable Nucleus GUIDs. Keep human documentation near each game, with reusable lessons under `docs/`. Use source sidecars for implementation rationale; keep their paths and anchors current. Avoid shared global controller fixes until the individual handler has been tested. Never turn a machine-specific saved layout into a universal profile.
