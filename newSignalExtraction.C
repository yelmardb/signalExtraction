#include "includes/newSignalExtraction.h"
#include "includes/inputData.h"


void CalculateAndApplyEff(const char* outputSignalExtractionData, const char* outputSignalExtractionMC,  const char*  outputSignalExtractionTrueMC, const char*  inFileTrainTrueMC, const char* meson);


void newSignalExtraction(){

    const char* inFileDataName = strdup(Form("inputFiles/%s/AnalysisResults_DATA_%d.root", period,runNumberData));
    char* inFileMCName = strdup(Form("inputFiles/%s/AnalysisResults_MC_%d.root", period, runNumberMC));    
    char* inFileTrueMCName = strdup(Form("inputFiles/%s/AnalysisResults_MC_%d.root", period, runNumberTrueMC));

    char* outputSignalExtractionPi0TrueMC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_TrueMC_%d.root", period, runNumberTrueMC));
    char* outputSignalExtractionPi0MC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_MC_%d.root", period, runNumberMC));
    char* outputSignalExtractionPi0Data = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root", period, runNumberData));
    char* outputSignalExtractionEtaTrueMC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_TrueMC_%d.root", period, runNumberTrueMC));
    char* outputSignalExtractionEtaMC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_MC_%d.root", period, runNumberMC));
    char* outputSignalExtractionEtaData = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_Data_%d.root", period, runNumberData));

    Double_t minBackgroundRangePi0 = 0.2;
    Double_t maxBackgroundRangePi0 = 0.3;

    Double_t massWindowLowPi0 = 0.05;
    Double_t massWindowHighPi0 = 0.25;

    Double_t minIntegrationRangePi0 = -0.035;
    Double_t maxIntegrationRangePi0 = 0.024;

   MesonSignalExtraction pi0SignalExtractionData;
    pi0SignalExtractionData.SetPathToFile(inFileDataName);
    pi0SignalExtractionData.SetOutputFileName(outputSignalExtractionPi0Data);
    pi0SignalExtractionData.SetPtBinning(fBinsPi0PtMC);
    pi0SignalExtractionData.SetMeson("Pi0");
    pi0SignalExtractionData.SetData();
    pi0SignalExtractionData.SetPeriod(period);
    pi0SignalExtractionData.SetRun("Run3");
    pi0SignalExtractionData.SetCutMode("");
    pi0SignalExtractionData.SetPathsToHistos();
    pi0SignalExtractionData.SetMassWindow(massWindowLowPi0, massWindowHighPi0);
    pi0SignalExtractionData.SetIntegrationRange(minIntegrationRangePi0, maxIntegrationRangePi0);
    pi0SignalExtractionData.SetBackgroundRange(minBackgroundRangePi0, maxBackgroundRangePi0);
    pi0SignalExtractionData.LoadHistograms();
    pi0SignalExtractionData.InitHistograms();
    pi0SignalExtractionData.ExtractSignal();

    MesonSignalExtraction pi0SignalExtractionMC;
    pi0SignalExtractionMC.SetPathToFile(inFileMCName);
    pi0SignalExtractionMC.SetOutputFileName(outputSignalExtractionPi0MC);
    pi0SignalExtractionMC.SetMC();
    pi0SignalExtractionMC.SetPeriod(period);
    pi0SignalExtractionMC.SetPtBinning(fBinsPi0PtMC);
    pi0SignalExtractionMC.SetMeson("Pi0");
    pi0SignalExtractionMC.SetRun("Run3");
    pi0SignalExtractionMC.SetCutMode("");
    pi0SignalExtractionMC.SetPathsToHistos();
    pi0SignalExtractionMC.SetMassWindow(massWindowLowPi0, massWindowHighPi0);
    pi0SignalExtractionMC.SetIntegrationRange(minIntegrationRangePi0, maxIntegrationRangePi0);
    pi0SignalExtractionMC.SetBackgroundRange(minBackgroundRangePi0, maxBackgroundRangePi0);
    pi0SignalExtractionMC.LoadHistograms();
    pi0SignalExtractionMC.InitHistograms();
    pi0SignalExtractionMC.ExtractSignal();

    MesonSignalExtraction pi0SignalExtractionTrueMC;
    pi0SignalExtractionTrueMC.SetPathToFile(inFileTrueMCName);
    pi0SignalExtractionTrueMC.SetOutputFileName(outputSignalExtractionPi0TrueMC);
    pi0SignalExtractionTrueMC.SetTrueMC();
    pi0SignalExtractionTrueMC.SetPeriod(period);
    pi0SignalExtractionTrueMC.SetPtBinning(fBinsPi0PtMC);
    pi0SignalExtractionTrueMC.SetMeson("Pi0");
    pi0SignalExtractionTrueMC.SetRun("Run3");
    pi0SignalExtractionTrueMC.SetCutMode("");
    pi0SignalExtractionTrueMC.SetPathsToHistos();
    pi0SignalExtractionTrueMC.SetMassWindow(massWindowLowPi0, massWindowHighPi0);
    pi0SignalExtractionTrueMC.SetIntegrationRange(minIntegrationRangePi0, maxIntegrationRangePi0);
    pi0SignalExtractionTrueMC.SetBackgroundRange(minBackgroundRangePi0, maxBackgroundRangePi0);
    pi0SignalExtractionTrueMC.LoadHistograms();
    pi0SignalExtractionTrueMC.InitHistograms();
    pi0SignalExtractionTrueMC.ExtractSignal();

   CalculateAndApplyEff(outputSignalExtractionPi0Data, outputSignalExtractionPi0MC, outputSignalExtractionPi0TrueMC, inFileTrueMCName, "Pi0");

   ///Closure test
   char* outputSignalExtractionPi0ClosureTest = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_ClosureTest_%d.root", period, runNumberMC));

   MesonSignalExtraction  pi0SignalExtractionClosureTest;
   pi0SignalExtractionClosureTest.SetPathToFile(inFileMCName);
   pi0SignalExtractionClosureTest.SetOutputFileName(outputSignalExtractionPi0ClosureTest);
   pi0SignalExtractionClosureTest.SetPtBinning(fBinsPi0PtMC);
   pi0SignalExtractionClosureTest.SetMC();
   pi0SignalExtractionClosureTest.SetPeriod(period);
   pi0SignalExtractionClosureTest.SetRun("Run3");
   pi0SignalExtractionClosureTest.SetCutMode("");
   pi0SignalExtractionClosureTest.SetPathsToHistos();
   pi0SignalExtractionClosureTest.SetMassWindow(massWindowLowPi0, massWindowHighPi0);
   pi0SignalExtractionClosureTest.SetIntegrationRange(minIntegrationRangePi0, maxIntegrationRangePi0);
   pi0SignalExtractionClosureTest.SetBackgroundRange(minBackgroundRangePi0, maxBackgroundRangePi0);
   pi0SignalExtractionClosureTest.LoadHistograms();
   pi0SignalExtractionClosureTest.InitHistograms();
   pi0SignalExtractionClosureTest.ExtractSignal();

   CalculateAndApplyEff(outputSignalExtractionPi0ClosureTest, outputSignalExtractionPi0MC, outputSignalExtractionPi0TrueMC, inFileTrueMCName, "Pi0");





//standard:

    // Double_t massWindowLowEta = 0.4;
    // Double_t massWindowHighEta = 0.69;

    // Double_t minBackgroundRangeEta = 0.45;
    // Double_t maxBackgroundRangeEta = 0.76;


    Double_t massWindowLowEta = 0.35;
    Double_t massWindowHighEta = 0.75;

    // Double_t minBackgroundRangeEta = 0.47;  //0.48,0.52 for pass7
    // Double_t maxBackgroundRangeEta = 0.51;

    Double_t minBackgroundRangeEta = 0.6;  //0.48,0.52 for pass7
    Double_t maxBackgroundRangeEta = 0.8;

    Double_t minIntegrationRangeEta = -0.065;
    Double_t maxIntegrationRangeEta = 0.03;

    MesonSignalExtraction etaSignalExtraction;
    etaSignalExtraction.SetPathToFile(inFileDataName);
    etaSignalExtraction.SetOutputFileName(outputSignalExtractionEtaData);
    etaSignalExtraction.SetPtBinning(fBinsEtaPt);
    etaSignalExtraction.SetData();
    etaSignalExtraction.SetPeriod(period);
    etaSignalExtraction.SetMeson("Eta");
    etaSignalExtraction.SetRun("Run3");
    etaSignalExtraction.SetCutMode("");
    etaSignalExtraction.SetPathsToHistos();
    etaSignalExtraction.SetMassWindow(massWindowLowEta, massWindowHighEta);
    etaSignalExtraction.SetIntegrationRange(minIntegrationRangeEta, maxIntegrationRangeEta);
    etaSignalExtraction.SetBackgroundRange(minBackgroundRangeEta, maxBackgroundRangeEta);
    etaSignalExtraction.LoadHistograms();
    etaSignalExtraction.InitHistograms();
    etaSignalExtraction.ExtractSignal();

    MesonSignalExtraction etaSignalExtractionMC;
    etaSignalExtractionMC.SetPathToFile(inFileMCName);
    etaSignalExtractionMC.SetOutputFileName(outputSignalExtractionEtaMC);
    etaSignalExtractionMC.SetMC();
    etaSignalExtractionMC.SetPeriod(period);
    etaSignalExtractionMC.SetPtBinning(fBinsEtaPt);
    etaSignalExtractionMC.SetMeson("Eta");
    etaSignalExtractionMC.SetRun("Run3");
    etaSignalExtractionMC.SetCutMode("");
    etaSignalExtractionMC.SetPathsToHistos();
    etaSignalExtractionMC.SetMassWindow(massWindowLowEta, massWindowHighEta);
    etaSignalExtractionMC.SetIntegrationRange(minIntegrationRangeEta, maxIntegrationRangeEta);
    etaSignalExtractionMC.SetBackgroundRange(minBackgroundRangeEta, maxBackgroundRangeEta);
    etaSignalExtractionMC.LoadHistograms();
    etaSignalExtractionMC.InitHistograms();
    etaSignalExtractionMC.ExtractSignal();

    MesonSignalExtraction etaSignalExtractionTrueMC;
    etaSignalExtractionTrueMC.SetPathToFile(inFileTrueMCName);
    etaSignalExtractionTrueMC.SetOutputFileName(outputSignalExtractionEtaTrueMC);
    etaSignalExtractionTrueMC.SetTrueMC();
    etaSignalExtractionTrueMC.SetPeriod(period);
    etaSignalExtractionTrueMC.SetPtBinning(fBinsEtaPt);
    etaSignalExtractionTrueMC.SetMeson("Eta");
    etaSignalExtractionTrueMC.SetRun("Run3");
    etaSignalExtractionTrueMC.SetCutMode("");
    etaSignalExtractionTrueMC.SetPathsToHistos();
    etaSignalExtractionTrueMC.SetMassWindow(massWindowLowEta, massWindowHighEta);
    etaSignalExtractionTrueMC.SetIntegrationRange(minIntegrationRangeEta, maxIntegrationRangeEta);
    etaSignalExtractionTrueMC.SetBackgroundRange(minBackgroundRangeEta, maxBackgroundRangeEta);
    etaSignalExtractionTrueMC.LoadHistograms();
    etaSignalExtractionTrueMC.InitHistograms();
    etaSignalExtractionTrueMC.ExtractSignal();

    CalculateAndApplyEff(outputSignalExtractionEtaData, outputSignalExtractionEtaMC, outputSignalExtractionEtaTrueMC, inFileTrueMCName, "Eta");

/*
    for(auto cutMode : cutModes){
        char* outputSignalExtractionPi0TrueMC_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_TrueMC_%d_.root", period, cutMode,runNumberTrueMC));
        char* outputSignalExtractionPi0MC_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_MC_%d.root", period, cutMode,runNumberMC));
        char* outputSignalExtractionPi0Data_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_Data_%d.root", period, cutMode,runNumberData));
        char* outputSignalExtractionEtaTrueMC_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_TrueMC_%d.root", period, cutMode,runNumberTrueMC));
        char* outputSignalExtractionEtaMC_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_MC_%d.root", period, cutMode,runNumberMC));
        char* outputSignalExtractionEtaData_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_Data_%d.root", period, cutMode,runNumberData));


        MesonSignalExtraction pi0SignalExtractionData_cutVar;
        pi0SignalExtractionData_cutVar.SetPathToFile(inFileDataName);
        pi0SignalExtractionData_cutVar.SetOutputFileName(outputSignalExtractionPi0Data_cutVar);
        pi0SignalExtractionData_cutVar.SetPtBinning(fBinsPi0PtMC);
        pi0SignalExtractionData_cutVar.SetMeson("Pi0");
        pi0SignalExtractionData_cutVar.SetData();
        pi0SignalExtractionData_cutVar.SetPeriod(period);
        pi0SignalExtractionData_cutVar.SetRun("Run3");
        pi0SignalExtractionData_cutVar.SetCutMode(cutMode);
        pi0SignalExtractionData_cutVar.SetPathsToHistos();
        pi0SignalExtractionData_cutVar.SetMassWindow(massWindowLowPi0, massWindowHighPi0);
        pi0SignalExtractionData_cutVar.SetIntegrationRange(minIntegrationRangePi0, maxIntegrationRangePi0);
        pi0SignalExtractionData_cutVar.SetBackgroundRange(minBackgroundRangePi0, maxBackgroundRangePi0);
        pi0SignalExtractionData_cutVar.LoadHistograms();
        pi0SignalExtractionData_cutVar.InitHistograms();
        pi0SignalExtractionData_cutVar.ExtractSignal();

        MesonSignalExtraction pi0SignalExtractionMC_cutVar;
        pi0SignalExtractionMC_cutVar.SetPathToFile(inFileMCName);
        pi0SignalExtractionMC_cutVar.SetOutputFileName(outputSignalExtractionPi0MC_cutVar);
        pi0SignalExtractionMC_cutVar.SetPtBinning(fBinsPi0PtMC);
        pi0SignalExtractionMC_cutVar.SetMeson("Pi0");
        pi0SignalExtractionMC_cutVar.SetMC();
        pi0SignalExtractionMC_cutVar.SetPeriod(period);
        pi0SignalExtractionMC_cutVar.SetRun("Run3");
        pi0SignalExtractionMC_cutVar.SetCutMode(cutMode);
        pi0SignalExtractionMC_cutVar.SetPathsToHistos();
        pi0SignalExtractionMC_cutVar.SetMassWindow(massWindowLowPi0, massWindowHighPi0);
        pi0SignalExtractionMC_cutVar.SetIntegrationRange(minIntegrationRangePi0, maxIntegrationRangePi0);
        pi0SignalExtractionMC_cutVar.SetBackgroundRange(minBackgroundRangePi0, maxBackgroundRangePi0);
        pi0SignalExtractionMC_cutVar.LoadHistograms();
        pi0SignalExtractionMC_cutVar.InitHistograms();
        pi0SignalExtractionMC_cutVar.ExtractSignal();

        MesonSignalExtraction pi0SignalExtractionTrueMC_cutVar;
        pi0SignalExtractionTrueMC_cutVar.SetPathToFile(inFileTrueMCName);
        pi0SignalExtractionTrueMC_cutVar.SetOutputFileName(outputSignalExtractionPi0TrueMC_cutVar);
        pi0SignalExtractionTrueMC_cutVar.SetPtBinning(fBinsPi0PtMC);
        pi0SignalExtractionTrueMC_cutVar.SetMeson("Pi0");
        pi0SignalExtractionTrueMC_cutVar.SetTrueMC();
        pi0SignalExtractionTrueMC_cutVar.SetPeriod(period);
        pi0SignalExtractionTrueMC_cutVar.SetRun("Run3");
        pi0SignalExtractionTrueMC_cutVar.SetCutMode(cutMode);
        pi0SignalExtractionTrueMC_cutVar.SetPathsToHistos();
        pi0SignalExtractionTrueMC_cutVar.SetMassWindow(massWindowLowPi0, massWindowHighPi0);
        pi0SignalExtractionTrueMC_cutVar.SetIntegrationRange(minIntegrationRangePi0, maxIntegrationRangePi0);
        pi0SignalExtractionTrueMC_cutVar.SetBackgroundRange(minBackgroundRangePi0, maxBackgroundRangePi0);
        pi0SignalExtractionTrueMC_cutVar.LoadHistograms();
        pi0SignalExtractionTrueMC_cutVar.InitHistograms();
        pi0SignalExtractionTrueMC_cutVar.ExtractSignal();

        
        CalculateAndApplyEff(outputSignalExtractionPi0Data_cutVar, outputSignalExtractionPi0MC_cutVar, outputSignalExtractionPi0TrueMC_cutVar, inFileTrueMCName, "Pi0");

        MesonSignalExtraction etaSignalExtractionData_cutVar;
        etaSignalExtractionData_cutVar.SetPathToFile(inFileDataName);
        etaSignalExtractionData_cutVar.SetOutputFileName(outputSignalExtractionEtaData_cutVar);
        etaSignalExtractionData_cutVar.SetPtBinning(fBinsEtaPt);
        etaSignalExtractionData_cutVar.SetMeson("Eta");
        etaSignalExtractionData_cutVar.SetData();
        etaSignalExtractionData_cutVar.SetPeriod(period);
        etaSignalExtractionData_cutVar.SetRun("Run3");
        etaSignalExtractionData_cutVar.SetCutMode(cutMode);
        etaSignalExtractionData_cutVar.SetPathsToHistos();
        etaSignalExtractionData_cutVar.SetMassWindow(massWindowLowEta, massWindowHighEta);
        etaSignalExtractionData_cutVar.SetIntegrationRange(minIntegrationRangeEta, maxIntegrationRangeEta);
        etaSignalExtractionData_cutVar.SetBackgroundRange(minBackgroundRangeEta, maxBackgroundRangeEta);
        etaSignalExtractionData_cutVar.LoadHistograms();
        etaSignalExtractionData_cutVar.InitHistograms();
        etaSignalExtractionData_cutVar.ExtractSignal();

        MesonSignalExtraction etaSignalExtractionMC_cutVar;
        etaSignalExtractionMC_cutVar.SetPathToFile(inFileMCName);
        etaSignalExtractionMC_cutVar.SetOutputFileName(outputSignalExtractionEtaMC_cutVar);
        etaSignalExtractionMC_cutVar.SetPtBinning(fBinsEtaPt);
        etaSignalExtractionMC_cutVar.SetMeson("Eta");
        etaSignalExtractionMC_cutVar.SetMC();
        etaSignalExtractionMC_cutVar.SetPeriod(period);
        etaSignalExtractionMC_cutVar.SetRun("Run3");
        etaSignalExtractionMC_cutVar.SetCutMode(cutMode);
        etaSignalExtractionMC_cutVar.SetPathsToHistos();
        etaSignalExtractionMC_cutVar.SetMassWindow(massWindowLowEta, massWindowHighEta);
        etaSignalExtractionMC_cutVar.SetIntegrationRange(minIntegrationRangeEta, maxIntegrationRangeEta);
        etaSignalExtractionMC_cutVar.SetBackgroundRange(minBackgroundRangeEta, maxBackgroundRangeEta);
        etaSignalExtractionMC_cutVar.LoadHistograms();
        etaSignalExtractionMC_cutVar.InitHistograms();
        etaSignalExtractionMC_cutVar.ExtractSignal();

        MesonSignalExtraction etaSignalExtractionTrueMC_cutVar;
        etaSignalExtractionTrueMC_cutVar.SetPathToFile(inFileTrueMCName);
        etaSignalExtractionTrueMC_cutVar.SetOutputFileName(outputSignalExtractionEtaTrueMC_cutVar);
        etaSignalExtractionTrueMC_cutVar.SetPtBinning(fBinsEtaPt);
        etaSignalExtractionTrueMC_cutVar.SetMeson("Eta");
        etaSignalExtractionTrueMC_cutVar.SetTrueMC();
        etaSignalExtractionTrueMC_cutVar.SetPeriod(period);
        etaSignalExtractionTrueMC_cutVar.SetRun("Run3");
        etaSignalExtractionTrueMC_cutVar.SetCutMode(cutMode);
        etaSignalExtractionTrueMC_cutVar.SetPathsToHistos();
        etaSignalExtractionTrueMC_cutVar.SetMassWindow(massWindowLowEta, massWindowHighEta);
        etaSignalExtractionTrueMC_cutVar.SetIntegrationRange(minIntegrationRangeEta, maxIntegrationRangeEta);
        etaSignalExtractionTrueMC_cutVar.SetBackgroundRange(minBackgroundRangeEta, maxBackgroundRangeEta);
        etaSignalExtractionTrueMC_cutVar.LoadHistograms();
        etaSignalExtractionTrueMC_cutVar.InitHistograms();
        etaSignalExtractionTrueMC_cutVar.ExtractSignal();

        CalculateAndApplyEff(outputSignalExtractionEtaData_cutVar, outputSignalExtractionEtaMC_cutVar, outputSignalExtractionEtaTrueMC_cutVar, inFileTrueMCName, "Eta");
  
    }


*/
}




