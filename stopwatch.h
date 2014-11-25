// A simple stop watch implementation.

#ifndef __STOPWATCH_H
#define __STOPWATCH_H

#include <stdint.h>
#include <sys/time.h>

// Stop watch using gettimeofday, measures time in us.
class StopWatch {
public:
    StopWatch() : _total_sec(0), _total_usec(0), _count(0) {
        _last.tv_sec = 0;
        _last.tv_usec = 0;
    }

    void start() {
        gettimeofday(&_last, NULL);
    }

    void stop() {
        struct timeval now;
        gettimeofday(&now, NULL);
        _total_sec += now.tv_sec - _last.tv_sec;
        _total_usec += now.tv_usec - _last.tv_usec;
        _count++;
    }

    void reset() {
        _total_sec = 0;
        _total_usec = 0;
        _count = 0;
    }

    uint64_t total() { return (uint64_t)(_total_sec * 1e6 + _total_usec); }
    uint64_t count() { return _count; }

private:
    struct timeval _last;
    int64_t _total_sec;
    int64_t _total_usec;
    uint64_t _count;
};

static inline __attribute__((always_inline)) uint64_t rdtsc(void) {
    uint32_t a, d;
    __asm __volatile("rdtsc":"=a"(a), "=d"(d));
    return ((uint64_t)a) | (((uint64_t)d) << 32);
}

// Stop watch using rdtsc, measures time in cycles.
class StopWatchTSC {
public:
    StopWatchTSC() : _last(0), _total(0), _count(0) {}

    void start() {
        _last = rdtsc();
    }

    void stop() {
        _total += rdtsc() - _last;
        _count++;
    }

    void reset() {
        _total = 0;
        _count = 0;
    }

    uint64_t total() { return _total; }
    uint64_t count() { return _count; }

private:
    uint64_t _last;
    uint64_t _total;
    uint64_t _count;
};

template<typename T>
class ScopeTimeT {
public:
    ScopeTimeT(T &watch) : _watch(watch) { _watch.start(); }
    ~ScopeTimeT() { _watch.stop(); }

private:
    T &_watch;
};

typedef ScopeTimeT<StopWatch> ScopeTime;
typedef ScopeTimeT<StopWatchTSC> ScopeTimeTSC;

#endif // __STOPWATCH_H

