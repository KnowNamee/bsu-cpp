#ifndef MY_HARDWARE_INTERFACE_H_
#define MY_HARDWARE_INTERFACE_H_

#include <vector>
#include <cstdint>

// Implementation of these methods can be different in author's tests
namespace hardware_interface {

class RandomLineNumberGenerator {
 public:
  RandomLineNumberGenerator() = delete;

  // This method should only be called in tests to initialise the generator;
  // ATTENTION: Using it in CacheEmulator is FORBIDDEN and may cause CE!
  static void Initialise(int associativity);

  // Generates a number in range [0; associativity).
  static uint64_t Generate();

 private:
  static int associativity_;
  static uint64_t random_number_;
};

class MainMemoryEmulator {
 public:
  MainMemoryEmulator() = delete;

  // These methods should only be called in tests!
  // ATTENTION: Using them in CacheEmulator is FORBIDDEN and may cause CE!
  static void Set(std::vector<std::vector<uint8_t>> data);
  static const std::vector<std::vector<uint8_t>>& Get();

  // Read/write a line with address 'address' from/to memory
  static const std::vector<uint8_t>& ReadLine(uint64_t address);
  static void WriteLine(uint64_t address, const std::vector<uint8_t>& value);

 private:
  static std::vector<std::vector<uint8_t>> data_;
};

}  // namespace hardware_interface

#endif  // MY_HARDWARE_INTERFACE_H_
