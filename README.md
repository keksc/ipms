# ipms

#### Une application pour envoyer des messages à des adresses ip

### Récupérer l'exécutable

Alpha dans [le dossier release](https://github.com/EuropaFantasy/ipms/tree/main/release), c'est un exécutable linux

### Commentaires

Pour l'instant c'est plus un éditeur de fichiers `.ctc` qu'un envoyeur de pépites testé et fonctionnel sur ubuntu, un peu de debug si vous ouvrez en console

Tous les fichiers et dossiers présents dans le dossier release doivent être dans le même dossier sinon il y aura des problèmes de textures (pour l'instant cela n'affecte que l'icône comme c'est la seule image)

Si vous voulez signaler des bugs c'est dans [ce serveur discord](https://discord.gg/YPfErepBav) channel ipms-bug-report ou ouvrez une github issue

### Compiler

#### Sous linux

Installer le compilateur `gcc` avec:

`sudo apt install gcc`

Télécharger les sources de la librairie wxwidgets sur wxwidgets.org/downloads puis les décompresser

Télécharger ipms en dossier `.zip` et le décompresser

Ouvrir un terminal et naviguer jusqu'à la racine du dossier contenant la source avec `cd`

Exécuter `./configure --disable-shared --with-gtk=3`

Exécuter avec si vous voulez utiliser plus de threads l'argument `-jnbredethreads` `make` puis attendre la fin de la compilation

Exécuter `make install`

Naviguer jusqu'à la racine du dossier d'ipms avec `cd`

Exécuter `make release`

L'exécutable sera dans le sous dossier `release`! 
