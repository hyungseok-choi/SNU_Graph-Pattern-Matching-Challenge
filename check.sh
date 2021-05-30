#!/usr/bin
echo "-compile start-"

# Compile
mkdir build
mkdir -p my_output

cd build
cmake ..
make


echo "-execute your program-"

for i in n1 s1 n3 s3 n5 s5 n8 s8
do
        start=$SECONDS

        # 무한루프를 방지하기 위해 input 당 시간제한
        
        gtimeout 60s ./main/program /Users/hyesoo/Desktop/Graph-Pattern-Matching-Challenge/data/lcc_hprd.igraph /Users/hyesoo/Desktop/Graph-Pattern-Matching-Challenge/query/lcc_hprd_$i.igraph /Users/hyesoo/Desktop/Graph-Pattern-Matching-Challenge/my_output/my_human_$i.txt
        echo "Execution time : $((SECONDS-start)) seconds"

done

