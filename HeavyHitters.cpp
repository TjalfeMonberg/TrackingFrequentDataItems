//
// Created by Tjalfe on 02-05-2023.
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

class HeavyHitters {
private:
    uint64_t  universe, norm;
    uint32_t highestElemInTree;
    double eps, entryRequirement;
    vector<CountMinSketch> sketches; // keeping track of every CS we work with
    vector<uint32_t> heavyHitters;


public:
    HeavyHitters(double _eps, uint64_t _norm, uint64_t _universe) : eps(_eps), norm(_norm), universe(_universe) {
        // Initialize all CountMinSketches
        float delta = 1/4.0;
        entryRequirement = eps*norm;
        for(int i=0; i<log(universe); i++) {
            if (i == 0) {
                int t = 2/(eps/2);
                uint64_t intermediate = universe*universe;
                double newIntermediate = 1.0/intermediate;
                int k = log(1/newIntermediate);
                CountMinSketch newCM = *new CountMinSketch(t, k);
                sketches.push_back(newCM);
            }
            else {
                CountMinSketch newCM = *new CountMinSketch(2/(eps/2), log(1/delta));
                sketches.push_back(newCM);
            }
        }
    }

    void updateCounters(uint32_t elem, int c) {
        for(int i=0; i<log(universe); i++) {
            uint32_t elemToSave = elem>>i;
            sketches[i].updateCounters(elemToSave, c);
            if (i+1 > log(universe)) {
                highestElemInTree = elemToSave;
            }
        }
    }

    vector<uint32_t> HH(uint32_t elem, int i) {
        heavyHitters.clear();
        findHeavyElements(elem, i);
        return heavyHitters;
    }
    void findHeavyElements(uint32_t elem, int i) {
        if (elem == -1) {
            elem = highestElemInTree;
        }
        if (i>0) {
            if (sketches[i-1].findMinElem(2*elem)>=entryRequirement) {
                findHeavyElements(2*elem, i-1);
            }
            if (sketches[i-1].findMinElem(2*elem+1)>=entryRequirement) {
                findHeavyElements(2*elem+1, i-1);
            }
        }
        else {
            heavyHitters.push_back(elem);
        }
    }
};