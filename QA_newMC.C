#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "includes/inputData.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>

void SetMeanHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetSigmaHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
TH1D* GetTrueMC_InvMass(const char* inFileDataName, Int_t color, Int_t markerStyle, const char* meson, Int_t nEvents);
void ComparePtSpectra(TH1D* hPt_LHC24f3b2, TH1D* hPt_LHC24f3c, const char* legendEntry);
void ComparePtSpectraRunWise(TH1D* hPt_LHC24f3b2, TH1D* hPt_LHC24f3c, TLegend* legend);
TH1D* GetTrueMC_PtReso(const char* inFileDataName, Int_t color, Int_t markerStyle);


void QA_newMC(){

  // 333270, pass7 LHC24f3c, 10% of statistics
  // 333566, pass7 LHC24f3b, 10% of statistics
   // 337722, pass7 LHC24f3b2, 10% of statistics


    //*******************************************************************************************************///
    ///Load 
    const char* inFileName_Pi0_Corrected_wLHC24f3c = "Pi0_Corrected_wLHC24f3c.root";
    const char* inFileName_Pi0_Corrected_wLHC24f3b2 = "Pi0_Corrected_wLHC24f3b2.root";

    const char* inFileName_Eta_Corrected_wLHC24f3c = "Eta_Corrected_wLHC24f3c.root";
    const char* inFileName_Eta_Corrected_wLHC24f3b2 = "Eta_Corrected_wLHC24f3b2.root";

    const char* inFileName_Pi0_TrueMC_wLHC24f3b2 = "outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_MC_337722.root";
    const char* inFileName_Pi0_TrueMC_wLHC24f3c = "outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_MC_333270.root";

    const char* inFileName_Eta_TrueMC_wLHC24f3b2 = "outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_MC_337722.root";
    const char* inFileName_Eta_TrueMC_wLHC24f3c = "outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_MC_333270.root";

    const char* trainOutput_LHC24f3b2 = "inputFiles/LHC22oPass7/AnalysisResults_MC_337722.root";
    const char* trainOutput_LHC24f3c = "inputFiles/LHC22oPass7/AnalysisResults_MC_333270.root";


    ///*******************************************cross************************************************************///
 
    ///Events
    TH1D* hCollisionCounter_LHC24f3b2 = (TH1D*)extractHistogram("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_TrueMC_337722.root:/hCollisionCounter");
    SetHistogramPropertiesAlt(hCollisionCounter_LHC24f3b2, "", "events", 0);
    hCollisionCounter_LHC24f3b2->GetYaxis()->SetRangeUser(1e5, 1e10);
    hCollisionCounter_LHC24f3b2->SetLineWidth(2);

    TH1D* hCollisionCounter_LHC24f3c = (TH1D*)extractHistogram(" outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_TrueMC_333270.root:/hCollisionCounter");
    SetHistogramPropertiesAlt(hCollisionCounter_LHC24f3c, "", "", 1);
    hCollisionCounter_LHC24f3c->GetYaxis()->SetRangeUser(1e5, 1e10);
    hCollisionCounter_LHC24f3c->SetLineWidth(2);

    ///Plotting
    TLegend* legendEvents = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEvents->AddEntry((TObject*)0x0,"pp collisions", "");
    legendEvents->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEvents->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
    legendEvents->AddEntry(hCollisionCounter_LHC24f3c, "LHC24f3c", "lp");
    legendEvents->AddEntry(hCollisionCounter_LHC24f3b2, "LHC24f3b2", "lp");

      
    TObjArray* arrEvents = new TObjArray();
    arrEvents->Add(hCollisionCounter_LHC24f3b2);
    arrEvents->Add(hCollisionCounter_LHC24f3c);
    arrEvents->Add(legendEvents);
    TCanvas* canEvents = (TCanvas*)makeCanvas(arrEvents, 0, "CMYK|NoTime|LogY|", 0, 0);


    ///Efficiencies 

    TH1D* mesonAccXEffxBRTruePi0_wLHC24f3c = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", inFileName_Pi0_Corrected_wLHC24f3c));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0_wLHC24f3c, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
    
    TH1D* mesonAccXEffxBRTrueEta_wLHC24f3c = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", inFileName_Eta_Corrected_wLHC24f3c));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTrueEta_wLHC24f3c, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);


    TH1D* mesonAccXEffxBRTruePi0_wLHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", inFileName_Pi0_Corrected_wLHC24f3b2));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0_wLHC24f3b2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
    mesonAccXEffxBRTruePi0_wLHC24f3b2->SetMarkerStyle(4);

    TH1D* mesonAccXEffxBRTrueEta_wLHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", inFileName_Eta_Corrected_wLHC24f3b2));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTrueEta_wLHC24f3b2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
    mesonAccXEffxBRTrueEta_wLHC24f3b2->SetMarkerStyle(4);
    
    TH1D* effRatios_Pi0 = (TH1D*)mesonAccXEffxBRTruePi0_wLHC24f3c->Clone("effRatios_Pi0");
    effRatios_Pi0->Divide(mesonAccXEffxBRTruePi0_wLHC24f3b2);
    effRatios_Pi0->GetYaxis()->SetTitle("LHC24f3c / LHC24f3b2");

    TH1D* effRatios_Eta = (TH1D*)mesonAccXEffxBRTrueEta_wLHC24f3c->Clone("effRatios_Eta");
    effRatios_Eta->Divide(mesonAccXEffxBRTrueEta_wLHC24f3b2);
    effRatios_Eta->GetYaxis()->SetTitle("LHC24f3c / LHC24f3b2");


    ///Plotting
    TLegend* legendEfficiencies = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEfficiencies->AddEntry((TObject*)0x0,"pp collisions", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTruePi0_wLHC24f3c, "#pi^{0}, LHC24f3c", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTruePi0_wLHC24f3b2, "#pi^{0}, LHC24f3b2", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTrueEta_wLHC24f3c, "#eta, LHC24f3c", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTrueEta_wLHC24f3b2, "#eta, LHC24f3b2", "lp");

      
    TObjArray* arrEfficienciesPi0 = new TObjArray();
    arrEfficienciesPi0->Add(mesonAccXEffxBRTruePi0_wLHC24f3c);
    arrEfficienciesPi0->Add(mesonAccXEffxBRTruePi0_wLHC24f3b2);
    arrEfficienciesPi0->Add(mesonAccXEffxBRTrueEta_wLHC24f3c);
    arrEfficienciesPi0->Add(mesonAccXEffxBRTrueEta_wLHC24f3b2);

    TObjArray* arrEffRatiosPi0 = new TObjArray();
    arrEffRatiosPi0->Add(effRatios_Pi0);
    arrEffRatiosPi0->Add(effRatios_Eta);

    arrEfficienciesPi0->Add(legendEfficiencies);
    TCanvas* canEffPi0 = (TCanvas*)makeCanvas(arrEfficienciesPi0, arrEffRatiosPi0, "CMYK|NoTime|LogX|LogY", 0, 0);



    ///Means

    TH1D* h1mean_Pi0_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/h1mean",inFileName_Pi0_TrueMC_wLHC24f3b2));
    SetMeanHisto(h1mean_Pi0_TrueMC_LHC24f3b2, 0, 4);

    TH1D* h1mean_Pi0_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/h1mean",inFileName_Pi0_TrueMC_wLHC24f3c));
    SetMeanHisto(h1mean_Pi0_TrueMC_LHC24f3c, 0, 8);


    TH1D* h1mean_Eta_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/h1mean",inFileName_Eta_TrueMC_wLHC24f3b2));
    SetMeanHisto(h1mean_Eta_TrueMC_LHC24f3b2, 1, 4);

    TH1D* h1mean_Eta_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/h1mean",inFileName_Eta_TrueMC_wLHC24f3c));
    SetMeanHisto(h1mean_Eta_TrueMC_LHC24f3c, 1, 8);

    

    TLegend* legendMeansPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendMeansPi0->AddEntry((TObject*)0x0,"pp collisions", "");
    legendMeansPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendMeansPi0->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legendMeansPi0->AddEntry(h1mean_Pi0_TrueMC_LHC24f3c, "LHC24f3c", "lp");
    legendMeansPi0->AddEntry(h1mean_Pi0_TrueMC_LHC24f3b2, "LHC24f3b2", "lp");

    TObjArray* arrMeansPi0 = new TObjArray();
    arrMeansPi0->Add(h1mean_Pi0_TrueMC_LHC24f3c);
    arrMeansPi0->Add(h1mean_Pi0_TrueMC_LHC24f3b2);
    arrMeansPi0->Add(legendMeansPi0);
    //TCanvas* canMeansPi0 = (TCanvas*)makeCanvas(arrMeansPi0, 0, "CMYK|NoTime", 0, 0);

     TLegend* legendMeansEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendMeansEta->AddEntry((TObject*)0x0,"pp collisions", "");
    legendMeansEta->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendMeansEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendMeansEta->AddEntry(h1mean_Eta_TrueMC_LHC24f3c, "LHC24f3c", "lp");
    legendMeansEta->AddEntry(h1mean_Eta_TrueMC_LHC24f3b2, "LHC24f3b2", "lp");

    TObjArray* arrMeansEta = new TObjArray();
    arrMeansEta->Add(h1mean_Eta_TrueMC_LHC24f3c);
    arrMeansEta->Add(h1mean_Eta_TrueMC_LHC24f3b2);
    arrMeansEta->Add(legendMeansEta);
    //TCanvas* canMeansEta = (TCanvas*)makeCanvas(arrMeansEta, 0, "CMYK|NoTime", 0, 0);

    //Widths
    TH1D* h1sigma_Pi0_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Pi0_TrueMC_wLHC24f3b2));
    SetSigmaHisto(h1sigma_Pi0_TrueMC_LHC24f3b2, 0, 4);
    TH1D* h1sigma_Pi0_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Pi0_TrueMC_wLHC24f3c));
    SetSigmaHisto(h1sigma_Pi0_TrueMC_LHC24f3c, 0, 8);
    TH1D* h1sigma_Eta_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Eta_TrueMC_wLHC24f3b2));
    SetSigmaHisto(h1sigma_Eta_TrueMC_LHC24f3b2, 1, 4);
    TH1D* h1sigma_Eta_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Eta_TrueMC_wLHC24f3c));
    SetSigmaHisto(h1sigma_Eta_TrueMC_LHC24f3c, 1, 8);


    TLegend* legendSigmaPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendSigmaPi0->AddEntry((TObject*)0x0,"pp collisions", "");
    legendSigmaPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendSigmaPi0->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legendSigmaPi0->AddEntry(h1sigma_Pi0_TrueMC_LHC24f3c, "LHC24f3c", "lp");
    legendSigmaPi0->AddEntry(h1sigma_Pi0_TrueMC_LHC24f3b2, "LHC24f3b2", "lp");

    TObjArray* arrSigmaPi0 = new TObjArray();
    arrSigmaPi0->Add(h1sigma_Pi0_TrueMC_LHC24f3c);
    arrSigmaPi0->Add(h1sigma_Pi0_TrueMC_LHC24f3b2);
    arrSigmaPi0->Add(legendSigmaPi0);
    //TCanvas* canSigmaPi0 = (TCanvas*)makeCanvas(arrSigmaPi0, 0, "CMYK|NoTime", 0, 0);


    TLegend* legendSigmaEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendSigmaEta->AddEntry((TObject*)0x0,"pp collisions", "");
    legendSigmaEta->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendSigmaEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendSigmaEta->AddEntry(h1sigma_Eta_TrueMC_LHC24f3c, "LHC24f3c", "lp");
    legendSigmaEta->AddEntry(h1sigma_Eta_TrueMC_LHC24f3b2, "LHC24f3b2", "lp");

    TObjArray* arrSigmaEta = new TObjArray();
    arrSigmaEta->Add(h1sigma_Eta_TrueMC_LHC24f3c);
    arrSigmaEta->Add(h1sigma_Eta_TrueMC_LHC24f3b2);
    arrSigmaEta->Add(legendSigmaEta);
    //TCanvas* canSigmaEta = (TCanvas*)makeCanvas(arrSigmaEta, 0, "CMYK|NoTime", 0, 0);



    ///Generated spectra
    //TH1D* hCollisionCounter_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter",trainOutput_LHC24f3b2));
    Double_t nEvents_LHC24f3b2 = hCollisionCounter_LHC24f3b2->GetBinContent(12);

    //TH1D* hCollisionCounter_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter",trainOutput_LHC24f3c));
    Double_t nEvents_LHC24f3c = hCollisionCounter_LHC24f3c->GetBinContent(12);


    TH1D* hPtPi0_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Generated/Pi0/hPt",trainOutput_LHC24f3b2));
    hPtPi0_TrueMC_LHC24f3b2->Scale(1/nEvents_LHC24f3b2, "width");
    
    TH1D* hPtPi0_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Generated/Pi0/hPt",trainOutput_LHC24f3c));
    hPtPi0_TrueMC_LHC24f3c->Scale(1/nEvents_LHC24f3c, "width");
    
    TH1D* hPtEta_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Generated/Eta/hPt",trainOutput_LHC24f3b2));
    hPtEta_TrueMC_LHC24f3b2->Scale(1/nEvents_LHC24f3b2, "width");
    
    TH1D* hPtEta_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Generated/Eta/hPt",trainOutput_LHC24f3c));
    hPtEta_TrueMC_LHC24f3c->Scale(1/nEvents_LHC24f3c, "width");


    SetHistogramPropertiesAlt(hPtPi0_TrueMC_LHC24f3b2, "#it{p}_{T} (GeV/c)", "(1/#it{N}_{events}) dN/dp_{T} (GeV/c)^{-1}", 0);
    SetHistogramPropertiesAlt(hPtPi0_TrueMC_LHC24f3c, "#it{p}_{T} (GeV/c)", "(1/#it{N}_{events}) dN/dp_{T} (GeV/c)^{-1}", 0);
    hPtPi0_TrueMC_LHC24f3b2->SetMarkerStyle(4);

    SetHistogramPropertiesAlt(hPtEta_TrueMC_LHC24f3b2, "#it{p}_{T} (GeV/c)", "(1/#it{N}_{events}) dN/dp_{T} (GeV/c)^{-1}", 1);
    SetHistogramPropertiesAlt(hPtEta_TrueMC_LHC24f3c, "#it{p}_{T} (GeV/c)", "(1/#it{N}_{events}) dN/dp_{T} (GeV/c)^{-1}", 1);
    hPtEta_TrueMC_LHC24f3b2->SetMarkerStyle(4);

    hPtPi0_TrueMC_LHC24f3b2->GetXaxis()->SetRangeUser(0.4, 1e1);
    hPtPi0_TrueMC_LHC24f3c->GetXaxis()->SetRangeUser(0.4, 1e1);

    hPtEta_TrueMC_LHC24f3b2->GetXaxis()->SetRangeUser(0.4, 1e1);
    hPtEta_TrueMC_LHC24f3c->GetXaxis()->SetRangeUser(0.4, 1e1);

    TH1D* hRatioPtPi0 = (TH1D*)hPtPi0_TrueMC_LHC24f3c->Clone("hRatioPtPi0");
    hRatioPtPi0->Divide(hPtPi0_TrueMC_LHC24f3b2);
    hRatioPtPi0->GetYaxis()->SetTitle("LHC24f3c / LHC24f3b2");
    hRatioPtPi0->GetYaxis()->SetRangeUser(0.9, 1.1);

    TH1D* hRatioPtEta = (TH1D*)hPtEta_TrueMC_LHC24f3c->Clone("hRatioPtEta");
    hRatioPtEta->Divide(hPtEta_TrueMC_LHC24f3b2);
    hRatioPtEta->GetYaxis()->SetTitle("LHC24f3c / LHC24f3b2");
    hRatioPtEta->GetYaxis()->SetRangeUser(0.9, 1.1);

    TLegend* legendGen = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendGen->AddEntry((TObject*)0x0,"True MC pp collisions", "");
    legendGen->AddEntry((TObject*)0x0,"Generated spectra", "");
    legendGen->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
    legendGen->AddEntry(hPtPi0_TrueMC_LHC24f3c, "#pi^{0} LHC24f3c", "lp");
    legendGen->AddEntry(hPtPi0_TrueMC_LHC24f3b2, "#pi^{0} LHC24f3b2", "lp");
    legendGen->AddEntry(hPtEta_TrueMC_LHC24f3c, "#eta LHC24f3c", "lp");
    legendGen->AddEntry(hPtEta_TrueMC_LHC24f3b2, "#eta LHC24f3b2", "lp");

    TObjArray* arrGen = new TObjArray();
    arrGen->Add(hPtPi0_TrueMC_LHC24f3c);
    arrGen->Add(hPtPi0_TrueMC_LHC24f3b2);
    arrGen->Add(hPtEta_TrueMC_LHC24f3c);
    arrGen->Add(hPtEta_TrueMC_LHC24f3b2);
    arrGen->Add(legendGen);

    TObjArray* arrRatio = new TObjArray();
    arrRatio->Add(hRatioPtPi0);
    arrRatio->Add(hRatioPtEta);
    
    //TCanvas* canGen = (TCanvas*)makeCanvas(arrGen, arrRatio, "CMYK|NoTime LogY LogX", 0, 0);



    ///Generated photons
    TH1D* hPtGeneratedPhotons_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/Generated/hPt",trainOutput_LHC24f3b2));
    hPtGeneratedPhotons_TrueMC_LHC24f3b2->Scale(1./nEvents_LHC24f3b2, "width");
    TH1D* hPtGeneratedPhotons_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/Generated/hPt",trainOutput_LHC24f3c));
    hPtGeneratedPhotons_TrueMC_LHC24f3c->Scale(1./nEvents_LHC24f3c, "width");

    ComparePtSpectra(hPtGeneratedPhotons_TrueMC_LHC24f3b2, hPtGeneratedPhotons_TrueMC_LHC24f3c, "Generated photons");

    //Converted photons
    TH1D* hPtConvertedPhotons_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/Generated/hPt_ConvertedPhoton",trainOutput_LHC24f3b2));
    hPtConvertedPhotons_TrueMC_LHC24f3b2->Scale(1./nEvents_LHC24f3b2, "width");
    TH1D* hPtConvertedPhotons_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/Generated/hPt_ConvertedPhoton",trainOutput_LHC24f3c));
    hPtConvertedPhotons_TrueMC_LHC24f3c->Scale(1./nEvents_LHC24f3c, "width");

    ComparePtSpectra(hPtConvertedPhotons_TrueMC_LHC24f3b2, hPtConvertedPhotons_TrueMC_LHC24f3c, "Converted photons");

    
    TH1D* hPtV0_TrueMC_LHC24f3b2 = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt",trainOutput_LHC24f3b2));
    hPtV0_TrueMC_LHC24f3b2->Rebin(10);
    hPtV0_TrueMC_LHC24f3b2->Scale(1./nEvents_LHC24f3b2, "width");
    TH1D* hPtV0_TrueMC_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt",trainOutput_LHC24f3c));
    hPtV0_TrueMC_LHC24f3c->Rebin(10);
    hPtV0_TrueMC_LHC24f3c->Scale(1./nEvents_LHC24f3c, "width");

    ComparePtSpectra(hPtV0_TrueMC_LHC24f3b2, hPtV0_TrueMC_LHC24f3c, "V0 photons");

    
    ///Converted photons: Run by Run
    std::vector<const char*> runs = {"526641", "526964", "527041", "527057", "527109", "527240", "527850", "527871", "527895", "527899", "528292", "528461", "528531"};

    for(auto run: runs){
        const char* trainOutput_LHC24f3b2_RUN = Form("inputFiles/LHC22oPass7/AnalysisResults_MC_337722_RUN%s.root", run);
        const char* trainOutput_LHC24f3c_RUN = Form("inputFiles/LHC22oPass7/AnalysisResults_MC_333270_RUN%s.root", run);

        TH1D* hCollisionCounter_LHC24f3b2_RUN = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter",trainOutput_LHC24f3b2_RUN));
        TH1D* hCollisionCounter_LHC24f3c_RUN = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter",trainOutput_LHC24f3c_RUN));

        Double_t nEvents_LHC24f3b2_RUN = hCollisionCounter_LHC24f3b2_RUN->GetBinContent(12);
        Double_t nEvents_LHC24f3c_RUN = hCollisionCounter_LHC24f3c_RUN->GetBinContent(12);

        TH1D* hPtV0_TrueMC_LHC24f3b2_RUN = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt",trainOutput_LHC24f3b2_RUN));
        hPtV0_TrueMC_LHC24f3b2_RUN->Rebin(10);
        hPtV0_TrueMC_LHC24f3b2_RUN->Scale(1./nEvents_LHC24f3b2_RUN, "width");
        TH1D* hPtV0_TrueMC_LHC24f3c_RUN = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt",trainOutput_LHC24f3c_RUN));
        hPtV0_TrueMC_LHC24f3c_RUN->Rebin(10);
        hPtV0_TrueMC_LHC24f3c_RUN->Scale(1./nEvents_LHC24f3c_RUN, "width");


        TLegend* legend = new TLegend(0.528249,0.672012,0.805085,0.87172);
        legend->AddEntry((TObject*)0x0,"True MC pp collisions", "");
        legend->AddEntry((TObject*)0x0, Form("Run: %s", run), "");
        legend->AddEntry(hPtV0_TrueMC_LHC24f3c_RUN, Form("LHC24f3c, nEvents = %.0f", nEvents_LHC24f3c_RUN), "lp");
        legend->AddEntry(hPtV0_TrueMC_LHC24f3b2_RUN, Form("LHC24f3b2, nEvents = %.0f", nEvents_LHC24f3b2_RUN), "lp");
        
        ComparePtSpectraRunWise(hPtV0_TrueMC_LHC24f3b2_RUN, hPtV0_TrueMC_LHC24f3c_RUN, legend);
    }


    ///Converted photons: Same period (LHC24f3b2)
    std::vector<const char*> runsSamePeriod = {"526964", "527041", "527057", "527109", "527240", "527850", "527871", "527895", "527899", "528292", "528461", "528531"};
    const char* referenceRun = "526641";

    for(auto run: runsSamePeriod){
        const char* trainOutput_LHC24f3b2_Reference_RUN = Form("inputFiles/LHC22oPass7/AnalysisResults_MC_337722_RUN%s.root", referenceRun);
        const char* trainOutput_LHC24f3b2_RUN = Form("inputFiles/LHC22oPass7/AnalysisResults_MC_337722_RUN%s.root", run);

        TH1D* hCollisionCounter_LHC24f3b2_Reference_RUN = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter",trainOutput_LHC24f3b2_Reference_RUN));
        TH1D* hCollisionCounter_LHC24f3b2_RUN = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter",trainOutput_LHC24f3b2_RUN));

        Double_t nEvents_LHC24f3b2_Reference_RUN = hCollisionCounter_LHC24f3b2_Reference_RUN->GetBinContent(12);
        Double_t nEvents_LHC24f3b2_RUN = hCollisionCounter_LHC24f3b2_RUN->GetBinContent(12);

        TH1D* hPtV0_TrueMC_LHC24f3b2_Reference_RUN = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt",trainOutput_LHC24f3b2_Reference_RUN));
        hPtV0_TrueMC_LHC24f3b2_Reference_RUN->Rebin(10);
        hPtV0_TrueMC_LHC24f3b2_Reference_RUN->Scale(1./nEvents_LHC24f3b2_Reference_RUN, "width");
        TH1D* hPtV0_TrueMC_LHC24f3b2_RUN = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt",trainOutput_LHC24f3b2_RUN));
        hPtV0_TrueMC_LHC24f3b2_RUN->Rebin(10);
        hPtV0_TrueMC_LHC24f3b2_RUN->Scale(1./nEvents_LHC24f3b2_RUN, "width");


        TLegend* legend = new TLegend(0.528249,0.672012,0.805085,0.87172);
        legend->AddEntry((TObject*)0x0,"True MC pp collisions", "");
        legend->AddEntry((TObject*)0x0, "MC sample: LHC24f3b2", "");
        legend->AddEntry(hPtV0_TrueMC_LHC24f3b2_Reference_RUN, Form("Reference run: %s", referenceRun), "lp");
        legend->AddEntry(hPtV0_TrueMC_LHC24f3b2_RUN, Form("Run: %s", run), "lp");

        //ComparePtSpectraRunWise(hPtV0_TrueMC_LHC24f3b2_Reference_RUN, hPtV0_TrueMC_LHC24f3b2_RUN, legend);
    }

    ///Converted photons: Same period (LHC24f3c)
    std::vector<const char*> runsSamePeriod2 = {"526964", "527041", "527057", "527109", "527240", "527850", "527871", "527895", "527899", "528292", "528461", "528531"};
    const char* referenceRun2 = "526641";

    for(auto run: runsSamePeriod2){
        const char* trainOutput_LHC24f3c_Reference_RUN = Form("inputFiles/LHC22oPass7/AnalysisResults_MC_333270_RUN%s.root", referenceRun2);
        const char* trainOutput_LHC24f3c_RUN = Form("inputFiles/LHC22oPass7/AnalysisResults_MC_333270_RUN%s.root", run);

        TH1D* hCollisionCounter_LHC24f3c_Reference_RUN = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter",trainOutput_LHC24f3c_Reference_RUN));
        TH1D* hCollisionCounter_LHC24f3c_RUN = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter",trainOutput_LHC24f3c_RUN));

        Double_t nEvents_LHC24f3c_Reference_RUN = hCollisionCounter_LHC24f3c_Reference_RUN->GetBinContent(12);
        Double_t nEvents_LHC24f3c_RUN = hCollisionCounter_LHC24f3c_RUN->GetBinContent(12);

        TH1D* hPtV0_TrueMC_LHC24f3c_Reference_RUN = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt",trainOutput_LHC24f3c_Reference_RUN));
        hPtV0_TrueMC_LHC24f3c_Reference_RUN->Rebin(10);
        hPtV0_TrueMC_LHC24f3c_Reference_RUN->Scale(1./nEvents_LHC24f3c_Reference_RUN, "width");
        TH1D* hPtV0_TrueMC_LHC24f3c_RUN = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt",trainOutput_LHC24f3c_RUN));
        hPtV0_TrueMC_LHC24f3c_RUN->Rebin(10);
        hPtV0_TrueMC_LHC24f3c_RUN->Scale(1./nEvents_LHC24f3c_RUN, "width");


        TLegend* legend = new TLegend(0.528249,0.672012,0.805085,0.87172);
        legend->AddEntry((TObject*)0x0,"True MC pp collisions", "");
        legend->AddEntry((TObject*)0x0, "MC sample: LHC24f3c", "");
        legend->AddEntry(hPtV0_TrueMC_LHC24f3c_Reference_RUN, Form("Reference run: %s", referenceRun2), "lp");
        legend->AddEntry(hPtV0_TrueMC_LHC24f3c_RUN, Form("Run: %s", run), "lp");

       //ComparePtSpectraRunWise(hPtV0_TrueMC_LHC24f3c_Reference_RUN, hPtV0_TrueMC_LHC24f3c_RUN, legend);
    }

    

    

    //Invariant mass True MC
    TH1D* hMggPtPi0_LHC24f3b2 = GetTrueMC_InvMass(trainOutput_LHC24f3b2, 0, 4, "Pi0", nEvents_LHC24f3b2);
    TH1D* hMggPtPi0_LHC24f3c = GetTrueMC_InvMass(trainOutput_LHC24f3c, 0, 8, "Pi0", nEvents_LHC24f3c);

    TLegend* legendPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendPi0->AddEntry((TObject*)0x0,"True MC pp collisions", "");
    legendPi0->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legendPi0->AddEntry(hMggPtPi0_LHC24f3c, "#pi^{0} LHC24f3c", "lp");
    legendPi0->AddEntry(hMggPtPi0_LHC24f3b2, "#pi^{0} LHC24f3b2", "lp");


    TObjArray* arrPi0 = new TObjArray();
    arrPi0->Add(hMggPtPi0_LHC24f3c);
    arrPi0->Add(hMggPtPi0_LHC24f3b2);
    arrPi0->Add(legendPi0);

    TCanvas* canMassPi0 = (TCanvas*)makeCanvas(arrPi0, 0, "CMYK|NoTime", 0, 0);


    TH1D* hMggPtEta_LHC24f3b2 = GetTrueMC_InvMass(trainOutput_LHC24f3b2, 1, 4, "Eta", nEvents_LHC24f3b2);
    TH1D* hMggPtEta_LHC24f3c = GetTrueMC_InvMass(trainOutput_LHC24f3c, 1, 8, "Eta", nEvents_LHC24f3c);

    TLegend* legendEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEta->AddEntry((TObject*)0x0,"True MC pp collisions", "");
    legendEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendEta->AddEntry(hMggPtEta_LHC24f3c, "#eta LHC24f3c", "lp");
    legendEta->AddEntry(hMggPtEta_LHC24f3b2, "#eta LHC24f3b2", "lp");


    TObjArray* arrEta = new TObjArray();
    arrEta->Add(hMggPtEta_LHC24f3c);
    arrEta->Add(hMggPtEta_LHC24f3b2);
    arrEta->Add(legendEta);

    TCanvas* canMassEta = (TCanvas*)makeCanvas(arrEta, 0, "CMYK|NoTime", 0, 0);



    //Pt resolutions
    TH1D* hPtRes_LHC24f3b2 = GetTrueMC_PtReso(trainOutput_LHC24f3b2, 0, 4);
    TH1D* hPtRes_LHC24f3c = GetTrueMC_PtReso(trainOutput_LHC24f3c, 0, 8);

    TLegend* legendPtRes = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendPtRes->AddEntry((TObject*)0x0,"True MC pp collisions", "");
    legendPtRes->AddEntry(hPtRes_LHC24f3c, "LHC24f3c", "lp");
    legendPtRes->AddEntry(hPtRes_LHC24f3b2, "LHC24f3b2", "lp");

    TH1D* hPtResRatio = (TH1D*)hPtRes_LHC24f3c->Clone("hPtResRatio");
    hPtResRatio->Divide(hPtRes_LHC24f3b2);
    hPtResRatio->GetYaxis()->SetRangeUser(0.9, 1.1);
    hPtResRatio->GetXaxis()->SetRangeUser(0.1, 10.);



    TObjArray* arrPtRes = new TObjArray();
    arrPtRes->Add(hPtRes_LHC24f3c);
    arrPtRes->Add(hPtRes_LHC24f3b2);
    arrPtRes->Add(legendPtRes);

    TObjArray* arrPtResRatio = new TObjArray();
    arrPtResRatio->Add(hPtResRatio);

    TCanvas* canPtRes = (TCanvas*)makeCanvas(arrPtRes, arrPtResRatio, "CMYK|NoTime  LogX", 0, 0);

     
}



