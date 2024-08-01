#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>

void SetPtSpectrum(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetCrossSection(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetMeanHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetSigmaHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetMassPeak(TH1D* hist, Int_t binColor, Int_t markerStyle, const char* meson);
void PlotMassPeaks(const char* inFileDataName, vector<Double_t> pT, const char* meson);

void plot(){

    std::vector<Double_t> fBinsPi0Pt= {0.40, 0.60, 0.80, 0.90, 1.00, 1.20, 1.40, 1.60, 1.80, 2.0, 2.50, 3.00, 3.50, 4.00,  4.50, 5.00, 5.5, 6.00, 7.00, 8.00, 10.00};
    std::vector<Double_t> fBinsPi0PtMC = {0.40, 0.80,  1.2,  1.6,  2.0,  2.5,  3.0,  3.5,  4.0,  4.5,  6.0,  10.};
    std::vector<Double_t> fBinsEtaPt= {1.00, 2.00, 3.00, 4.0,5.0,10.0}; 

    Double_t massPi0 = 0.1349766;
    TF1 *TwoCompModelPi0 = new TF1("twoCompModel_Dummy",Form("[0]*TMath::Exp(-(TMath::Sqrt(x*x+%.10f*%.10f)-%.10f)/[1]) + [2]/(TMath::Power(1+x*x/([3]*[3]*[4]),[4]) )",massPi0,massPi0,massPi0));
    TwoCompModelPi0->SetParameters(450.,0.3,1,0.3,8.); // standard parameter optimize if necessary
    Double_t massEta = 0.547862;
    TF1 *TwoCompModelEta = new TF1("twoCompModel_Dummy",Form("[0]*TMath::Exp(-(TMath::Sqrt(x*x+%.10f*%.10f)-%.10f)/[1]) + [2]/(TMath::Power(1+x*x/([3]*[3]*[4]),[4]) )",massEta,massEta,massEta));
    TwoCompModelEta->SetParameters(450.,0.3,1,0.3,8.); // standard parameter optimize if necessary



    ///*******************************************************************************************************///
    ///COMPARISON OF SPECTRA
    const char* inFileName_Pi0_DATA = "outputSignalExtraction/SignalExtraction_Pi0_DATA_220661.root";
    const char* inFileName_Eta_DATA = "outputSignalExtraction/SignalExtraction_Eta_DATA_220661.root";
    const char* inFileName_Pi0_MC = "outputSignalExtraction/SignalExtraction_Pi0_MC_223972.root";
    const char* inFileName_Eta_MC = "outputSignalExtraction/SignalExtraction_Eta_MC_223972.root";
    const char* inFileName_Pi0_DATA_Afterburner = "/Users/yelmard/myAnalysis/AnalysisSoftware/00010113_0dm00009f9730000dge0404000_0152103500000000/13.6TeV/SignalExtraction_Pi0_DATA_Afterburner_220661.root";
    const char* inFileName_Eta_DATA_Afterburner = "/Users/yelmard/myAnalysis/AnalysisSoftware/00010113_0dm00009f9730000dge0404000_0152103500000000/13.6TeV/SignalExtraction_Eta_DATA_Afterburner_220661.root";


    TH1D* h1yield_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Pi0_DATA));
    TH1D* h1yield_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Eta_DATA));
    TH1D* h1yieldCorr_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1yieldCorr",inFileName_Pi0_DATA));
    TH1D* h1yieldCorr_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1yieldCorr",inFileName_Eta_DATA));
    TH1D* h1yield_pi0_MC = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Pi0_MC));
    TH1D* h1yield_pi0_Data_Afterburner = (TH1D*)extractHistogram(Form("%s:/histoYieldMesonPerEvent",inFileName_Pi0_DATA_Afterburner));
    TH1D* h1yield_eta_Data_Afterburner = (TH1D*)extractHistogram(Form("%s:/histoYieldMesonPerEvent",inFileName_Eta_DATA_Afterburner));
    TH1D* hYieldPi0Run2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/RAWYieldPerEventsPi0_INT7");
    TH1D* hYieldEtaRun2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/RAWYieldPerEventsEta_INT7");
    TH1D* hYieldCorrPi0Run2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/CorrectedYieldPi0");
    TH1D* hYieldCorrEtaRun2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/CorrectedYieldEta");
    TH1D* hCrossSectionPi0_Data = (TH1D*)extractHistogram(Form("%s:/hCrossSection",inFileName_Pi0_DATA));
    TH1D* hCrossSectionPi0Run2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/InvCrossSectionPi0");
    TF1* TwoComponentModelFitPi0Unscaled = (TF1*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/TwoComponentModelFitPi0");
    double scale_factor = 1e+9*57.9;
    TF1 *TwoComponentModelFitPi0 = new TF1("TwoComponentModelFitPi0", [TwoComponentModelFitPi0Unscaled, scale_factor](double *x, double *p){
    return scale_factor * TwoComponentModelFitPi0Unscaled->Eval(x[0]); }, 0, 10, 0); // Create a new TF1 that scales the original function

    SetPtSpectrum(h1yield_pi0_Data, 0, 8);
    SetPtSpectrum(h1yieldCorr_pi0_Data, 0, 8);
    SetPtSpectrum(h1yield_pi0_Data_Afterburner, 0, 4); 
    SetPtSpectrum(hYieldCorrPi0Run2, 0, 4); 
    SetPtSpectrum(h1yield_pi0_MC, 0, 4); 
    SetPtSpectrum(hYieldPi0Run2, 0, 4);

    SetPtSpectrum(h1yield_Eta_Data, 1, 8); 
    SetPtSpectrum(h1yieldCorr_Eta_Data, 1, 8); 
    SetPtSpectrum(h1yield_eta_Data_Afterburner, 1, 4); 
    SetPtSpectrum(hYieldEtaRun2, 1, 4);
    SetPtSpectrum(hYieldCorrEtaRun2, 1, 4); 

    SetCrossSection(hCrossSectionPi0_Data, 0, 8);
    SetCrossSection(hCrossSectionPi0Run2, 0, 4); 
    hCrossSectionPi0Run2->Fit(TwoComponentModelFitPi0, "R");
    TwoComponentModelFitPi0->SetLineColor(hCrossSectionPi0Run2->GetMarkerColor());


    TH1D* ratioYield_Pi0_Data = MakeRatioSpectra(h1yield_pi0_Data, h1yield_pi0_Data_Afterburner);
    TH1D* ratioYield_Eta_Data = MakeRatioSpectra(h1yield_Eta_Data, h1yield_eta_Data_Afterburner);
    ratioYield_Pi0_Data->GetXaxis()->SetRangeUser(0.4, 10.0);
    ratioYield_Pi0_Data->GetYaxis()->SetRangeUser(0.8, 1.2);


    TLegend* legendYield = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendYield->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendYield->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendYield->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
    legendYield->AddEntry((TObject*)0x0,"Uncorrected spectra", "");
    legendYield->AddEntry(h1yield_pi0_Data, "#pi^{0}", "lp");
    legendYield->AddEntry(hYieldPi0Run2, "#pi^{0} Run 2", "lp");
    TLegend* legendYield2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendYield2->AddEntry(h1yield_Eta_Data, "#eta", "lp");
    legendYield2->AddEntry(hYieldEtaRun2, "#eta Run 2", "lp");
    
    TObjArray* arrYield = new TObjArray();
    arrYield->Add(h1yield_pi0_Data);
    arrYield->Add(h1yield_Eta_Data);
    arrYield->Add(hYieldPi0Run2);
    arrYield->Add(hYieldEtaRun2);
    arrYield->Add(legendYield);
    arrYield->Add(legendYield2);
    TObjArray* arrRatios = new TObjArray();
   // arrRatios->Add(ratioYield_Pi0_Data);
    //arrRatios->Add(ratioYield_Eta_Data);

    
    TCanvas* canYield = (TCanvas*)makeCanvas(arrYield, 0, "CMYK|NoTime| LogY LogX ", 0, 0);
    canYield->SaveAs("Plots/yields.png");

    h1yieldCorr_pi0_Data->GetYaxis()->SetRangeUser(1e-6, 5e2);
    h1yieldCorr_pi0_Data->GetYaxis()->SetRangeUser(1e-6, 5e2);
    
    TLegend* legendCorrYield = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendCorrYield->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendCorrYield->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendCorrYield->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
    legendCorrYield->AddEntry((TObject*)0x0,"Corrected spectra", "");
    legendCorrYield->AddEntry(h1yieldCorr_pi0_Data, "#pi^{0}", "lp");
    legendCorrYield->AddEntry(hYieldCorrPi0Run2, "#pi^{0} Run 2", "lp");
    TLegend* legendCorrYield2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendCorrYield2->AddEntry(h1yieldCorr_Eta_Data, "#eta", "lp");
    legendCorrYield2->AddEntry(hYieldCorrEtaRun2, "#eta Run 2", "lp");
    
    TObjArray* arrCorrYield = new TObjArray();
    arrCorrYield->Add(h1yieldCorr_pi0_Data);
    arrCorrYield->Add(h1yieldCorr_Eta_Data);
    arrCorrYield->Add(hYieldCorrPi0Run2);
    arrCorrYield->Add(hYieldCorrEtaRun2);
    arrCorrYield->Add(legendCorrYield);
    arrCorrYield->Add(legendCorrYield2);

    TCanvas* canCorrYield = (TCanvas*)makeCanvas(arrCorrYield, 0, "CMYK|NoTime| LogY LogX ", 0, 0);
    canCorrYield->SaveAs("Plots/correctedYields.png");



    TH1D* ratioCrossSection_Pi0_Data = (TH1D*)hCrossSectionPi0_Data->Clone("ratioCrossSection_Pi0_Data");
    ratioCrossSection_Pi0_Data->Divide(TwoComponentModelFitPi0);
    ratioCrossSection_Pi0_Data->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratioCrossSection_Pi0_Data->GetYaxis()->SetTitle("Run3/fit");

    TLegend* legendCrossSection = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendCrossSection->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendCrossSection->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendCrossSection->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
    legendCrossSection->AddEntry(hCrossSectionPi0_Data, "#pi^{0}", "lp");
    legendCrossSection->AddEntry(hCrossSectionPi0Run2, "#pi^{0} Run 2", "lp");
    legendCrossSection->AddEntry(TwoComponentModelFitPi0, "TCM fit", "lp");

    hCrossSectionPi0_Data->GetYaxis()->SetRangeUser(1e3, 1e14);
    TObjArray* arrCrossSection = new TObjArray();
    arrCrossSection->Add(hCrossSectionPi0_Data);
    arrCrossSection->Add(hCrossSectionPi0Run2);
    arrCrossSection->Add(TwoComponentModelFitPi0);
    arrCrossSection->Add(legendCrossSection);
    TObjArray* arrRatioCrossSection = new TObjArray();
    arrRatioCrossSection->Add(ratioCrossSection_Pi0_Data);

    TCanvas* canCrossSection = (TCanvas*)makeCanvas(arrCrossSection, arrRatioCrossSection, "CMYK|NoTime| LogY LogX ", 0, 0);
    canCrossSection->SaveAs("Plots/crossSections.png");



    ///*******************************************************************************************************///
    ///COMPARISON OF CORRECTED SPECTRA
/*
    TLegend* legendYieldCorr = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendYieldCorr->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendYieldCorr->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendYieldCorr->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
    legendYieldCorr->AddEntry(h1yieldCorr_pi0_Data, "#pi^{0}", "lp");
    legendYieldCorr->AddEntry(hYieldCorrPi0Run2, "#pi^{0} Run2", "lp");
    TLegend* legendYieldCorr2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendYieldCorr2->AddEntry(h1yieldCorr_Eta_Data, "#eta", "lp");
    legendYieldCorr2->AddEntry(hYieldCorrEtaRun2, "#eta Run2", "lp");

    TObjArray* arrYieldCorr = new TObjArray();
    arrYieldCorr->Add(h1yieldCorr_pi0_Data);
    arrYieldCorr->Add(hYieldCorrPi0Run2);
    arrYieldCorr->Add(h1yieldCorr_Eta_Data);
    arrYieldCorr->Add(hYieldCorrEtaRun2);
    arrYieldCorr->Add(legendYieldCorr);
    arrYieldCorr->Add(legendYieldCorr2);

    TCanvas* canYieldCorr = (TCanvas*)makeCanvas(arrYieldCorr, 0, "CMYK|NoTime| LogY LogX ", 0, 0);*/

    ///*******************************************************************************************************///
    ///PEAK POSITIONS Pi0
    TH1D* h1mean_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1mean",inFileName_Pi0_DATA));
    TH1D* h1mean_pi0_MC = (TH1D*)extractHistogram(Form("%s:/h1mean",inFileName_Pi0_MC));
    SetMeanHisto(h1mean_pi0_Data, 0, 8);
    SetMeanHisto(h1mean_pi0_MC, 0, 4);
         
    TLegend* legendMean = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendMean->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendMean->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendMean->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legendMean->AddEntry(h1mean_pi0_Data, "Data", "lp");
    legendMean->AddEntry(h1mean_pi0_MC, "MC", "lp");
    TObjArray* arrMean = new TObjArray();
    arrMean->Add(h1mean_pi0_Data);
    arrMean->Add(h1mean_pi0_MC);
    arrMean->Add(legendMean);
    
    TCanvas* canMean = (TCanvas*)makeCanvas(arrMean, 0, "CMYK|NoTime|", 0, 0);
    canMean->SaveAs("Plots/means_Pi0.png");

    ///PEAK POSITIONS Eta
    TH1D* h1mean_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1mean",inFileName_Eta_DATA));
    TH1D* h1mean_Eta_MC = (TH1D*)extractHistogram(Form("%s:/h1mean",inFileName_Eta_MC));
    SetMeanHisto(h1mean_Eta_Data, 1, 8);
    SetMeanHisto(h1mean_Eta_MC, 1, 4);
    h1mean_Eta_Data->GetYaxis()->SetRangeUser(500, 600);
    h1mean_Eta_MC->GetXaxis()->SetRangeUser(500, 600);

    TLegend* legendMeanEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendMeanEta->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendMeanEta->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendMeanEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendMeanEta->AddEntry(h1mean_Eta_Data, "Data", "lp");
    legendMeanEta->AddEntry(h1mean_Eta_MC, "MC", "lp");
    TObjArray* arrMeanEta = new TObjArray();
    arrMeanEta->Add(h1mean_Eta_Data);
    arrMeanEta->Add(h1mean_Eta_MC);
    arrMeanEta->Add(legendMeanEta);
    
    TCanvas* canMeanEta = (TCanvas*)makeCanvas(arrMeanEta, 0, "CMYK|NoTime|", 0, 0);
    canMeanEta->SaveAs("Plots/means_Eta.png");

    ///*******************************************************************************************************///
    ///PEAK WIDTHS Pi0
    TH1D* h1sigma_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Pi0_DATA));
    TH1D* h1sigma_pi0_MC = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Pi0_MC));
    SetSigmaHisto(h1sigma_pi0_Data, 0, 8);
    SetSigmaHisto(h1sigma_pi0_MC, 0, 4);

    TLegend* legendSigma = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendSigma->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendSigma->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendSigma->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legendSigma->AddEntry(h1sigma_pi0_Data, "Data", "lp");
    legendSigma->AddEntry(h1sigma_pi0_MC, "MC", "lp");
    TObjArray* arrSigma = new TObjArray();
    arrSigma->Add(h1sigma_pi0_Data);
    arrSigma->Add(h1sigma_pi0_MC);
    arrSigma->Add(legendSigma);
    
    TCanvas* canSigma = (TCanvas*)makeCanvas(arrSigma, 0, "CMYK|NoTime", 0, 0);
    canSigma->SaveAs("Plots/sigma_Pi0.png");

    ///PEAK WIDTHS Eta
    TH1D* h1sigma_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Eta_DATA));
    TH1D* h1sigma_Eta_MC = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Eta_MC));
    SetSigmaHisto(h1sigma_Eta_Data, 1, 8);
    SetSigmaHisto(h1sigma_Eta_MC, 1, 4);

    TLegend* legendSigmaEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendSigmaEta->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendSigmaEta->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendSigmaEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendSigmaEta->AddEntry(h1sigma_Eta_Data, "Data", "lp");
    legendSigmaEta->AddEntry(h1sigma_Eta_MC, "MC", "lp");
    TObjArray* arrSigmaEta = new TObjArray();
    arrSigmaEta->Add(h1sigma_Eta_Data);
    arrSigmaEta->Add(h1sigma_Eta_MC);
    arrSigmaEta->Add(legendSigmaEta);
    
    TCanvas* canSigmaEta = (TCanvas*)makeCanvas(arrSigmaEta, 0, "CMYK|NoTime", 0, 0);
    canSigmaEta->SaveAs("Plots/sigma_Eta.png");

    ///*******************************************************************************************************///
   ///Mass peaks 
    PlotMassPeaks(inFileName_Pi0_DATA, fBinsPi0Pt, "pi0");
    PlotMassPeaks(inFileName_Eta_DATA, fBinsEtaPt, "eta");

}


