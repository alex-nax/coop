#!/usr/bin/env python3
"""Mirror public source and prepared packages without touching runtime installations."""
import argparse
import base64
import json
from pathlib import Path
import subprocess
import zipfile

ROOT = Path(__file__).resolve().parents[1]
PUBLIC_DIRS = ('games', 'docs', 'scripts', 'templates', 'tests', 'tools')
PUBLIC_FILES = ('README.md', 'AGENTS.md', 'Codex-progress.md', 'features.json', 'init.sh', 'LICENSE', 'THIRD_PARTY.md', '.gitignore', '.gitattributes')


def public_files():
    files = [ROOT / name for name in PUBLIC_FILES]
    for folder in PUBLIC_DIRS:
        files.extend(p for p in (ROOT / folder).rglob('*') if p.is_file() and not p.is_symlink() and '__pycache__' not in p.parts and p.suffix not in ('.pyc', '.dll', '.exe'))
    files.extend((ROOT / 'dist').glob('*.nc'))
    files.extend(ROOT / 'dist' / name for name in ('SHA256SUMS', 'SUBMISSION.md') if (ROOT / 'dist' / name).exists())
    return sorted(files)


def quote_ps(value):
    return "'" + value.replace("'", "''") + "'"


def remote(target, code):
    code = "$ProgressPreference='SilentlyContinue'; " + code
    encoded = base64.b64encode(code.encode('utf-16le')).decode()
    subprocess.run(['ssh', '-o', 'BatchMode=yes', target, 'powershell', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-EncodedCommand', encoded], check=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--dry-run', action='store_true')
    args = parser.parse_args()
    config = json.loads((ROOT / 'local/machines.json').read_text())
    target, destination = config['ssh_target'], config['windows_workspace'].replace('\\', '/')
    if not target or target.startswith('-') or not destination or "\n" in destination:
        raise SystemExit('Invalid machine configuration')
    files = public_files()
    print(f'{len(files)} public files/packages -> {target}:{destination}', flush=True)
    if not args.dry_run:
        archive = ROOT / 'dist/workspace-mirror.zip'
        with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as output:
            for path in files:
                output.write(path, path.relative_to(ROOT).as_posix())
        remote(target, f"$ErrorActionPreference='Stop'; New-Item -ItemType Directory -Force -Path {quote_ps(destination)} | Out-Null")
        upload = destination + '/workspace-mirror.zip'
        subprocess.run(['scp', '-q', str(archive), target + ':' + upload], check=True)
        remote(target, f"$ErrorActionPreference='Stop'; Expand-Archive -LiteralPath {quote_ps(upload)} -DestinationPath {quote_ps(destination)} -Force; Remove-Item -LiteralPath {quote_ps(upload)}; Write-Output 'Public workspace mirror updated; runtime files untouched.'")
