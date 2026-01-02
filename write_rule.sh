#! /usr/bin/env bash

a_rule () {
    FILE="$1"
    TYPE="$2"
    ALG="$3"
    ( sed s/file/${FILE}/g | sed s/alg/${ALG}/g | sed s/type/${TYPE}/g ) <<EOF
file_alg_type.data: file.data
	echo "x, y" >file_alg_type.data
	grep type file.data | grep '::alg' | awk '{print(\$\$6", "\$\$9)}' >>file_alg_type.data
EOF
    echo
}

b_rule () {
    FILE="$1"
    TYPE="$2"
    ALG="$3"
    ( sed s/file/${FILE}/g | sed s/alg/${ALG}/g | sed s/type/${TYPE}/g ) <<EOF
file_alg_type.data: file.data
	echo "x, y" >file_alg_type.data
	grep type file.data | grep '::alg' | awk '{print(\$\$6" "\$\$10)}' >>file_alg_type.data
EOF
    echo
}

c_rule () {
    FILE="$1"
    TYPE="$2"
    ALG="$3"
    ( sed s/file/${FILE}/g | sed s/alg/${ALG}/g | sed s/type/${TYPE}/g ) <<EOF
file_alg_type.data: file.data
	echo "x, y" >file_alg_type.data
	grep type file.data | grep 'alg' | awk '{print(\$\$6" "\$\$10)}' >>file_alg_type.data
EOF
    echo
}

for FILE in timings; do
    for ALG in optimal_rotate block_cycle block_async_cycle block_swap std trinity buffering_trinity triple_reverse buffering_triple_reverse blend; do
        for TYPE in long_double int64 int32 short char u_ptr x_ptr s_ptr; do
            a_rule $FILE $TYPE $ALG
        done
    done
done

for FILE in sort_timings; do
    for ALG in block_cycle block_async_cycle block_swap trinity buffering_trinity triple_reverse buffering_triple_reverse blend std_implementation; do
        for TYPE in long_double int64 int32 short char; do
            b_rule $FILE $TYPE $ALG
        done
    done
done

for FILE in sort_timings; do
    for ALG in std_sort std_stable_sort std_partial_sort; do
        for TYPE in long_double int64 int32 short char; do
            c_rule $FILE $TYPE $ALG
        done
    done
done
