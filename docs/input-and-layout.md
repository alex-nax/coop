# Input and layout lessons

Both games work with two physical XInput gamepads and Proto Input startup injection. The decisive fix for the second pad was `Game.ProtoInput.UseOpenXinput = true`: Windows detected both pads, but the second window did not respond with the prior backend. Keep this setting when refactoring.

SDL can read a controller through HIDAPI, Raw Input, Windows.Gaming.Input, DirectInput or GameInput rather than XInput. Such a path can bypass Nucleus's assignment. The handlers set process-level SDL hints to disable the competing paths and enable XInput and background events. SS2 additionally disables SDL GameInput. These are process environment settings, not permanent Windows controller changes.

NWN2 uses Proto Input without the competing legacy XInput/DInput DLL hook. SS2 retains its tested hook settings. Do not make the two handlers identical merely for style: changing input backends needs a new controller test.

The existing Nucleus setup uses API/XInput indices. `Settings.ini` has `UseXinputIndex=True`; per-game metadata enables API indices and saved profiles. When changing settings, preserve unrelated entries and use UTF-8 **without BOM**. A BOM previously caused Nucleus to crash at startup.

| Game | Requested split | Tested display / per-player size |
| --- | --- | --- |
| SS2 Remaster | Top and bottom | 3840×2160 / 3840×1080 |
| NWN2 Enhanced | Left and right | 3840×2160 / 1920×2160 |

Nucleus's saved profile controls the split. The handler reads the assigned rectangle; it does not force these pixel dimensions. A public package should not contain this PC's saved device IDs or layout JSON. Connect pads before launching, then drag one to each half in Nucleus. Reconnection can change the physical pad order, so reassign if necessary.

A complete routing test moves pad 1 only, then pad 2 only, in menus and gameplay, including when the other window has desktop focus. Both windows accepting any input is insufficient evidence of isolation. Keep both pads awake during all launch and connection tests.
