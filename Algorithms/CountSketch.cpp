//
// Created by Tjalfe on 14-03-2023.
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

class CountSketch {
private:
    int t, k; // t for amount of arrays, k for amount of counters per array
    int **counters; // Counters for the sketch itself
    vector<tuple<uint64_t, uint64_t>> a_b_hashvector; // Vector of random a and b values to be saved per array in our counters
    vector<tuple<uint64_t, uint64_t>> a_b_hashvector_for_g;

    [[nodiscard]] int customHashFunction(uint64_t a, uint64_t b, uint32_t elem) const {
        // Prime has to be bigger than the biggest number in our possible elem interval, so here 700001 fits the job
        return ((a * elem + b) % 26000003) % k;
    }

    [[nodiscard]] unsigned countBits(uint64_t number) const {
        // log function in base 2
        // take only integer part
        return (int)log2(number)+1;
    }

    [[nodiscard]] int oneOrMinusOneHash(uint64_t a, uint64_t b, uint32_t elem) const {
        uint64_t bitLength = (a*elem+b);
        int some = 2*(bitLength >> (64-1))-1;
        return some;
    }


public:
    // Constructor for initializing our counter matrix, as well as initializing our prime value and our random values of a and b for every hash function
    CountSketch(int _t, int _k) : t(_t), k(_k) {
        // Ensuring that we get a random start seed to pick our a and b values
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        // Generating different distributions
        default_random_engine generator(seed);

        // Initialize prime to 700001 as the prime only needs to be bigger than the integers we have of different values our data can take,
        // this is to ensure that it will give us a proper value when we do "mod k" in our hash function
        uniform_int_distribution<uint64_t> distribution(0,26000003-1);
        uniform_int_distribution<uint64_t> bitGenerator(9223372036854775808, 18446744073709551615);


        counters = new int*[t];
        for (int i=0; i < t; i++) {
            counters[i] = new int[k];
            memset(counters[i], 0, sizeof(int) * k);
            // Selecting random a and b
            uint64_t a = distribution(generator);
            uint64_t b = distribution(generator);
            a_b_hashvector.push_back(make_tuple(a,b));
        }

        unsigned gseed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine g_generator(gseed);
        for (int i=0; i < t; i++) {
            uint64_t g_a = bitGenerator(g_generator);
            uint64_t g_b = bitGenerator(g_generator);
            a_b_hashvector_for_g.push_back(make_tuple(g_a, g_b));
        }
    }

    // Destructor to free memory
    ~CountSketch() {
        for (int i=0; i < t; i++) {
            delete[] counters[i];
        }
        delete[] counters;

    }




    // As per the paper on CountSketch, elems will come in the form of (elem, c) where elem is elem to be counted, and c being by how much, given our processed data,
    // we will start the implementation by simply always passing 1 as the number for c
    void updateCounters(uint32_t elem, int c) {
        // Can potentially be made faster to use vectorized notation instead of for loop. TODO: Look into vectorizing this function
        for (int i=0; i < t; i++) {
            // Grab tuple of a and b from vector
            tuple<uint64_t, uint64_t> a_b_tuple = a_b_hashvector[i];
            tuple<uint64_t, uint64_t> a_b_tuple_for_g = a_b_hashvector_for_g[i];
            // Get the hash h_i by getting the randomly assigned a and b values
            uint64_t hash = customHashFunction(get<0>(a_b_tuple), get<1>(a_b_tuple), elem);
            // The hash simply returns what position in the array we want to count, so can use as follows
            counters[i][hash] += oneOrMinusOneHash(get<0>(a_b_tuple_for_g), get<1>(a_b_tuple_for_g), elem) * c;
        }
    }

    int findMedianElem(uint32_t elem) {
        vector<int> tempMedian;
        for (int i=0; i < t; i++) {
            tuple<uint64_t, uint64_t> a_b_tuple = a_b_hashvector[i];
            tuple<uint64_t, uint64_t> a_b_tuple_for_g = a_b_hashvector_for_g[i];
            int hash = customHashFunction(get<0>(a_b_tuple), get<1>(a_b_tuple), elem);
            int tempValue = counters[i][hash] * oneOrMinusOneHash(get<0>(a_b_tuple_for_g), get<1>(a_b_tuple_for_g), elem);
            tempMedian.push_back(tempValue);
        }
        sort(tempMedian.begin(), tempMedian.end());
        int median = 0;
        if (!(tempMedian.size() % 2)) {
            median = (tempMedian[tempMedian.size()/2] + tempMedian[(tempMedian.size()/2)-1])/2;
        }
        else {
            int middleOfArray = tempMedian.size()/2;
            median = tempMedian[middleOfArray];
        }
        return median;
    }
};