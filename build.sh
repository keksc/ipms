#!/bin/bash
g++ Main.cpp Main.h App.cpp App.h `wx-config --cxxflags --libs` -g -O0 -DDEBUG -o ipms.out