void SetMassPeak(TH1D* hist, Int_t binColor, Int_t markerStyle, const char* meson){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->Scale(1, "width"); 
    hist->GetXaxis()->SetTitle("#it{m}_{#gamma#gamma} (GeV/#it{c}^{2})");
    hist->SetYTitle("dM_{#gamma#gamma}/d#it{m}_{#gamma#gamma}");
    hist->SetMarkerStyle(markerStyle);
    //hist->GetYaxis()->SetRangeUser(-0.001, 0.09);
    if(strcmp(meson, "pi0") == 0) hist->GetXaxis()->SetRangeUser(0.04, 0.24);
    else if(strcmp(meson, "eta") == 0) hist->GetXaxis()->SetRangeUser(0.4, 0.7);
    hist->GetXaxis()->SetTitleOffset(0.003);  
}


void SetPtSpectrum(TH1D* hist, Int_t binColor, Int_t markerStyle){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->SetMarkerStyle(markerStyle);
    hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hist->SetYTitle("#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}");
    hist->GetXaxis()->SetRangeUser(0.4, 10.);
    hist->GetYaxis()->SetRangeUser(1e-9, 1e-3);  
    hist->GetXaxis()->SetTitleOffset(0.003);   

    // Customize the x-axis
    TAxis *xaxis = hist->GetXaxis();
    xaxis->SetNdivisions(-7); // -7 means 7 labels are to be drawn, and they are user-defined
    xaxis->SetLabelSize(0.03); // Set the label size (optional)
    xaxis->SetBinLabel(xaxis->FindBin(0.4), "0.4");
    xaxis->SetBinLabel(xaxis->FindBin(1), "1");
    xaxis->SetBinLabel(xaxis->FindBin(2), "2");
    xaxis->SetBinLabel(xaxis->FindBin(3), "3");
    xaxis->SetBinLabel(xaxis->FindBin(4), "4");
    xaxis->SetBinLabel(xaxis->FindBin(5), "5");
}

