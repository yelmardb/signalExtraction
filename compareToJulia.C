#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>
#include "includes/inputData.h"

void SetPtSpectrum(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetCrossSection(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetMeanHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetSigmaHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetMassPeak(TH1D* hist, Int_t binColor, Int_t markerStyle, const char* meson);
void PlotMassPeaks(const char* inFileDataName, const char* inFileMCTrueName, vector<Double_t> pT, vector<Double_t> pTMC, const char* meson);

void compareToJulia(){

    //std::vector<Double_t> fBinsPi0Pt= {0.40, 0.60, 0.80, 0.90, 1.00, 1.20, 1.40, 1.60, 1.80, 2.0, 2.50, 3.00, 3.50, 4.00,  4.50, 5.00, 5.5, 6.00, 7.00, 8.00, 10.00};
    std::vector<Double_t> fBinsPi0Pt = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 5.0, 10.0};
    std::vector<Double_t> fBinsPi0PtMC = {0.5, 1.0, 1.25, 2.0, 2.5, 3.0, 5.0, 10.0};


    TH1D* effRun2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/EfficiencyPi0_INT7"));
    SetHistogramPropertiesAlt(effRun2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 14);

    TH1D* effJulia = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults.root:/PCMPCM/h_E_A");
    SetHistogramPropertiesAlt(effJulia, "#it{p}_{T} GeV/#it{c}", "Acc x Eff", 4);

    TH1D* myEffRun3 = (TH1D*)extractHistogram(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root:/mesonAccXEffxBRTrue_Pi0" , period, runNumberData));
    SetHistogramPropertiesAlt(myEffRun3, "#it{p}_{T} GeV/#it{c}", "Acc x Eff", 3);

    TLegend* legendEfficiencies = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEfficiencies->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"#pi #rightarrow #gamma + #gamma", "");
    legendEfficiencies->AddEntry(effJulia, "Heidelberg", "lp");
    legendEfficiencies->AddEntry(myEffRun3, "this analysis", "lp");

    TObjArray* arrEfficiencies = new TObjArray();
    //effJulia->GetYaxis()->SetRangeUser(1e-6, 1e-2);
    //effJulia->GetXaxis()->SetRangeUser(0.4, 10.0);
    arrEfficiencies->Add(effJulia);
    arrEfficiencies->Add(myEffRun3);  
    arrEfficiencies->Add(legendEfficiencies);    

    for(int i = 1; i <= effJulia->GetNbinsX(); i++){  
        cout << "pt: " << effJulia->GetXaxis()->GetBinLowEdge(i) << endl;
    }
    TObjArray* arrratio = new TObjArray();
    TH1D* hratioEff = (TH1D*)effJulia->Clone("hratioEff");
    hratioEff->Reset();
    hratioEff->Divide(effJulia, myEffRun3, 1, 1, "B");
    SetHistogramProperties(hratioEff,  "eff/trueEff", 14);
    arrratio->Add(hratioEff);
    TCanvas* can2 = (TCanvas*)makeCanvas(arrEfficiencies, 0, "LogX LogY CMYK|NoTime", 0, 0);



    ///Efficiencies eta 

    TH1D* effRun2Eta = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/EfficiencyEta_INT7"));
    SetHistogramPropertiesAlt(effRun2Eta, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 14);

    TH1D* effJuliaEta = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults(1).root:/PCMPCM/h_E_A");
    SetHistogramPropertiesAlt(effJuliaEta, "#it{p}_{T} GeV/#it{c}", "Acc x Eff", 4);

    TH1D* myEffEtaRun3 = (TH1D*)extractHistogram(Form("outputSignalExtraction/%s/SignalExtraction_Eta_Data_%d.root:/mesonAccXEffxBRTrue_Eta" , period, runNumberData));
    SetHistogramPropertiesAlt(myEffEtaRun3, "#it{p}_{T} GeV/#it{c}", "Acc x Eff", 3);

    TLegend* legendEfficiencies2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEfficiencies2->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendEfficiencies2->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEfficiencies2->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendEfficiencies2->AddEntry(effJulia, "Heidelberg", "lp");
    legendEfficiencies2->AddEntry(myEffEtaRun3, "this analysis", "lp");

    TObjArray* arrEfficiencies2 = new TObjArray();
    //effJulia->GetYaxis()->SetRangeUser(1e-6, 1e-2);
    //effJulia->GetXaxis()->SetRangeUser(0.4, 10.0);
    arrEfficiencies2->Add(effJulia);
    arrEfficiencies2->Add(myEffEtaRun3);  
    arrEfficiencies2->Add(legendEfficiencies2);    

    for(int i = 1; i <= effJulia->GetNbinsX(); i++){  
        cout << "pt: " << effJulia->GetXaxis()->GetBinLowEdge(i) << endl;
    }
    TObjArray* arrratio2 = new TObjArray();
    TH1D* hratioEffEta = (TH1D*)effJulia->Clone("hratioEff");
    hratioEffEta->Reset();
    hratioEffEta->Divide(effJulia, myEffEtaRun3, 1, 1, "B");
    SetHistogramProperties(hratioEffEta,  "eff/trueEff", 14);
    arrratio->Add(hratioEffEta);
    TCanvas* can3 = (TCanvas*)makeCanvas(arrEfficiencies2, 0, "LogX LogY CMYK|NoTime", 0, 0);



    ///*******************************************************************************************************///
    ///COMPARISON OF SPECTRA
    const char* inFileName_Pi0_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root", period, runNumberData));
    const char* inFileName_Pi0_MC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_MC_%d.root", period, runNumberMC));
    const char* inFileName_Pi0_TrueMC =  strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_TrueMC_%d.root", period, runNumberMC));

    const char* inFileName_Eta_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_Data_%d.root", period, runNumberData));
    const char* inFileName_Eta_MC =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_MC_%d.root", period, runNumberMC));
    const char* inFileName_Eta_TrueMC =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_TrueMC_%d.root", period, runNumberTrueMC));
  

    ///Pi0
    TH1D* h1yield_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Pi0_DATA));
    TH1D* h1yieldCorr_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1yieldCorr",inFileName_Pi0_DATA));
    TH1D* h1yieldCorr_pi0_Julia = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults.root:/PCMPCM/h_corrected");
    TH1D* h1yieldUnCorr_pi0_Julia = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults.root:/PCMPCM/h1_yield_data");
    TH1D* h1yieldUnCorr_pi0_JuliaMC = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults.root:/PCMPCM/h1_yield_mc");

    double scale_factor = 1e+9*57.9;

    SetPtSpectrum(h1yield_pi0_Data, 0, 8);
    SetPtSpectrum(h1yieldCorr_pi0_Data, 0, 8);
    SetPtSpectrum(h1yieldCorr_pi0_Julia, 0, 4);
    SetPtSpectrum(h1yieldUnCorr_pi0_Julia, 0, 4);
    SetPtSpectrum(h1yieldUnCorr_pi0_JuliaMC, 0, 4);


   TArrayD *binarrPi0 = (TArrayD*)h1yieldCorr_pi0_Julia->GetXaxis()->GetXbins();
   TH1D* h1yieldCorr_pi0_DataRebinned = RebinPtSpectrum(h1yieldCorr_pi0_Data, h1yieldCorr_pi0_Julia->GetNbinsX(), binarrPi0->GetArray());
        


    TH1D* ratioYield_Pi0_Data = MakeRatioSpectra(h1yieldCorr_pi0_DataRebinned, h1yieldUnCorr_pi0_Julia);
    ratioYield_Pi0_Data->GetXaxis()->SetRangeUser(0.4, 10.0);
    ratioYield_Pi0_Data->GetYaxis()->SetRangeUser(0.8, 1.2);


    TLegend* legendYield = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendYield->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendYield->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendYield->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legendYield->AddEntry((TObject*)0x0,"Uncorrected spectra", "");
    legendYield->AddEntry(h1yield_pi0_Data, "#pi^{0} this work", "lp");
    legendYield->AddEntry(h1yieldUnCorr_pi0_Julia, "#pi^{0} Heidelberg", "lp");

    
    TObjArray* arrYield = new TObjArray();
    arrYield->Add(h1yield_pi0_Data);
    arrYield->Add(h1yieldUnCorr_pi0_Julia);
    arrYield->Add(legendYield);
    TObjArray* arrRatios = new TObjArray();
    arrRatios->Add(ratioYield_Pi0_Data);

    
    TCanvas* canYield = (TCanvas*)makeCanvas(arrYield, 0, "CMYK|NoTime| LogY LogX ", 0, 0);

    h1yieldCorr_pi0_Data->GetYaxis()->SetRangeUser(1e-6, 5e2);
    h1yieldCorr_pi0_Data->GetYaxis()->SetRangeUser(1e-6, 5e2);
    h1yieldCorr_pi0_Data->GetYaxis()->SetTitle("1/(2#pi p_{T} N_{evt}) d^{2}N_{ch}/(d#eta dp_{T}) (GeV/c)^{-2}");
    
    TLegend* legendCorrYield = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendCorrYield->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendCorrYield->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendCorrYield->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legendCorrYield->AddEntry((TObject*)0x0,"Corrected spectra", "");
    legendCorrYield->AddEntry(h1yieldCorr_pi0_Data, "#pi^{0} this work", "lp");
    legendCorrYield->AddEntry(h1yieldCorr_pi0_Julia, "#pi^{0} Heidelberg", "lp");


    TH1D* ratioCrossSection_Pi0_DatvsJulia = (TH1D*)h1yield_pi0_Data->Clone("ratioCrossSection_Pi0_DatvsHeidelberg");
    ratioCrossSection_Pi0_DatvsJulia = MakeRatioSpectra(h1yieldCorr_pi0_Data, h1yieldCorr_pi0_Julia, "quiet");
    ratioCrossSection_Pi0_DatvsJulia->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratioCrossSection_Pi0_DatvsJulia->GetYaxis()->SetTitle("Me/Heidelberg");

    
    TObjArray* arrCorrYield = new TObjArray();
    arrCorrYield->Add(h1yieldCorr_pi0_Data);
    arrCorrYield->Add(h1yieldCorr_pi0_Julia);
    arrCorrYield->Add(legendCorrYield);

    TObjArray* arrRatioJulia = new TObjArray();
    arrRatioJulia->Add(ratioCrossSection_Pi0_DatvsJulia);

    TCanvas* canCorrYield = (TCanvas*)makeCanvas(arrCorrYield, 0, "CMYK|NoTime| LogY LogX ", 0, 0);

    ///Eta
    TH1D* h1yield_eta_Data = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Eta_DATA));
    TH1D* h1yieldCorr_eta_Data = (TH1D*)extractHistogram(Form("%s:/h1yieldCorr",inFileName_Eta_DATA));
    TH1D* h1yieldCorr_eta_Julia = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults(1).root:/PCMPCM/h_corrected");
    TH1D* h1yieldUnCorr_eta_Julia = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults(1).root:/PCMPCM/h1_yield_data");
    TH1D* h1yieldUnCorr_eta_JuliaMC = (TH1D*)extractHistogram("/Users/yelmard/Downloads/this_analysis_pi0eta-to-gammagamma-pcmpcmresults(1).root:/PCMPCM/h1_yield_mc");

    TH1D* h1yield_eta_TrueMC = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Eta_TrueMC));

    SetPtSpectrum(h1yield_eta_Data, 0, 8);
    SetPtSpectrum(h1yieldCorr_eta_Data, 0, 8);
    SetPtSpectrum(h1yield_eta_TrueMC, 0, 8);
    SetPtSpectrum(h1yieldCorr_eta_Julia, 0, 4);
    SetPtSpectrum(h1yieldUnCorr_eta_Julia, 0, 4);
    SetPtSpectrum(h1yieldUnCorr_eta_JuliaMC, 0, 4);

    TH1D* ratioYield_eta_Data = MakeRatioSpectra(h1yield_eta_Data, h1yieldUnCorr_eta_Julia);
    ratioYield_eta_Data->GetXaxis()->SetRangeUser(0.4, 10.0);
    ratioYield_eta_Data->GetYaxis()->SetRangeUser(0.8, 1.2);

    TLegend* legendYieldEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendYieldEta->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendYieldEta->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendYieldEta->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
    legendYieldEta->AddEntry((TObject*)0x0,"Uncorrected spectra", "");
    legendYieldEta->AddEntry(h1yield_eta_Data, "#eta", "lp");
    legendYieldEta->AddEntry(h1yieldUnCorr_eta_Julia, "#eta Heidelberg", "lp");

        
    TObjArray* arrYield2 = new TObjArray();
    arrYield2->Add(h1yield_eta_Data);
    arrYield2->Add(h1yieldUnCorr_eta_Julia);
    arrYield2->Add(legendYieldEta);
    TObjArray* arrRatios2 = new TObjArray();
    arrRatios2->Add(ratioYield_eta_Data);
    TCanvas* canYield2 = (TCanvas*)makeCanvas(arrYield2, 0, "CMYK|NoTime| LogY LogX ", 0, 0);

    h1yieldCorr_eta_Data->GetYaxis()->SetRangeUser(1e-6, 5e2);
    h1yieldCorr_eta_Data->GetYaxis()->SetRangeUser(1e-6, 5e2);
    h1yieldCorr_eta_Data->GetYaxis()->SetTitle("1/(2#pi p_{T} N_{evt}) d^{2}N_{ch}/(d#eta dp_{T}) (GeV/c)^{-2}");
    
    TLegend* legendCorrYield2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendCorrYield2->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendCorrYield2->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendCorrYield2->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendCorrYield2->AddEntry((TObject*)0x0,"Corrected spectra", "");
    legendCorrYield2->AddEntry(h1yieldCorr_eta_Data, "#eta this work", "lp");
    legendCorrYield2->AddEntry(h1yieldCorr_eta_Julia, "#eta Heidelberg", "lp");


    TH1D* ratioCrossSection_Eta_DatvsJulia = (TH1D*)h1yieldCorr_eta_Data->Clone("ratioCrossSection_Eta_DatvsHeidelberg");
    ratioCrossSection_Eta_DatvsJulia = MakeRatioSpectra(h1yieldCorr_eta_Data, h1yieldCorr_eta_Julia, "quiet");
    ratioCrossSection_Eta_DatvsJulia->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratioCrossSection_Eta_DatvsJulia->GetYaxis()->SetTitle("Me/Heidelberg");

    
    TObjArray* arrCorrYield2 = new TObjArray();
    arrCorrYield2->Add(h1yieldCorr_eta_Julia);
    arrCorrYield2->Add(h1yieldCorr_eta_Data);
    arrCorrYield2->Add(legendCorrYield2);

    TObjArray* arrRatioJulia2 = new TObjArray();
    arrRatioJulia2->Add(ratioCrossSection_Eta_DatvsJulia);

    TCanvas* canCorrYieldEta = (TCanvas*)makeCanvas(arrCorrYield2, 0, "CMYK|NoTime| LogY LogX ", 0, 0);



    TLegend* legendYieldEta3 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendYieldEta3->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendYieldEta3->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendYieldEta3->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendYieldEta3->AddEntry((TObject*)0x0,"MC spectra", "");
    legendYieldEta3->AddEntry(h1yield_eta_TrueMC, "#eta", "lp");
    legendYieldEta3->AddEntry(h1yieldUnCorr_eta_JuliaMC, "#eta Heidelberg", "lp");

    TObjArray* arrMCYield= new TObjArray();
    arrMCYield->Add(h1yield_eta_TrueMC);
    arrMCYield->Add(h1yieldUnCorr_eta_JuliaMC);
    arrMCYield->Add(legendYieldEta3);

    TCanvas* canMCieldEta = (TCanvas*)makeCanvas(arrMCYield, 0, "CMYK|NoTime| LogY LogX ", 0, 0);




}


