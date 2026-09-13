param([string]$NucleusDirectory = 'C:\NucleusCo-op')
$ErrorActionPreference = 'Stop'
Get-Process NucleusCoop,nwn2,SystemShock2Remastered,service_trace -ErrorAction SilentlyContinue | Select-Object ProcessName,Id,Path
$nucleus = Join-Path $NucleusDirectory 'NucleusCoop.exe'
if (Test-Path -LiteralPath $nucleus) { (Get-Item -LiteralPath $nucleus).VersionInfo | Select-Object FileVersion,ProductVersion }
Get-ItemProperty -LiteralPath 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders' -Name Personal | Select-Object Personal
if (!('CoopXInput' -as [type])) {
    Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class CoopXInput {
    [DllImport("xinput1_4.dll")] private static extern uint XInputGetState(uint index, IntPtr state);
    public static bool Connected(uint index) {
        IntPtr state = Marshal.AllocHGlobal(16);
        try { return XInputGetState(index, state) == 0; }
        finally { Marshal.FreeHGlobal(state); }
    }
}
'@
}
0..3 | ForEach-Object { [pscustomobject]@{ XInputSlot=$_; Connected=[CoopXInput]::Connected($_) } }
