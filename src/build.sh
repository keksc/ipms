#!/bin/bash
x86_64-w64-mingw32-g++ Main.cpp Main.hpp App.cpp App.hpp NouveauContact.cpp NouveauContact.hpp `/usr/local/wxWidgets/wxMSW/Unicode/bin/wx-config --cxxflags --libs std,aui` -o ipms.exe