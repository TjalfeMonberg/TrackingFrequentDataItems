//
// Created by Tjalfe on 07-03-2023.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <random>
#include <cstdlib>
#include <chrono>
#include <stdint.h>

using namespace std;

class CountMinSketch {
private:
    int t, k; // t for amount of arrays, k for amount of counters per array
    int **counters; // Counters for the sketch itself
    vector<tuple<uint64_t, uint64_t>> a_b_hashvector; // Vector of random a and b values to be saved per array in our counters

    [[nodiscard]] uint64_t customHashFunction(uint64_t a, uint64_t b, int elem) const {
        // Prime has to be bigger than the biggest number in our possible elem interval, so here 700001 fits the job
        return ((a * elem + b) % 26000003) % k;
    }


public:
    // Constructor for initializing our counter matrix, as well as initializing our prime value and our random values of a and b for every hash function
    CountMinSketch(int _t, int _k) : t(_t), k(_k) {
        // Ensuring that we get a random start seed to pick our a and b values
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        // Generating different distributions
        default_random_engine generator(seed);

        // Initialize prime to 700001 as the prime only needs to be bigger than the integers we have of different values our data can take,
        // this is to ensure that it will give us a proper value when we do "mod k" in our hash function
        uniform_int_distribution<uint64_t> distribution(0,26000003-1);


        counters = new int*[t];
        for (int i=0; i < t; i++) {
            counters[i] = new int[k];
            memset(counters[i], 0, sizeof(int) * k);
            // Selecting random a and b, that are actually random (tested)
            uint64_t a = distribution(generator);
            uint64_t b = distribution(generator);
            a_b_hashvector.push_back(make_tuple(a,b));
        }

    }

    // Destructor to free memory
    ~CountMinSketch() {
        for (int i=0; i < t; i++) {
            delete[] counters[i];
        }
        delete[] counters;
        // Since c++ is coded in such a way that the vector gets deleted when it's out of scope, We do not need to delete it manually
    }

    // As per the paper on CountSketch, elems will come in the form of (elem, c) where elem is elem to be counted, and c being by how much, given our processed data,
    // we will start the implementation by simply always passing 1 as the number for c
    void updateCounters(uint32_t elem, int c) {
        // Can potentially be made faster to use vectorized notation instead of for loop. TODO: Look into vectorizing this function
        for (int i=0; i < t; i++) {
            // Grab tuple of a and b from vector
            tuple<uint64_t, uint64_t> a_b_tuple = a_b_hashvector[i];
            // Get the hash h_i by getting the randomly assigned a and b values
            uint64_t hash = customHashFunction(get<0>(a_b_tuple), get<1>(a_b_tuple), elem);
            // The hash simply returns what position in the array we want to count, so can use as follows
            counters[i][hash] += c;
        }
    }

    int findMinElem(uint32_t elem) {
        // Initialize temp value to biggest possible value for the given data type.
        int tempMin = numeric_limits<int>::max();
        // Once again, could this be vectorized instead of for loop?
        // TODO: Check for vectorization of this query <- Maybe not possible
        /*
         * In this version, we use the #pragma omp parallel for directive to instruct the compiler to parallelize the loop. The reduction(min:tempMin) clause tells OpenMP to keep track of the minimum value of tempMin across all threads, and combine the results at the end of the loop. This allows us to avoid race conditions and ensure that the final result is correct.
         * By using parallelism, this implementation can take advantage of multiple CPU cores to speed up the computation, potentially achieving significant performance gains compared to the original sequential implementation.
         */
        #pragma omp parallel for reduction(min:tempMin)
        for (int i=0; i < t; i++) {
            tuple<uint64_t, uint64_t> a_b_tuple = a_b_hashvector[i];
            int hash = customHashFunction(get<0>(a_b_tuple), get<1>(a_b_tuple), elem);
            tempMin = min(tempMin, counters[i][hash]);
        }
        return tempMin;
    }
};