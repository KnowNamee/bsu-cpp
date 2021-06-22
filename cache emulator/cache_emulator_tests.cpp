//
// Created by ZenBook on 25.11.2019.
//


#include "gtest.h"
#include "cache_emulator.h"

#ifdef YANDEX_CONTEST_MODE
#include "hardware_interface.h"
#else
#include "my_hardware_interface.h"
#endif  // YANDEX_CONTEST_MODE


#include <fstream>
#include <numeric>

// all tests contain :
// line_size = 8
// associativity = 4
// lines_count = 6
// cache_size = line_size * associativity -> 1 bank

namespace cache {

using cache::CacheEmulator;
using hardware_interface::MainMemoryEmulator;
using hardware_interface::RandomLineNumberGenerator;

static uint64_t line_size = 8;
static uint64_t associativity = 4;
static uint64_t line_count = 6;

typedef std::vector<std::vector<uint8_t>> dvec;

std::vector<std::vector<uint8_t>> FormData() {
    return dvec(6, std::vector<uint8_t>(8, 0));
}

void WriteInData(
        dvec* data,
        const std::vector<uint8_t>& bytes,
        uint64_t address) {
    uint64_t line = address / line_size;
    uint64_t line_start_index = address % line_size;
    uint64_t cur_byte_index = 0;
    for (int i = line; i < data->size(); i++) {
        for (int j = address % line_size; j < line_size; j++) {
            if (cur_byte_index == bytes.size()) {
                return;
            }
            (*data)[i][j] = bytes[cur_byte_index++];
            address++;
        }
    }
}

void PrintDataInFile(
        const std::string& filename,
        const dvec& data) {
    std::ofstream out(filename);
    for (int i = 0; i < data.size(); i++) {
        for (int e : data[i]) {
            out << e << " ";
        }
        out << "\n\n";
    }
    out.close();
}

TEST(Cache_Tests, WriteThrough1) {
    dvec data  = FormData();

    ASSERT_EQ(data.size(), 6);

    MainMemoryEmulator::Set(data);
    RandomLineNumberGenerator::Initialise(associativity);

    CacheEmulator::Init(
            associativity * line_size,
            associativity,
            line_size,
            CacheReplacementPolicy::kLeastRecentlyUsed,
            CacheWritePolicy::kWriteThrough);
        std::vector<uint8_t> bytes(12);
        std::iota(bytes.begin(), bytes.end(), 1);

        CacheEmulator::WriteBytes(bytes, 1);

        dvec res_data = {{0, 1, 2,  3,  4,  5, 6, 7},
                         {8, 9, 10, 11, 12, 0, 0, 0},
                         {0, 0, 0,  0,  0,  0, 0, 0},
                         {0, 0, 0,  0,  0,  0, 0, 0},
                         {0, 0, 0,  0,  0,  0, 0, 0},
                         {0, 0, 0,  0,  0,  0, 0, 0}};

        ASSERT_EQ(res_data, MainMemoryEmulator::Get());
        ASSERT_EQ(CacheEmulator::GetHitsCount(), 0);
        ASSERT_EQ(CacheEmulator::GetMissesCount(), 2);

        std::vector<uint8_t> read_bytes = CacheEmulator::ReadBytes(1, 12);

        ASSERT_EQ(read_bytes, bytes);
        ASSERT_EQ(CacheEmulator::GetHitsCount(), 2);
        ASSERT_EQ(CacheEmulator::GetMissesCount(), 2);
}

TEST(Cache_Tests, WriteThrough2) {
        dvec data  = FormData();

        ASSERT_EQ(data.size(), 6);

        MainMemoryEmulator::Set(data);
        RandomLineNumberGenerator::Initialise(associativity);

        CacheEmulator::Init(
                associativity * line_size,
                associativity,
                line_size,
                CacheReplacementPolicy::kLeastRecentlyUsed,
                CacheWritePolicy::kWriteThrough);

        dvec res_data = {{1, 2, 3, 4, 5, 6, 7, 8},
                         {9, 10, 11, 12, 13, 14, 15, 16},
                         {17, 18, 19, 20, 21, 22, 23, 24},
                         {25, 26, 27, 28, 29, 30, 31, 32},
                         {33, 34, 35, 36, 37, 38, 39, 40},
                         {41, 42, 43, 44, 45, 46, 47, 48}};

        std::vector<uint8_t> bytes(48);
        std::iota(bytes.begin(), bytes.end(), 1);

        CacheEmulator::WriteBytes(bytes, 0);

        ASSERT_EQ(res_data, MainMemoryEmulator::Get());
        ASSERT_EQ(CacheEmulator::GetMissesCount(), 6);

        ASSERT_EQ(CacheEmulator::ReadBytes(0,  48), bytes);
        ASSERT_EQ(CacheEmulator::GetHitsCount(), 3);
}

TEST(Cache_Tests, from_back_to_through) {
        dvec data  = FormData();

        ASSERT_EQ(data.size(), 6);

        MainMemoryEmulator::Set(data);
        RandomLineNumberGenerator::Initialise(associativity);

        CacheEmulator::Init(
                associativity * line_size,
                associativity,
                line_size,
                CacheReplacementPolicy::kLeastRecentlyUsed,
                CacheWritePolicy::kWriteBack);

        std::vector<uint8_t> bytes(12);
        std::iota(bytes.begin(), bytes.end(), 1);

        CacheEmulator::WriteBytes(bytes, 0);

        dvec not_updated_data = {
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0}
        };

