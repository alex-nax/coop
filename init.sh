#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
command -v python3 >/dev/null || { echo 'Python 3 is required.' >&2; exit 1; }
command -v node >/dev/null || { echo 'Node.js is required for handler checks.' >&2; exit 1; }
mkdir -p build dist local storage/assets storage/legacy
python3 scripts/validate.py
echo 'Workspace ready. See README.md for native builds, Windows testing and packaging.'
