//
// Created by Tjalfe on 14-03-2023.
//

#include "../Algorithms/HeavyHitters.cpp"
#include "../Algorithms/CountSketch.cpp"
#include "../Algorithms/MisraGries.cpp"
#include "math.h"
#include <stdint.h>
#include "iostream"
#include <chrono>

void misraGries(int k, vector<int>& incomming) {
    // Initialize the new map
    unordered_map<int, int> freqMap;

    // Iterate the incomming stream
    for (int i : incomming) {
        // If the element already exists in the map, add one to its frequency
        if(freqMap.find(i) != freqMap.end()) {
            freqMap[i]++;
        }
            // Else if the size of D is less than k-1, insert a in D with value 1
        else if (freqMap.size() < k - 1) {
            freqMap[i] = 1;
        }
            // Else decrement all counters in D by 1, and remove all elements with count 0
        else {
            for (auto& it : freqMap) {
                it.second--;
                if (it.second == 0) {
                    freqMap.erase(it.first);
                }
            }
        }
    }
}

vector<uint32_t> vectorizationOfDataset() {
    // Loading the dataset for testing
    ifstream dataStream(R"(TrackingFrequentDataItems/dataset/CAIDAProcessed.txt)");

    //Vector for the values in the file
    vector<uint32_t> processedValues;

    // Temporary buffer and string stream for converting string to int.
    string temp;
    uint32_t elem;
    stringstream ss;

    // Get the input from the input file until EOF
    while (getline(dataStream, temp)) {
        ss << temp;
        ss >> elem;
        // Add to the list of output strings
        processedValues.push_back(elem);
        ss.clear();
    }

    return processedValues;
}
vector<uint32_t> vectorizationOfDatasetForExperiments(const string& dataset) {
    // Loading the dataset for testing
    string readFile = "TrackingFrequentDataItems/trainingDatasets/" + dataset;
    ifstream dataStream(readFile);

    //Vector for the values in the file
    vector<uint32_t> processedValues;

    // Temporary buffer and string stream for converting string to int.
    string temp;
    uint32_t elem;
    stringstream ss;

    // Get the input from the input file until EOF
    while (getline(dataStream, temp)) {
        ss << temp;
        ss >> elem;
        // Add to the list of output strings
        processedValues.push_back(elem);
        ss.clear();
    }

    return processedValues;
}
void errorGraphExperiment(double e, double d){
    vector<uint32_t> datasetValues = vectorizationOfDatasetForExperiments("dataset12.txt");
    sort(datasetValues.begin(), datasetValues.end());
    CountMinSketch CMS = *new CountMinSketch(40, 200);
    CountSketch CS = *new CountSketch(40, 200);
    MisraGries MG = *new MisraGries(200);
    auto startTime = chrono::high_resolution_clock::now();
    for (auto x : datasetValues) {

    }
}
void testMGCache() {
    vector<uint32_t> datasetValues = vectorizationOfDatasetForExperiments("dataset12.txt");
    ofstream runAndQueryTimesMG(R"(TrackingFrequentDataItems/experimentresults/runAndQueryTimeMGCache.txt)");

    for(int i=1; i<=200; i++) {
        int k = 50*i;
        MisraGries MG = *new MisraGries(k);
        auto startTimeMG = chrono::high_resolution_clock::now();
        for(auto x : datasetValues) {
            MG.updateCounters(x, 1);
        }
        MG.findElem(7);
        auto stopTimeMG = chrono::high_resolution_clock::now();
        auto totalTimeMG = chrono::duration_cast<chrono::milliseconds>(stopTimeMG - startTimeMG);
        runAndQueryTimesMG << i << " " << totalTimeMG.count() << " k: "<< k << endl;
    }
}
void cacheSizeRunExperiment() {
    vector<uint32_t> datasetValues = vectorizationOfDatasetForExperiments("dataset12.txt");
    ofstream runAndQueryTimes(R"(TrackingFrequentDataItems/experimentresults/runAndQueryTimeCMS1to40.txt)");
    ofstream runAndQueryTimesCS(R"(TrackingFrequentDataItems/experimentresults/runAndQueryTimeCS1to40.txt)");
    ofstream runAndQueryTimesMG(R"(TrackingFrequentDataItems/experimentresults/runAndQueryTimeMG1to40.txt)");

    for(int i=1; i<=200; i++) {
        int k = 50*i;
        int t = i;
        CountMinSketch CMS = *new CountMinSketch(t, k);
        auto startTime = chrono::high_resolution_clock::now();
        for(auto x : datasetValues) {
            CMS.updateCounters(x, 1);
        }
        CMS.findMinElem(7);
        auto stopTime = chrono::high_resolution_clock::now();
        auto totalTime = chrono::duration_cast<chrono::milliseconds>(stopTime - startTime);
        runAndQueryTimes << totalTime.count() << endl;

        CountSketch CS = *new CountSketch(t, k);
        auto startTimeCS = chrono::high_resolution_clock::now();
        for(auto x : datasetValues) {
            CS.updateCounters(x, 1);
        }
        CS.findMedianElem(7);
        auto stopTimeCS = chrono::high_resolution_clock::now();
        auto totalTimeCS = chrono::duration_cast<chrono::milliseconds>(stopTimeCS - startTimeCS);
        runAndQueryTimesCS << totalTimeCS.count() << endl;

        MisraGries MG = *new MisraGries(k);
        auto startTimeMG = chrono::high_resolution_clock::now();
        for(auto x : datasetValues) {
            MG.updateCounters(x, 1);
        }
        MG.findElem(7);
        auto stopTimeMG = chrono::high_resolution_clock::now();
        auto totalTimeMG = chrono::duration_cast<chrono::milliseconds>(stopTimeMG - startTimeMG);
        runAndQueryTimesMG << totalTimeMG.count() << endl;
    }
}
void deterministicCheck() {
    vector<uint32_t> dataset = vectorizationOfDatasetForExperiments("dataset0850000.txt");
    ofstream deterministicDat(R"(TrackingFrequentDataItems/experimentresults/deterministicDat0850000.txt)");
    int index = 0;
    sort(dataset.begin(), dataset.end());
    for (int i=1; i<=50000; i++) {
        int acc = 0;
        while(i==dataset[index]) {
            acc++;
            index++;
        }
        deterministicDat << acc << endl;
    }
}
void averageErrorExperiment() {
    vector<uint32_t> dataset = vectorizationOfDatasetForExperiments("dataset0850000.txt");
    ofstream averageErrorCS(R"(TrackingFrequentDataItems/experimentresults/averageErrorCSWithProperT.txt)");
    ofstream averageErrorCMS(R"(TrackingFrequentDataItems/experimentresults/averageErrorCMS.txt)");
    ofstream averageErrorMG(R"(TrackingFrequentDataItems/experimentresults/averageErrorMG.txt)");
    vector<uint32_t> datasetAmount = vectorizationOfDatasetForExperiments("deterministicDat0850000.txt");
    cout << datasetAmount.size() << endl;
    double delta = 1/pow(dataset.size(), 2);
    double eps = 0.01;
    cout << delta << endl;
    int tForCMS = ceil(log2(1/delta));
    int tForCS = ceil(4*(log2(1/delta)/log2(4/exp(1))));
    int kForCMS = ceil(2 / eps);
    int kForCS = ceil(4/pow(eps, 2)); // Is this fair? It essentially gives CS 40000 counters vs CMS 200, maybe change to be even 200.
    cout << delta << " " << tForCS << " " << tForCMS << " " << kForCMS << " " << kForCS << endl;
    CountSketch CS = *new CountSketch(tForCMS, kForCMS);
    CountMinSketch CMS = *new CountMinSketch(tForCMS, kForCMS);
    MisraGries MG = *new MisraGries(kForCMS);
    for (auto x : dataset) {
        CS.updateCounters(x, 1);
        CMS.updateCounters(x, 1);
        MG.updateCounters(x, 1);
    }
    cout << MG.findElem(878) << endl;
    cout << datasetAmount[878-1] << endl;
    for (auto y : dataset) {
        int CSERR = CS.findMedianElem(y)-datasetAmount[y-1];
        int CMSERR = CMS.findMinElem(y)-datasetAmount[y-1];
        int MGERR = MG.findElem(y)-datasetAmount[y-1];
        averageErrorCS << abs(CSERR) << endl;
        averageErrorCMS << abs(CMSERR) << endl;
        averageErrorMG << abs(MGERR) << endl;
    }
}


