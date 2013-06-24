# Copyright by Raphael Marques
#! /bin/bash
echo $1


RESULTFILE=resultlog


NODECOUNT=11
scriptnames=( "./mesh" "./mesh2" )
scriptcount=2
source=$((RANDOM%$NODECOUNT))
sink=$((RANDOM%$NODECOUNT))
nodes=(0 1 2 3 4 5 6 7 8 9 10)
echo "Starting nodes"
while [ $source -eq $sink ]
do
sink=$((RANDOM%$NODECOUNT))
done
for i in "${nodes[@]}" 
do
	if [ $i -eq $source ];
	then
		echo "Source: ${scriptnames[$((RANDOM%$scriptcount))]} -q 333$i"
		if [ $1 -eq 1 ]
		then
			${scriptnames[$((RANDOM%$scriptcount))]}  -q 333$i &	
		else	
			${scriptnames[$((RANDOM%$scriptcount))]}  -q 333$i &>> $RESULTFILE$i.log &
		fi
	elif [ $i -eq $sink ];
	then
		echo "Sink: ${scriptnames[$((RANDOM%$scriptcount))]} -z 333$i"
		if [ $1 -eq 1 ]
		then
			${scriptnames[$((RANDOM%$scriptcount))]}  -z 333$i  &
		else
			${scriptnames[$((RANDOM%$scriptcount))]}  -z 333$i &>> $RESULTFILE$i.log &
		fi
	else
		echo "Node: ${scriptnames[$((RANDOM%$scriptcount))]}  333$i"
		if [ $1 -eq 1 ]
		then
			${scriptnames[$((RANDOM%$scriptcount))]}  333$i   &
		else
			${scriptnames[$((RANDOM%$scriptcount))]}  333$i  &>> $RESULTFILE$i.log &
		fi
	fi
done

for i in "${nodes[@]}"
do
	for j in 1 2 3 
		do
		echo	"./util.py connect 333$i 333$((RANDOM%$NODECOUNT))"
		./util.py connect 333$i 333$((RANDOM%$NODECOUNT))
		done

done
FIRST=$((RANDOM%$NODECOUNT))
SECOND=$((RANDOM%$NODECOUNT))
THIRD=$((RANDOM%$NODECOUNT))
FOURTH=$((RANDOM%$NODECOUNT))

echo "echo "first package for the network"|./util.py pipe 333$FIRST 1 11 "
echo "first package for the network"|./util.py pipe 333$FIRST 1 11 

echo "echo "next packet same route package for the network"|./util.py pipe 333$FIRST 1 12 "
echo "next packet same route package for the network"|./util.py pipe 333$FIRST 1 12
sleep 5
echo "echo "same package other route, any timeout? package to another node to target"|./util.py pipe 333$SECOND 1 12"
echo "same package other route, any timeout? package to another node to target"|./util.py pipe 333$SECOND 1 12 
echo "echo "now first to the source"|./util.py pipe 333$THIRD 0  15 "
echo "now first to the source"|./util.py pipe 333$THIRD 0 15 
echo "echo "now second to the source"|./util.py pipe 333$FOURTH 0 16 "
echo "now second to the source"|./util.py pipe 333$FOURTH 0 16 
sleep 1
for i in ${scriptnames[@]}
do
	killall $i;
done
