# NWN2: resolving “online service unavailable”

**Outcome:** the user confirmed that Multiplayer opens, the left-hand host appears on the right, and the right-hand player joins. The final handler uses a local Galaxy ABI adapter; it does not force IsLoggedOn to return true and does not patch the game executable.

This investigation applies to GOG 521755.27 (92708), the game's Galaxy SDK 1.152.10.0, and the exact Nemirtingas 0.8.5 backend in [dependencies.json](../games/nwn2-ee-gog/dependencies.json). Addresses below are image-relative offsets for this build, not portable signatures. The original `nwn2.exe` MD5 is `AAADCD6079527807653DDEBF84D1B3C2`.

## Why the message appeared

At `nwn2+0x5f60a7`, the multiplayer preparation code calls IUser's vtable entry `+0xc8` (slot 25). At `+0x5f60ad`, AL was zero. The next branch selected localization ID 235403: “Sorry, the online service is currently unavailable. Please try again later.” Method strings in the original DLL independently identified the call as IsLoggedOn. The AML core pointer at RVA `0x1dc43c8` and EOS platform pointer at `0x1db6328` were still null.

This was a locally generated prerequisite failure, not evidence of a remote service outage. The Steam replacement solved executable startup but did not provide the native GOG identity expected by this path.

A diagnostic one-time change of AL to 1 passed the first gate but reached the five-second authentication timeout at `+0x5f69ed`, which selected the same message. The experiment restored its breakpoint bytes and left the game file unchanged. Merely bypassing a boolean did not initialize the platform or provide the required authentication material; the release does not use this bypass.

## Correct API version was necessary but insufficient

The [Nucleus API documentation](https://www.splitscreen.me/docs/handler-api/#11-nemirtingas-galaxy-gog-emulator) describes selecting the Galaxy API version. The shipped DLL reports 1.152.10.0, and the handler uses that setting with GOG product ID 1993442013. However, a version string cannot repair a different C++ virtual interface layout.

The new `SignInGalaxy(bool, uint32 timeout, IAuthListener*)` call passes timeout 15 in R8 and the listener in R9 on Windows x64. The old emulator implements `(bool, IAuthListener*)`, so it interpreted `0xF` as a listener pointer. A separate probe reproduced its access violation at backend `+0x2d4f9`; calling the old signature succeeded.

IUser also inserted SignInAuthorizationCode at new slot 15. The adapter maps these slots:

| New interface slot | Old backend slot / treatment |
| --- | --- |
| 0–6 | unchanged |
| 7: SignInGalaxy | C thunk drops the new timeout argument |
| 8–14 | unchanged |
| 15 | unsupported diagnostic exception |
| 16–27 | 15–26; includes IsLoggedOn 25→24 and ticket methods 26→25, 27→26 |
| 28–29 | unsupported |
| 30–32 | 27–29: session ID, access token, token copy |
| 33–36 | unsupported |
| 37 | 30: report invalid token |

Requesting an encrypted ticket and pumping callbacks must precede retrieving it. The isolated corrected probe observed OnAuthSuccess, IsLoggedOn=1 and a 107-byte emulated ticket. It did not print ticket contents.

## The second mismatch: IStats

The first adapter still crashed during an achievement callback. The original SDK IStats table at RVA `0xa408f8` contains three extra methods at slots 7, 8 and 9: GetAchievementsNumber, GetAchievementName and GetAchievementNameCopy. The older public [GOG header](https://docs.gog.com/galaxyapi/IStats_8h_source.html) omitted these methods. The actual binaries were decisive: the old emulator table is at RVA `0x4b9868`.

New GetAchievement at slot 10 reached old StoreStatsAndAchievements at slot 10. That method queued an `ACH_NEVER...` string as a listener. Later the callback treated its first bytes, `0x4556454e5f484341`, as a vtable address and crashed at backend `+0x2d4ff`.

The adapter preserves slots 0–6, rejects the three unimplemented enumeration methods, and maps new slots 10–37 to old slots 7–34. Its probe then set, read and cleared a test achievement successfully. Do not infer C++ interface shape solely from an online header when the deployed binary differs.

## Adapter design and limits

The original source is [galaxy_compat.c](../games/nwn2-ee-gog/src/galaxy_compat.c), with its [export definition](../games/nwn2-ee-gog/src/galaxy_compat.def) and source sidecar. Separate proxy objects contain `{vtable, real_object}`. The x64 tail-call thunks replace RCX with the real object, load its original vtable, and jump to the mapped slot. This preserves other argument registers, stack arguments and MSVC hidden structure-return buffers. The backend's own vtable is never overwritten.

User, GameServerUser and Stats are wrapped. Other required exports forward by ordinal to **one pinned backend DLL**. Replacing that backend with an arbitrary version is unsafe even if the filenames and exported names look similar. Unsupported new interface methods raise a visible noncontinuable diagnostic exception instead of pretending to succeed. This is a bounded adapter for the tested game path, not a complete modern Galaxy implementation.

Pointer-only sign-in logs and an access-violation logger aid diagnosis. The vectored handler returns CONTINUE_SEARCH; it does not swallow faults or collect ticket contents. Reinitialization paths, every API method, achievements integration beyond the probe, long campaigns and save/reload are not comprehensively tested.

With both interface fixes, the full game returned AL=1 naturally, AML setup and login-start returned 1, the platform pointers became nonzero, and EOS local login completed. Human host/join confirmation followed. Modern Steam exports and local EOS remain part of this tested combination.

## Other research lessons

- NWN2's startup path uses legacy ANSI Documents APIs before applying `-home`. A Cyrillic OneDrive Documents path caused an early failure. Nucleus's isolated environment supplied usable Documents paths; its normal exit restored the original registry value. Keep the user's real Documents folder intact.
- The original GOG Steam wrapper was tested as an alternative, but the final confirmed combination retains modern `gbe_fork`. Nucleus's older bundled Steam library lacked `SteamInternal_SteamAPI_Init`.
- A leaf callback may have no `.pdata` entry. Do not assign the previous runtime-function entry when the address is beyond its EndAddress.
- A debugger's detach return value alone was insufficient: independent post-exit checks confirmed no debugger and original breakpoint bytes. Set `DebugSetProcessKillOnExit(false)` and verify cleanup.
- Earlier research files describe intermediate failures as ongoing. The final validation record supersedes those chronological snapshots; they are retained privately for evidence.

---

Source and updates: [alex-nax/coop](https://github.com/alex-nax/coop).

Made with AI assistance using OpenAI Codex. Human testing is documented per game. Unofficial community project; no claim of official Hub verification.