void SetMeanHisto(TH1D* hist, Int_t binColor, Int_t markerStyle){
    hist->Scale(1000); //unit change to MeV
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->SetMarkerStyle(markerStyle);
    hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hist->SetYTitle("peak position (MeV/#it{c}^{2})");
    hist->GetXaxis()->SetRangeUser(0.4, 10.);   
    hist->GetYaxis()->SetRangeUser(125., 141.);
    hist->GetXaxis()->SetTitleOffset(0.003);   
}


void SetSigmaHisto(TH1D* hist, Int_t binColor, Int_t markerStyle){
    hist->Scale(1000); //unit change to MeV
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->SetMarkerStyle(markerStyle);
    hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hist->SetYTitle("peak width (MeV/#it{c}^{2})");
    hist->GetXaxis()->SetRangeUser(0.4, 10.);   
    hist->GetYaxis()->SetRangeUser(2., 15.);
    hist->GetXaxis()->SetTitleOffset(0.003);   
}

TH1D* GetTrueMC_InvMass(const char* inFileDataName, Int_t color, Int_t markerStyle, const char* meson, Int_t nEvents){
    auto *hsSame = (THnSparse*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Pair/%s/hs_Primary",inFileDataName, meson)); 
    TH1D* hMggPt = (TH1D*)hsSame->Projection(0);

    SetHistogramPropertiesAlt(hMggPt, "m_{#gamma#gamma} (GeV/#it{c}^{2})", "counts/num. events", color);
    hMggPt->Scale(1./nEvents);
    hMggPt->SetMarkerStyle(markerStyle);
    hMggPt->Rebin(4);

    if(strcmp(meson, "Pi0") == 0) hMggPt->GetXaxis()->SetRangeUser(0.04, 0.24);
    else if(strcmp(meson, "Eta") == 0) hMggPt->GetXaxis()->SetRangeUser(0.4, 0.7);
    hMggPt->GetYaxis()->SetRangeUser(-1e-7, 1.5*hMggPt->GetMaximum());

    return hMggPt;
}

TH1D* GetTrueMC_PtReso(const char* inFileDataName, Int_t color, Int_t markerStyle){
    TH2D* hPtResolution = (TH2D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPtResolution",inFileDataName)); 
    TH1D* hPtResolution1D = (TH1D*)hPtResolution->ProfileX();

    SetHistogramPropertiesAlt(hPtResolution1D, "#it{p}_{T} (GeV/#it{c})", "#delta#it{p}_{T}/#it{p}_{T}", color);
    hPtResolution1D->SetMarkerStyle(markerStyle);

    hPtResolution1D->GetYaxis()->SetRangeUser(0.001, 0.1);
    hPtResolution1D->GetXaxis()->SetRangeUser(0.1, 10.);

    return hPtResolution1D;
}



void ComparePtSpectra(TH1D* hPt_LHC24f3b2, TH1D* hPt_LHC24f3c, const char* legendEntry){
    SetHistogramPropertiesAlt(hPt_LHC24f3b2, "#it{p}_{T} (GeV/c)", "(1/#it{N}_{events}) dN/dp_{T} (GeV/c)^{-1}", 11);
    hPt_LHC24f3b2->SetMarkerStyle(25);
    SetHistogramPropertiesAlt(hPt_LHC24f3c, "#it{p}_{T} (GeV/c)", "(1/#it{N}_{events}) dN/dp_{T} (GeV/c)^{-1}", 12);
    hPt_LHC24f3b2->SetMarkerStyle(26);

    hPt_LHC24f3b2->GetXaxis()->SetRangeUser(0.4, 1e1);
    hPt_LHC24f3c->GetXaxis()->SetRangeUser(0.4, 1e1);

    TH1D* hRatio = (TH1D*)hPt_LHC24f3c->Clone("hRatio");
    hRatio->Divide(hPt_LHC24f3b2);
    SetHistogramPropertiesAlt(hRatio, "#it{p}_{T} (GeV/c)", "", 14);
    hRatio->GetYaxis()->SetTitle("LHC24f3c / LHC24f3b2");
    hRatio->GetYaxis()->SetRangeUser(0.9, 1.1);

    TLegend* legend = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legend->AddEntry((TObject*)0x0,"True MC pp collisions", "");
    legend->AddEntry((TObject*)0x0, Form("%s", legendEntry), "");
    legend->AddEntry(hPt_LHC24f3c, "LHC24f3c", "lp");
    legend->AddEntry(hPt_LHC24f3b2, "LHC24f3b2", "lp");

    TObjArray* arr = new TObjArray();
    arr->Add(hPt_LHC24f3c);
    arr->Add(hPt_LHC24f3b2);
    arr->Add(legend);

    TObjArray* arrRatio = new TObjArray();
    arrRatio->Add(hRatio);

    TCanvas* canGenPhotons = (TCanvas*)makeCanvas(arr, arrRatio, "CMYK|NoTime LogY LogX", 0, 0);

}

void ComparePtSpectraRunWise(TH1D* hPt_LHC24f3b2, TH1D* hPt_LHC24f3c, TLegend* legend){
    SetHistogramPropertiesAlt(hPt_LHC24f3b2, "#it{p}_{T} (GeV/c)", "(1/#it{N}_{events}) dN/dp_{T} (GeV/c)^{-1}", 11);
    hPt_LHC24f3b2->SetMarkerStyle(25);
    SetHistogramPropertiesAlt(hPt_LHC24f3c, "#it{p}_{T} (GeV/c)", "(1/#it{N}_{events}) dN/dp_{T} (GeV/c)^{-1}", 12);
    hPt_LHC24f3b2->SetMarkerStyle(26);

    hPt_LHC24f3b2->GetXaxis()->SetRangeUser(0.4, 1e1);
    hPt_LHC24f3c->GetXaxis()->SetRangeUser(0.4, 1e1);

    TH1D* hRatio = (TH1D*)hPt_LHC24f3c->Clone("hRatio");
    hRatio->Divide(hPt_LHC24f3b2);
    SetHistogramPropertiesAlt(hRatio, "#it{p}_{T} (GeV/c)", "", 14);
    hRatio->GetYaxis()->SetTitle("LHC24f3c / LHC24f3b2");
    hRatio->GetYaxis()->SetRangeUser(0.9, 1.1);

    TObjArray* arr = new TObjArray();
    arr->Add(hPt_LHC24f3c);
    arr->Add(hPt_LHC24f3b2);
    arr->Add(legend);

    TObjArray* arrRatio = new TObjArray();
    arrRatio->Add(hRatio);

    TCanvas* canGenPhotons = (TCanvas*)makeCanvas(arr, arrRatio, "CMYK|NoTime LogY LogX", 0, 0);

}