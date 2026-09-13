# Authoring on macOS

The canonical checkout is `~/coop`. Initialize or resume it with `./init.sh`. Python 3 handles packaging and metadata checks, Node.js executes the safe handler simulation, and Git tracks source. These checks do not require the Windows PC.

To compile the original Windows adapter and diagnostic tools, install an x64 MinGW toolchain. On the development Mac the compiler is `x86_64-w64-mingw32-gcc` from Homebrew's `mingw-w64`; `7zz` is available for inspecting downloaded archives. Installation is separate from `init.sh`; the bootstrap never changes the system automatically.

```sh
./scripts/build-native.sh
python3 scripts/package.py
python3 scripts/validate.py --packages
```

Outputs appear in `build/` and `dist/`. Hashes are written into the package manifest and `dist/SHA256SUMS`. Rebuilding with a different compiler can change a DLL hash; do not claim binary identity without comparing hashes and rerunning the isolated Windows probe. The initial release uses the preserved, human-tested adapter binary when its manifest hash matches.

Copy `templates/machines.example.json` to `local/machines.json` for a new target, then edit its SSH target and Windows workspace. `local/MACHINE.md` records the existing installation's exact paths and migration details. Neither belongs in Git.

Use `python3 scripts/sync_windows.py --dry-run` to inspect a mirror operation and run without `--dry-run` when ready. It transfers only public source and prepared Hub packages to the configured workspace. It does not install handlers, start Nucleus, stop a game, copy saves or change Windows settings.

Raw reverse-engineering artifacts and proprietary files remain in the original workspaces, linked beneath `storage/legacy/`. Use their documented observations, not old experimental handlers, as the starting point for changes.

---

Source and updates: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human testing is documented per game. Unofficial community project; no claim of official Hub verification.
