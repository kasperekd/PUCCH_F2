#ifndef ORCHESTRATOR_HPP
#define ORCHESTRATOR_HPP

#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>

#include "block_decoder.hpp"

/**
 * @brief Simulation orchestrator class.
 */
class Orchestrator {
   public:
    /**
     * @brief Constructor.
     * @param k Number of information bits.
     * @param numSimulations Number of simulations.
     * @param generatorMatrix Generator matrix.
     */
    Orchestrator(int k, int numSimulations, const Matrix<int>& generatorMatrix);

    /**
     * @brief Run simulations for different sigma values.
     * @param folderName Folder name for saving results.
     * @param startSigma Starting value of sigma.
     * @param endSigma Ending value of sigma.
     * @param stepSigma Step size for sigma.
     */
    void runSimulations(const char* folderName, float startSigma,
                        float endSigma, float stepSigma);

   private:
    /**
     * @brief Simulate a single transmission cycle.
     * @param sigma Standard deviation of the noise.
     * @return True if an error occurred during decoding, false otherwise.
     */
    bool simulateSingle(float sigma);

    BlockDecoder decoder;  // Decoder instance (also encoder)
    int k;                 // Number of information bits
    int n;                 // Length of the codeword
    int numSimulations;    // Number of simulations for each sigma
    std::mt19937 rng;      // Random number generator
};

#endif  // ORCHESTRATOR_HPP