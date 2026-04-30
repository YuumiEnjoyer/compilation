param(
    [string]$SourceDir = "../src"
)

$headerPath = "./Ве_крест_крест.h"
if (-not (Test-Path $headerPath)) {
    Write-Error "Header file not found: $headerPath"
    exit 1
}

$mappings = @{}
$headerContent = Get-Content $headerPath
foreach ($line in $headerContent) {
    if ($line -match '^#define\s+(\S+)\s+(.+)$') {
        $key = $matches[1]
        $value = $matches[2].Trim()
        if ($value.EndsWith(';')) {
            $value = $value.Substring(0, $value.Length - 1)
        }
        $mappings[$key] = $value
    }
    elseif ($line -match '^внедрить\s+(\S+)\s+=(.+)$') {
        $key = "$($matches[1])"
        $value = $matches[2].Trim()
        if ($value.EndsWith(';')) {
            $value = $value.Substring(0, $value.Length - 1)
        }
        $mappings[$key] = $value
    }
}

Write-Host "Loaded $($mappings.Count) mappings from $headerPath"

$srcFiles = Get-ChildItem -Path $SourceDir -Filter "*.cpp" -File

foreach ($file in $srcFiles) {
    Write-Host "Processing $($file.Name)"
    $content = Get-Content $file.FullName -Raw
    $sortedMappings = $mappings.GetEnumerator() | Sort-Object { $_.Key.Length } -Descending
    $content = $content -creplace "#include <iostream>", ""

    foreach ($mapping in $sortedMappings) {
        $pattern = "$($mapping.Value)"
        $content = $content -creplace [regex]::Escape($mapping.Value), $mapping.Key
    }

    if (-not ($content -match '#include\s+"Ве_крест_крест.h"')) {
        $content = "#include `"Ве_крест_крест.h`"$content"
    }

    $newFileName = [System.IO.Path]::ChangeExtension($file.Name, ".В††")
    $outputPath = Join-Path $SourceDir $newFileName
    Set-Content -Path $outputPath -Value $content -Encoding UTF8
    Remove-Item $file.FullName

    Write-Host "Converted file saved as $outputPath"
}

Write-Host "Conversion completed successfully!"
