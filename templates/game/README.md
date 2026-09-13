# New game template

Copy this directory to `games/<slug>/`, choose a stable GUID and create `handler.js`, `release.json`, `VALIDATION.md` and `hub-description.md`. Add one entry to `games/index.json` using an existing game as the metadata example. Add `dependencies.json` only if the game needs external assets, and extend the explicit package mapping for those assets.

Document the exact store/build, native executable, supported controllers, preferred split, install/launch/host/join steps, per-player saves, known limitations and evidence. Follow [the development recipe](../../docs/adding-games.md).

Include: **Made with AI assistance using OpenAI Codex, with human testing where recorded. Unofficial community handler; no claim of official Hub verification.** Revise the tool attribution if a later contribution uses different assistance.

A new template has no passed gameplay checks. Do not copy another game's validation results or machine-specific layout JSON.
