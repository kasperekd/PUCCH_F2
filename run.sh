#!/bin/bash

if [ "$#" -ne 4 ]; then
    echo "Usage: $0 <start_sigma> <end_sigma> <step_sigma> <num_simulations>"
    exit 1
fi

if [ ! -f "build/main" ]; then
    echo "Executable not found. Please compile the project first using compile.sh"
    exit 1
fi

if ! command -v python3 &> /dev/null
then
    echo "Python3 could not be found."
    exit 1
fi

if ! python3 -c "import pandas; import matplotlib" &> /dev/null
then
    echo "Pandas and Matplotlib are required. Please install them using 'pip install pandas matplotlib'"
    exit 1
fi

echo "Running simulations..."
./build/main "$1" "$2" "$3" "$4"

if [ $? -ne 0 ]; then
    echo "Simulation failed."
    exit 1
fi

echo "Visualizing results..."
python3 visualization/visual.py simulation_results

if [ $? -ne 0 ]; then
    echo "Visualization failed."
    exit 1
fi

echo "All operations completed"