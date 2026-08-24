#include <iostream>
#include <fstream>
#include <string>
#include "TFile.h"
#include "TTree.h"

using namespace std;

void convertFile(const char* inputFileName, const char* outputFileName) {
    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        cout << "ERROR: Cannot open file: " << inputFileName << endl;
        return;
    }

    TFile *outputFile = new TFile(outputFileName, "RECREATE");
    TTree *tree = new TTree("tree", "SRIM simulation data");

    double e, y, z;

    tree->Branch("e", &e, "e/D");
    tree->Branch("y", &y, "y/D");
    tree->Branch("z", &z, "z/D");

    string header;
    for (int i = 0; i < 12; i++) {
        getline(inputFile, header);
    }

    char type;
    int ion, atom;
    double depth;
    double cx, cy, cz;

    int count = 0;

    while (inputFile >> type >> ion >> atom >> e >> depth >> y >> z >> cx >> cy >> cz) {
        if (type == 'T') {
            tree->Fill();
            count++;
        }
    }

    cout << "Saved " << count << " ions from file " << inputFileName << endl;

    tree->Write();
    outputFile->Close();
}

void converter() {
    convertFile("target_Cu_11um.txt", "scenario1.root");
    convertFile("backing_C_12um_target_Cu_11um.txt", "scenario2.root");
    convertFile("backing_C_12um_target_Cu_22um.txt", "scenario3.root");
}
