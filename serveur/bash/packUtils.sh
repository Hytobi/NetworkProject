#!/bin/bash

# @author: PLOUVIN Patrice, PLOUVIN Patrick
# @date: 20/12/2022
# @brief: Download the necessary packages to compile the program
# @file: packUtils.sh

# Verifies if the exploitation system is Ubuntu
sys=`cut  -f2 <<< $(lsb_release -d)`
[ "${sys:0:6}" != "Ubuntu" ] && echo "This is not Ubuntu i can't check the following packages" && exit 1

# Do this to upgrade the system
sudo apt update && sudo apt upgrade && echo

######## Install the following packages

# Check if gcc is installed, if not install it
gcc --version &> /dev/null 
if [ `cut -d' ' -f1 <<< $?` -eq 0 ] 
then
    echo "gcc is installed" 
else
    echo "gcc is not installed"
    read -p 'Do you want to install gcc? (y/n): ' gccdl
    if [ $gccdl == "y" ]
    then
        sudo apt-get install gcc
    else
        echo "you don't have gcc, you can't still compile the program" 
    fi
fi
echo

# Check if make is installed, if not install it
make --version &> /dev/null 
if [ `cut -d' ' -f1 <<< $?` -eq 0 ] 
then
    echo "make is installed" 
else
    echo "make is not installed"
    read -p 'Do you want to install make? (y/n): ' makedl
    if [ $makedl == "y" ]
    then
        sudo apt-get install make
    else
        echo "you don't have make, you can't still compile the program" 
    fi
fi
echo

# Check if valgrind is installed, if not install it
valgrind --version &> /dev/null 
if [ `cut -d' ' -f1 <<< $?` -eq 0 ]
then
    echo "valgrind is installed" 
else 
    echo "valgrind is not installed" 
    read -p 'Do you want to install valgrind? (y/n): ' valgrinddl
    if [ $valgrinddl == "y" ]
    then
        sudo apt-get install valgrind
    else
        echo "you don't have valgrind, you can still compile the program but not use the make valgrind command" 
    fi
fi
echo

# We can't check if cunit is installed, so we ask the user
read -p 'Do you have cunit? (y/n) :' cunit
if [ $cunit == "y" ]
then
    echo "cunit is installed" 
else
    echo "cunit is not installed" 
    read -p 'Do you want to install cunit? (y/n): ' cunitdl
    if [ $cunitdl == "y" ]
    then
        sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
    else
        echo "you don't have cunit, you can't still compile the program" 
    fi
fi
echo

# Check if SDL2 is installed, if not install it
sdl2-config --version &> /dev/null 
if [ `cut -d' ' -f1 <<< $?` -eq 0 ] 
then 
    echo "SDL2 is installed"
else
    echo "SDL2 is not installed"
    read -p 'Do you want to install SDL? (y/n): ' sdldl
    if [ $sdldl == "y" ]
    then
        sudo apt-get install libsdl2-dev 
    else
        echo "you don't have SDL, you can't still compile the program" 
    fi
fi
echo

exit 0