void SetCrossSection(TH1D* hist, Int_t binColor, Int_t markerStyle){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->SetMarkerStyle(markerStyle);
    hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hist->SetYTitle("#it{E} #frac{d^{3}#it{#sigma}}{d#it{p}^{3}} (pb GeV^{#minus2} #it{c}^{3})");
    hist->GetXaxis()->SetRangeUser(0.4, 10.);
    hist->GetYaxis()->SetRangeUser(1e4, 2e13);  
    hist->GetXaxis()->SetTitleOffset(0.003);   
    //hist->GetXaxis()->SetNdivisions(510, kFALSE); 

    TAxis *xaxis = hist->GetXaxis();
    xaxis->SetNdivisions(-7, kFALSE); // -7 means 7 labels are to be drawn, and they are user-defined
  /*  xaxis->SetBinLabel(xaxis->FindBin(0.4), "0.4");
    xaxis->SetBinLabel(xaxis->FindBin(1), "1");
    xaxis->SetBinLabel(xaxis->FindBin(2), "2");
    xaxis->SetBinLabel(xaxis->FindBin(3), "3");
    xaxis->SetBinLabel(xaxis->FindBin(4), "4");
    xaxis->SetBinLabel(xaxis->FindBin(5), "5");*/

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

void PlotMassPeaks(const char* inFileDataName, vector<Double_t> pT, const char* meson){
    
    TObjArray* arrMassPeak;
    for(int i = 0; i < pT.size()-1; i++){
        TH1D* hSignal = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileDataName, pT[i], pT[i+1]));
        if(!hSignal) cout << "hSignal not found" << endl;
        TH1D* hBck = (TH1D*)extractHistogram(Form("%s:/hMggPt_Mixed1D_pT_%.2f_%.2f",inFileDataName, pT[i], pT[i+1]));
        if(!hBck) cout << "hBck not found" << endl;
        TH1D* hSignalBck = (TH1D*)extractHistogram(Form("%s:/hMggPt_Same1D_pT_%.2f_%.2f",inFileDataName, pT[i], pT[i+1]));
        if(!hSignalBck) cout << "hSignalBck not found" << endl;

        hSignal->Scale(1.0/hSignal->Integral()); 
        hSignalBck->Scale(1.0/hSignalBck->Integral());
        hSignalBck->GetYaxis()->SetRangeUser(-0.001, 0.05);
        hBck->Scale(1.0/hBck->Integral());

        TFile* f = TFile::Open(Form("%s",inFileDataName));
        TF1* fitSignal = (TF1*)f->Get(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));
        if(!fitSignal) cout << "fitSignal not found" << endl;
        fitSignal->SetLineColor(kRed+1);

        if(strcmp(meson, "pi0") == 0){
            SetMassPeak(hSignal, 0, 8, "pi0");
            SetMassPeak(hBck, 1, 4, "pi0");
            SetMassPeak(hSignalBck, 14, 8, "pi0");
        }
        else if(strcmp(meson, "eta") == 0){
            SetMassPeak(hSignal, 0, 8, "eta");
            SetMassPeak(hBck, 1, 4, "eta");
            SetMassPeak(hSignalBck, 14, 8, "eta");
        }

        TLegend* legendMassPeak = new TLegend(0.528249,0.672012,0.805085,0.87172);
        legendMassPeak->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
        legendMassPeak->AddEntry((TObject*)0x0,"PCMPCM", "");
        if(strcmp(meson, "pi0") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
        else if(strcmp(meson, "eta") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
        legendMassPeak->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", pT[i], pT[i+1]), "");
        legendMassPeak->AddEntry(hSignalBck, "same events", "lp");
        legendMassPeak->AddEntry(hBck, "mixed events", "lp");
        legendMassPeak->AddEntry(hSignal, "signal", "lp");
        legendMassPeak->AddEntry(fitSignal, "fit to signal", "lp");

        arrMassPeak = new TObjArray();
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(hBck);
        arrMassPeak->Add(hSignal);
        arrMassPeak->Add(legendMassPeak);
        
        TCanvas* canMassPeak = (TCanvas*)makeCanvas(arrMassPeak, 0, "CMYK|NoTime|Quiet", 0, 0);
        canMassPeak->SaveAs(Form("Plots/InvMassPeak_%s_%.2f_%.2f.png", meson, pT[i], pT[i+1]));
    }
 

}
