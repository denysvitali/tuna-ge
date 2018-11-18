$shaCommit = git describe --match=NeVeRmAtCh --always --abbrev=7 --dirty
$commitDate = git log -1 --format=%ad --date=local
$commitSubject = git log -1 --format=%s

$versionFile = Get-Content .\version.cc.in
$versionFile = $versionFile -replace "@GIT_SHA1@", "$shaCommit"
$versionFile = $versionFile -replace "@GIT_DATE@", "$commitDate"
$versionFile = $versionFile -replace "@GIT_COMMIT_SUBJECT@", "$commitSubject"

Set-Content .\version.cc $versionFile