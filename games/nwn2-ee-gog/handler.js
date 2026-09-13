Game.ExecutableName = "nwn2.exe";
Game.ExecutableContext = ["Galaxy64.dll", "nwn2server64.exe"];
Game.GUID = "NWN2EEGOG";
Game.GameName = "Neverwinter Nights 2 Enhanced Edition (GOG, 2 gamepads)";
Game.Description = "GOG 521755.27. Before first Play, run Prepare-Assets.ps1 from this handler's asset folder; see README.md there. Two gamepads, left/right split; hosting and joining confirmed by human testing. Made with AI assistance (OpenAI Codex). Unofficial community handler.";
Game.SteamID = "2738630";
Game.UseGoldberg = true;
Game.GoldbergNoWarning = true;
Game.MaxPlayers = 2;
Game.MaxPlayersOneMonitor = 2;
Game.HandlerInterval = 100;
Game.PauseBetweenStarts = 5;
Game.SymlinkGame = true;
Game.SymlinkExe = false;
Game.KeepSymLinkOnExit = true;
Game.FileSymlinkCopyInstead = ["nwn2.ini", "nwn2player.ini", "nwncdkey.ini", "GalaxyConfig.json"];
Game.SupportsKeyboard = false;
Game.SupportsPositioning = true;
Game.HasDynamicWindowTitle = true;
Game.Hook.CustomDllEnabled = false;
Game.Hook.XInputEnabled = false;
Game.Hook.DInputEnabled = false;
Game.Hook.DInputForceDisable = false;
Game.ProtoInput.InjectStartup = true;
Game.ProtoInput.XinputHook = true;
Game.ProtoInput.UseOpenXinput = true;
Game.ProtoInput.FocusHooks = true;
Game.ProtoInput.ClipCursorHook = true;
Game.ProtoInput.FreezeExternalInputWhenInputNotLocked = false;
Game.UseNucleusEnvironment = true;
Game.DocumentsConfigPath = "Neverwinter Nights 2";
Game.DocumentsConfigPathNoCopy = true;
Game.DocumentsSavePath = "Neverwinter Nights 2";
Game.DocumentsSavePathNoCopy = true;

Game.Play = function () {
    var playerHome = System.Environment.GetEnvironmentVariable("USERPROFILE") +
        "\\nwn2-coop\\players\\Player" + (Context.PlayerID + 1);
    System.IO.Directory.CreateDirectory(playerHome);
    var emulator = Context.ScriptFolder + "\\steam_api64.dll";
    var instanceDll = Context.RootFolder + "\\steam_api64.dll";
    System.IO.File.Delete(instanceDll);
    System.IO.File.Copy(emulator, instanceDll);
    var steamSettings = Context.RootFolder + "\\steam_settings";
    System.IO.Directory.CreateDirectory(steamSettings);
    System.IO.File.WriteAllText(steamSettings + "\\steam_appid.txt", Game.SteamID);
    System.IO.File.WriteAllText(steamSettings + "\\configs.user.ini",
        "[user::general]\r\naccount_name=Player" + (Context.PlayerID + 1) +
        "\r\naccount_steamid=" + Context.PlayerSteamID + "\r\nlanguage=english\r\n" +
        "[user::saves]\r\nlocal_save_path=" + playerHome + "\\SteamData\r\n");
    System.IO.File.WriteAllText(steamSettings + "\\configs.main.ini",
        "[main::connectivity]\r\ndisable_networking=0\r\nlisten_port=" + (47584 + Context.PlayerID) + "\r\n");
    System.IO.File.WriteAllText(steamSettings + "\\custom_broadcasts.txt", "127.0.0.1\r\n");
    var galaxyDll = Context.RootFolder + "\\Galaxy64.dll";
    System.IO.File.Delete(galaxyDll);
    System.IO.File.Copy(Context.ScriptFolder + "\\Galaxy64.dll", galaxyDll);
    var backendDll = Context.RootFolder + "\\NemirtingasGalaxy64.dll";
    System.IO.File.Delete(backendDll);
    System.IO.File.Copy(Context.ScriptFolder + "\\NemirtingasGalaxy64.dll", backendDll);
    var eosDll = Context.RootFolder + "\\EOSSDK-Win64-Shipping.dll";
    System.IO.File.Delete(eosDll);
    System.IO.File.Copy(Context.ScriptFolder + "\\EOSSDK-Win64-Shipping.dll", eosDll);
    var galaxySettings = Context.RootFolder + "\\ngalaxye_settings";
    System.IO.Directory.CreateDirectory(galaxySettings);
    System.IO.File.WriteAllText(galaxySettings + "\\NemirtingasGalaxyEmu.json",
        '{"api_version":"1.152.10.0","disable_online_networking":true,"enable_lan":true,"log_level":"warn","enable_overlay":false,"galaxyid":14549624462898294' +
        (Context.PlayerID + 1) + ',"language":"en","productid":1993442013,"username":"Player' +
        (Context.PlayerID + 1) + '"}');
    var configs = ["nwn2.ini", "nwn2player.ini"];
    for (var i = 0; i < configs.length; i++) {
        var destination = playerHome + "\\" + configs[i];
        if (!System.IO.File.Exists(destination)) {
            System.IO.File.Copy(Context.OrigRootFolder + "\\" + configs[i], destination);
        }
    }
    var display = new Nucleus.IniFile(playerHome + "\\nwn2.ini");
    display.IniWriteValue("Display Options", "FullScreen", "0");
    display.IniWriteValue("Display Options", "Width", String(Context.Width));
    display.IniWriteValue("Display Options", "Height", String(Context.Height));
    var multiplayer = new Nucleus.IniFile(playerHome + "\\nwn2player.ini");
    multiplayer.IniWriteValue("Profile", "Player Name", "Player" + (Context.PlayerID + 1));
    multiplayer.IniWriteValue("Profile", "Client Port", String(5120 + Context.PlayerID * 2));
    multiplayer.IniWriteValue("Server Options", "Game Port", String(5121 + Context.PlayerID * 2));
    var hints = {
        SDL_JOYSTICK_HIDAPI: "0", SDL_JOYSTICK_RAWINPUT: "0", SDL_JOYSTICK_WGI: "0",
        SDL_JOYSTICK_DINPUT: "0", SDL_XINPUT_ENABLED: "1",
        SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS: "1"
    };
    for (var hint in hints) System.Environment.SetEnvironmentVariable(hint, hints[hint]);
    System.Environment.SetEnvironmentVariable("EOSLAN_USERNAME", "NWN2Player" + (Context.PlayerID + 1));
    System.Environment.SetEnvironmentVariable("EOSLAN_LOCALHOST_MODE", "1");
    System.Environment.SetEnvironmentVariable("EOSLAN_LOG_PATH", "NUL");
    Context.StartArguments = '-home "' + playerHome + '"';
};
