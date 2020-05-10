#include <iostream>
#include <sys/time.h>

#include <benchmark/benchmark.h>


#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#include <chrono>

#endif


uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

static void assemblyVersionrdrsc (benchmark::State& state) {
    long l {0};
    for (auto _ : state ) {
        auto t1=rdtsc();
        auto t2=rdtsc();
        l+=t2-t1;
    }
}

static void prebuildVersionrdrsc (benchmark::State& state) {
    long l {0};
    for (auto _ : state ) {
        auto t1=__rdtsc();
        auto t2=__rdtsc();
        l+=t2-t1;
    }
}

static void getTimeOfDay (benchmark::State& state) {
    timeval tv1;
    timeval tv2;

    long l {0};
    for (auto _ : state ) {
        gettimeofday (&tv1,nullptr);
        gettimeofday (&tv2,nullptr);
        l+=(tv2.tv_usec+tv2.tv_sec*1000000)-(tv1.tv_usec+tv1.tv_sec*1000000);
    }
}

static void highResolutionTimer (benchmark::State& state) {
    decltype (std::chrono::high_resolution_clock::now()) tp1;
    decltype (std::chrono::high_resolution_clock::now()) tp2;

    long l {0};
    for (auto _ : state ) {
        tp1 = std::chrono::high_resolution_clock::now();
        tp2 = std::chrono::high_resolution_clock::now();
        l+=std::chrono::duration_cast<std::chrono::nanoseconds>(tp2 - tp1).count () ;
    }
}

/*
int main() {

    for (int i=0; i<100; i++) {
        uint64_t tsc = rdtsc();
        uint64_t tsc2 = rdtsc();
        std::cout << tsc2 - tsc << std::endl;
    };

    for (int i=0; i<100; i++) {
        uint64_t tsc = __rdtsc();
        uint64_t tsc2 = __rdtsc();
        std::cout << tsc2 - tsc << std::endl;
    };

    timeval tv1;
    timeval tv2;

    for (int i=0; i<100; i++) {
        gettimeofday(&tv1, nullptr);
        gettimeofday(&tv2, nullptr);
        std::cout << (tv2.tv_usec + tv2.tv_sec * 1000000) - (tv1.tv_usec + tv1.tv_sec * 1000000) <<std::endl;
    }

    for (int i=0; i<100; i++) {
        auto tp1 = std::chrono::high_resolution_clock::now();
        auto tp2 = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(tp2 - tp1).count () << std::endl;
    };

    return 0;
}
*/

BENCHMARK (assemblyVersionrdrsc);
BENCHMARK (prebuildVersionrdrsc);
BENCHMARK (getTimeOfDay);
BENCHMARK (highResolutionTimer);

BENCHMARK_MAIN();

