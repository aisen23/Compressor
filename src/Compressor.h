#pragma once

#include "Data.h"
#include "ThreadPool.h"

#include <cstdint>
#include <vector>

namespace ai
{
    class Compressor;
    class CompressorImpl
    {
    public:
        explicit CompressorImpl(Compressor* comp);
        virtual ~CompressorImpl() = default;

        virtual std::vector<uint8_t> Compress(const std::vector<int>& array) = 0;
        virtual std::vector<int> Uncompress(const std::vector<uint8_t>& buffer) = 0;

    protected:
        Compressor* _comp = nullptr;
    };


    enum class eCompressorType : uint8_t {
        Compressor,
        Uncompressor
    };


    class Compressor
    {
    public:
        explicit Compressor(eCompressorType type, size_t numThreads = 8);
        ~Compressor();

        std::vector<uint8_t> Compress(const std::vector<int>& array);
        std::vector<int> Uncompress(const std::vector<uint8_t>& buffer);

    private:
        template <typename Function, typename... Args>
        auto ScheduleTask(Function&& f, Args&&... args) {
            if (_pool) {
                _pool->AddTask(std::forward<Function>(f), std::forward<Args>(args)...);
            }
        }

    private:
        eCompressorType _type;
        ThreadPool* _pool = nullptr;

    private:
        Compressor(const Compressor&) = delete;
        Compressor& operator=(const Compressor&) = delete;
    };
};