void heavyHitterNodesVisitedMinimum() {
    vector<uint32_t> caidaSet = vectorizationOfDataset();
    HeavyHitters newHH = *new HeavyHitters(0.01, caidaSet.size(), 3735400830);
    for (auto x : caidaSet) {
        newHH.updateCounters(x, 1);
    }
    vector<uint32_t> result = newHH.HH(0, round(log2(3735400830)));
    cout << result.size() << endl;
    for (auto x : result) {
        cout << x << endl;
    }
    cout << newHH.getCounter() << endl;
}

void heavyHitterNodesVisitedProper() {
    vector<uint32_t> dataset1 = vectorizationOfDatasetForExperiments("dataset0850000.txt");
    ofstream countersSeen(R"(TrackingFrequentDataItems/experimentresults/heavyHittersNodesVisited.txt)");
    for (int i=0; i < 50; i++) {
        HeavyHitters newHH = *new HeavyHitters(0.01, dataset1.size(), 50000);
        for (auto x : dataset1) {
            newHH.updateCounters(x, 1);
        }
        vector<uint32_t> heavyHitters = newHH.HH(0, ceil(log2(50000)));
        int currentCounter = newHH.getCounter();
        for (auto y : heavyHitters) {
            cout << y << endl;
        }
        countersSeen << currentCounter << endl;
    }
}

