$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location -LiteralPath $projectRoot
Write-Host 'Opening the PulseBar desktop interface at http://127.0.0.1:8080'
Start-Process 'http://127.0.0.1:8080'
python tools/preview_server.py
