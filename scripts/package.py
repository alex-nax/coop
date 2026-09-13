#!/usr/bin/env python3
"""Build allowlisted Hub archives; never walk private storage into a release."""
import hashlib
import json
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[1]
SOURCE_REPOSITORY = 'https://github.com/alex-nax/coop'


def digest(data):
    return hashlib.sha256(data).hexdigest()


def make_package(game):
    folder = ROOT / 'games' / game['slug']
    payload = {'handler.js': (folder / 'handler.js').read_bytes()}
    for name in ('README.md', 'VALIDATION.md', 'hub-description.md'):
        # Keep the same relative tree so links inside the guides remain usable.
        payload[f'assets/games/{game["slug"]}/{name}'] = (folder / name).read_bytes()
    for doc in sorted((ROOT / 'docs').glob('*.md')):
        payload['assets/docs/' + doc.name] = doc.read_bytes()
    for other in sorted((ROOT / 'games').iterdir()):
        if other.is_dir():
            for name in ('README.md', 'VALIDATION.md', 'dependencies.json'):
                if (other / name).exists():
                    payload[f'assets/games/{other.name}/{name}'] = (other / name).read_bytes()
            for src in sorted((other / 'src').glob('*')):
                if src.suffix in ('.c', '.def'):
                    payload[f'assets/games/{other.name}/src/{src.name}'] = src.read_bytes()
    payload['assets/tools/diagnostics/README.md'] = (ROOT / 'tools/diagnostics/README.md').read_bytes()
    for name in ('LICENSE', 'THIRD_PARTY.md'):
        payload['assets/' + name] = (ROOT / name).read_bytes()
    payload['assets/README.md'] = (
        '# Handler installation\n\nMade with AI assistance using OpenAI Codex, with human testing.\n\n'
        f'[Source code and updates on GitHub]({SOURCE_REPOSITORY})\n\n'
        f'Read [the game guide](games/{game["slug"]}/README.md) for installation, play and limitations.\n\n'
        + ('Before the first Play, close Nucleus and run Prepare-Assets.ps1 in this folder. '
           'It retrieves the pinned upstream dependencies and verifies their hashes.\n' if game['native_adapter'] else '')
    ).encode()
    if game['native_adapter']:
        dependencies = json.loads((folder / 'dependencies.json').read_text())
        original = ROOT / 'storage/assets/nwn2-ee-gog/Galaxy64.dll'
        adapter = original if original.exists() else ROOT / 'build/nwn2-ee-gog/Galaxy64.dll'
        if not adapter.exists():
            raise SystemExit('Build the adapter with ./scripts/build-native.sh before packaging NWN2.')
        data = adapter.read_bytes()
        entry = next(a for a in dependencies['assets'] if a['file'] == 'Galaxy64.dll')
        if original.exists() and digest(data) != entry['sha256']:
            raise SystemExit('The preserved adapter hash differs from the tested dependency manifest.')
        entry['sha256'] = digest(data)
        entry['binary_origin'] = 'preserved tested build' if original.exists() else 'local source build; Windows validation required before publishing'
        payload['assets/Galaxy64.dll'] = data
        for name in ('Prepare-Assets.ps1',):
            payload['assets/' + name] = (folder / name).read_bytes()
        encoded = (json.dumps(dependencies, indent=2) + '\n').encode()
        payload['assets/dependencies.json'] = encoded
        payload[f'assets/games/{game["slug"]}/dependencies.json'] = encoded
        for src in sorted((folder / 'src').iterdir()):
            if src.suffix in ('.c', '.def'):
                payload[f'assets/games/{game["slug"]}/src/{src.name}'] = src.read_bytes()
    manifest = {'game': game, 'ai_assisted': True, 'source_repository': SOURCE_REPOSITORY, 'files': {n: digest(v) for n, v in sorted(payload.items())}}
    payload['assets/package-manifest.json'] = (json.dumps(manifest, indent=2) + '\n').encode()
    target = ROOT / 'dist' / f'{game["slug"]}-{game["version"]}.nc'
    directories = set()
    for name in payload:
        for parent in Path(name).parents:
            if str(parent) != '.':
                directories.add(parent.as_posix() + '/')
    with zipfile.ZipFile(target, 'w', compression=zipfile.ZIP_DEFLATED) as archive:
        for name in sorted(directories) + sorted(payload):
            info = zipfile.ZipInfo(name, date_time=(2026, 9, 13, 0, 0, 0))
            info.create_system = 3
            info.external_attr = ((0o40755 if name.endswith('/') else 0o100644) << 16)
            info.compress_type = zipfile.ZIP_DEFLATED
            archive.writestr(info, b'' if name.endswith('/') else payload[name])
    print(f'{target.name}: {target.stat().st_size:,} bytes')
    return target


if __name__ == '__main__':
    (ROOT / 'dist').mkdir(exist_ok=True)
    catalog = json.loads((ROOT / 'games/index.json').read_text())['games']
    outputs = [make_package(game) for game in catalog]
    (ROOT / 'dist/SHA256SUMS').write_text(''.join(f'{digest(p.read_bytes())}  {p.name}\n' for p in outputs))
    (ROOT / 'dist/SUBMISSION.md').write_text((ROOT / 'docs/hub-submission.md').read_text())
