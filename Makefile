cost: cost.cpp stopwatch.h
	g++ cost.cpp -o $@

clean:
	rm -f cost
