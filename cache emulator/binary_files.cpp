//
// Created by ZenBook on 03.12.2019.
//

#include "binary_files.h"

#include <fstream>
#include <cstdint>
#include <iostream>
#include <cassert>

namespace binary_files {

struct BlockRealSize {
    uint32_t szi = 0;
    uint32_t szj = 0;
};

enum BlockProcessMode {
    kRead,
    kWrite,
};

const uint16_t kMod = 256;
const uint16_t kBlockSize = 128;

uint8_t a_block[kBlockSize*kBlockSize];
uint8_t b_block[kBlockSize*kBlockSize];
uint8_t c_block[kBlockSize*kBlockSize];
uint32_t aszi;
uint32_t aszj;
uint32_t bszi;
uint32_t bszj;
uint32_t cszi;
uint32_t cszj;
uint64_t asz;

void MakeBlockNull(uint8_t* block) {
    uint64_t sz = kBlockSize * kBlockSize;
    for (int i = 0; i < sz; i++) {
        *(block + i) = 0;
    }
}

void ProcessBlock(BlockProcessMode mode, std::fstream *stream, uint8_t *block,
                  uint32_t block_i, uint32_t block_j, uint32_t szi,
                  uint32_t szj, uint64_t offset,
                  BlockRealSize* r_block_size = nullptr) {
    uint64_t bi = block_i * kBlockSize;
    uint64_t bj = block_j * kBlockSize;
    uint32_t lines = (szi - bi > kBlockSize ? kBlockSize : szi - bi);
    uint32_t bytes = (szj - bj > kBlockSize ? kBlockSize : szj - bj);

    for (int line = 0; line < lines; line++) {
        if (mode == kRead) {
            stream->seekg(offset + (line + bi) * szj + bj);
            stream->read(reinterpret_cast<char*>(block + line * kBlockSize),
                         bytes);
        } else {
            stream->seekp(offset + (line + bi) * szj + bj);
            stream->write(reinterpret_cast<char*>(block + line * kBlockSize),
                          bytes);
        }
    }

    if (r_block_size != nullptr) {
        *r_block_size = {lines, bytes};
    }
}

/*!
 * @brief Multiplies A * B and writes answer to C
 * @param a - real size of block(matrix) A
 * @param b - real size of block(matrix) B
 */
void Multiply(const BlockRealSize& a, const BlockRealSize& b) {
    for (int i = 0; i < a.szi; i++) {
        for (int k = 0; k < a.szj; k++) {
            int aa = *(a_block + i*kBlockSize + k);
            for (int j = 0; j < b.szj; j++) {
                int bb = *(b_block + k*kBlockSize + j);
                int cc = *(c_block + i*kBlockSize + j);
                cc = (cc + aa*bb) % kMod;
                *(c_block + i*kBlockSize + j) = cc;
            }
        }
    }
}

/*!
 * @brief Multiplies A * B from input binary file and writes answer to
 * output binary file in format (szi szj matrix)
 */
void Multiply(const std::string& input_filename,
              const std::string& output_filename) {
    std::fstream fin(input_filename, std::ios::binary   | std::ios::in);
    std::fstream fout(output_filename, std::ios::binary | std::ios::out);

    fin.read(reinterpret_cast<char*>(&aszi), 4);
    fin.read(reinterpret_cast<char*>(&aszj), 4);
    fin.seekg(4+4+aszi*aszj);
    fin.read(reinterpret_cast<char*>(&bszi), 4);
    fin.read(reinterpret_cast<char*>(&bszj), 4);

    asz = aszi * aszj;
    cszi = aszi;
    cszj = bszj;
    assert(aszj == bszi);

    uint32_t a_block_i_count = (aszi + kBlockSize - 1) / kBlockSize;
    uint32_t b_block_j_count = (bszj + kBlockSize - 1) / kBlockSize;
    uint32_t k_count = (aszj + kBlockSize - 1) / kBlockSize;

    fout.write(reinterpret_cast<char*>(&cszi), 4);
    fout.write(reinterpret_cast<char*>(&cszj), 4);
    for (int i = 0; i < a_block_i_count; i++) {
        for (int j = 0; j < b_block_j_count; j++) {
            MakeBlockNull(c_block);
            for (int k = 0; k < k_count; k++) {
                BlockRealSize a_block_real_size;
                BlockRealSize b_block_real_size;
                ProcessBlock(kRead, &fin, a_block, i, k, aszi, aszj,
                        8, &a_block_real_size);
                ProcessBlock(kRead, &fin, b_block, k, j, bszi, bszj,
                        16 + asz, &b_block_real_size);
                Multiply(a_block_real_size, b_block_real_size);
            }
            ProcessBlock(kWrite, &fout, c_block, i, j, cszi, cszj, 8);
        }
    }

    fin.close();
    fout.close();
}

void Add(const std::string& input_filename,
         const std::string& output_filename) {
    std::ifstream fin(input_filename, std::ios::binary | std::ios::in);
    std::ofstream fout(output_filename, std::ios::binary | std::ios::out);

    int32_t buffer[2];
    fin.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
    int32_t result = buffer[1] + buffer[0];
    fout.write(reinterpret_cast<char*>(&result), sizeof(result));

    fin.close();
    fout.close();
}

}  // namespace binary_files

