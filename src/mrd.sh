#!/bin/bash

# Compilation du projet avec make
make 2>&1 | grep --color=auto -E '^|.*error.*|.*warning.*'
make_exit_code=$?

# Si la compilation s'est déroulée sans erreur
if [ $make_exit_code -eq 0 ]; then
  # Exécution de la solution pour tester gcc
  ./debug/ipms.out
else
  echo "La compilation a échoué"
fi
