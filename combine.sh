#! /usr/bin/env

function wrap() {
    for file in $@; do
        ( echo "\begin{filecontents*}{${file}}"; cat ${file}; echo "\end{filecontents*}" ) 
    done
}

ln -sf rotate.bbl single-file-version.bbl

( wrap rotate.bib single-file-version.bbl *.cc *_*_*.data rotate_cost.data; cat rotate.tex ) >dummy/single-file-version.tex
