#include "my_hardware_interface.h"
#include "random"

namespace hardware_interface {

int RandomLineNumberGenerator::associativity_ = 0;
uint64_t RandomLineNumberGenerator::random_number_ = 0;

std::vector<std::vector<uint8_t>> MainMemoryEmulator::data_;

void RandomLineNumberGenerator::Initialise(int associativity) {
  associativity_ = associativity;
}

uint64_t RandomLineNumberGenerator::Generate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, INTMAX_MAX);
    random_number_ = dis(gen);
    return random_number_ % associativity_;
}

void MainMemoryEmulator::Set(std::vector<std::vector<uint8_t>> data) {
  data_ = data;
}

const std::vector<std::vector<uint8_t>>& MainMemoryEmulator::Get() {
  return data_;
}

const std::vector<uint8_t>& MainMemoryEmulator::ReadLine(uint64_t address) {
  return data_[address];
}

void MainMemoryEmulator::WriteLine(uint64_t address,
                                   const std::vector<uint8_t>& value) {
  data_[address] = value;
}

}  // namespace hardware_interface
