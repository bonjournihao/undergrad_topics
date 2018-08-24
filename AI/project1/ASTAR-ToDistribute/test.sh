#! /bin/bash

echo "Start Shell Script for p1"
trap 'echo -e "\nBe patient\n"' INT
javac ExpAI.java
javac DivAI.java
javac dijkstraai.java

echo "---------Division Cost Function Test--------------------------------------"

java Main DivAI  -div -seed 1&
sleep 10
kill $!
echo -e "Optimal Cost is: 198.40953301482975\n\n"

echo "-----------------seed 2---------------------------------------------------"
java Main DivAI  -div -seed 2&
sleep 10
kill $!
echo -e "Optimal Cost is 198.3505826306458\n\n"
echo "-----------------seed 3---------------------------------------------------"

java Main DivAI  -div -seed 3&
sleep 10
kill $!
echo -e "Optimal Cost is 198.45353447487392\n\n"
echo "-----------------seed 4---------------------------------------------------"

java Main DivAI  -div -seed 4&
sleep 10
kill $!
echo -e "Optimal Cost is 198.29551443251432\n\n"
echo "-----------------seed 5-------------------------------------------------- "

java Main DivAI  -div -seed 5&
sleep 10
kill $!
echo -e "Optimal Cost is 198.60345691194533\n\n"
echo "--------------------------------------------------------------------------"
echo -e "\n\n\n"

echo "---------Exponent Cost Function Test--------------------------------------"

java Main ExpAI -seed 1&
sleep 10
kill $!
echo -e "Optimal Cost is: 416.0625\n\n"

echo "-----------------seed 2---------------------------------------------------"
java Main ExpAI -seed 2&
sleep 10
kill $!
echo -e "Optimal Cost is 404.34375\n\n"
echo "-----------------seed 3---------------------------------------------------"

java Main ExpAI -seed 3&
sleep 10
kill $!
echo -e "Optimal Cost is 396.15625\n\n"
echo "-----------------seed 4---------------------------------------------------"

java Main ExpAI  -seed 4&
sleep 10
kill $!
echo -e "Optimal Cost is 419.453125\n\n"
echo "-----------------seed 5-------------------------------------------------- "

java Main ExpAI -seed 5&
sleep 10
kill $!
echo -e "Optimal Cost is 378.875\n\n"
echo "--------------------------------------------------------------------------"

echo "-------------Doing last Mountain File ------------------------------------"
java Main ExpAI  -chaotic -load MTAFT.XYZ&
sleep 10
kill $!
echo -e "Optimal Cost is 505.0 \n\n"
echo "-----------------END OF SHELL SCRIPT---------------------------------------"
