#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>
#include "includes/inputData.h"


TH2D* makeRatio(TH2* h1, TH2* h2);
TH2D* RebinTH2WithMatchingRange(TH2* h1, TH2* h2);
TH2D* RebinTH2WithAnotherBinning(TH2D* h1, TH2D* h2);
TH2D* Compute2DRatio(TH2D* h1, TH2D* h2); 


void efficiencyStudies(){
  //*******************************************************************************************************///
    ///Load 



    const char* inFileName_Pi0_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root", period, runNumberData));
    const char* inFileName_Pi0_MC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_MC_%d.root", period, runNumberMC));
    const char* inFileName_Pi0_TrueMC =  strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_MC_%d.root", period, runNumberMC));
    const char* inFileName_Pi0_DATA_Afterburner = "/Users/yelmard/myAnalysis/AnalysisSoftware/00010113_0dm00009f9730000dge0404000_0152103500000000/13.6TeV/SignalExtraction_Pi0_DATA_Afterburner_220661.root";

    const char* inFileName_Eta_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_Data_%d.root", period, runNumberData));
    const char* inFileName_Eta_MC =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_MC_%d.root", period, runNumberMC));
    const char* inFileName_Eta_TrueMC =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_TrueMC_%d.root", period, runNumberTrueMC));
    const char* inFileName_Eta_DATA_Afterburner = "/Users/yelmard/myAnalysis/AnalysisSoftware/00010113_0dm00009f9730000dge0404000_0152103500000000/13.6TeV/SignalExtraction_Eta_DATA_Afterburner_220661.root";


    char* outputSignalExtractionPi0Data = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root", period, runNumberData));
    char* outputSignalExtractionPi0MC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_MC_%d.root", period, runNumberMC));
    char* outputSignalExtractionPi0TrueMC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_TrueMC_%d.root", period, runNumberTrueMC));

    char* outputSignalExtractionEtaData = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_Data_%d.root", period, runNumberData));
    char* outputSignalExtractionEtaMC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_MC_%d.root", period, runNumberMC));
    char* outputSignalExtractionEtaTrueMC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_TrueMC_%d.root", period, runNumberTrueMC));




        ///*******************************************************************************************************///
    ///Efficiencies pi0 and eta, run 2 and run 3

    ///Efficiencies Run 2
    TH1D* effPi0Run2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/EfficiencyPi0_INT7"));
    SetHistogramPropertiesAlt(effPi0Run2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
    effPi0Run2->SetMarkerStyle(5);
    TH1D* effEtaRun2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/EfficiencyEta_INT7"));
    SetHistogramPropertiesAlt(effEtaRun2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
    effEtaRun2->SetMarkerStyle(5);

     ///Efficiencies Run 3
    TH1D* mesonAccXEffxBRTrueEta = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", inFileName_Eta_DATA));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTrueEta, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
    //mesonAccXEffxBRTrueEta->Scale( 0.3941);

    TH1D* mesonAccXEffxBREta = (TH1D*)extractHistogram(Form("%s:/mesonAccXEff_Eta", inFileName_Eta_DATA));
    SetHistogramPropertiesAlt(mesonAccXEffxBREta, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
    mesonAccXEffxBREta->SetMarkerStyle(4);

    TH1D* mesonAccXEffxBRTruePi0 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", inFileName_Pi0_DATA));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);

    TH1D* mesonAccXEffxBRPi0 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEff_Pi0", inFileName_Pi0_DATA));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
    mesonAccXEffxBRPi0->SetMarkerStyle(4);

    TLegend* legendEfficiencies = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEfficiencies->AddEntry((TObject*)0x0,"pp collisions", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTruePi0, "#pi^{0} Run3, true MC", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRPi0, "#pi^{0} Run3", "lp");
    legendEfficiencies->AddEntry(effPi0Run2, "#pi^{0} Run2", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTrueEta, "#eta Run3, true MC", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBREta, "#eta Run3", "lp");
    legendEfficiencies->AddEntry(effEtaRun2, "#eta Run2", "lp");

    effPi0Run2->GetXaxis()->SetRangeUser(0.4, 10.);


    TObjArray* arrEfficiencies = new TObjArray();
    arrEfficiencies->Add(effPi0Run2);
    arrEfficiencies->Add(effEtaRun2);
    arrEfficiencies->Add(mesonAccXEffxBRTruePi0);
    arrEfficiencies->Add(mesonAccXEffxBRPi0);
    arrEfficiencies->Add(mesonAccXEffxBRTrueEta);
    arrEfficiencies->Add(mesonAccXEffxBREta);
    arrEfficiencies->Add(legendEfficiencies);
    TCanvas* canMesonEfficiencies = (TCanvas*)makeCanvas(arrEfficiencies, 0, "CMYK|NoTime|LogX|LogY", 0, 0);
    canMesonEfficiencies->SaveAs("Plots/mesonEfficiencies.png");

    ///************************************************************s*******************************************///
    ///Single photon efficiencies

    //Run3

    //pt dependence
    Double_t fBinsPi0Pt[] = {0.40, 0.60, 0.80, 0.90, 1.00, 1.20, 1.40, 1.60, 1.80, 2.0, 2.50, 3.00, 3.50, 4.00,  4.50, 5.00, 5.5, 6.00, 7.00, 8.00, 10.00};
    Int_t nBinsPi0Pt = sizeof(*fBinsPi0Pt);
    TH1D* genPrimGammasPtUnrebinned;
    TH1D* convPrimGammasPtUnrebinned;
    TH1D* recGammasUnrebinned;
    TH2D* hrecPhotonRZ;
    TH2D* hgenPhotonRZUnRebinned;

    if(strcmp(period, "LHC22oPass6") == 0) {
        genPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/hPt_Photon", period, runNumberMC));
        if(!genPrimGammasPtUnrebinned) std::cout<<"WARNING: genPrimGammasPtUnrebinned is null pointer\n";

        convPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/hPt_ConvertedPhoton", period, runNumberMC));
        if(!convPrimGammasPtUnrebinned) std::cout<<"WARNING: convPrimGammasPtUnrebinned is null pointer\n";

        recGammasUnrebinned = (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/qc/hPt", period, runNumberMC));
        if(!recGammasUnrebinned) std::cout<<"WARNING: recGammasUnrebinned is null pointer\n";

        hrecPhotonRZ = (TH2D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/qc/hRZ_Photon_Primary_MC", period, runNumberMC));
        if(!hrecPhotonRZ) std::cout<<"WARNING: hrecPhotonRZ is null pointer\n";

        hgenPhotonRZUnRebinned = (TH2D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/qc/hPhotonRZ", period, runNumberMC));
        if(!hgenPhotonRZUnRebinned) std::cout<<"WARNING: hgenPhotonRZUnRebinned is null pointer\n";
    }
    else if(strcmp(period, "LHC22oPass7") == 0) {
        genPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/hPt", period, runNumberMC));
        if(!genPrimGammasPtUnrebinned) std::cout<<"WARNING: genPrimGammasPtUnrebinned is null pointer\n";

        convPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/hPt_ConvertedPhoton", period, runNumberMC));
        if(!convPrimGammasPtUnrebinned) std::cout<<"WARNING: convPrimGammasPtUnrebinned is null pointer\n";

        recGammasUnrebinned = (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hPt", period, runNumberMC));
        if(!recGammasUnrebinned) std::cout<<"WARNING: recGammasUnrebinned is null pointer\n";


        hrecPhotonRZ = (TH2D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hRZ_Photon_MC", period, runNumberMC));
        if(!hrecPhotonRZ) std::cout<<"WARNING: hrecPhotonRZ is null pointer\n";

        hgenPhotonRZUnRebinned = (TH2D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/qc/hPhotonRZ", period, runNumberMC));
        if(!hgenPhotonRZUnRebinned) std::cout<<"WARNING: hgenPhotonRZUnRebinned is null pointer\n";

    }
    else if(strcmp(period, "LHC22oPass4_lowIR") == 0) {
        genPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/hPt", period, runNumberMC));
        if(!genPrimGammasPtUnrebinned) std::cout<<"WARNING: genPrimGammasPtUnrebinned is null pointer\n";

        convPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/hPt_ConvertedPhoton", period, runNumberMC));
        if(!convPrimGammasPtUnrebinned) std::cout<<"WARNING: convPrimGammasPtUnrebinned is null pointer\n";

        recGammasUnrebinned = (TH1D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hPt", period, runNumberMC));
        if(!recGammasUnrebinned) std::cout<<"WARNING: recGammasUnrebinned is null pointer\n";

        hrecPhotonRZ = (TH2D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hRZ_Photon_MC", period, runNumberMC));
        if(!hrecPhotonRZ) std::cout<<"WARNING: hrecPhotonRZ is null pointer\n";

        hgenPhotonRZUnRebinned = (TH2D*)extractHistogram(Form("../signalExtraction/inputFiles/%s/AnalysisResults_MC_%d.root:/pcm-qc-mc/Generated/hPhotonRZ", period, runNumberMC));
        if(!hgenPhotonRZUnRebinned) std::cout<<"WARNING: hgenPhotonRZUnRebinned is null pointer\n";
    }

    TH1D *genPrimGammasPt = RebinHistogram(genPrimGammasPtUnrebinned,nBinsPi0Pt, fBinsPi0Pt);
    genPrimGammasPt->SetName("hpTGenTrueRebinned");

    TH1D *convPrimGammasPt = RebinHistogram(convPrimGammasPtUnrebinned,nBinsPi0Pt, fBinsPi0Pt);
    convPrimGammasPt->SetName("hpTConvTrueRebinned");

    TH1D *recGammas = RebinHistogram(recGammasUnrebinned,nBinsPi0Pt, fBinsPi0Pt);
    recGammas->SetName("hpTRecTrueRebinned");



    TH1D* gammaConvProbabilityPt = (TH1D*)convPrimGammasPt->Clone("gammaConvProbabilityPt");
    gammaConvProbabilityPt->Divide(convPrimGammasPt, genPrimGammasPt, 1., 1., "b");
    SetHistogramProperties(gammaConvProbabilityPt, "efficiency", 0);
    gammaConvProbabilityPt->GetYaxis()->SetRangeUser(0.001, 0.2);

    TH1D* gammaRecEfficiencyPt = (TH1D*)recGammas->Clone("gammaRecEfficiencyPt");
    gammaRecEfficiencyPt->Divide(recGammas, genPrimGammasPt, 1., 1., "b");
    SetHistogramProperties(gammaRecEfficiencyPt, "efficiency", 1);
    gammaRecEfficiencyPt->GetYaxis()->SetRangeUser(0.001, 0.2);

    TH1D* gammaRecEfficiencyPtRun2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/SinglePhotonEff.root:/Photon_AccTimesEff");
    SetHistogramProperties(gammaRecEfficiencyPtRun2, "efficiency", 1);
    gammaRecEfficiencyPtRun2->SetMarkerStyle(4);
    gammaRecEfficiencyPtRun2->GetYaxis()->SetRangeUser(0.001, 0.2);

    
    TLegend* legendSinglePhotonEfficiencies = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendSinglePhotonEfficiencies->AddEntry((TObject*)0x0,"pp collisions", "");
    legendSinglePhotonEfficiencies->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendSinglePhotonEfficiencies->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
    legendSinglePhotonEfficiencies->AddEntry(gammaConvProbabilityPt, "conversion probability", "lp");
    legendSinglePhotonEfficiencies->AddEntry(gammaRecEfficiencyPt, "gamma rec. eff. Run 3", "lp");
    legendSinglePhotonEfficiencies->AddEntry(gammaRecEfficiencyPtRun2, "gamma rec. eff. Run 2", "lp");



    TObjArray* arrSinglePhotonEfficiencies = new TObjArray();
    arrSinglePhotonEfficiencies->Add(gammaConvProbabilityPt);
    arrSinglePhotonEfficiencies->Add(gammaRecEfficiencyPt);
    arrSinglePhotonEfficiencies->Add(gammaRecEfficiencyPtRun2);
    arrSinglePhotonEfficiencies->Add(legendSinglePhotonEfficiencies);
    TCanvas* canSinglePhotonEfficiencies = (TCanvas*)makeCanvas(arrSinglePhotonEfficiencies, 0, "CMYK|NoTime|LogX|LogY", 0, 0);
    canSinglePhotonEfficiencies->SaveAs("Plots/singlePhotonEfficiencies.png");


    ///radius dependence
    cout << "test, line: " << __LINE__ << endl;
    TH1D* hPhotonOnR = (TH1D*)hrecPhotonRZ->ProjectionY("hPhotonOnR", hrecPhotonRZ->GetXaxis()->FindBin(-100.), hrecPhotonRZ->GetXaxis()->FindBin(100.), "e");
    cout << "test, line: " << __LINE__ << endl;
    SetHistogramProperties(hPhotonOnR, "efficiency", 14);

    TH2D* hgenPhotonRZ = RebinTH2WithMatchingRange(hgenPhotonRZUnRebinned, hrecPhotonRZ);
    TH1D* hgenPhotonOnR = (TH1D*)hgenPhotonRZ->ProjectionY("hgenPhotonOnR", hgenPhotonRZ->GetXaxis()->FindBin(-100.), hgenPhotonRZ->GetXaxis()->FindBin(100.), "e");
    SetHistogramProperties(hgenPhotonOnR, "efficiency", 14);
    cout << "test, line: " << __LINE__ << endl;


    TH1D* recPhotonEfficiencyOnR = (TH1D*)hPhotonOnR->Clone("recPhotonEfficiencyOnR");
    recPhotonEfficiencyOnR->Divide(hPhotonOnR, hgenPhotonOnR, 1., 1., "b");
    SetHistogramProperties(recPhotonEfficiencyOnR, "efficiency", 14);
    recPhotonEfficiencyOnR->GetXaxis()->SetTitle("#it{R} (cm)");

    TObjArray* arrSinglePhotonEfficienciesOnR = new TObjArray();
    arrSinglePhotonEfficienciesOnR->Add(recPhotonEfficiencyOnR);
    TCanvas* canSinglePhotonEfficienciesOnR = (TCanvas*)makeCanvas(arrSinglePhotonEfficienciesOnR, 0, "CMYK|NoTime|", 0, 0);
    canSinglePhotonEfficienciesOnR->SaveAs("Plots/singlePhotonEfficiencyOnR.png");

    cout << "test, line: " << __LINE__ << endl;

    //2Dim
    TH2D *h2DrecPhotonEfficiencyOnR = makeRatio(hrecPhotonRZ, hgenPhotonRZUnRebinned);
    h2DrecPhotonEfficiencyOnR->SetName("h2DrecPhotonEfficiencyOnR");
    h2DrecPhotonEfficiencyOnR->GetZaxis()->SetRangeUser(1e-2, 1.);
    h2DrecPhotonEfficiencyOnR->SetStats(0);

    TCanvas *canSinglePhotonEffOnR2D = new TCanvas("canSinglePhotonEffOnR2D", "", 800, 800);
    canSinglePhotonEffOnR2D->SetLogz();
    h2DrecPhotonEfficiencyOnR->Draw("COLZ");
    //canSinglePhotonEffOnR2D->SaveAs("Plots/singlePhotonEfficiencyOnR2D.png");
        cout << "test, line: " << __LINE__ << endl;

}


TH2D* Compute2DRatio(TH2D* hRec, TH2D* hGen){
    if (!hRec || !hGen) {
        std::cerr << "Error: Null histogram provided!" << std::endl;
        return nullptr;
    }
    // hRec->GetXaxis()->SetRangeUser(-100., 100.);
    // hGen->GetXaxis()->SetRangeUser(-100., 100.);
    // hRec->GetYaxis()->SetRangeUser(0., 100.);
    // hGen->GetYaxis()->SetRangeUser(0., 100.);

    TH2D* hRatio = (TH2D*)hGen->Clone("hRatio");
    hRatio->Reset();
    hRatio->Divide(hRec, hGen, 1., 1., "B");
    return hRatio;
}


TH2D* RebinTH2WithMatchingRange(TH2* h1, TH2* h2) {
    // Get the binning information and ranges from h2
    int nXbins = h2->GetNbinsX();
    int nYbins = h2->GetNbinsY();
    double xMin = h2->GetXaxis()->GetXmin();
    double xMax = h2->GetXaxis()->GetXmax();
    double yMin = h2->GetYaxis()->GetXmin();
    double yMax = h2->GetYaxis()->GetXmax();

    // Define new bin edges based on h2's axes
    std::vector<double> xBins(nXbins + 1);
    std::vector<double> yBins(nYbins + 1);
    for (int i = 0; i <= nXbins; ++i) xBins[i] = h2->GetXaxis()->GetBinLowEdge(i + 1);
    for (int j = 0; j <= nYbins; ++j) yBins[j] = h2->GetYaxis()->GetBinLowEdge(j + 1);

    // Create a new histogram with the same binning and range as h2
    TH2D* rebinnedHist = new TH2D("rebinnedHist", h1->GetTitle(),
                                  nXbins, &xBins[0],
                                  nYbins, &yBins[0]);
    rebinnedHist->GetXaxis()->SetRangeUser(xMin, xMax);
    rebinnedHist->GetYaxis()->SetRangeUser(yMin, yMax);

    // Fill the new histogram with data from h1, within the overlapping range
    for (int x = 1; x <= h1->GetNbinsX(); ++x) {
        for (int y = 1; y <= h1->GetNbinsY(); ++y) {
            double xCenter = h1->GetXaxis()->GetBinCenter(x);
            double yCenter = h1->GetYaxis()->GetBinCenter(y);

            // Check if bin center falls within the range of the new histogram
            if (xCenter >= xMin && xCenter <= xMax && yCenter >= yMin && yCenter <= yMax) {
                double content = h1->GetBinContent(x, y);
                double error = h1->GetBinError(x, y);

                // Find the corresponding bin in rebinnedHist
                int newBinX = rebinnedHist->GetXaxis()->FindBin(xCenter);
                int newBinY = rebinnedHist->GetYaxis()->FindBin(yCenter);

                // Add content and error to the new histogram bin
                rebinnedHist->AddBinContent(rebinnedHist->GetBin(newBinX, newBinY), content);
                double combinedError = sqrt(pow(rebinnedHist->GetBinError(newBinX, newBinY), 2) + pow(error, 2));
                rebinnedHist->SetBinError(newBinX, newBinY, combinedError);
            }
        }
    }

    return rebinnedHist;
    
}


TH2D* RebinTH2WithAnotherBinning(TH2D* h1, TH2D* h2) {
    // Get the binning information from h2
    int nXbins = h2->GetNbinsX();
    int nYbins = h2->GetNbinsY();

    // Create arrays for new bin edges based on h2's axes
    std::vector<double> xBins(nXbins + 1);
    std::vector<double> yBins(nYbins + 1);

    // Fill the bin edges arrays for X and Y from h2
    for (int i = 0; i <= nXbins; ++i) {
        xBins[i] = h2->GetXaxis()->GetBinLowEdge(i + 1);
    }
    for (int j = 0; j <= nYbins; ++j) {
        yBins[j] = h2->GetYaxis()->GetBinLowEdge(j + 1);
    }

    // Create a new histogram with the same binning as h2
    TH2D* rebinnedHist = new TH2D("rebinnedHist", h1->GetTitle(),
                                  nXbins, &xBins[0],
                                  nYbins, &yBins[0]);

    // Fill the new histogram with data from h1
    for (int x = 1; x <= h1->GetNbinsX(); ++x) {
        for (int y = 1; y <= h1->GetNbinsY(); ++y) {
            double content = h1->GetBinContent(x, y);
            double error = h1->GetBinError(x, y);
            double xCenter = h1->GetXaxis()->GetBinCenter(x);
            double yCenter = h1->GetYaxis()->GetBinCenter(y);

            // Find the corresponding bin in rebinnedHist
            int newBinX = rebinnedHist->GetXaxis()->FindBin(xCenter);
            int newBinY = rebinnedHist->GetYaxis()->FindBin(yCenter);

            // Add the content and error to the corresponding bin in rebinnedHist
            rebinnedHist->AddBinContent(rebinnedHist->GetBin(newBinX, newBinY), content);
            double combinedError = sqrt(pow(rebinnedHist->GetBinError(newBinX, newBinY), 2) + pow(error, 2));
            rebinnedHist->SetBinError(newBinX, newBinY, combinedError);
        }
    }
    
    return rebinnedHist;
    
}



TH2D* makeRatio(TH2* h1, TH2* h2) {
    // Get binning information from h1
    int nXbins = h1->GetNbinsX();
    int nYbins = h1->GetNbinsY();
    std::vector<double> xBins(nXbins + 1);
    std::vector<double> yBins(nYbins + 1);

    // Fill bin edges for X and Y from h1
    for (int i = 0; i <= nXbins; ++i) xBins[i] = h1->GetXaxis()->GetBinLowEdge(i + 1);
    for (int j = 0; j <= nYbins; ++j) yBins[j] = h1->GetYaxis()->GetBinLowEdge(j + 1);

    // Create a new histogram with the same binning as h1 for the rebinned h2
    TH2D* rebinnedH2 = new TH2D("rebinnedH2", h2->GetTitle(),
                                nXbins, &xBins[0],
                                nYbins, &yBins[0]);

    // Loop over all bins in h2 and transfer content to rebinnedH2 in h1's binning
    for (int x = 1; x <= h2->GetNbinsX(); ++x) {
        for (int y = 1; y <= h2->GetNbinsY(); ++y) {
            // Bin center of current bin in h2
            double xCenter = h2->GetXaxis()->GetBinCenter(x);
            double yCenter = h2->GetYaxis()->GetBinCenter(y);

            // Find corresponding bin in rebinnedH2 (matching h1's binning)
            int newBinX = rebinnedH2->GetXaxis()->FindBin(xCenter);
            int newBinY = rebinnedH2->GetYaxis()->FindBin(yCenter);

            // Add content and error from h2 bin to the matching rebinnedH2 bin
            double content = h2->GetBinContent(x, y);
            double error = h2->GetBinError(x, y);

            rebinnedH2->AddBinContent(rebinnedH2->GetBin(newBinX, newBinY), content);
            double combinedError = sqrt(pow(rebinnedH2->GetBinError(newBinX, newBinY), 2) + pow(error, 2));
            rebinnedH2->SetBinError(newBinX, newBinY, combinedError);
        }
    }

    // Now we have rebinned h2 (rebinnedH2) matching h1, we can compute the ratio
    TH2D* ratioHist = (TH2D*)h1->Clone("ratioHist");
    ratioHist->SetTitle("Ratio Histogram");
    ratioHist->Reset();  // Clear content and errors

    // Calculate the ratio bin-by-bin
    for (int x = 1; x <= nXbins; ++x) {
        for (int y = 1; y <= nYbins; ++y) {
            double h1Value = h1->GetBinContent(x, y);
            double h2Value = rebinnedH2->GetBinContent(x, y);

            if (h2Value != 0) {
                double ratio = h1Value / h2Value;
                ratioHist->SetBinContent(x, y, ratio);

                // Error propagation for the ratio
                double h1Error = h1->GetBinError(x, y);
                double h2Error = rebinnedH2->GetBinError(x, y);
                double ratioError = ratio * sqrt(pow(h1Error / h1Value, 2) + pow(h2Error / h2Value, 2));
                ratioHist->SetBinError(x, y, ratioError);
            } else {
                // Set zero if the denominator is zero
                ratioHist->SetBinContent(x, y, 0);
                ratioHist->SetBinError(x, y, 0);
            }
        }
    }

    // Clean up temporary histogram
    delete rebinnedH2;

    // Return the ratio histogram
    return ratioHist;
}
