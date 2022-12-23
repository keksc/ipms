#!/bin/bash
x86_64-w64-mingw32-g++ Main.cpp Main.h App.cpp App.h NouveauContact.cpp NouveauContact.h `/usr/local/wxWidgets/wxMSW/Unicode/bin/wx-config --cxxflags --libs std,aui` -o ipms.exe