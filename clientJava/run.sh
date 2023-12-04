#!/bin/bash

name="vueTexte/SokobanTexte"

# On compile 
javac -d bin -cp bin:src src/$name.java && echo "Compilation terminée"

# On execute 
#read -p "Lancer le programme ? (y/n) " var
#[ $var = "y" ] && java -cp bin $name && echo

# Le .jar 
if [ ! -f "myapp.mf" ]
then
    touch "myapp.mf"
    echo "Manifest-Version: 1.0" > ./myapp.mf
    echo "Main-Class: $name" > ./myapp.mf
fi
jar cmf myapp.mf myapp.jar -C bin . && mv myapp.jar bin && echo


# On le lance
java -jar bin/myapp.jar