void SetMassPeak(TH1D* hist, Int_t binColor, Int_t markerStyle, const char* meson){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->Scale(1, "width"); 
    hist->GetXaxis()->SetTitle("#it{m}_{#gamma#gamma} (GeV/#it{c}^{2})");
    hist->SetYTitle("dM_{#gamma#gamma}/d#it{m}_{#gamma#gamma}");
    hist->SetMarkerStyle(markerStyle);
    hist->GetYaxis()->SetRangeUser(-0.001, hist->GetMaximum()*1.5);
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
    hist->GetYaxis()->SetRangeUser(1e-9, 1e-2);  
    hist->GetXaxis()->SetTitleOffset(0.003);   

    // Customize the x-axis
   /* TAxis *xaxis = hist->GetXaxis();
    xaxis->SetNdivisions(-7); // -7 means 7 labels are to be drawn, and they are user-defined
    xaxis->SetLabelSize(0.03); // Set the label size (optional)
    xaxis->SetBinLabel(xaxis->FindBin(0.4), "0.4");
    xaxis->SetBinLabel(xaxis->FindBin(1), "1");
    xaxis->SetBinLabel(xaxis->FindBin(2), "2");
    xaxis->SetBinLabel(xaxis->FindBin(3), "3");
    xaxis->SetBinLabel(xaxis->FindBin(4), "4");
    xaxis->SetBinLabel(xaxis->FindBin(5), "5");*/
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

    /*  TAxis *xaxis = hist->GetXaxis();
    xaxis->SetNdivisions(-7, kFALSE); // -7 means 7 labels are to be drawn, and they are user-defined
   xaxis->SetBinLabel(xaxis->FindBin(0.4), "0.4");
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

void PlotMassPeaks(const char* inFileDataName, const char* inFileMCTrueName, vector<Double_t> pT, vector<Double_t> pTMC, const char* meson){
    
    TObjArray* arrMassPeak;
    //cout << "pT.size(): " << pT.size() << endl;
    //cout << "pTMC.size(): " << pTMC.size() << endl;
    for(int i = 0; i < pT.size()-1; i++){

        cout << "pT: " << pT[i] << " - " << pT[i+1] << endl;
        TH1D* hSignal = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileDataName, pT[i], pT[i+1]));
        if(!hSignal) cout << "hSignal not found" << endl;
        TH1D* hBck = (TH1D*)extractHistogram(Form("%s:/hCalculatedBackground_pT_%.2f_%.2f",inFileDataName, pT[i], pT[i+1]));
        if(!hBck) cout << "hBck not found" << endl;
        TH1D* hSignalBck = (TH1D*)extractHistogram(Form("%s:/hMggPt_Same1D_pT_%.2f_%.2f",inFileDataName, pT[i], pT[i+1]));
        if(!hSignalBck) cout << "hSignalBck not found" << endl;

        TH1D* hSignalTrue;
        if(!pTMC.empty()){
            hSignalTrue = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileMCTrueName, pTMC[i], pTMC[i+1]));
            if(!hSignalTrue) cout << "hSignalTrue not found" << endl;
        } 
        else  cout << "No MC True Signal shown!" << endl;
        

        TFile* f = TFile::Open(Form("%s",inFileDataName));
        TF1* fitSignal = (TF1*)f->Get(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));
        if(!fitSignal) cout << "fitSignal not found" << endl;
        double integralFit = fitSignal->Integral(0.04, 0.24);
        cout << "integralFit: " << integralFit << endl;
        cout << "hSignal->GetBinWidth(5): " << hSignal->GetBinWidth(5) << endl;
        cout << "scaleFactor: " << (1./hSignal->GetBinWidth(5))/integralFit << endl;
        double scaleFactor = (1./hSignal->GetBinWidth(5));  //constant bin width, so just take one
        cout << "fitSignal->GetParameter(0): " << fitSignal->GetParameter(0) << endl;
        cout << "fitSignal->GetParameter(0)*scaleFactor: " << fitSignal->GetParameter(0) * scaleFactor << endl;
        fitSignal->SetParameter(0, fitSignal->GetParameter(0) * scaleFactor);
        fitSignal->SetParLimits(0, hSignal->GetMaximum() * 0.8, hSignal->GetMaximum() * 1.3);
        //fitSignal->Scale(scaleFactor);

        fitSignal->SetLineColor(kRed+1);

        //hSignal->Fit(fitSignal, "R");

        if(strcmp(meson, "pi0") == 0){
            SetMassPeak(hSignal, 0, 8, "pi0");
            if(!pTMC.empty()) SetMassPeak(hSignalTrue, 2, 8, "pi0");
            SetMassPeak(hBck, 1, 4, "pi0");
            SetMassPeak(hSignalBck, 14, 8, "pi0");
        }
        else if(strcmp(meson, "eta") == 0){
            SetMassPeak(hSignal, 0, 8, "eta");
            if(!pTMC.empty()) SetMassPeak(hSignalTrue, 2, 8, "eta");
            SetMassPeak(hBck, 1, 4, "eta");
            SetMassPeak(hSignalBck, 14, 8, "eta");
        }

        TLegend* legendMassPeak = new TLegend(0.528249,0.672012,0.805085,0.87172);
        legendMassPeak->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
        legendMassPeak->AddEntry((TObject*)0x0,"PCMPCM", "");
        if(strcmp(meson, "pi0") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
        else if(strcmp(meson, "eta") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
        legendMassPeak->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", pT[i], pT[i+1]), "");
        legendMassPeak->AddEntry(hSignalBck, "signal+background", "lp");
        legendMassPeak->AddEntry(hBck, "mixed events", "lp");
        legendMassPeak->AddEntry(hSignal, "signal", "lp");
        legendMassPeak->AddEntry(fitSignal, "fit to signal", "lp");
        if(!pTMC.empty()) legendMassPeak->AddEntry(hSignalTrue, "true MC signal", "lp");


        hBck->GetYaxis()->SetRangeUser(-0.05, hBck->GetMaximum()*1.2);
        arrMassPeak = new TObjArray();
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(hBck);
        arrMassPeak->Add(hSignal);
        arrMassPeak->Add(fitSignal);
        if(!pTMC.empty()) arrMassPeak->Add(hSignalTrue);
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(legendMassPeak);
        
        TCanvas* canMassPeak = (TCanvas*)makeCanvas(arrMassPeak, 0, "CMYK|NoTime|Quiet", 0, 0);
        canMassPeak->SaveAs(Form("Plots/InvMassPeak_%s_%.2f_%.2f.png", meson, pT[i], pT[i+1]));
    }
 

}
