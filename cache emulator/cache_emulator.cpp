//
// Created by ZenBook on 25.11.2019.
//

#include "cache_emulator.h"

#ifdef YANDEX_CONTEST_MODE
#include "hardware_interface.h"
#else
#include "my_hardware_interface.h"
#endif  // YANDEX_CONTEST_MODE

#include <cassert>

namespace cache {

using hardware_interface::MainMemoryEmulator;
using hardware_interface::RandomLineNumberGenerator;

uint64_t CacheEmulator::cache_size_ = 0;
uint64_t CacheEmulator::cache_hit_;
uint64_t CacheEmulator::cache_miss_;
uint64_t CacheEmulator::line_size_;
uint64_t CacheEmulator::bank_count_;
uint64_t CacheEmulator::total_age_;
uint64_t CacheEmulator::associativity_;

CacheWritePolicy CacheEmulator::write_policy_;
CacheReplacementPolicy CacheEmulator::replacement_policy_;

std::vector<CacheEmulator::Bank> CacheEmulator::banks_;

uint64_t CacheEmulator::GetHitsCount() {
    return cache_hit_;
}

uint64_t CacheEmulator::GetMissesCount() {
    return cache_miss_;
}

void CacheEmulator::ResetStatistics() {
    cache_hit_ = cache_miss_ = 0;
}

