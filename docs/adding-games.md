# Adding the next game

1. **Identify the exact release.** Record the store, build, installer/executable hashes, architecture and native executable. Check the [Hub](https://hub.splitscreen.me/) for an existing handler and the [official development guide](https://www.splitscreen.me/docs/create-handlers/) for supported mechanisms. An edition or store change can alter networking and controller APIs.
2. **Establish a normal baseline.** Finish and verify the installer, launch the executable normally on Windows, record where it writes configuration, and test native multiplayer. A partly downloaded `dnd_` file is not an installer-ready signal: require all parts, stable sizes and no temporary download extension. Keep installers private.
3. **Create an isolated entry.** Copy `templates/game/`, choose a stable slug/GUID, add `games/index.json` metadata, and create a handler with just the necessary settings. Create a separate Windows experiment folder. Back up original DLLs and save directories before any trial.
4. **Make two instances work.** Confirm both visible windows belong to the expected executable. Discover real mutex or startup constraints before adding workarounds. Nucleus's symlinked trees avoid full game duplication, but writable files and replacement DLLs must be private copies.
5. **Separate identity and storage.** Give each player stable data directories, names, IDs and ports. Preserve exact 64-bit IDs as strings in JavaScript; avoid arithmetic beyond its integer precision. Change only necessary INI keys and retain the user's other settings.
6. **Isolate controllers.** Start with two awake XInput pads. Check Proto Input startup injection and OpenXinput; then restrict alternative SDL controller backends if they bypass the hook. Test each pad with the other untouched, on both focused and background windows. Do not stack competing hooks without evidence.
7. **Test multiplayer by stages.** Separate opening the menu, local authentication, hosting, discovery, joining and actual gameplay. Match each middleware API to the shipped binary. Missing exports and virtual-table ABI mismatches are different failures. See [networking](networking.md).
8. **Build the human workflow.** Document layout, host/join sequence, saves, initial setup and known limitations. Provide a simple launcher or normal Nucleus entry; do not make a debug task the user's launch path.
9. **Validate and freeze.** Record versions, hashes and observations in `VALIDATION.md`. Run local checks and an isolated ABI probe if native compatibility code changed. Back up the last successful handler and dependency manifest. Ask concise human questions only where remote checks cannot establish behavior.
10. **Prepare a release.** Update AI disclosure and Hub description, dependency sources, game index and progress. Build and inspect the allowlisted archive. Actual Hub publication and public verification are separate from local success.

If a new game needs additional assets, extend the package builder's explicit mapping and add validation before introducing them. Never solve packaging by including the entire research directory. Compare any architectural change against both existing handlers before deploying shared Nucleus settings.

---

Source and updates: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human testing is documented per game. Unofficial community project; no claim of official Hub verification.
