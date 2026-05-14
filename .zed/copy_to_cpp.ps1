New-Item -Path "./build/cpp" -ItemType Directory -Force > $null

$srcDir = "../src"
$destDir = "./build/cpp"
$files = Get-ChildItem -Path $srcDir -File
foreach ($file in $files)
{
    if ($file.Extension -eq ".В††")
    {
        $newName = [System.IO.Path]::ChangeExtension($file.Name, ".cpp")
    } else
    {
        $newName = $file.Name
    }

    Copy-Item -Path $file.FullName -Destination (Join-Path $destDir $newName)
}
Copy-Item -Path "./Ве_крест_крест.h" -Destination (Join-Path $destDir "Ве_крест_крест.h")

$cmakePath = "./CMakeLists.txt.template"
$cmakeContent = Get-Content $cmakePath -Raw
$fileList = @(
    $files | ForEach-Object {
        if ($_.Extension -eq ".В††")
        {
            [System.IO.Path]::ChangeExtension($_.Name, ".cpp")
        } else
        {
            $_.Name
        }
    }

    "Ве_крест_крест.h"
) -join "`n    "

$updatedCmakeContent = $cmakeContent -replace '\{files\}', $fileList
Set-Content -Path (Join-Path $destDir "CMakeLists.txt") -Value $updatedCmakeContent
Copy-Item ./utfcpp ./build/cpp -recurse -Force
