cc := g++
src := main.cpp
exe :=  -o digitalClock
libs := -lfmt
flags := -std=c++2a $(libs)

release:
	$(cc) $(src) $(exe) $(flags) -O3

debug:
	$(cc) $(src) $(exe)_dbg $(flags) -Og