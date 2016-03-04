all: cost cc/string

CFLAGS = -std=c++1y

cost: cost.cpp stopwatch.h
	g++ $(CFLAGS) $< -o $@

cc/string: cc/string.cc stopwatch.h
	g++ $(CFLAGS) $< -o $@

clean:
	rm -f cost string