    void CacheEmulator::SetPolicy(CacheReplacementPolicy policy) {
        replacement_policy_ = policy;
    }

void CacheEmulator::Init(int cache_size, int associativity, int line_size,
        CacheReplacementPolicy replacement_policy,
        CacheWritePolicy write_policy) {
    line_size_ = line_size;
    cache_size_ = cache_size;
    write_policy_ = write_policy;
    associativity_ = associativity;
    replacement_policy_ = replacement_policy;
    bank_count_ = cache_size_ / (line_size * associativity);
    total_age_ = 0;

    banks_.resize(bank_count_);
    for (Bank &bank : banks_) {
        bank.cells.resize(associativity);
    }
}

/*!
 * @param address - start index in main memory to read
 * @param count - number of bytes to read
 * @return vector of read bytes
 */
std::vector<uint8_t> CacheEmulator::ReadBytes(
        uint64_t address,
        uint64_t count) {
    assert(cache_size_);

    std::vector<uint8_t> bytes;
    uint64_t last_address = address + count;

    while (address < last_address) {
        uint64_t line_index = address / line_size_;  // index of line in
                                                     // main memory

        Bank &bank = GetBank(line_index);
        std::vector<uint8_t> line = LoadLineFromCache(line_index, &bank);

        for (uint64_t i = address % line_size_;  i < line_size_; i++) {
            bytes.push_back(line[i]);
        }

        // go to the first address of next line
        address = address / line_size_ * line_size_ + line_size_;
    }

    bytes.resize(count);
    return bytes;
}

/*!
 * @param line_index - index of the line in main memory
 * @param bank - bank that may contain line
 * @return vector of found bytes
 */
std::vector<uint8_t> CacheEmulator::LoadLineFromCache(
        uint64_t line_index,
        Bank* bank) {
    total_age_++;

    for (Cell& cell : bank->cells) {
        if (cell.line_index == line_index) {
            cache_hit_++;
            cell.age = total_age_;
            return cell.line;
        }
    }

    cache_miss_++;
    uint64_t loaded_cell_index = LoadLineInCache(line_index, bank);
    bank->cells[loaded_cell_index].age = total_age_;
    return bank->cells[loaded_cell_index].line;
}

/*!
 * @param line_index - index of line in main memory
 * @param bank - bank that will contain loaded line
 * @return index of loaded line in bank
 */
uint64_t CacheEmulator::LoadLineInCache(uint64_t line_index, Bank *bank) {
    if (replacement_policy_ == kRandom) {
        return LoadRandomLine(line_index, bank);
    } else {
        return LoadLeastRecentlyUsed(line_index, bank);
    }
}

/*!
 * @brief is used with ReplacementPolicy = kRandom
 * @param line_index - index of line in main memory
 * @param bank - bank that will contain loaded line
 * @return index of loaded RANDOM line in bank
 */
uint64_t CacheEmulator::LoadRandomLine(
        uint64_t line_index,
        Bank *bank) {
    uint64_t cell_index = RandomLineNumberGenerator::Generate();
    LoadLineInCache(line_index, cell_index, bank);
    return cell_index;
}

/*!
 * @brief is used with ReplacementPolicy = kLeastRecentlyUsed
 * @param line_index - index of line in main memory
 * @param bank - bank that will contain loaded line
 * @return index of loaded LeastRecentlyUsed line in bank
 */
uint64_t CacheEmulator::LoadLeastRecentlyUsed(
        uint64_t line_index,
        Bank *bank) {
    uint64_t cell_index = FindRecentlyUsedCellIndex(bank);
    LoadLineInCache(line_index, cell_index, bank);
    return cell_index;
}

/*!
 * @param line_index - index of line in main memory
 * @param cell_index - index of line that will be replaced
 * @param bank - bank that will contain loaded line
 */
void CacheEmulator::LoadLineInCache(
        uint64_t line_index,
        uint64_t cell_index,
        Bank* bank
        ) {
    Cell &cell = bank->cells[cell_index];

    if (write_policy_ == kWriteBack && cell.is_dirty) {
        MainMemoryEmulator::WriteLine(cell.line_index, cell.line);
    }

    cell.is_dirty = false;
    cell.age = total_age_;
    cell.line_index = line_index;
    cell.line = MainMemoryEmulator::ReadLine(line_index);
}

/*!
 * @param bank - where we will search
 * @return index of line in bank with min 'age' param
 */
uint64_t CacheEmulator::FindRecentlyUsedCellIndex(Bank *bank) {
    assert(!bank->cells.empty());

    uint64_t min_age_index = 0;
    uint64_t min_age = bank->cells[0].age;

    uint64_t i = 0;
    for (Cell &cell : bank->cells) {
        if (cell.age < min_age) {
            min_age = cell.age;
            min_age_index = i;
        }
        i++;
    }

    return min_age_index;
}

void CacheEmulator::SetPolicy(CacheWritePolicy policy) {
    if (write_policy_ == kWriteBack && policy == kWriteThrough) {
        for (Bank &bank : banks_) {
            for (Cell &cell : bank.cells) {
                if (cell.is_dirty) {
                    cell.is_dirty = false;
                    MainMemoryEmulator::WriteLine(
                            cell.line_index,
                            cell.line);
                }
            }
        }
    }
    write_policy_ = policy;
}

/*!
 * @param data - bytes to write
 * @param address - address of start byte in main memory
 */
void CacheEmulator::WriteBytes(
        const std::vector<uint8_t> &data,
        uint64_t address) {
    assert(cache_size_);
    total_age_++;

    uint64_t last_address = address + data.size();
    uint64_t data_index = 0;

    while (address < last_address) {
        uint64_t line_index = address / line_size_;
        bool hit = false;

        for (Cell &cell : GetBank(line_index).cells) {
            if (cell.line_index == line_index) {
                hit = true;
                cache_hit_++;
                cell.age = total_age_;
                WriteLine(&cell, data, &data_index, address);
                break;
            }
        }

        if (!hit) {
            cache_miss_++;
            Bank &bank = GetBank(line_index);
            Cell &cell = bank.cells[LoadLineInCache(line_index, &bank)];
            cell.age = total_age_;
            WriteLine(&cell, data, &data_index, address);
        }
        address = address / line_size_ * line_size_ + line_size_;
    }
}

/*!
 * @param cell - cell we're going to replace
 * @param data - new data that contains needed line
 * @param data_index - the start index of line in data
 * @param address - address of first byte in main memory
 */
void CacheEmulator::WriteLine(
        Cell* cell,
        const std::vector<uint8_t> &data,
        uint64_t *data_index,
        uint64_t address) {
    uint64_t line_index = address / line_size_;

    for (uint64_t i = address % line_size_; i < line_size_; i++) {
        if (*data_index == data.size()) {
            break;
        }
        cell->line[i] = data[(*data_index)++];
    }

    if (write_policy_ == kWriteThrough) {
        MainMemoryEmulator::WriteLine(line_index, cell->line);
    } else {
        cell->is_dirty = true;
    }
}

CacheEmulator::Bank &CacheEmulator::GetBank(uint64_t line_index) {
    return banks_[line_index % bank_count_];
}

}  // namespace cache