void testCMS(double e, double d) {
    // Deprecated, use direct values instead and calculate backwards in report with math
    // int k = ceil(2 / e);
    // int t = ceil(log2(1/d));

    int k = e;
    int t = d;
    cout << "T: " << t << "\n";
    cout << "K: " << k << "\n";

    // Creating the CountMinSketch model
    CountMinSketch CMS = *new CountMinSketch(t, k);

    vector<uint32_t> processedValues = vectorizationOfDataset();

    for (auto x : processedValues) {
        CMS.updateCounters(x, 1);
    }

    int res = CMS.findMinElem(3107889171);

    cout << "Res: " << res << endl;
}

void makePlotFileCMS(int epoch) {
    vector<uint32_t> processedValues = vectorizationOfDataset();
    ofstream plotFileCMS(R"(TrackingFrequentDataItems/plotfiles/plotFileCMS.txt)");
    for (int i=0; i<epoch; i++) {
        // For this plot testing we are using t value 40 and k value 200
        CountMinSketch CMS = *new CountMinSketch(40, 200);
        for (auto x : processedValues) {
            CMS.updateCounters(x, 1);
        }
        for (auto x : processedValues) {
            plotFileCMS << CMS.findMinElem(x) << endl;
        }
    }
}

void makePlotFileCS(int epoch) {
    vector<uint32_t> processedValues = vectorizationOfDataset();
    ofstream plotFileCMS(R"(TrackingFrequentDataItems/plotfiles/plotFileCS.txt)");
    for (int i=0; i<epoch; i++) {
        // For this plot testing we are using t value 40 and k value 200
        CountSketch CS = *new CountSketch(40, 200);
        for (auto x : processedValues) {
            CS.updateCounters(x, 1);
        }
        for (auto x : processedValues) {
            plotFileCMS << CS.findMedianElem(x) << endl;
        }
    }
}

double l2() {
    vector<uint32_t> something = vectorizationOfDataset();

    double accum = 0;
    for (double x : something) {
        accum += 1 * 1;
    }
    double norm = sqrt(accum);

    return norm;
}

void testCS(double e, double d) {
    // Deprecated, use direct values instead and calculate backwards in report with math
    // int k = ceil(2 / e);
    // int t = ceil(log2(1/d));

    int k = e;
    int t = d;

    cout << "T: " << t << "\n";
    cout << "K: " << k << "\n";

    CountSketch CS = *new CountSketch(t, k);

    vector<uint32_t> processedValues = vectorizationOfDataset();

    for (auto x : processedValues) {
        CS.updateCounters(x, 1);
    }

    int res = CS.findMedianElem(3107889171);

    cout << "Res: " << res << endl;

}

void notUsed() {
    double d = l2();
    cout << "d: " << d << endl;
    auto start = chrono::high_resolution_clock::now();
    // We are choosing epsilon e = 0.05 and delta d = 0.05 for initial testing
    //testCMS(0.05, 0.05);
    d *= 0.05;
    testCS(0.05, d);

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Duration: " << duration.count() << " milliseconds" << endl;
}

void checkHH() {
    vector<uint32_t> caidaSet = vectorizationOfDataset();
    HeavyHitters newHH = *new HeavyHitters(0.01, caidaSet.size(), 3735400830);
    for (auto x : caidaSet) {
        newHH.updateCounters(x, 1);
    }
    vector<uint32_t> result = newHH.HH(0, round(log2(3735400830)));
    cout << result.size() << endl;
    for (auto x : result) {
        cout << x << endl;
    }
}

int main(){
return 0;
}