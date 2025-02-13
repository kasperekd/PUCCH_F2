#include "orchestrator.hpp"

#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>

Orchestrator::Orchestrator(int k, int numSimulations,
                           const Matrix<int>& generatorMatrix)
    : decoder(generatorMatrix, k),
      k(k),
      n(static_cast<int>(generatorMatrix.rows())),
      numSimulations(numSimulations) {
    rng.seed(std::random_device{}());
}

void Orchestrator::runSimulations(const char* folderName, float startSigma,
                                  float endSigma, float stepSigma) {
    std::string filename = std::string(folderName) + "/results" +
                           std::to_string(n) + "x" + std::to_string(k) + ".csv";
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }
    out << "snr_db,sigma,error_rate,time\n";
    int totalSteps = static_cast<int>((endSigma - startSigma) / stepSigma) + 1;
    for (float sigma = startSigma; sigma <= endSigma; sigma += stepSigma) {
        int errors = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (int sim = 0; sim < numSimulations; ++sim) {
            if (simulateSingle(sigma)) {
                ++errors;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = (end - start) / numSimulations;
        double errorRate = static_cast<double>(errors) / numSimulations;

        double snrDb = 10.0 * log10(1.0 / (sigma * sigma));

        out << snrDb << "," << sigma << "," << errorRate << ","
            << elapsed.count() << "\n";
    }
}

bool Orchestrator::simulateSingle(float sigma) {
    std::uniform_int_distribution<int> dist(0, (1 << k) - 1);
    int data = dist(rng);
    Matrix<bool> codeword = decoder.encode(data);
    std::unique_ptr<float[]> transmitted(new float[n]);
    for (int i = 0; i < n; ++i) {
        transmitted[i] = codeword(i, 0) ? 1.0f : -1.0f;
    }
    std::unique_ptr<float[]> noisy(new float[n]);
    std::normal_distribution<float> normalDist(0.0f, 1.0f);
    for (int i = 0; i < n; ++i) {
        noisy[i] = transmitted[i] + sigma * normalDist(rng);
    }
    int decodedData = decoder.decode(noisy.get());
    return (decodedData != data);
}