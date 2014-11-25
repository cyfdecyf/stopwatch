#include "stopwatch.h"

#include <stdio.h>

const int N = 10000000;

template<typename T>
void measure_cost(T &stopwatch, const char *msg) {
    // Measure the cost of StopWatchTSC which uses rdtsc.
    StopWatch watch;
    {
        ScopeTime time(watch);
        T t;
        for (int i = 0; i < N; i++) {
            t.start();
            t.stop();
        }
    }
    printf("%s: %f us\n", msg, (double)(watch.total())/N);
}

int main(int argc, const char *argv[])
{
    StopWatch gtod;
    measure_cost(gtod, "StopWatch (gettimeofday)");

    StopWatchTSC tsc;
    measure_cost(tsc, "StopWatchTSC (rdtsc)");

    return 0;
}

