//
// Created by ZenBook on 25.11.2019.
//

#ifndef CACHE_EMULATOR_H_
#define CACHE_EMULATOR_H_

#include <cstdint>
#include <vector>
#include <memory>

namespace cache {

enum CacheReplacementPolicy {
    kLeastRecentlyUsed,
    kRandom,
};

enum CacheWritePolicy {
    kWriteThrough,
    kWriteBack,
};

class CacheEmulator {
 public:
    CacheEmulator() = delete;

    static void Init(int cache_size, int associativity, int line_size,
            CacheReplacementPolicy replacement_policy,
            CacheWritePolicy write_policy);

    static std::vector<uint8_t> ReadBytes(
            uint64_t address,
            uint64_t count);

    static void WriteBytes(
            const std::vector<uint8_t>& data,
            uint64_t address);

    static uint64_t GetHitsCount();
    static uint64_t GetMissesCount();
    static void ResetStatistics();

    static void SetPolicy(CacheReplacementPolicy);
    static void SetPolicy(CacheWritePolicy);

    template<typename ObjectType>
    static ObjectType ReadObject(uint64_t address) {
        std::vector<uint8_t> obj_bytes =
                ReadBytes(address, sizeof(ObjectType));
        uint8_t *ptr_obj = &obj_bytes[0];
        ObjectType *obj = reinterpret_cast<ObjectType*>(ptr_obj);
        return *obj;
    }

    template<typename ObjectType>
    static void WriteObject(const ObjectType& value, uint64_t address) {
        std::vector<uint8_t> bytes(sizeof(value));
        const ObjectType* obj = reinterpret_cast<const ObjectType*>(&value);
        const uint8_t* byte = reinterpret_cast<const uint8_t*>(obj);
        for (int i = 0; i < sizeof(ObjectType); i++) {
            bytes[i] = *byte++;
        }
        WriteBytes(bytes, address);
    }

 private:
    struct Cell {
        Cell() = default;

        bool is_dirty = false;
        uint64_t age = 0;
        uint64_t line_index = -1;
        std::vector<uint8_t> line;
    };

    struct Bank {
        std::vector<Cell> cells;
    };

    static uint64_t cache_hit_;
    static uint64_t cache_miss_;
    static uint64_t cache_size_;
    static uint64_t line_size_;
    static uint64_t bank_count_;
    static uint64_t total_age_;
    static uint64_t associativity_;
    static std::vector<Bank> banks_;
    static CacheWritePolicy write_policy_;
    static CacheReplacementPolicy replacement_policy_;

    static std::vector<uint8_t> LoadLineFromCache(
            uint64_t line_index,
            Bank* bank);

    static uint64_t LoadRandomLine(
            uint64_t line_index,
            Bank *bank);

    static uint64_t LoadLeastRecentlyUsed(
            uint64_t line_index,
            Bank *bank);

    static uint64_t LoadLineInCache(
            uint64_t line_index,
            Bank *bank);

    static void LoadLineInCache(
            uint64_t line_index,
            uint64_t cell_index,
            Bank *bank);

    static uint64_t FindRecentlyUsedCellIndex(Bank *bank);

    static void WriteLine(Cell* cell,
                          const std::vector<uint8_t> &data,
                          uint64_t *data_index,
                          uint64_t address);

    static Bank& GetBank(uint64_t line_index);
};

}  // namespace cache

#endif  // CACHE_EMULATOR_H_


