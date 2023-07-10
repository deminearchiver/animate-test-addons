# param(
#     [Parameter(Mandatory)]
#     [ValidateSet('animate11')]
#     [string]
#     $Addon
# )

Clear-Host

Write-Host 'Animate Native Addon Installer'
Write-Host ''
Write-Host 'animate11'
Write-Host '"make Animate truly immersive"'
Write-Host ''
Write-Host 'Author: DeMineArchiver'
Write-Host ''

Pause

Write-Host 'Scanning for Adobe Animate installations'
$registry = Get-ChildItem -Path 'HKLM:/SOFTWARE/Adobe/Animate'
# $registry | ForEach-Object { Get-ChildItem -Path "Registry::$_" } | Where-Object { $_.Name -imatch 'Capabilities$' } | ForEach-Object { Get-ItemProperty -Path "Registry::$_" -Name 'ApplicationName' }

[PSCustomObject[]]$animate_versions = @()

foreach($key in $registry) {
    $language = (Get-ItemProperty -Path "Registry::$($key.Name)").LanguageInstalled
    if($null -eq $language) { continue }
    $name_key = Get-ChildItem -Path "Registry::$($key.Name)" | Where-Object { $_.Name -imatch '\\Capabilities$' }
    $animate_name = (Get-ItemProperty -Path "Registry::$($name_key.Name)" -Name 'ApplicationName').ApplicationName
    if($null -eq $animate_name) { continue }

    $animate_versions += [PSCustomObject]@{
        name = $animate_name
        language = $language
    }
}

if($animate_versions.Length -eq 0) {
    Write-Host 'No Adobe Animate installations found!'
}

$choices = @()
foreach($version in $animate_versions) {
    $choices += [System.Management.Automation.Host.ChoiceDescription]::new("$($version.name -replace '(\d)$', '&$1')", "$($version.name)")
}
$animate_version = $animate_versions[$host.UI.PromptForChoice('', 'Select an animate version', $choices, -1)]


Write-Host 'Fetching GitHub releases'

$releases = (Invoke-WebRequest 'https://api.github.com/repos/DeMineArchiver/animate-test-addons/releases' | ConvertFrom-Json) | Where-Object { $_.tag_name -imatch 'animate11' }
$release = $releases[0]
$asset = $release.assets | Where-Object { $_.name -imatch '\.dll$' }

Write-Host "Latest release: $($release.tag_name)"

$animate_path = Join-Path -Path $env:LOCALAPPDATA -ChildPath 'Adobe' -AdditionalChildPath $animate_version.name, $animate_version.language, 'Configuration', 'External Libraries', $asset.name.ToString()

Pause

Write-Host 'Downloading latest release'
Write-Host "Destination: $animate_path"
Start-BitsTransfer -Source $asset.browser_download_url -Destination $animate_path