void CalculateAndApplyEff(const char* outputSignalExtractionData, const char* outputSignalExtractionMC,  const char*  outputSignalExtractionTrueMC, const char*  inFileTrainTrueMC, const char* meson){

    Double_t etaRange;
    if(strcmp(period, "LHC22oPass6") == 0)  etaRange = 1.8;
    else etaRange = 1.6;
    Double_t totXSection = 59.4*1e+9; // in pbarn
    Double_t branchingRatio;
    if(strcmp(meson, "Pi0") == 0) branchingRatio = 0.98823;
    else if(strcmp(meson, "Eta") == 0)  branchingRatio = 0.3941;
    else {cout << "no meson specified" << endl; return;}

    TH1D* hpTRecTrue;
    TH1D* hpTRec;
    hpTRec = (TH1D*)extractHistogram(Form("%s:/h1yieldUnNorm", outputSignalExtractionMC));
    hpTRec->SetName(Form("h1yield_%s", meson));
    hpTRecTrue = (TH1D*)extractHistogram(Form("%s:/h1yieldUnNorm", outputSignalExtractionTrueMC));
    hpTRecTrue->SetName(Form("hpTRecTrue_%s", meson));

    TH1D* hpTGen;
    TH1D* hpTGenTrue;
    TH1D* hCollisionCounterData = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", outputSignalExtractionData));
    Double_t nEventsData;
    if(strcmp(period, "LHC22oPass6") == 0) nEventsData = hCollisionCounterData->GetBinContent(10);
    else nEventsData = hCollisionCounterData->GetBinContent(12);

    cout << "/:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

    if(strcmp(period, "LHC22oPass6") == 0){
        Bool_t dirtyPatch = true;
        if(dirtyPatch) hpTGen = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc/Generated/PCMPCM/hPt_%s", inFileTrainTrueMC, meson));
        else  hpTGen = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Generated/%s/hPt", inFileTrainTrueMC, meson));
        hpTGen->SetName(Form("hPtGen_%s", meson));
        hpTGenTrue = (TH1D*)hpTGen->Clone(Form("hPtGenTrue_%s", meson));
    }
    else if(strcmp(period, "LHC22oPass7") == 0){
        hpTGen = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Generated/%s/hPt", inFileTrainTrueMC, meson));
        hpTGen->SetName(Form("hPtGen_%s", meson));
        hpTGenTrue = (TH1D*)hpTGen->Clone(Form("hPtGenTrue_%s", meson));
    }
    else{
        hpTGen = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Generated/%s/hPt", inFileTrainTrueMC, meson)); 
        hpTGen->SetName(Form("hPtGen_%s", meson));
        hpTGenTrue = (TH1D*)hpTGen->Clone(Form("hPtGenTrue_%s", meson));
    } 


    std::vector<Double_t> fBinsPt;
    if(strcmp(meson, "Eta") == 0){
        fBinsPt = fBinsEtaPt;
    }
    else if(strcmp(meson, "Pi0") == 0){
        fBinsPt = fBinsPi0PtMC;
    }
    TArrayD *binarrRecTrue = (TArrayD*)hpTRecTrue->GetXaxis()->GetXbins();
    TH1D *hpTGenTrueRebinned = RebinHistogram(hpTGenTrue,hpTRecTrue->GetNbinsX(),binarrRecTrue->GetArray());
    hpTGenTrueRebinned->SetName(Form("hpTGenTrueRebinned_%s", meson));

    TArrayD *binarrRec = (TArrayD*)hpTRec->GetXaxis()->GetXbins();
    TH1D *hpTGenRebinned = RebinHistogram(hpTGen,hpTRec->GetNbinsX(),binarrRec->GetArray());
    hpTGenRebinned->SetName(Form("hPt_%s", meson));




    TH1D* mesonAccXEffxBR = (TH1D*)hpTRec->Clone(Form("mesonAccXEff_%s", meson));
    mesonAccXEffxBR->Divide(hpTGenTrueRebinned);
    mesonAccXEffxBR->SetName(Form("mesonAccXEff_%s", meson));
    //mesonAccXEffxBR->Scale(branchingRatio);

    TH1D* mesonAccXEffxBRTrue = (TH1D*)hpTRecTrue->Clone(Form("mesonAccXEffxBRTrue_%s", meson));
    mesonAccXEffxBRTrue->Divide(hpTGenTrueRebinned);
    mesonAccXEffxBRTrue->SetName(Form("mesonAccXEffxBRTrue_%s", meson));
    //mesonAccXEffxBRTrue->Scale(branchingRatio);


     TH1D* h1yield = (TH1D*)extractHistogram(Form("%s:/h1yieldUnNorm", outputSignalExtractionData));
     TH1D* h1yieldCorr = (TH1D*)h1yield->Clone("h1yieldCorr");
     h1yieldCorr->Reset();

    // TH1D* effJulia = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults(1).root:/PCMPCM/h_E_A");
    // if(strcmp(meson, "Eta") == 0) mesonAccXEffxBRTrue = (TH1D*)effJulia->Clone(Form("mesonAccXEffxBRTrue_%s", meson));


    for(int i = 0; i <= h1yieldCorr->GetNbinsX(); i++){
        h1yieldCorr->SetBinContent(i, h1yield->GetBinContent(i)/mesonAccXEffxBRTrue->GetBinContent(mesonAccXEffxBRTrue->FindBin(h1yield->GetBinCenter(i))));

        Double_t contentYield = h1yield->GetBinContent(i);
        Double_t errorYield = h1yield->GetBinError(i);
        Double_t contentAccXEffxBR = mesonAccXEffxBRTrue->GetBinContent(mesonAccXEffxBRTrue->FindBin(h1yield->GetBinCenter(i)));
        Double_t errorAccXEffxBR = mesonAccXEffxBRTrue->GetBinError(i);
        Double_t error = TMath::Sqrt( TMath::Power(errorYield/contentAccXEffxBR, 2) + TMath::Power(contentYield*errorAccXEffxBR/contentAccXEffxBR*contentAccXEffxBR, 2));
        h1yieldCorr->SetBinError(i, error);
    }
    
    TH1D* hCrossSection = (TH1D*)h1yieldCorr->Clone("hCrossSection");
    hCrossSection->SetName("hCrossSection");
    FinalizePt(hCrossSection, nEventsData, etaRange); // cval = (etarange*nevents>0) ? (val)/(width * 2.0 * TMath::Pi() * etarange * nevents * pt) : 0;
    hCrossSection->Scale(totXSection);

    FinalizePt(h1yieldCorr, nEventsData, etaRange); // cval = (etarange*nevents>0) ? (val)/(width * 2.0 * TMath::Pi() * etarange * nevents * pt) : 0;

    //FinalizeInvariantYield(h1yieldCorr, nEventsData, etaRange); // cval = ( (etarange*nevents>0) ? (val)/(width * etarange * nevents) : 0;

    SetHistogramPropertiesAlt(mesonAccXEffxBR, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
    SetHistogramPropertiesAlt(mesonAccXEffxBRTrue, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
    SetHistogramPropertiesAlt(hCrossSection, "#it{p}_{T} GeV/#it{c}", "#it{E} #frac{d^{3}#it{#sigma}}{d#it{p}^{3}} (pb GeV^{#minus2} #it{c}^{3})", 1);
    SetHistogramPropertiesAlt(h1yieldCorr, "#it{p}_{T} GeV/#it{c}", "#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}", 1);


    TFile* inFileSignalExtraction = TFile::Open(Form("%s", outputSignalExtractionData), "UPDATE");
    inFileSignalExtraction->cd();
    mesonAccXEffxBR->Write();
    mesonAccXEffxBRTrue->Write();
    hpTRecTrue->Write();
    hpTGenTrueRebinned->Write();
    hpTGenTrue->Write();
    hpTRec->Write();
    hpTGenRebinned->Write();
    h1yieldCorr->Write();
    hCrossSection->Write();
    inFileSignalExtraction->Close();

    TFile* inFileSignalExtraction_TrueMC = TFile::Open(Form("%s", outputSignalExtractionTrueMC), "UPDATE");
    inFileSignalExtraction_TrueMC->cd();
    mesonAccXEffxBRTrue->Write();
    inFileSignalExtraction_TrueMC->Close();

    TFile* inFileSignalExtraction_MC = TFile::Open(Form("%s", outputSignalExtractionMC), "UPDATE");
    inFileSignalExtraction_MC->cd();
    mesonAccXEffxBR->Write();
    h1yieldCorr->Write();

}


