#ifndef BLOCK_DECODER_HPP
#define BLOCK_DECODER_HPP

#include "matrix.hpp"

/**
 * @brief Block decoder class.
 */
class BlockDecoder {
   public:
    /**
     * @brief Constructor.
     * @param generatorMatrix Generator matrix.
     * @param k Number of information bits.
     */
    BlockDecoder(const Matrix<int>& generatorMatrix, int k);

    /**
     * @brief Encode data into a codeword.
     * @param data Integer representing information bits.
     * @return Codeword as a matrix column of boolean values.
     */
    Matrix<bool> encode(int data) const;

    /**
     * @brief Decode received LLR-like values.
     * @param c_tilde Array of LLR-like values.
     * @return Decoded integer representing information bits.
     */
    int decode(const float* c_tilde) const;

   private:
    /**
     * @brief Precompute codewords for all possible information bits.
     */
    void precomputeTable();

    Matrix<int> A;  // Generator matrix (n x k)
    int k;          // Number of information bits
    int n;  // Length of the codeword (number of rows in the generator matrix)
    Matrix<bool> precomputedTable;  // Precomputed table of codewords (2^k x n)
};

#endif  // BLOCK_DECODER_HPP