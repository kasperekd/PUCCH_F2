#include "block_decoder.hpp"

#include <iostream>
#include <limits>
#include <random>

BlockDecoder::BlockDecoder(const Matrix<int>& generatorMatrix, int k)
    : A(generatorMatrix),
      k(k),
      n(static_cast<int>(generatorMatrix.rows())),
      precomputedTable(1 << k, generatorMatrix.rows()) {
    precomputeTable();
}

Matrix<bool> BlockDecoder::encode(int data) const {
    Matrix<bool> codeword(n, 1);
    for (int i = 0; i < n; ++i) {
        int sum = 0;
        for (int j = 0; j < k; ++j) {
            int bit = (data >> (k - 1 - j)) & 1;
            sum += A(i, j) * bit;
        }
        codeword(i, 0) = (sum % 2) != 0;
    }
    return codeword;
}

int BlockDecoder::decode(const float* c_tilde) const {
    int numCodewords = 1 << k;
    float max_correlation = -std::numeric_limits<float>::max();
    int best_candidate = 0;
    for (int i = 0; i < numCodewords; ++i) {
        float correlation = 0.0f;
        for (int j = 0; j < n; ++j) {
            if (precomputedTable(i, j)) {
                correlation += c_tilde[j];
            }
        }
        if (correlation > max_correlation) {
            max_correlation = correlation;
            best_candidate = i;
        }
    }
    return best_candidate;
}

void BlockDecoder::precomputeTable() {
    int numCodewords = 1 << k;
    for (int i = 0; i < numCodewords; ++i) {
        for (int row = 0; row < n; ++row) {
            int sum = 0;
            for (int col = 0; col < k; ++col) {
                int bit = (i >> (k - 1 - col)) & 1;
                sum += A(row, col) * bit;
            }
            precomputedTable(i, row) = (sum % 2) != 0;
        }
    }
}