#!/bin/bash

#### compile
make clean
make run  

#### methods: 
# [0]: ENUM(S)
# [1]: ENUM*
# [2]: ENUM*-L
# [3]: ENUM*-R
# [4]: ENUM*-LR
# [5]: ENUM*-h
# [6]: ENUM*-h-LR


# all methods
key="test"
methods=(0)
k=5

for method in ${methods[*]}
    do
        echo ${key} "m"${method} "k="${k}
        ./run ${key} ${method} ${k}
    done


