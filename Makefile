CXX = g++ 
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

test: 
	$(CXX) $(CXXFLAGS) tests.cpp -o test.exe
	test.exe

clean: del test.exe

.PHONY: test