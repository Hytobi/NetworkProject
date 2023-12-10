#!/bin/bash

name="Main"

# On supprime les .class
rm -rf bin/*.class

# On compile 
#javac -d bin -cp bin:src src/$name.java && echo "Compilation terminée"
javac -d ./bin -cp ./lib/jackson-annotations-2.16.0.jar:./lib/jackson-core-2.16.0.jar:./lib/jackson-databind-2.16.0-rc1.jar:src src/*.java

# On execute 
#read -p "Lancer le programme ? (y/n) " var
#[ $var = "y" ] && java -cp bin $name && echo

# Le .jar 
if [ ! -f "myapp.mf" ]
then
    touch "myapp.mf"
    echo "Manifest-Version: 1.0" > ./myapp.mf
    echo "Main-Class: $name" >> ./myapp.mf
    echo "Class-Path: ./lib/jackson-annotations-2.16.0.jar ./lib/jackson-core-2.16.0.jar ./lib/jackson-databind-2.16.0-rc1.jar" >> ./myapp.mf
fi
jar cmf myapp.mf myapp.jar -C bin . && mv myapp.jar bin && echo


# On le lance
java -cp ./lib/jackson-annotations-2.16.0.jar:./lib/jackson-core-2.16.0.jar:./lib/jackson-databind-2.16.0-rc1.jar:./bin $name
#java -jar bin/myapp.jar