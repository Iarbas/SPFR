#!/bin/bash

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

SCRIPTPATH_PARENT="$(dirname "$SCRIPTPATH")"

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

echo -e "Deleting ${RED}build${NC} folder in ${SCRIPTPATH_PARENT}." 
rm -r -v $SCRIPTPATH_PARENT/build
echo -e "${GREEN}Done${NC}.\n"

echo -e "Deleting all ${RED}CMakeCache.txt${NC} files:"
find $SCRIPTPATH_PARENT -name "CMakeCache.txt" -delete -print
echo -e "${GREEN}Done${NC}.\n"

echo -e "Deleting all ${RED}cmake_install.cmake${NC} files:"
find $SCRIPTPATH_PARENT -name "cmake_install.cmake" -delete -print
echo -e "${GREEN}Done${NC}.\n"

echo -e "Deleting all ${RED}Makefile${NC} files:"
find $SCRIPTPATH_PARENT -name "Makefile" -delete -print
echo -e "${GREEN}Done${NC}.\n"

echo -e "Deleting all files containing ${RED}CMakeFiles${NC}:"
find $SCRIPTPATH_PARENT -path '*CMakeFiles*' -delete -print
echo -e "${GREEN}Done${NC}."
