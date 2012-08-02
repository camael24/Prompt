Prompt
======

Executable , permettant de lancer des programmes en temps que service , avec ou sans fenêtre console.

Prérequis : 
	-	Présence de srvany dans le dossier C:\Windows\system32\ - http://support.microsoft.com/kb/137890
	
Utilisation :
>	$ Prompt.exe
>	Path = php-cgi -b 127.0.0.1:9000, Name = Launchee, Signal = help
>	Usage of Prompt:
>		-p  <path>      PathProgram (Path to php-cgi , if php-cgi are already in the global path do nothing ;) )
>		-n  <name>      NameService
>		-s  <signal>    Signal (install | uninstall | start | stop | restart)
>		-h              This Help
>		-v              Version
		

Prompt a été crée à l'origine pour lancé php-cgi en temps que service , mais c'est adaptable pour n'importe quel executable a n'importe quel endroit ;).
	$ Prompt.exe -p /path/to/you/exe -n AnNameOfService -s start

Va crée et lancé le service AnNameOfService et lancé l'executable /path/to/your/exe

Les Signaux
-------
* install : Installe le service
* uninstall : Désinstalle le service (ou sc delete NameService)
* start : [Install ou Initialize] et Démarre le servite
* stop : [Install ou Initialize] et Stoppe le servite
* restart : Stop && Start signal

Silent
======

Silent est un alias pleinement fonctionnel de Prompt même code source , mais avec un Build Target "GUI Application" de façon a être "invisible"




