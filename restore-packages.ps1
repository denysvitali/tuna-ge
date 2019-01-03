function Restore-Packages() {
	$proj = get-project
	get-package -project $proj.name | % {
	  Write-Host $_.id;
	  uninstall-package -projectname $proj.name -id $_.id -version $_.version -RemoveDependencies -force ;
	  install-package -projectname $proj.name -id $_.id -version $_.version
	}
}

Restore-Packages