//
// Created by Tjalfe on 29-05-2023.
//
#include <vector>
using namespace std;

class MisraGries {
private:
    uint64_t k; // k amount of counters
    unordered_map<uint32_t, uint64_t> counters; // the counters themselves

public:
    explicit MisraGries(uint64_t _k) : k(_k) {
        // Do nothing really, the structure is saved in the counters vector.
    }

    void updateCounters(uint32_t elem, int c) {
        if(counters.find(elem) != counters.end()) {
            counters[elem] += c;
        }
        else if (counters.size() < k) {
            counters[elem] = 1;
        }
        else {
            vector<uint32_t> deleteList;
            for (auto& it : counters) {
                it.second--;
                if(it.second == 0) {
                    deleteList.push_back(it.first);
                }
            }
            for (auto x : deleteList) {
                counters.erase(x);
            }
        }
    }

    uint64_t findElem(uint32_t elem) {
        if(counters.find(elem) != counters.end()) {
            return counters[elem] ;
        }
        else return 0;
    }
};