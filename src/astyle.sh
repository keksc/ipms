#!/bin/bash
astyle Main.cpp -A2
astyle Main.hpp -A2
astyle App.cpp -A2
astyle App.hpp -A2
astyle NouveauContact.cpp -A2
astyle NouveauContact.hpp -A2

rm Main.cpp.orig
rm Main.hpp.orig
rm App.cpp.orig
rm App.hpp.orig
rm NouveauContact.cpp.orig
rm NouveauContact.hpp.orig