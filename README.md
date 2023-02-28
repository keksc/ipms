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

Un exécutable windows dans le dossier `release`, il dépend des dlls qui sont dans le dossier `windowsdlls` et qui doivent etre dans le meme dossier que l'exécutable (ou dans un dossier qui est dans la variable `path` (non testé))

### En ce moment

Presque fini
