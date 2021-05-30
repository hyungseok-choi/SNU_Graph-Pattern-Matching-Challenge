#!/bin/bash

echo "Running cmake .."
cmake ..
echo "cmake done"

echo "--------------------------------"

echo "Running make"
make
echo "make done"

echo "--------------------------------"

echo "Executing First Graph with First Query"
for i in s8
do
        start=$SECONDS

        # 무한루프를 방지하기 위해 input 당 시간제한
        
        gtimeout 60s ./main/program /Users/hyesoo/Desktop/Graph-Pattern-Matching-Challenge/data/lcc_yeast.igraph /Users/hyesoo/Desktop/Graph-Pattern-Matching-Challenge/query/lcc_yeast_$i.igraph /Users/hyesoo/Desktop/Graph-Pattern-Matching-Challenge/candidate_set/lcc_yeast_$i.cs
        echo "Execution time : $((SECONDS-start)) seconds"

done
echo "end"

# graphdir=../data
# querydir=../query
# csdir=../candidate_set

# for cs in $csdir/*

# do
#     echo "${graph##*/} and lcc_hprd_n1.igraph"
# done