        ASSERT_EQ(not_updated_data, MainMemoryEmulator::Get());

        dvec updated_data = {
                {1, 2, 3, 4, 5, 6, 7, 8},
                {9, 10, 11, 12, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0}
        };

        CacheEmulator::SetPolicy(kWriteThrough);

        ASSERT_EQ(updated_data, MainMemoryEmulator::Get());

        ASSERT_EQ(CacheEmulator::GetHitsCount(), 0);
        ASSERT_EQ(CacheEmulator::GetMissesCount(), 2);
}

TEST(Cache_Tests, SimpleCacheHits) {
    dvec data  = FormData();

    ASSERT_EQ(data.size(), 6);

    MainMemoryEmulator::Set(data);
    RandomLineNumberGenerator::Initialise(associativity);

    CacheEmulator::Init(
            associativity * line_size,
            associativity,
            line_size,
            CacheReplacementPolicy::kLeastRecentlyUsed,
            CacheWritePolicy::kWriteBack);

    std::vector<uint8_t> bytes(48);
    std::iota(bytes.begin(), bytes.end(), 1);

    CacheEmulator::WriteBytes(bytes, 0);

    ASSERT_EQ(CacheEmulator::GetMissesCount(), 6);

    bytes.resize(8);

    std::iota(bytes.begin(), bytes.end(), 41);
    ASSERT_EQ(bytes, CacheEmulator::ReadBytes(40, 8));

    std::iota(bytes.begin(), bytes.end(), 25);
    ASSERT_EQ(bytes, CacheEmulator::ReadBytes(24, 8));

    std::iota(bytes.begin(), bytes.end(), 17);
    ASSERT_EQ(bytes, CacheEmulator::ReadBytes(16, 8));

    ASSERT_EQ(CacheEmulator::GetHitsCount(), 3);

    CacheEmulator::WriteBytes(bytes, 40);
    ASSERT_EQ(CacheEmulator::GetHitsCount(), 4);

    dvec res_data = {{1, 2, 3, 4, 5, 6, 7, 8},
                     {9, 10, 11, 12, 13, 14, 15, 16},
                     {17, 18, 19, 20, 21, 22, 23, 24},
                     {25, 26, 27, 28, 29, 30, 31, 32},
                     {33, 34, 35, 36, 37, 38, 39, 40},
                     {41, 42, 43, 44, 45, 46, 47, 48}};

    ASSERT_EQ(MainMemoryEmulator::Get(), res_data);

    CacheEmulator::SetPolicy(kWriteThrough);

    res_data = {{1, 2, 3, 4, 5, 6, 7, 8},
                {9, 10, 11, 12, 13, 14, 15, 16},
                {17, 18, 19, 20, 21, 22, 23, 24},
                {25, 26, 27, 28, 29, 30, 31, 32},
                {33, 34, 35, 36, 37, 38, 39, 40},
                {17, 18, 19, 20, 21, 22, 23, 24}};

    ASSERT_EQ(res_data, MainMemoryEmulator::Get());
}

