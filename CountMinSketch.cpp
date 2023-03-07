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


public:
    // Constructor for initializing our counter matrix, as well as initializing our prime value and our random values of a and b for every hash function
    CountMinSketch(int _t, int _k) : t(_t), k(_k) {
        // Ensuring that we get a random start seed to pick our a and b values
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        // Generating different distributions
        default_random_engine generator(seed);

        // Initialize prime to 700001 as the prime only needs to be bigger than the integers we have of different values our data can take,
        // this is to ensure that it will give us a proper value when we do "mod k" in our hash function
        uniform_int_distribution<uint64_t> distribution(0,700001);


        counters = new int*[t];
        for (int i=0; i < t; i++) {
            counters[i] = new int[k];
            memset(counters[i], 0, sizeof(int) * k);
            // Selecting random a and b, TODO: Make sure that this actually generates random numbers per iteration, based on the seed
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
        // TODO: Unsure how to clean up hashvector memory, as it is of type vector
    }

    uint64_t customHashFunction(uint64_t a, uint64_t b, uint64_t elem) {
        // Prime has to be bigger than the biggest number in our possible elem interval, so here 700001 fits the job
        return ((a * elem + b) % 700001) % k;
    }

    // As per the paper on CountSketch, elems will come in the form of (elem, c) where elem is elem to be counted, and c being by how much, given our processed data,
    // we will start the implementation by simply always passing 1 as the number for c
    void updateCounters(uint64_t elem, int c) {
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

    int findMinElem(uint64_t elem) {
        // Initialize temp value to biggest possible value for the given data type. TODO: Change this to uint64_t datatype to be consistent, find different function for min or use if statement
        int tempMin = numeric_limits<uint64_t>::max();
        // Once again, could this be vectorized instead of for loop? TODO: Check for vectorization of this query
        for (int i=0; i < t; i++) {
            tuple<uint64_t, uint64_t> a_b_tuple = a_b_hashvector[i];
            uint64_t hash = customHashFunction(get<0>(a_b_tuple), get<1>(a_b_tuple), elem);
            tempMin = min(tempMin, counters[i][hash]);
        }
        return tempMin;
    }
};