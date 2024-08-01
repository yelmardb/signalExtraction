#include "includes/Plotting.h"
#include "includes/extractHistogram.h"


TH1D* rebinHisto(TH1D* h1, TH1D* h2) {
    // Extract the bin edges from h1
    int nBins1 = h1->GetNbinsX();
    std::vector<double> binEdges1(nBins1 + 1);
    for (int i = 0; i <= nBins1; ++i) {
        binEdges1[i] = h1->GetBinLowEdge(i + 1);
    }

    // Rebin h2 to match the binning of h1
    TH1D* h2Rebinned = (TH1D*)h2->Rebin(nBins1, "h2Rebinned", &binEdges1[0]);
    return h2Rebinned;
}

void efficiency(){

    Double_t etaRange = 1.8;

    ///************************************************************************************************************************************************ */

    TFile* inFileSignalExtractionPi0 = TFile::Open("outputSignalExtraction/SignalExtraction_Pi0_DATA_220661.root", "UPDATE");

    TH1D* hpTRecPi0 = (TH1D*)extractHistogram("outputSignalExtraction/SignalExtraction_Pi0_MC_223972.root:/h1yield");
    TH1D* hpTAccPi0 = (TH1D*)extractHistogram("inputFiles/AnalysisResults_MC_214158.root:/pi0eta-to-gammagamma-mc/Generated/PCMPCM/hPt_Pi0_Acc");
    TH1D* hpTGenPi0 = (TH1D*)extractHistogram("inputFiles/AnalysisResults_MC_214158.root:/pi0eta-to-gammagamma-mc/Generated/PCMPCM/hPt_Pi0");
    TH1D* hCollisionCounterPi0 = (TH1D*)extractHistogram("inputFiles/AnalysisResults_MC_214158.root:/pi0eta-to-gammagamma-mc/Generated/PCMPCM/hCollisionCounter"); 
    Double_t nEvents = hCollisionCounterPi0->GetBinContent(hCollisionCounterPi0->FindBin(3.));

    hpTAccPi0->Scale(1/nEvents); 
    hpTAccPi0->Scale(1., "width");   
    hpTGenPi0->Scale(1/nEvents); 
    hpTGenPi0->Scale(1., "width");

 
    TH1D* hpTAccPi0Rebinned = rebinHisto(hpTRecPi0, hpTAccPi0);
    TH1D* hpTGenPi0Rebinned = rebinHisto(hpTRecPi0, hpTGenPi0);


    // Rebin histograms
    TH1D* mesonEffPi0 = (TH1D*)hpTRecPi0->Clone("mesonEffPi0");
    mesonEffPi0 = MakeRatioSpectra(hpTRecPi0, hpTAccPi0, "quiet");
    mesonEffPi0->SetName("mesonEffPi0");

    TH1D* mesonAccPi0 = (TH1D*)hpTAccPi0->Clone("mesonAccPi0");
    mesonAccPi0 = MakeRatioSpectra(hpTAccPi0Rebinned, hpTGenPi0Rebinned , "quiet");
    mesonAccPi0->SetName("mesonAccPi0");

    TH1D* mesonAccXEffPi0 = (TH1D*)hpTRecPi0->Clone("mesonAccXEffPi0");
    mesonAccXEffPi0 = MakeRatioSpectra(hpTRecPi0, hpTGenPi0, "quiet");
    mesonAccXEffPi0->SetName("mesonAccXEffPi0");
    mesonAccXEffPi0->Scale(0.988131);

    SetHistogramPropertiesAlt(mesonEffPi0, "#it{p}_{T} GeV/#it{c}", "efficiency", 1);
    SetHistogramPropertiesAlt(mesonAccPi0, "#it{p}_{T} GeV/#it{c}", "acceptance", 1);
    SetHistogramPropertiesAlt(mesonAccXEffPi0, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);

    TH1D* h1yieldPi0 = (TH1D*)extractHistogram("outputSignalExtraction/SignalExtraction_Pi0_DATA_220661.root:/h1yield");
    TH1D* h1yieldPi0Rebinned = rebinHisto(mesonAccXEffPi0, h1yieldPi0);
    TH1D* h1yieldCorrPi0 = (TH1D*)h1yieldPi0Rebinned->Clone("h1yieldCorrPi0");
    h1yieldCorrPi0->Reset();
    h1yieldCorrPi0->SetName("h1yieldCorr");

    for(int i = 0; i <= h1yieldPi0Rebinned->GetNbinsX(); i++){
        Double_t pT = h1yieldCorrPi0->GetBinLowEdge(i);
        h1yieldCorrPi0->SetBinContent(i, h1yieldPi0Rebinned->GetBinContent(i)/mesonAccXEffPi0->GetBinContent(i));
        h1yieldCorrPi0->SetBinError(i, h1yieldPi0Rebinned->GetBinError(i)/mesonAccXEffPi0->GetBinError(i));
    }
    TH1D* hCrossSectionPi0 = (TH1D*)h1yieldCorrPi0->Clone("hCrossSection");
   
    h1yieldCorrPi0->Scale(1., "width");
    SetHistogramPropertiesAlt(h1yieldCorrPi0, "#it{p}_{T} GeV/#it{c}", "#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}", 1);

    hCrossSectionPi0->SetName("hCrossSection");
    hCrossSectionPi0->Scale(59.4*1e+9);
    FinalizePt(hCrossSectionPi0, 1.0, etaRange);
    SetHistogramPropertiesAlt(hCrossSectionPi0, "#it{p}_{T} GeV/#it{c}", "#it{E} #frac{d^{3}#it{#sigma}}{d#it{p}^{3}} (pb GeV^{#minus2} #it{c}^{3})", 1);

    inFileSignalExtractionPi0->cd();
    mesonEffPi0->Write("",TObject::kOverwrite);
    mesonAccPi0->Write("",TObject::kOverwrite);
    mesonAccXEffPi0->Write("",TObject::kOverwrite);
    h1yieldCorrPi0->Write("",TObject::kOverwrite);
    hCrossSectionPi0->Write("",TObject::kOverwrite);

    inFileSignalExtractionPi0->Close();

    TObjArray* arr = new TObjArray();
    arr->Add(h1yieldCorrPi0);    
    //TCanvas* can = (TCanvas*)makeCanvas(arr, 0, "LogX LogY CMYK|NoTime", 0, 0);

    TObjArray* arr2 = new TObjArray();
    arr2->Add(mesonAccXEffPi0);    
    TCanvas* can2 = (TCanvas*)makeCanvas(arr2, 0, "LogX LogY CMYK|NoTime", 0, 0);
    

    ///************************************************************************************************************************************************ */
    ///EFFICIENCIES FOR ETA

    }