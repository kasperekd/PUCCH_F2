#include <filesystem>
#include <iostream>
#include <string>

#include "matrix.hpp"
#include "orchestrator.hpp"

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0]
                  << " <start_sigma> <end_sigma> <step_sigma> <num_simulations>"
                  << std::endl;
        return 1;
    }
    try {
        float startSigma = std::stof(argv[1]);
        float endSigma = std::stof(argv[2]);
        float stepSigma = std::stof(argv[3]);
        int numSimulations = std::stoi(argv[4]);
        if (startSigma >= endSigma || stepSigma <= 0) {
            throw std::invalid_argument("Invalid sigma values or step size.");
        }
        Matrix<int> generatorMatrix = {{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
                                       {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
                                       {1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1},
                                       {1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
                                       {1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1},
                                       {1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
                                       {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
                                       {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1},
                                       {1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1},
                                       {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1},
                                       {1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
                                       {1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1},
                                       {1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1},
                                       {1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
                                       {1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1},
                                       {1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1},
                                       {1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1},
                                       {1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1},
                                       {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                                       {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0}};
        std::string folderName = "simulation_results";
        std::filesystem::create_directory(folderName);
        int totalKSteps = 13;  // всего шагов
        int currentKStep = 0;
        for (int k = 1; k <= 13; ++k) {
            Orchestrator orchestrator(k, numSimulations, generatorMatrix);
            orchestrator.runSimulations(folderName.c_str(), startSigma,
                                        endSigma, stepSigma);
            currentKStep++;
            double progress =
                static_cast<double>(currentKStep) / totalKSteps * 100.0;
            std::cout << "Overall progress: " << progress << "%\r";
            std::cout.flush();
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}