TEST(Cache_Tests, WriteBack1) {
    dvec data  = FormData();

    ASSERT_EQ(data.size(), 6);

    MainMemoryEmulator::Set(data);
    RandomLineNumberGenerator::Initialise(associativity);

    CacheEmulator::Init(
            associativity * line_size,
            associativity,
            line_size,
            CacheReplacementPolicy::kLeastRecentlyUsed,
            CacheWritePolicy::kWriteBack);

    std::vector<uint8_t> bytes(8);
    std::iota(bytes.begin(), bytes.end(), 1);
    CacheEmulator::WriteBytes(bytes, 0);


    dvec not_updated_data = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0}
    };

    ASSERT_EQ(MainMemoryEmulator::Get(), not_updated_data);

    std::iota(bytes.begin(), bytes.end(), 9);
    CacheEmulator::WriteBytes(bytes, 8);
    ASSERT_EQ(MainMemoryEmulator::Get(), not_updated_data);

    std::iota(bytes.begin(), bytes.end(), 17);
    CacheEmulator::WriteBytes(bytes, 16);
    ASSERT_EQ(MainMemoryEmulator::Get(), not_updated_data);

    std::iota(bytes.begin(), bytes.end(), 25);
    CacheEmulator::WriteBytes(bytes, 24);
    ASSERT_EQ(MainMemoryEmulator::Get(), not_updated_data);

    dvec updated_data = {
            {1, 2, 3, 4, 5, 6, 7, 8},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0}
    };

    std::iota(bytes.begin(), bytes.end(), 33);
    CacheEmulator::WriteBytes(bytes, 32);
    ASSERT_EQ(MainMemoryEmulator::Get(), updated_data);

    updated_data[1] = {9, 10, 11, 12, 13, 14, 15, 16};

    std::iota(bytes.begin(), bytes.end(), 41);
    CacheEmulator::WriteBytes(bytes, 40);
    ASSERT_EQ(MainMemoryEmulator::Get(), updated_data);
}

TEST(Cache_Tests, Object) {
        dvec data  = FormData();

        ASSERT_EQ(data.size(), 6);

        MainMemoryEmulator::Set(data);
        RandomLineNumberGenerator::Initialise(associativity);

        CacheEmulator::Init(
                associativity * line_size,
                associativity,
                line_size,
                CacheReplacementPolicy::kLeastRecentlyUsed,
                CacheWritePolicy::kWriteThrough);

        uint8_t m[4] = {1, 2, 3, 4};
        ASSERT_EQ(sizeof(m), 4);

        dvec new_data = {
                {1, 2, 3, 4, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0}
        };

        CacheEmulator::WriteObject(m, 0);
        ASSERT_EQ(MainMemoryEmulator::Get(), new_data);

        struct NewObj {
            char a = 1;
            char b = 2;
            char c = 3;
            char d = 4;
        } obj;

        ASSERT_EQ(obj.a, 1);
        ASSERT_EQ(obj.b, 2);
        ASSERT_EQ(obj.c, 3);
        ASSERT_EQ(obj.d, 4);

        new_data = {
                {1, 2, 3, 4, 1, 2, 3, 4},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0}
        };

        CacheEmulator::WriteObject(obj, 4);

        ASSERT_EQ(MainMemoryEmulator::Get(), new_data);

        const NewObj new_obj = obj;
        new_data[1] = {1, 2, 3, 4, 0, 0, 0, 0};
        CacheEmulator::WriteObject(new_obj, 8);

        ASSERT_EQ(MainMemoryEmulator::Get(), new_data);

        NewObj read_obj = CacheEmulator::ReadObject<NewObj>(2);
        ASSERT_EQ(read_obj.a, 3);
        ASSERT_EQ(read_obj.b, 4);
        ASSERT_EQ(read_obj.c, 1);
        ASSERT_EQ(read_obj.d, 2);
}

}  // namespace cache
