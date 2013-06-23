#! /bin/bash
RESULTFILE=resultlog.log
scriptnames=( "./mesh" "./mesh2" )
scriptcount=2
source=$((RANDOM%10))
sink=$((RANDOM%10))
nodes=(0 1 2 3 4 5 6 7 8 9)
echo "Starting nodes"
while [ $source -eq $sink ]
do
sink=$((RANDOM%10))
done
for i in "${nodes[@]}" 
do
	if [ $i -eq $source ];
	then
		echo "Source: ${scriptnames[$((RANDOM%$scriptcount))]} -q 333$i"
		${scriptnames[$((RANDOM%$scriptcount))]}  -q 333$i &
	elif [ $i -eq $sink ];
	then
		echo "Sink: ${scriptnames[$((RANDOM%$scriptcount))]} -z 333$i"
		${scriptnames[$((RANDOM%$scriptcount))]}  -z 333$i &
	else
		echo "Node: ${scriptnames[$((RANDOM%$scriptcount))]}  333$i"
		${scriptnames[$((RANDOM%$scriptcount))]}  333$i   &
	fi
done

for i in "${nodes[@]}"
do
	for j in 1 2 3 
		do
		echo	"./util.py connect 333$i 333$((RANDOM%10))"
		./util.py connect 333$i 333$((RANDOM%10))
		done

done

echo "echo "first package for the network"|./util.py pipe 333$((RANDOM%10)) 1  "
echo "first package for the network"|./util.py pipe 333$((RANDOM%10)) 1  
echo "echo "package to another node to target"|./util.py pipe 333$((RANDOM%10)) 1 "
echo "package to another node to target"|./util.py pipe 333$((RANDOM%10)) 1  
echo "echo "now first to the source"|./util.py pipe 333$((RANDOM%10)) 0  "
echo "now first to the source"|./util.py pipe 333$((RANDOM%10)) 0  
echo "echo "now second to the source"|./util.py pipe 333$((RANDOM%10)) 0 "
echo "now second to the source"|./util.py pipe 333$((RANDOM%10)) 0  
sleep 1
for i in ${scriptnames[@]}
do
	killall $i;
done
