#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

struct StudentResult {
    int studentID;
    std::string name;
    std::string course;
    double score;
};

template <typename T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int count;

    void resize() {
        int newCapacity = (capacity == 0) ? 2 : capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    template <typename Comparator>
    void merge(int left, int mid, int right, Comparator comp) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        T* L = new T[n1];
        T* R = new T[n2];

        for (int i = 0; i < n1; ++i) L[i] = data[left + i];
        for (int j = 0; j < n2; ++j) R[j] = data[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (!comp(R[j], L[i])) {
                data[k++] = L[i++];
            } else {
                data[k++] = R[j++];
            }
        }
        while (i < n1) data[k++] = L[i++];
        while (j < n2) data[k++] = R[j++];

        delete[] L;
        delete[] R;
    }

    template <typename Comparator>
    void mergeSortHelper(int left, int right, Comparator comp) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortHelper(left, mid, comp);
            mergeSortHelper(mid + 1, right, comp);
            merge(left, mid, right, comp);
        }
    }

public:
    DynamicArray(int initialCap = 4) {
        if (initialCap <= 0) initialCap = 4;
        capacity = initialCap;
        count = 0;
        data = new T[capacity];
    }

    DynamicArray(const DynamicArray& other) {
        capacity = other.capacity;
        count = other.count;
        data = new T[capacity];
        for (int i = 0; i < count; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            count = other.count;
            data = new T[capacity];
            for (int i = 0; i < count; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }

    void addRecord(const T& item) {
        if (count == capacity) {
            resize();
        }
        data[count++] = item;
    }

    T& get(int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Error: Index out of bounds!");
        }
        return data[index];
    }

    const T& get(int index) const {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Error: Index out of bounds!");
        }
        return data[index];
    }

    bool removeAt(int index) {
        if (index < 0 || index >= count) {
            std::cerr << "Warning: Cannot remove invalid index (" << index << ").\n";
            return false;
        }
        for (int i = index; i < count - 1; ++i) {
            data[i] = data[i + 1];
        }
        count--;
        return true;
    }

    bool updateRecord(int index, const T& newItem) {
        if (index < 0 || index >= count) {
            std::cerr << "Warning: Cannot update invalid index.\n";
            return false;
        }
        data[index] = newItem;
        return true;
    }

    template <typename Comparator>
    void insertionSort(Comparator comp) {
        for (int i = 1; i < count; ++i) {
            T key = data[i];
            int j = i - 1;
            while (j >= 0 && comp(key, data[j])) {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }

    template <typename Comparator>
    void mergeSort(Comparator comp) {
        if (count > 1) {
            mergeSortHelper(0, count - 1, comp);
        }
    }

    template <typename KeyType, typename KeyExtractor>
    int binarySearch(const KeyType& targetKey, KeyExtractor getKey) const {
        int low = 0;
        int high = count - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            KeyType midKey = getKey(data[mid]);
            if (midKey == targetKey) {
                return mid;
            } else if (midKey < targetKey) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return -1;
    }
};

#endif