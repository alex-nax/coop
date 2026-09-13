#!/usr/bin/env python3
"""Validate public metadata, source links, handler behavior and optional packages."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import zipfile

ROOT = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser()
parser.add_argument('--packages', action='store_true')
args = parser.parse_args()
games = json.loads((ROOT / 'games/index.json').read_text())['games']
assert len({g['guid'] for g in games}) == len(games), 'Duplicate Nucleus GUID'
assert len({g['slug'] for g in games}) == len(games), 'Duplicate catalog slug'
features = json.loads((ROOT / 'features.json').read_text())['features']
ids = {f['id'] for f in features}
assert len(ids) == len(features)
for feature in features:
    assert type(feature['passes']) is bool and feature['acceptance_criteria']
    assert set(feature['dependencies']) <= ids
for game in games:
    folder = ROOT / 'games' / game['slug']
    source = (folder / 'handler.js').read_text()
    assert f'Game.GUID = "{game["guid"]}";' in source
    assert 'AI assistance' in source
    assert json.loads((folder / 'release.json').read_text())['version'] == game['version']
    for name in ('README.md', 'VALIDATION.md', 'hub-description.md'):
        assert (folder / name).is_file()
    subprocess.run(['node', '--check', str(folder / 'handler.js')], check=True)
for directory in ('docs', 'games', 'templates', 'tools'):
    for path in (ROOT / directory).rglob('*.md'):
        for match in re.finditer(r'\[[^\]]+\]\(([^)]+)\)', path.read_text()):
            link = match.group(1).split('#', 1)[0]
            if link and not re.match(r'[a-z]+:', link):
                assert (path.parent / link).exists(), f'Broken link: {path.relative_to(ROOT)} -> {link}'
subprocess.run(['node', str(ROOT / 'tests/handlers.mjs')], check=True)
if args.packages:
    for game in games:
        path = ROOT / 'dist' / f'{game["slug"]}-{game["version"]}.nc'
        with zipfile.ZipFile(path) as archive:
            assert archive.testzip() is None
            names = archive.namelist()
            assert 'handler.js' in names and 'assets/' in names
            assert len(names) == len(set(names))
            assert archive.read('handler.js') == (ROOT / 'games' / game['slug'] / 'handler.js').read_bytes()
            for name in names:
                assert not name.startswith('/') and '..' not in Path(name).parts and '\\' not in name
                assert not any(p in Path(name).parts for p in ('storage', 'local', 'installers', 'research', '.git'))
                if Path(name).suffix.lower() in ('.dll', '.exe', '.bin'):
                    assert game['native_adapter'] and name == 'assets/Galaxy64.dll', f'Unexpected binary: {name}'
            manifest = json.loads(archive.read('assets/package-manifest.json'))
            assert manifest['ai_assisted'] is True
            for name, expected in manifest['files'].items():
                assert hashlib.sha256(archive.read(name)).hexdigest() == expected
            import posixpath
            for name in names:
                if name.endswith('.md'):
                    for match in re.finditer(r'\[[^\]]+\]\(([^)]+)\)', archive.read(name).decode()):
                        link = match.group(1).split('#', 1)[0]
                        if link and not re.match(r'[a-z]+:', link):
                            resolved = posixpath.normpath(posixpath.join(posixpath.dirname(name), link))
                            assert resolved in names, f'Broken package link: {name} -> {link}'
            if game['native_adapter']:
                deps = json.loads(archive.read('assets/dependencies.json'))['assets']
                expected = next(x['sha256'] for x in deps if x['file'] == 'Galaxy64.dll')
                assert hashlib.sha256(archive.read('assets/Galaxy64.dll')).hexdigest() == expected
        print(f'Package valid: {path.name}')
print(f'Validated {len(games)} games, {len(features)} features, source links and two-player configuration checks.')
