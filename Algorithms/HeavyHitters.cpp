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
#include "../Algorithms/CountMinSketch.cpp"

using namespace std;

class HeavyHitters {
private:
    uint64_t  universe, norm;
    double eps, entryRequirement;
    int amountOfSketches, counter;
    vector<CountMinSketch*> sketches; // keeping track of every CS we work with
    vector<uint32_t> heavyHitters;
    int version;


public:
    HeavyHitters(double _eps, uint64_t _norm, uint64_t _universe, int _version) : eps(_eps), norm(_norm), universe(_universe), version(_version) {
        // Initialize all CountMinSketches
        double delta = 0.25;
        counter = 0;
        entryRequirement = eps*norm;
        int check = ceil(log2(universe));
        amountOfSketches = check;
        int k = ceil(2/(eps/2));
        uint64_t intermediate = universe*universe;
        double newIntermediate = 1.0/intermediate;
        int t = ceil(log2(1/newIntermediate));
        for(int i=0; i<=amountOfSketches; i++) {
            if (version == 0) {
                if (i == 0) {
                    CountMinSketch* newCM = new CountMinSketch(t, k);
                    sketches.push_back(newCM);
                }
                else {
                    CountMinSketch* newCM = new CountMinSketch(ceil(log2(1/delta)), ceil(2/(eps/2)));
                    sketches.push_back(newCM);
                }
            }
            else {
                CountMinSketch* newCM = new CountMinSketch(t, k);
                sketches.push_back(newCM);
            }
        }
    }

    void updateCounters(uint32_t elem, int c) {
        bool hasHitZero = false;
        for(int i=0; i<=amountOfSketches; i++) {
            uint32_t elemToSave;
            if (hasHitZero) {
                elemToSave = 0;
            }
            else {
                elemToSave = elem>>i;
            }
            if (elemToSave == 0) {
                hasHitZero = true;
            }
            sketches[i]->updateCounters(elemToSave, c);
        }
    }

    vector<uint32_t> HH(uint32_t elem, int i) {
        heavyHitters.clear();
        findHeavyElements(elem, i);
        return heavyHitters;
    }
    void findHeavyElements(uint32_t elem, int i) {
        if (i>0) {
            if (sketches[i-1]->findMinElem(2*elem)>=entryRequirement) {
                counter++;
                findHeavyElements(2*elem, i-1);
            }
            if (sketches[i-1]->findMinElem(2*elem+1)>=entryRequirement) {
                counter++;
                findHeavyElements(2*elem+1, i-1);
            }
        }
        else {
            counter++;
            heavyHitters.push_back(elem);
        }
    }

    int getCounter() {
        return counter;
    }

    void checkLowestLevel(uint32_t elem) {
        cout << sketches[amountOfSketches-14]->findMinElem(elem) << endl;
    }
};