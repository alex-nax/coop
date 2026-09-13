# Windows and SSH operations

Read `local/machines.json` for the real target and `local/MACHINE.md` for exact installed paths. Example connection:

```sh
ssh -o BatchMode=yes user@windows-pc
```

Use `-o BatchMode=yes` for automation so a password prompt cannot hang the session. Confirm the host key normally; do not disable host-key verification. From SSH, use `powershell -NoProfile -ExecutionPolicy Bypass -File C:\path\script.ps1`. A file avoids the layers of zsh, SSH, Windows command parsing and PowerShell interpolation. For generated one-shot commands, `-EncodedCommand` takes UTF-16LE Base64; see the mirror script for a tested example. JSON encoding is not shell quoting.

## Read-only preflight

Run `scripts/windows/Inspect-PC.ps1` to list relevant processes, Nucleus/version, Windows Documents path and XInput connection status. It makes no settings changes. A sleeping controller can be absent even if it paired earlier. Confirm both are connected before assigning them; Windows detection and correct per-window routing are separate checks.

Use `python3 scripts/sync_windows.py` from the Mac to update the new workspace. This does not deploy into the running Nucleus installation. After the user ends a session, import a prepared Hub archive through Nucleus or use `scripts/windows/Install-Handler.ps1` for a reviewed local handler deployment. This installer refuses to act while the affected game or Nucleus is running, backs up only owned files, and leaves library registration and saved layouts alone. Add the executable through Nucleus's normal UI.

## Interactive applications

An executable started directly by the SSH service can run on a noninteractive desktop. A process with no visible game window is not evidence that the user has seen a successful launch. Use the signed-in Windows session for Nucleus and game windows.

The legacy setup has interactive scheduled tasks named `NWN2Coop-Action`, `NWN2Coop-Inspect` and `NWN2Coop-NucleusInspect`. Their action paths still point to the original NWN2 workspace. `Session-Action.ps1` was left as a no-op after testing. Inspect a task and its action file before invoking it: an old debugging action may terminate a test process or restore experimental libraries. Do not blindly run historical tasks.

For future automation, create a narrowly scoped interactive task under the signed-in user's identity, with a specific action file and any elevation needed for Nucleus. Do not save a password in scripts. Test the action's syntax and preconditions before triggering it. Poll for results via a private output JSON/text file rather than guessing from SSH's exit status.

## Observability and cleanup

Inspect only relevant windows/processes, use application-window capture rather than desktop screenshots, and keep raw output in `storage/`. Record both instance PIDs, executable paths, command lines, loaded library hashes and controller routing. Do not log real tickets, credentials or unrelated applications.

NWN2's Nucleus player environment temporarily redirects the user's Documents registry value. Capture its original value before experiments; after normal exit verify it was restored. Do not permanently replace the user's OneDrive Documents folder to make a game launch. SS2 uses a native player-data argument and does not need this mechanism.

If a debugger reports failed explicit detach, independently check `CheckRemoteDebuggerPresent` and restored breakpoint bytes after its process exits. Avoid broad `Stop-Process` commands. Never shut down an active co-op session merely to validate repository packaging.
