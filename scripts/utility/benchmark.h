#pragma once

#include <string>

#include <SDL_timer.h>
#include <SDL_log.h>

struct Benchmark {
    std::string mark;
    uint64_t    timestamp;

public:
    explicit Benchmark(std::string mark)
        : mark(std::move(mark)), timestamp(SDL_GetTicks64())
    {}

    ~Benchmark() {
        SDL_Log("Benchmark [%s]: %llums", mark.data(), SDL_GetTicks64() - timestamp);
    }
};
