#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <iomanip>
#include "DynamicArray.h"

void loadFromFile(DynamicArray<StudentResult>& arr, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        StudentResult s;
        std::string idStr, scoreStr;
        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, s.name, ',') &&
            std::getline(ss, s.course, ',') &&
            std::getline(ss, scoreStr, ',')) {
            s.studentID = std::stoi(idStr);
            s.score = std::stod(scoreStr);
            arr.addRecord(s);
        }
    }
    file.close();
}

void saveToFile(const DynamicArray<StudentResult>& arr, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not save to file " << filename << "\n";
        return;
    }
    for (int i = 0; i < arr.size(); ++i) {
        const StudentResult& s = arr.get(i);
        file << s.studentID << "," << s.name << "," << s.course << "," << s.score << "\n";
    }
    file.close();
}

void printRecords(const DynamicArray<StudentResult>& arr) {
    std::cout << "------------------------------------------------------------\n";
    std::cout << std::left << std::setw(10) << "ID" 
              << std::setw(20) << "Name" 
              << std::setw(15) << "Course" 
              << std::setw(10) << "Score" << "\n";
    std::cout << "------------------------------------------------------------\n";
    for (int i = 0; i < arr.size(); ++i) {
        const auto& s = arr.get(i);
        std::cout << std::left << std::setw(10) << s.studentID
                  << std::setw(20) << s.name
                  << std::setw(15) << s.course
                  << std::setw(10) << s.score << "\n";
    }
    std::cout << "------------------------------------------------------------\n";
}

void runTimingExperiment() {
    std::cout << "\n=== TASK A4: TIMING EXPERIMENT ===\n";
    std::cout << std::left << std::setw(10) << "n"
              << std::setw(20) << "Insertion (ms)"
              << std::setw(15) << "Ins Ratio"
              << std::setw(20) << "Merge (ms)"
              << std::setw(15) << "Merge Ratio" << "\n";

    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000};
    double prevInsTime = 0.0, prevMergeTime = 0.0;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1000, 999999);

    auto byID = [](const StudentResult& a, const StudentResult& b) {
        return a.studentID < b.studentID;
    };

    for (int n : sizes) {
        double totalIns = 0.0, totalMerge = 0.0;

        for (int trial = 0; trial < 3; ++trial) {
            DynamicArray<StudentResult> baseArr(n);
            for (int i = 0; i < n; ++i) {
                baseArr.addRecord({dist(rng), "Student", "BCS", 75.0});
            }

            DynamicArray<StudentResult> arr1 = baseArr;
            auto start1 = std::chrono::high_resolution_clock::now();
            arr1.insertionSort(byID);
            auto end1 = std::chrono::high_resolution_clock::now();
            totalIns += std::chrono::duration<double, std::milli>(end1 - start1).count();

            DynamicArray<StudentResult> arr2 = baseArr;
            auto start2 = std::chrono::high_resolution_clock::now();
            arr2.mergeSort(byID);
            auto end2 = std::chrono::high_resolution_clock::now();
            totalMerge += std::chrono::duration<double, std::milli>(end2 - start2).count();
        }

        double avgIns = totalIns / 3.0;
        double avgMerge = totalMerge / 3.0;
        double insRatio = (prevInsTime > 0) ? (avgIns / prevInsTime) : 0.0;
        double mergeRatio = (prevMergeTime > 0) ? (avgMerge / prevMergeTime) : 0.0;

        std::cout << std::left << std::setw(10) << n
                  << std::setw(20) << std::fixed << std::setprecision(3) << avgIns
                  << std::setw(15) << std::setprecision(2) << insRatio
                  << std::setw(20) << std::setprecision(3) << avgMerge
                  << std::setw(15) << std::setprecision(2) << mergeRatio << "\n";

        prevInsTime = avgIns;
        prevMergeTime = avgMerge;
    }
}

int main() {
    DynamicArray<StudentResult> registerArr;

    loadFromFile(registerArr, "results.txt");
    std::cout << "Loaded records (" << registerArr.size() << " students):\n";
    printRecords(registerArr);

    registerArr.addRecord({1011, "Khadija Omar", "DSA", 88.5});
    registerArr.removeAt(99);

    auto compareByID = [](const StudentResult& a, const StudentResult& b) {
        return a.studentID < b.studentID;
    };
    registerArr.mergeSort(compareByID);
    std::cout << "\nSorted by Student ID (Merge Sort):\n";
    printRecords(registerArr);

    int searchID = 1005;
    int foundIdx = registerArr.binarySearch(searchID, [](const StudentResult& s) { return s.studentID; });
    if (foundIdx != -1) {
        std::cout << "Found ID " << searchID << ": " << registerArr.get(foundIdx).name << "\n";
    } else {
        std::cout << "ID " << searchID << " not found.\n";
    }

    saveToFile(registerArr, "results.txt");

    runTimingExperiment();

    return 0;
}