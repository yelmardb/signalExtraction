#include "includes/signalExtraction.h"



void signalExtraction(){

    //Parameters for pi0 analysis
    std::vector<Double_t> fBinsPi0Pt= {0.40, 0.60, 0.80, 0.90, 1.00, 1.20, 1.40, 1.60, 1.80, 2.0, 2.50, 3.00, 3.50, 4.00,  4.50, 5.00, 5.5, 6.00, 7.00, 8.00, 10.00};
    std::vector<Double_t> fBinsPi0PtMC = {0.40, 0.80,  1.2,  1.6,  2.0,  2.5,  3.0,  3.5,  4.0,  4.5,  6.0,  10.};
    const char* inFileDataName = "inputFiles/AnalysisResults_DATA_220661.root";
    const char* inFileMCName = "inputFiles/AnalysisResults_MC_214158.root";


    MesonSignalExtraction pi0SignalExtractionMC;
    pi0SignalExtractionMC.SetPathToFile(inFileMCName);
    pi0SignalExtractionMC.SetOutputFileName("outputSignalExtraction/SignalExtraction_Pi0_MC_223972.root");
    pi0SignalExtractionMC.SetMC();
    pi0SignalExtractionMC.SetPtBinning(fBinsPi0PtMC);
    pi0SignalExtractionMC.SetMeson("Pi0");
    pi0SignalExtractionMC.SetRun("Run3");
    pi0SignalExtractionMC.SetPathsToHistos();
    pi0SignalExtractionMC.SetFitParameters();
    pi0SignalExtractionMC.SetMassWindow(0.08, 0.25);
    pi0SignalExtractionMC.SetIntegrationRange(-0.035, 0.015);
    pi0SignalExtractionMC.SetBackgroundRange(0.24, 0.4);
    pi0SignalExtractionMC.LoadHistograms();
    pi0SignalExtractionMC.InitHistograms();
    pi0SignalExtractionMC.ExtractSignal();
    
    MesonSignalExtraction pi0SignalExtractionData;
    pi0SignalExtractionData.SetPathToFile(inFileDataName);
    pi0SignalExtractionData.SetOutputFileName("outputSignalExtraction/SignalExtraction_Pi0_DATA_220661.root");
    pi0SignalExtractionData.SetPtBinning(fBinsPi0Pt);
    pi0SignalExtractionData.SetMeson("Pi0");
    pi0SignalExtractionData.SetRun("Run3");
    pi0SignalExtractionData.SetPathsToHistos();
    pi0SignalExtractionData.SetFitParameters();
    pi0SignalExtractionData.SetMassWindow(0.04, 0.24);
    pi0SignalExtractionData.SetIntegrationRange(-0.035, 0.015);
    pi0SignalExtractionData.SetBackgroundRange(0.24, 0.4);
    pi0SignalExtractionData.LoadHistograms();
    pi0SignalExtractionData.InitHistograms();
    pi0SignalExtractionData.ExtractSignal();

    
    //Parameters for eta analysis
    std::vector<Double_t> fBinsEtaPt= {1.00, 2.00, 3.00, 4.0,5.0,10.0}; 
    MesonSignalExtraction etaSignalExtraction;
    etaSignalExtraction.SetPathToFile(inFileDataName);
    etaSignalExtraction.SetOutputFileName("outputSignalExtraction/SignalExtraction_Eta_DATA_220661.root");
    etaSignalExtraction.SetPtBinning(fBinsEtaPt);
    etaSignalExtraction.SetMeson("Eta");
    etaSignalExtraction.SetRun("Run3");
    etaSignalExtraction.SetPathsToHistos();
    etaSignalExtraction.SetFitParameters();
    etaSignalExtraction.SetMassWindow(0.35, 0.7);
    etaSignalExtraction.SetIntegrationRange(-0.047, 0.023);
    etaSignalExtraction.SetBackgroundRange(0.65, 0.79);
    etaSignalExtraction.LoadHistograms();
    etaSignalExtraction.InitHistograms();
    etaSignalExtraction.ExtractSignal();

    MesonSignalExtraction etaSignalExtractionMC;
    etaSignalExtractionMC.SetPathToFile(inFileMCName);
    etaSignalExtractionMC.SetOutputFileName("outputSignalExtraction/SignalExtraction_Eta_MC_223972.root");
    etaSignalExtractionMC.SetMC();
    etaSignalExtractionMC.SetPtBinning(fBinsEtaPt);
    etaSignalExtractionMC.SetMeson("Eta");
    etaSignalExtractionMC.SetRun("Run3");
    etaSignalExtractionMC.SetPathsToHistos();
    etaSignalExtractionMC.SetFitParameters();
    etaSignalExtractionMC.SetMassWindow(0.35, 0.7);
    etaSignalExtractionMC.SetIntegrationRange(-0.047, 0.023);
    etaSignalExtractionMC.SetBackgroundRange(0.65, 0.79);
    etaSignalExtractionMC.LoadHistograms();
    etaSignalExtractionMC.InitHistograms();
    etaSignalExtractionMC.ExtractSignal();

}
