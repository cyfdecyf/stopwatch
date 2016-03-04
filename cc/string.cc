#include <stdlib.h>
#include <string.h>

#include <string>
#include <sstream>
#include <vector>

#include "../stopwatch.h"

using std::string;
using std::vector;
using std::ostringstream;

const int N_STRING = 1000000;

// Speed order:
// 1. use_memcpy
// 2. use_string_append (with reserved memory, difference with memcpy is small)
// 3. use_stringstream (can't find an easy way to set internal buffer size)

static inline string use_memcpy(const vector<string> vec) {
    size_t bufsize = 0;
    for (const auto& vs : vec) {
        bufsize += vs.size();
    }
    char* buf = static_cast<char*>(operator new(bufsize));

    StopWatch watch;
    {
        ScopeTime timer(watch);
        char* p = buf;
        for (const auto& vs : vec) {
            memcpy(p, vs.c_str(), vs.size());
            p += vs.size();
        }
    }
    printf("memcpy=      : %f us\n", (double)(watch.total()));

    string s(buf, bufsize);
    delete buf;
    return s;
}

static inline string use_string_append(const vector<string> vec) {
    string s;
    s.reserve(15 * N_STRING);
    StopWatch watch;
    {
        ScopeTime timer(watch);
        for (const auto& vs : vec) {
            s += vs;
        }
    }
    printf("string+=     : %f us\n", (double)(watch.total()));

    return s;
}

static inline string use_stringstream(const vector<string> vec) {
    ostringstream os;
    StopWatch watch;
    {
        ScopeTime timer(watch);
        for (const auto& vs : vec) {
            os << vs;
        }
    }
    printf("ostringstream: %f us\n", (double)(watch.total()));

    return os.str();
}

int main(int argc, const char *argv[])
{
    srandom(0);

    vector<string> vec;
    for (int i = 0; i < N_STRING; i++) {
        vec.push_back(std::to_string(random()));
    }

    use_memcpy(vec);
    use_string_append(vec);
    use_stringstream(vec);

    return 0;
}
