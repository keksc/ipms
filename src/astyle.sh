#!/bin/bash
astyle Main.cpp -A2
astyle Main.hpp -A2
astyle App.cpp -A2
astyle App.hpp -A2
astyle Contact.cpp -A2
astyle Contact.hpp -A2
astyle Sockets.cpp -A2
astyle Discussion.cpp -A2
astyle Discussion.hpp -A2
astyle Profil.hpp -A2
astyle Profil.cpp -A2
astyle Settings.cpp -A2
astyle Settings.hpp -A2

rm Main.cpp.orig
rm Main.hpp.orig
rm App.cpp.orig
rm App.hpp.orig
rm Contact.cpp.orig
rm Contact.hpp.orig
rm Sockets.cpp.orig
rm Discussion.cpp.orig
rm Discussion.hpp.orig
rm Profil.cpp.orig
rm Profil.hpp.orig
rm Settings.cpp.orig
rm Settings.hpp.orig