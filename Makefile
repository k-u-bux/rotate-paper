rotate.pdf: rotate.bbl MakeTimings
	pdflatex rotate

MakeTimings: write_rule.sh
	bash write_rule.sh >MakeTimings

MakeTimingsClang: MakeTimings
	sed s/timings/clang_timings/g <MakeTimings >MakeTimingsClang

MakeTimingsGCC: MakeTimings
	sed s/timings/gcc_timings/g <MakeTimings >MakeTimingsGCC


rotate_cost: rotate_cost.cc
	clang++ -O3 -o rotate_cost rotate_cost.cc

rotate_cost.data: rotate_cost
	rotate_cost 0.00000000000001231 >rotate_cost.data

rotate_cost_01.data: rotate_cost
	rotate_cost 0.01 >rotate_cost_01.data

rotate_cost_02.data: rotate_cost
	rotate_cost 0.02 >rotate_cost_02.data

rotate_cost_10.data: rotate_cost
	rotate_cost 0.1 >rotate_cost_10.data

rotate_cost_20.data: rotate_cost
	rotate_cost 0.2 >rotate_cost_20.data

rotate_cost_30.data: rotate_cost
	rotate_cost 0.3 >rotate_cost_30.data

rotate_cost_50.data: rotate_cost
	rotate_cost 0.5 >rotate_cost_50.data


rotate.bbl: rotate.aux rotate.bib
	bibtex rotate

rotate.aux: rotate.tex rotate_cost.data \
cpu.info \
cache.info \
rotate_cost_01.data \
rotate_cost_02.data \
rotate_cost_10.data \
rotate_cost_20.data \
rotate_cost_30.data \
rotate_cost_50.data \
clang_timings_block_cycle_char.data \
clang_timings_block_cycle_int32.data \
clang_timings_block_cycle_int64.data \
clang_timings_block_cycle_long_double.data \
clang_timings_block_cycle_short.data \
clang_timings_block_swap_char.data \
clang_timings_block_swap_int32.data \
clang_timings_block_swap_int64.data \
clang_timings_block_swap_long_double.data \
clang_timings_block_swap_short.data \
clang_timings_buffering_trinity_char.data \
clang_timings_buffering_trinity_int32.data \
clang_timings_buffering_trinity_int64.data \
clang_timings_buffering_trinity_long_double.data \
clang_timings_buffering_trinity_short.data \
clang_timings_buffering_triple_reverse_char.data \
clang_timings_buffering_triple_reverse_int32.data \
clang_timings_buffering_triple_reverse_int64.data \
clang_timings_buffering_triple_reverse_long_double.data \
clang_timings_buffering_triple_reverse_short.data \
clang_timings_std_char.data \
clang_timings_std_int32.data \
clang_timings_std_int64.data \
clang_timings_std_long_double.data \
clang_timings_std_short.data \
clang_timings_trinity_char.data \
clang_timings_trinity_int32.data \
clang_timings_trinity_int64.data \
clang_timings_trinity_long_double.data \
clang_timings_trinity_short.data \
clang_timings_triple_reverse_char.data \
clang_timings_triple_reverse_int32.data \
clang_timings_triple_reverse_int64.data \
clang_timings_triple_reverse_long_double.data \
clang_timings_triple_reverse_short.data \
gcc_timings_block_cycle_char.data \
gcc_timings_block_cycle_int32.data \
gcc_timings_block_cycle_int64.data \
gcc_timings_block_cycle_long_double.data \
gcc_timings_block_cycle_short.data \
gcc_timings_block_swap_char.data \
gcc_timings_block_swap_int32.data \
gcc_timings_block_swap_int64.data \
gcc_timings_block_swap_long_double.data \
gcc_timings_block_swap_short.data \
gcc_timings_buffering_trinity_char.data \
gcc_timings_buffering_trinity_int32.data \
gcc_timings_buffering_trinity_int64.data \
gcc_timings_buffering_trinity_long_double.data \
gcc_timings_buffering_trinity_short.data \
gcc_timings_buffering_triple_reverse_char.data \
gcc_timings_buffering_triple_reverse_int32.data \
gcc_timings_buffering_triple_reverse_int64.data \
gcc_timings_buffering_triple_reverse_long_double.data \
gcc_timings_buffering_triple_reverse_short.data \
gcc_timings_std_char.data \
gcc_timings_std_int32.data \
gcc_timings_std_int64.data \
gcc_timings_std_long_double.data \
gcc_timings_std_short.data \
gcc_timings_trinity_char.data \
gcc_timings_trinity_int32.data \
gcc_timings_trinity_int64.data \
gcc_timings_trinity_long_double.data \
gcc_timings_trinity_short.data \
gcc_timings_triple_reverse_char.data \
gcc_timings_triple_reverse_int32.data \
gcc_timings_triple_reverse_int64.data \
gcc_timings_triple_reverse_long_double.data \
gcc_timings_triple_reverse_short.data
	pdflatex rotate.tex

