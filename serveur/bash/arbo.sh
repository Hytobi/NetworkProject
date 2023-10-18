#!/bin/bash

# @author: PLOUVIN Patrice, PLOUVIN Patrick
# @date: 20/12/2022
# @brief: Print the tree of the project
# @file: arbo.sh

# Check if tree is installed, if not install it
tree --version &> /dev/null 
if [ `cut -d' ' -f1 <<< $?` -eq 0 ] 
then
    tree
else
    echo "tree is not installed"
    read -p 'Do you want to install tree? (y/n): ' treedl
    [ $treedl == "y" ] && sudo apt install tree && tree
fi

exit 0