Game.ExecutableName = "SystemShock2Remastered.exe";
Game.ExecutableContext = ["base.kpf", "Galaxy64.dll"];
Game.GUID = "SS2RemasterGOG";
Game.GameName = "System Shock 2 Remaster (GOG, 2 gamepads)";
Game.Description = "GOG 1.3. Two gamepads, top/bottom split; controller isolation and LAN connection confirmed by human testing. Multiplayer > LAN: host on one side; join 127.0.0.1 on the other. Made with AI assistance (OpenAI Codex). Unofficial community handler.";
Game.MaxPlayers = 2;
Game.MaxPlayersOneMonitor = 2;
Game.HandlerInterval = 100;
Game.PauseBetweenStarts = 3;
Game.SymlinkGame = true;
Game.SymlinkExe = false;
Game.KeepSymLinkOnExit = true;
Game.FileSymlinkExclusions = ["kexengine.cfg", "kexengine.log", "stdout.txt", "stderr.txt"];
Game.SupportsKeyboard = false;
Game.SupportsPositioning = true;
Game.HasDynamicWindowTitle = true;
Game.Hook.ForceFocusWindowName = "System Shock 2: 25th Anniversary Remaster";
Game.Hook.ForceFocus = false;
Game.Hook.CustomDllEnabled = false;
Game.Hook.XInputEnabled = true;
Game.Hook.DInputEnabled = false;
Game.Hook.DInputForceDisable = true;
Game.ProtoInput.InjectStartup = true;
Game.ProtoInput.XinputHook = true;
Game.ProtoInput.UseOpenXinput = true;
Game.ProtoInput.FocusHooks = true;
Game.ProtoInput.ClipCursorHook = true;
Game.ProtoInput.FreezeExternalInputWhenInputNotLocked = false;
Game.UseNucleusEnvironment = false;

Game.Play = function () {
    var playerPath = Context.NucleusFolder + "\\players\\SS2RemasterGOG\\Player" + (Context.PlayerID + 1);
    System.IO.Directory.CreateDirectory(playerPath);
    var configPath = playerPath + "\\kexengine.cfg";
    var config = System.IO.File.Exists(configPath) ? String(System.IO.File.ReadAllText(configPath)) : "";
    var values = {
        v_windowmode: "0", v_width: String(Context.Width), v_height: String(Context.Height),
        v_windowXPos: String(Context.PosX), v_windowYPos: String(Context.PosY),
        v_windowResizable: "1", v_vsync: "1", in_allowbginput: "1", in_nosteaminput: "1",
        in_nojoy: "0", net_voip: "0", net_voipmic: "0", r_gamepadStyle: "xbox"
    };
    for (var key in values) {
        var pattern = new RegExp("^\\s*seta?\\s+" + key + "\\s+.*$", "gm");
        config = config.replace(pattern, "");
        config += '\r\nseta ' + key + ' "' + values[key] + '"';
    }
    System.IO.File.WriteAllText(configPath, config + "\r\n");
    var hints = {
        SDL_JOYSTICK_HIDAPI: "0", SDL_JOYSTICK_RAWINPUT: "0", SDL_JOYSTICK_WGI: "0",
        SDL_JOYSTICK_DINPUT: "0", SDL_JOYSTICK_GAMEINPUT: "0", SDL_XINPUT_ENABLED: "1",
        SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS: "1"
    };
    for (var hint in hints) System.Environment.SetEnvironmentVariable(hint, hints[hint]);
    Context.StartArguments = '-userpath "' + playerPath + '" -window -width ' + Context.Width +
        ' -height ' + Context.Height + ' -xpos ' + Context.PosX + ' -ypos ' + Context.PosY + ' -skipmovies';
};
