#! /bin/bash

echo "Start Shell Script for p1"
trap 'echo -e "\nBe patient\n"' INT
javac ExpAI.java
javac AStarExp_999200555_912143385.java
javac DivAI.java
javac dijkstraai.java

echo "---------Exponent Cost Function Test--------------------------------------"

java Main AStarExp_999200555_912143385 -seed 1&
sleep 5
kill $!
echo -e "Optimal Cost is: 416.0625\n\n"

echo "-----------------seed 2---------------------------------------------------"
java Main AStarExp_999200555_912143385 -seed 2&
sleep 5
kill $!
echo -e "Optimal Cost is 404.34375\n\n"
echo "-----------------seed 3---------------------------------------------------"

java Main AStarExp_999200555_912143385 -seed 3&
sleep 5
kill $!
echo -e "Optimal Cost is 396.15625\n\n"
echo "-----------------seed 4---------------------------------------------------"

java Main AStarExp_999200555_912143385  -seed 4&
sleep 5
kill $!
echo -e "Optimal Cost is 419.453125\n\n"
echo "-----------------seed 5-------------------------------------------------- "

java Main AStarExp_999200555_912143385 -seed 5&
sleep 5
kill $!
echo -e "Optimal Cost is 378.875\n\n"
echo "--------------------------------------------------------------------------"

echo "-------------Doing last Mountain File ------------------------------------"
java Main AStarExp_999200555_912143385 -load MTAFT.XYZ&
time java -AStarExp_999200555_912143385 Main -load MTAFT.XYZ
sleep 10
kill $!
echo -e "Optimal Cost is 505.0 \n\n"
echo "-----------------END OF SHELL SCRIPT---------------------------------------"
