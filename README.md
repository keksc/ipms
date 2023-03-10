# ipms

#### Une application pour envoyer des messages à des adresses ip

### Récupérer l'exécutable

Alpha dans [le dossier release](https://github.com/EuropaFantasy/ipms/tree/main/release), c'est un exécutable linux

### Commentaires

Pour l'instant c'est plus un éditeur de fichiers `.ctc` qu'un envoyeur de pépites testé et fonctionnel sur ubuntu, un peu de debug si vous ouvrez en console

Tous les fichiers et dossiers présents dans le dossier release doivent être dans le même dossier sinon il y aura des problèmes de textures (pour l'instant cela n'affecte que l'icône comme c'est la seule image)

Si vous voulez signaler des bugs c'est dans [ce serveur discord](https://discord.gg/YPfErepBav) channel ipms-bug-report ou ouvrez une github issue

### Compiler

#### Sous linux (testé kubuntu)

Installer le compilateur `gcc` avec:

`sudo apt install gcc`

Télécharger les sources de la librairie wxwidgets sur [le site officiel](wxwidgets.org/downloads) puis les décompresser

Télécharger ipms en dossier `.zip`, ou `.7z` si vous avez un utilitaire pour ouvrir les archives et le décompresser

Ouvrir un terminal et naviguer jusqu'à la racine du dossier contenant la source avec `cd`

Exécuter `./configure --disable-shared --with-gtk=3`

Exécuter `make` avec si vous voulez utiliser plus de threads l'argument `-jNbredethreads` puis attendre la fin de la compilation

Exécuter `make install`

Naviguer jusqu'à la racine du dossier d'ipms avec `cd`

Exécuter `make release`

L'exécutable sera dans le sous dossier `release`! 

#### Sous Windows
Installer `msys2`sur [le site officiel](https://msys2.org) (suivre les étapes dont celle sur l'installation de mingw sur le site).

Dans l'invite de commandes de `msys`installer les libs wxWidgets (pour le moment version 3.2) avec `pacman -S mingw-w64-x86_64-wxwidgets3.2-msw`

Ouvrir `msys2 UCRT64`

Télécharger le code source, puis naviguer jusqu'au dossier src avec l'invite `UCRT64` (pour changer de lecteur naviguez jusqu'à la racine avec `cd ..` puis `cd nomDuLecteur(ex:c)`

Executer `make release`, l'exécutable sera dans le dossier `release`. Il a neanmoins des dependances (dlls), elles se triuvent dans le dossier de libs de gcc dans le dossier de msys (je donnerai plus tard les noms mais elles sont sur github dans release/windowsdlls) et si vous voulez l'executer rapidement depuis la shell msys faites `cd release` puis `./ipms.out`. Si vous voulez l'executer a part avec les dlls il faudra renommer ipms.out en ipms.exe


### En ce moment

Éditeur de profils de cryptages
Bientôt fini ;) le plus dur est fait
