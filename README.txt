Student Results Register - C++ Data Structures Coursework
Student Registration Number: VU-BCS-2507-2174-DAY

Files Included:
- DynamicArray.h : Template class implementing Dynamic Array with capacity doubling, Insertion Sort, Merge Sort, and Iterative Binary Search.
- main.cpp       : Main driver program handling File I/O, testing, and A4 timing experiments.
- results.txt    : Sample dataset containing 10 student records.
- README.txt     : Compilation and execution instructions.

How to Compile:
Open a terminal in the project directory and run:
g++ -std=c++17 -O2 main.cpp -o results_app

To compile with AddressSanitizer (for memory leak checking):
g++ -std=c++17 -fsanitize=address -g main.cpp -o results_app

How to Run:
./results_app