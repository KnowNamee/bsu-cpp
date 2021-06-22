#include <iostream>
#include <fstream>
#include <cassert>
#include "binary_files.h"

int main() {
    int32_t buffer[3] = {5, 6, 7};
    std::ofstream fout("input.bin", std::ios::binary);
    fout.write(reinterpret_cast<char*>(buffer), sizeof(buffer));
    fout.close();

    int32_t back[3];
    std::ifstream fin("input.bin", std::ios::binary);
    fin.read(reinterpret_cast<char*>(&back[0]), sizeof(int32_t));
    fin.read(reinterpret_cast<char*>(&back[1]), sizeof(int32_t));
    fin.read(reinterpret_cast<char*>(&back[2]), sizeof(int32_t));
    fin.close();
    assert(back[0] == 5);
    assert(back[1] == 6);
    assert(back[2] == 7);

    binary_files::Add("input.bin", "output.bin");
    int32_t result;
    fin.open("output.bin", std::ios::binary);
    fin.read(reinterpret_cast<char*>(&result), sizeof(result));
    fin.close();

    assert(result == 11);
}
