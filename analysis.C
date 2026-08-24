#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveText.h"
#include <iostream>
#include <vector>

using namespace std;

void analysis() {
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetLegendBorderSize(0);

    gStyle->SetLegendTextSize(0.055);
    gStyle->SetLabelSize(0.055, "XY");
    gStyle->SetTitleSize(0.055, "XY");
    gStyle->SetTextFont(42);

    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.16);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.08);

    gStyle->SetNdivisions(505, "XY");

    const char* files[] = {"scenario1.root", "scenario2.root", "scenario3.root"};
    const char* titles[] = {"Scenario 1", "Scenario 2", "Scenario 3"};

    double E0 = 15200.0;

    TGraph *meanEnergyGraph = new TGraph();
    TGraph *energyDispersionGraph = new TGraph();
    TGraph *positionDispersionGraph = new TGraph();

    TCanvas *histogramCanvas = new TCanvas("histogramCanvas", "Histograms", 1200, 800);
    histogramCanvas->Divide(3, 2);

    for (int i = 0; i < 3; i++) {
        TFile *file = new TFile(files[i]);
        if (!file || file->IsZombie()) {
            cout << "File not found: " << files[i] << endl;
            continue;
        }

        TTree *tree = (TTree*)file->Get("tree");
        if (!tree) {
            cout << "Tree not found in file " << files[i] << endl;
            continue;
        }

        histogramCanvas->cd(i + 1);

        TH1D *energyHistogram = new TH1D(
            Form("energyHistogram_%d", i), "", 100, 15170, 15200
        );

        energyHistogram->GetXaxis()->SetTitle("Kinetic energy E [MeV]");
        energyHistogram->GetXaxis()->CenterTitle(true);
        energyHistogram->GetXaxis()->SetTitleOffset(1.2);

        energyHistogram->GetYaxis()->SetTitle("Counts");
        energyHistogram->GetYaxis()->CenterTitle(true);
        energyHistogram->GetYaxis()->SetTitleOffset(1.6);

        energyHistogram->SetLineColor(kBlack);
        energyHistogram->SetLineWidth(2);

        tree->Draw(Form("e/1e6 >> energyHistogram_%d", i));

        TLatex lat;
        lat.SetNDC();
        lat.SetTextSize(0.05);
        lat.SetTextFont(42);
        lat.DrawLatex(0.2, 0.93, titles[i]);

        energyHistogram->Fit("gaus", "Q");
        TF1 *energyFit = energyHistogram->GetFunction("gaus");

        if (energyFit) {
            energyFit->SetLineColor(kBlue);
            energyFit->SetLineWidth(2);
        }

        double meanEnergy = energyHistogram->GetMean();
        double sigmaEnergy = energyHistogram->GetRMS();

        if (energyFit) {
            meanEnergy = energyFit->GetParameter(1);
            sigmaEnergy = energyFit->GetParameter(2);
        }

        TLegend *energyLegend = new TLegend(0.2, 0.65, 0.55, 0.85);
        energyLegend->AddEntry(energyHistogram, "Simulation", "l");
        if (energyFit) {
            energyLegend->AddEntry(energyFit, "Gaussian Fit", "l");
        }
        energyLegend->Draw();

        double energyLoss = (E0 - meanEnergy) / E0 * 100.0;

        cout << "--- " << titles[i] << " ---" << endl;
        cout << "Mean Energy: " << meanEnergy << " MeV" << endl;
        cout << "Energy Loss: " << energyLoss << " %" << endl;
        cout << "Energy Dispersion: " << sigmaEnergy << " MeV" << endl;

        histogramCanvas->cd(i + 4);

        TH1D *positionHistogram = new TH1D(
            Form("positionHistogram_%d", i), "", 100, -0.05, 0.05
        );

        positionHistogram->GetXaxis()->SetTitle("Y position [#mum]");
        positionHistogram->GetXaxis()->CenterTitle(true);
        positionHistogram->GetXaxis()->SetTitleOffset(1.2);

        positionHistogram->GetYaxis()->SetTitle("Counts");
        positionHistogram->GetYaxis()->CenterTitle(true);
        positionHistogram->GetYaxis()->SetTitleOffset(1.6);

        positionHistogram->SetLineColor(kBlack);
        positionHistogram->SetLineWidth(2);

        tree->Draw(Form("y/10000.0 >> positionHistogram_%d", i));

        positionHistogram->Fit("gaus", "Q");
        TF1 *positionFit = positionHistogram->GetFunction("gaus");

        double sigmaPosition = positionHistogram->GetRMS();

        if (positionFit) {
            sigmaPosition = positionFit->GetParameter(2);
            positionFit->SetLineColor(kRed);
            positionFit->SetLineWidth(2);
        }

        TLegend *positionLegend = new TLegend(0.2, 0.65, 0.55, 0.85);
        positionLegend->AddEntry(positionHistogram, "Simulation", "l");
        if (positionFit) {
            positionLegend->AddEntry(positionFit, "Gaussian Fit", "l");
        }
        positionLegend->Draw();

        cout << "Y-Position Dispersion: " << sigmaPosition << " um" << endl << endl;

        meanEnergyGraph->SetPoint(i, i + 1, meanEnergy);
        energyDispersionGraph->SetPoint(i, i + 1, sigmaEnergy);
        positionDispersionGraph->SetPoint(i, i + 1, sigmaPosition);
    }

    TCanvas *summaryCanvas = new TCanvas("summaryCanvas", "Summary", 1200, 400);
    summaryCanvas->Divide(3, 1);

    gStyle->SetGridColor(kGray);

    summaryCanvas->cd(1);
    gPad->SetGrid(1, 1);
    meanEnergyGraph->SetTitle("Mean Kinetic Energy;Scenario;E [MeV]");
    meanEnergyGraph->SetMarkerStyle(21);
    meanEnergyGraph->SetMarkerColor(kBlue);
    meanEnergyGraph->SetLineWidth(2);
    meanEnergyGraph->SetLineColor(kBlue);
    meanEnergyGraph->Draw("ALP");

    summaryCanvas->cd(2);
    gPad->SetGrid(1, 1);
    energyDispersionGraph->SetTitle("Energy Dispersion (Straggling);Scenario;Sigma E [MeV]");
    energyDispersionGraph->SetMarkerStyle(21);
    energyDispersionGraph->SetMarkerColor(kRed);
    energyDispersionGraph->SetLineWidth(2);
    energyDispersionGraph->SetLineColor(kRed);
    energyDispersionGraph->Draw("ALP");

    summaryCanvas->cd(3);
    gPad->SetGrid(1, 1);
    positionDispersionGraph->SetTitle("Y-Position Dispersion;Scenario;Sigma Y [um]");
    positionDispersionGraph->SetMarkerStyle(21);
    positionDispersionGraph->SetMarkerColor(kGreen);
    positionDispersionGraph->SetLineWidth(2);
    positionDispersionGraph->SetLineColor(kGreen);
    positionDispersionGraph->Draw("ALP");
}