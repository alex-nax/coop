# Hub submission preparation

Two initial packages are prepared: `ss2-remaster-gog-0.1.0.nc` and `nwn2-ee-gog-0.1.0.nc`. They are ZIP archives with a root `handler.js` and an explicit `assets/` directory entry. Nucleus moves the contents of top-level asset folders into a folder named after the handler's `Game.GameName`; files merely placed next to `handler.js` may not be retained. The builder follows that import behavior.

This was checked against the Hub's public `AddPackage.jsx` client (accepts `.nc,.zip` and validates a `handler.js`) and Nucleus's [HubWebView import code](https://github.com/SplitScreen-Me/splitscreenme-nucleus/blob/master/Master/NucleusCoopTool/Controls/HubWebView.cs). See also the [official creation guide](https://www.splitscreen.me/docs/create-handlers/) and [API reference](https://www.splitscreen.me/docs/handler-api/). Hub moderation and account permissions can change; local archive validation does not imply server acceptance or official verification.

## Build and inspect

```sh
./init.sh
./scripts/build-native.sh
python3 scripts/package.py
python3 scripts/validate.py --packages
```

`dist/SHA256SUMS` identifies the output files. Each archive includes an internal file/hash manifest and game-specific description/validation. Packages omit proprietary game data, installers, controller layouts, screenshots, raw traces and third-party runtime DLLs. NWN2 includes the original adapter and a one-time dependency helper. Its setup requirement must remain prominent in the Hub description.

On the initial development machine the builder uses the preserved tested adapter after checking its hash. On a fresh clone it packages a compiled adapter and labels that origin in the dependency manifest. Run the isolated Windows probe and the relevant game regression test before publishing a newly built or changed binary. Do not mistake the source compiler check for Windows gameplay validation.

## Publish using the owner's account

1. Sign into [the Hub](https://hub.splitscreen.me/) and verify the account email if requested. Use **My handlers > Create new handler** and search for the exact enhanced/remastered game, not the original edition.
2. Create one GOG-specific entry per game, set two-player controller support, and paste the corresponding `games/<slug>/hub-description.md` text. Do not label it Hub-verified yourself.
3. In **Edit & Manage**, choose **Release a new version**, select its `.nc`, and paste the short release note from that same description. Review the visible metadata and submit with the owner's authorization.
4. Check that the archive installs into a clean Nucleus library. NWN2 users must run its dependency helper before Play. Keep that instruction in both the Hub page and the handler's in-app description.
5. After a first release exists, select **I want to publicly share my handler** and save. The inspected Hub form requires an administrator to authorize public availability; it can show **Pending validation** before that occurs. Do not claim publication before checking the resulting state.
6. Record actual Hub URLs and review status in `release.json`, the README index and feature tracker. Address moderator feedback before treating the packages as accepted public releases.

The owner has now authorized SS2 publication at [the created Hub entry](https://hub.splitscreen.me/handler/FFBhS68GK5T7u2CEP). Prepare and submit SS2 first; wait for the owner's review before proceeding with NWN2 EE. Every Hub description, release note and package must retain the GitHub source link and AI disclosure. Chrome now connects, and the SS2 listing description/title/input metadata have been saved. Its first release upload is pending the extension's Allow access to file URLs setting.

## Chrome upload setup

If choosing a local archive fails with a file-upload permission error, open `chrome://extensions`, open Details for the ChatGPT browser extension, and enable **Allow access to file URLs**. The supported file-chooser flow needs that permission. Keep the prepared Hub tab open and retry file selection after the user enables it. Do not submit repeatedly or assume a selected filename means an uploaded release.

## Disclosure and release scope

Both packages disclose AI-assisted development using OpenAI Codex and distinguish human testing from official review. SS2 connection testing and NWN2 host/join testing are user-confirmed. Neither claims comprehensive campaign/save-load validation or compatibility with untested builds. [Third-party notices](../THIRD_PARTY.md) explain dependency provisioning and authorship.

---

Source and updates: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human testing is documented per game. Unofficial community project; no claim of official Hub verification.
