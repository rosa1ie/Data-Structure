# makefile

# Programmers: Nam Ngo, Rosalie Lee
# Date: 11 November 2022
#****************************************************************

all: test

test: test_graph_example.cpp
	g++ -std=c++11 test_graph_example.cpp -o test

clean:
	rm -rf *.o test

