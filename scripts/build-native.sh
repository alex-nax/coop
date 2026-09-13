#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
compiler="${COOP_CC:-x86_64-w64-mingw32-gcc}"
command -v "$compiler" >/dev/null || { echo 'Install the MinGW x64 cross compiler first.' >&2; exit 1; }
mkdir -p build/nwn2-ee-gog build/diagnostics
"$compiler" -shared -O2 -Wall -Wextra -Werror -static -Wl,--no-insert-timestamp games/nwn2-ee-gog/src/galaxy_compat.c games/nwn2-ee-gog/src/galaxy_compat.def -o build/nwn2-ee-gog/Galaxy64.dll
"$compiler" -O2 -Wall -Wextra -Werror -static -Wl,--no-insert-timestamp tools/diagnostics/galaxy_probe.c -o build/diagnostics/galaxy_probe.exe
"$compiler" -O2 -Wall -Wextra -Werror -static -Wl,--no-insert-timestamp tools/diagnostics/service_trace.c -o build/diagnostics/service_trace.exe
echo 'Built original Windows adapter and diagnostics. Run the isolated Windows probe before deploying a changed adapter.'
