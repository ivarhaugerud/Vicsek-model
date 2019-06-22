CXXFLAGS = -Wall -O3 -larmadillo -std=c++11

program: MainClass.o run_many.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm program *.o
