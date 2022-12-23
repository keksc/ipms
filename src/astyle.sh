#!/bin/bash
astyle Main.cpp -A2
astyle Main.h -A2
astyle App.cpp -A2
astyle App.h -A2
astyle NouveauContact.cpp -A2
astyle NouveauContact.h -A2

rm Main.cpp.orig
rm Main.h.orig
rm App.cpp.orig
rm App.h.orig
rm NouveauContact.cpp.orig
rm NouveauContact.h.orig