tests: tests.cc rotate.cc \
incl_basic_triple_reverse.cc \
incl_blend.cc \
incl_block_cycle.cc \
incl_block_async_cycle.cc \
incl_block_swap.cc \
incl_buffering_algorithms.cc \
incl_buffering.cc \
incl_checking_algorithms.cc \
incl_checking.cc \
incl_unchecked_trinity.cc \
incl_optimal_rotate.cc
	clang++ -O3 -o tests tests.cc

clang_timings: timings.cc rotate.cc \
incl_basic_triple_reverse.cc \
incl_blend.cc \
incl_block_cycle.cc \
incl_block_async_cycle.cc \
incl_block_swap.cc \
incl_buffering_algorithms.cc \
incl_buffering.cc \
incl_checking_algorithms.cc \
incl_checking.cc \
incl_unchecked_trinity.cc \
incl_optimal_rotate.cc
	clang++ -O3 -DNDEBUG -o clang_timings timings.cc

gcc_timings: timings.cc rotate.cc \
incl_basic_triple_reverse.cc \
incl_blend.cc \
incl_block_cycle.cc \
incl_block_async_cycle.cc \
incl_block_swap.cc \
incl_buffering_algorithms.cc \
incl_buffering.cc \
incl_checking_algorithms.cc \
incl_checking.cc \
incl_unchecked_trinity.cc \
incl_optimal_rotate.cc
	g++ -O3 -DNDEBUG -o gcc_timings timings.cc

clang_timings.data: clang_timings
	clang_timings >clang_timings.data

gcc_timings.data: gcc_timings
	gcc_timings >gcc_timings.data

timings.data: clang_timings.data gcc_timings.data
	cp gcc_timings.data timings.data

inplace_merge_sorts_clang: inplace_merge_sorts.cc rotate.cc \
incl_basic_triple_reverse.cc \
incl_blend.cc \
incl_block_cycle.cc \
incl_block_async_cycle.cc \
incl_block_swap.cc \
incl_buffering_algorithms.cc \
incl_buffering.cc \
incl_checking_algorithms.cc \
incl_checking.cc \
incl_unchecked_trinity.cc \
incl_optimal_rotate.cc
	clang++ -O3 -DNDEBUG -o inplace_merge_sorts_clang inplace_merge_sorts.cc

inplace_merge_sorts_gcc: inplace_merge_sorts.cc rotate.cc \
incl_basic_triple_reverse.cc \
incl_blend.cc \
incl_block_cycle.cc \
incl_block_async_cycle.cc \
incl_block_swap.cc \
incl_buffering_algorithms.cc \
incl_buffering.cc \
incl_checking_algorithms.cc \
incl_checking.cc \
incl_unchecked_trinity.cc \
incl_optimal_rotate.cc
	g++ -O3 -DNDEBUG -o inplace_merge_sorts_gcc inplace_merge_sorts.cc

sort_clang_timings.data: inplace_merge_sorts_clang
	inplace_merge_sorts_clang >sort_clang_timings.data

sort_gcc_timings.data: inplace_merge_sorts_gcc
	inplace_merge_sorts_gcc >sort_gcc_timings.data

sort_timings.data: sort_clang_timings.data sort_gcc_timings.data
	cp sort_clang_timings.data sort_timings.data

cpu.info:
	lscpu | grep 'Model name' | cut -c39- >cpu.info

cache.info:
	lscpu -C >cache.info

clean:
	rm *.info *.log *.pdf *.aux *.bbl *.blg *.data MakeTimings MakeTimingsClang MakeTimingsGCC tests rotate_cost inplace_merge_sorts_gcc inplace_merge_sorts_clang gcc_timings clang_timings

include MakeTimings
include MakeTimingsClang
include MakeTimingsGCC

# end of file
