import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import vm from 'node:vm';
import { fileURLToPath } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
function run(slug, player) {
  const writes = new Map(), deleted = new Set(), env = {}, ini = new Map();
  const game = { Hook: {}, ProtoInput: {} };
  const context = {
    PlayerID: player, PlayerSteamID: `7656119800000000${player + 1}`,
    NucleusFolder: 'C:\\NucleusCo-op', ScriptFolder: 'C:\\NucleusCo-op\\handlers\\assets',
    RootFolder: `C:\\NucleusCo-op\\content\\test\\Instance${player}`,
    OrigRootFolder: 'C:\\Games\\Original', Width: 960, Height: 1080,
    PosX: player * 960, PosY: 0,
  };
  const guard = p => assert(!p.startsWith(context.OrigRootFolder), `Base install write: ${p}`);
  const io = {
    Directory: { CreateDirectory: guard },
    File: {
      Exists: p => p.endsWith('kexengine.cfg'),
      ReadAllText: () => 'seta custom_setting "keep-me"\nseta v_width "old"\n',
      WriteAllText: (p, data) => { guard(p); writes.set(p, data); },
      Delete: p => { guard(p); deleted.add(p); },
      Copy: (source, target) => {
        guard(target);
        if (target.endsWith('.dll')) assert(deleted.has(target), `DLL link not removed: ${target}`);
        writes.set(target, `copy:${source}`);
      },
    },
  };
  function IniFile(p) {
    this.IniWriteValue = (section, key, value) => { guard(p); ini.set(`${p}|${section}|${key}`, value); };
  }
  vm.runInNewContext(fs.readFileSync(path.join(root, 'games', slug, 'handler.js'), 'utf8'), {
    Game: game, Context: context, Nucleus: { IniFile },
    System: { IO: io, Environment: {
      GetEnvironmentVariable: name => { assert.equal(name, 'USERPROFILE'); return 'C:\\Users\\Tester'; },
      SetEnvironmentVariable: (name, value) => { env[name] = value; },
    } },
  }, { timeout: 1000 });
  game.Play();
  assert.equal(game.ProtoInput.UseOpenXinput, true);
  assert.equal(game.ProtoInput.InjectStartup, true);
  assert.equal(env.SDL_XINPUT_ENABLED, '1');
  assert.equal(env.SDL_JOYSTICK_HIDAPI, '0');
  return { writes, env, ini, context };
}
for (const slug of ['ss2-remaster-gog', 'nwn2-ee-gog']) {
  const players = [run(slug, 0), run(slug, 1)];
  assert.notEqual(players[0].context.StartArguments, players[1].context.StartArguments);
  for (const target of players[0].writes.keys()) assert(!players[1].writes.has(target), `Shared player write: ${target}`);
  for (const [number, result] of players.entries()) {
    if (slug === 'ss2-remaster-gog') {
      const config = [...result.writes.values()].find(v => v.includes('custom_setting'));
      assert(config.includes('keep-me'));
      assert(config.includes('seta v_width "960"'));
      assert(!config.includes('"old"'));
      assert(result.context.StartArguments.includes(`Player${number + 1}`));
    } else {
      const settings = [...result.writes.entries()].find(([p]) => p.endsWith('NemirtingasGalaxyEmu.json'))[1];
      assert(settings.includes(`14549624462898294${number + 1}`));
      assert.equal(JSON.parse(settings).api_version, '1.152.10.0');
      assert.equal(result.env.EOSLAN_USERNAME, `NWN2Player${number + 1}`);
      assert.equal(result.env.EOSLAN_LOG_PATH, 'NUL');
      assert([...result.ini.values()].includes(String(5121 + number * 2)));
      assert([...result.writes.values()].some(v => v.includes(`listen_port=${47584 + number}`)));
      assert.equal([...result.writes.keys()].filter(p => p.endsWith('.dll')).length, 4);
    }
  }
  console.log(`Two-player configuration checks passed: ${slug}`);
}
