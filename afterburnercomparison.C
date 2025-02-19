#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "includes/inputData.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>

void SetPtSpectrum(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetCrossSection(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetMeanHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetSigmaHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetExponentialHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetSignificanceHisto(TH1D* hist, Int_t binColor, Int_t markerStyle);
void SetMassPeak(TH1D* hist, Int_t binColor, Int_t markerStyle, const char* meson);
void PlotInvMass(const char* inFileDataName, const char* inFileMCTrueName, vector<Double_t> pT, vector<Double_t> pTMC, const char* meson, const char* cutMode);
void PlotInvMass_TrueMC(const char* inFileMCDataName,const char* inFileMCTrueName, vector<Double_t> pT, const char* meson, const char* cutMode);
void PlotInvMassFinal(const char* inFileDataName, const char* inFileMCTrueName, vector<Double_t> pT, const char* meson, const char* cutMode);

void afterburnercomparison(){


    //*******************************************************************************************************///
    ///Load 
    const char* inFileName_Pi0_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root", period, runNumberData));
    const char* inFileName_Pi0_MC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_MC_%d.root", period, runNumberMC));
    const char* inFileName_Pi0_TrueMC =  strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_TrueMC_%d.root", period, runNumberMC));

    const char* inFileName_Eta_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_Data_%d.root", period, runNumberData));
    const char* inFileName_Eta_MC =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_MC_%d.root", period, runNumberMC));
    const char* inFileName_Eta_TrueMC =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_TrueMC_%d.root", period, runNumberTrueMC));

    const char* inputAfterBurner_Pi0_Data = "/Users/yelmard/myAnalysis/AnalysisSoftware/00010113_0dm00009f9730000dge0404000_0152103500000000/13.6TeV/Pi0_data_GammaConvV1WithoutCorrection_00010113_0dm00009f9730000dge0404000_0152103500000000.root"; 
     const char* inputAfterBurner_Pi0_MC =  "/Users/yelmard/myAnalysis/AnalysisSoftware/00010113_0dm00009f9730000dge0404000_0152103500000000/13.6TeV/Pi0_data_GammaConvV1Correction_00010113_0dm00009f9730000dge0404000_0152103500000000.root";
    
    const char* inputAfterBurner_Eta_Data = "/Users/yelmard/myAnalysis/AnalysisSoftware/00010113_0dm00009f9730000dge0404000_0152103500000000/13.6TeV/Eta_data_GammaConvV1WithoutCorrection_00010113_0dm00009f9730000dge0404000_0152103500000000.root";
    const char* inputAfterBurner_Eta_MC = "/Users/yelmard/myAnalysis/AnalysisSoftware/00010113_0dm00009f9730000dge0404000_0152103500000000/13.6TeV/Eta_data_GammaConvV1Correction_00010113_0dm00009f9730000dge0404000_0152103500000000.root";

    ///*******************************************cross************************************************************///
 

    ///Efficiencies 
    TH1D* effPi0Afterburner = (TH1D*)extractHistogram(Form("%s:/TrueMesonEffiPt", inputAfterBurner_Pi0_MC));
    SetHistogramPropertiesAlt(effPi0Afterburner, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
    effPi0Afterburner->SetMarkerStyle(4);
    TH1D* mesonAccXEffxBRTruePi0 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", inFileName_Pi0_DATA));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);


    TH1D* effEtaAfterburner = (TH1D*)extractHistogram(Form("%s:/TrueMesonEffiPt", inputAfterBurner_Eta_MC));
    SetHistogramPropertiesAlt(effEtaAfterburner, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
    effEtaAfterburner->SetMarkerStyle(4);
    TH1D* mesonAccXEffxBRTrueEta = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", inFileName_Eta_DATA));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTrueEta, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);

    ///Plotting
    TLegend* legendEfficiencies = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEfficiencies->AddEntry((TObject*)0x0,"pp collisions", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTruePi0, "#pi^{0} this analysis", "lp");
    legendEfficiencies->AddEntry(effPi0Afterburner, "#pi^{0} afterburner", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTrueEta, "#eta this analysis", "lp");
    legendEfficiencies->AddEntry(effEtaAfterburner, "#eta afterburner", "lp");

      
    TObjArray* arrEfficienciesPi0 = new TObjArray();
    arrEfficienciesPi0->Add(effPi0Afterburner);
    arrEfficienciesPi0->Add(effEtaAfterburner);
    arrEfficienciesPi0->Add(mesonAccXEffxBRTruePi0);
    arrEfficienciesPi0->Add(mesonAccXEffxBRTrueEta);
    arrEfficienciesPi0->Add(legendEfficiencies);
    TCanvas* canEffPi0 = (TCanvas*)makeCanvas(arrEfficienciesPi0, 0, "CMYK|NoTime|LogX|LogY", 0, 0);


    ///*******************************************************************************************************///
    //Raw yields

    TH1D* h1RawYield_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Pi0_DATA));
    TH1D* h1RawYield_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Eta_DATA));
    TH1D* hRawYieldPi0AfterBurner = (TH1D*)extractHistogram(Form("%s:/histoYieldMesonPerEvent", inputAfterBurner_Pi0_Data));
    TH1D* hRawYieldEtaAfterBurner = (TH1D*)extractHistogram(Form("%s:/histoYieldMesonPerEvent", inputAfterBurner_Eta_Data));

    SetPtSpectrum(h1RawYield_pi0_Data, 0, 8);
    SetPtSpectrum(hRawYieldPi0AfterBurner, 0, 4);
    SetPtSpectrum(h1RawYield_Eta_Data, 1, 8);
    SetPtSpectrum(hRawYieldEtaAfterBurner, 1, 4);

    TH1D* h1RatioRawYields = (TH1D*)h1RawYield_pi0_Data->Clone("h1RatioRawYields");
    h1RatioRawYields->Reset();
    h1RatioRawYields = (TH1D*)MakeRatioSpectra(h1RawYield_pi0_Data, hRawYieldPi0AfterBurner, "quiet|B", h1RatioRawYields);

    TLegend* legendRawYields = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendRawYields->AddEntry((TObject*)0x0,"This work pp collisions", "");
    legendRawYields->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendRawYields->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
    legendRawYields->AddEntry((TObject*)0x0,"Raw yields", "");
    legendRawYields->AddEntry(h1RawYield_pi0_Data, "#pi^{0} this analysis", "lp");
    legendRawYields->AddEntry(hRawYieldPi0AfterBurner, "#pi^{0} afterburner", "lp");
    TLegend* legendRawYields2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendRawYields2->AddEntry(h1RawYield_Eta_Data, "#eta", "lp");
    legendRawYields2->AddEntry(hRawYieldEtaAfterBurner, "#eta afterburner", "lp");

    TObjArray* arrRawYields = new TObjArray();
    arrRawYields->Add(h1RawYield_pi0_Data);
    arrRawYields->Add(hRawYieldPi0AfterBurner);
    arrRawYields->Add(h1RawYield_Eta_Data);
    arrRawYields->Add(hRawYieldEtaAfterBurner);
    arrRawYields->Add(legendRawYields);
    arrRawYields->Add(legendRawYields2);
    TCanvas* canRawYields = (TCanvas*)makeCanvas(arrRawYields, 0, "CMYK|NoTime|LogX |LogY", 0, 0);

            ///*******************************************************************************************************///
            //Corrected Yields

  /*          TH1D* h1CorrectedYield_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1yieldCorr",inFileName_Pi0_DATA));
            TH1D* h1CorrectedYield_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1yieldCorr",inFileName_Eta_DATA));
            TH1D* hCorrectedYieldPi0Run2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/CorrectedYieldPi0");
            TH1D* hCorrectedYieldEtaRun2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/CorrectedYieldEta");

            SetPtSpectrum(h1CorrectedYield_pi0_Data, 0, 8);
            SetPtSpectrum(hCorrectedYieldPi0Run2, 0, 4);
            SetPtSpectrum(h1CorrectedYield_Eta_Data, 1, 8);
            SetPtSpectrum(hCorrectedYieldEtaRun2, 1, 4);


            TLegend* legendCorrectedYieldsPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendCorrectedYieldsPi0->AddEntry((TObject*)0x0,"This work pp collisions", "");
            legendCorrectedYieldsPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendCorrectedYieldsPi0->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
            legendCorrectedYieldsPi0->AddEntry((TObject*)0x0,Form("cutMode: %s", cutMode), "");
            legendCorrectedYieldsPi0->AddEntry((TObject*)0x0,"Corrected spectra", "");
            legendCorrectedYieldsPi0->AddEntry(h1CorrectedYield_pi0_Data, "13.6 TeV (Run 3)", "lp");
            legendCorrectedYieldsPi0->AddEntry(hCorrectedYieldPi0Run2, "13 TeV (Run 2)", "lp");
            h1CorrectedYield_pi0_Data->GetYaxis()->SetRangeUser(8e-6, 12.);


            TObjArray* arrCorrectedYieldsPi0 = new TObjArray();
            arrCorrectedYieldsPi0->Add(h1CorrectedYield_pi0_Data);
            arrCorrectedYieldsPi0->Add(hCorrectedYieldPi0Run2);
            arrCorrectedYieldsPi0->Add(legendCorrectedYieldsPi0);
            TCanvas* canCorrectedYieldsPi0 = (TCanvas*)makeCanvas(arrCorrectedYieldsPi0, 0, "CMYK|NoTime|LogX |LogY", 0, 0);
            canCorrectedYieldsPi0->SaveAs(Form("Plots/%s/correctedYieldsPi0.png", cutMode));


            TLegend* legendCorrectedYieldsEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendCorrectedYieldsEta->AddEntry((TObject*)0x0,"This work pp collisions", "");
            legendCorrectedYieldsEta->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendCorrectedYieldsEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
            legendCorrectedYieldsEta->AddEntry((TObject*)0x0,Form("cutMode: %s", cutMode), "");
            legendCorrectedYieldsEta->AddEntry((TObject*)0x0,"Corrected spectra", "");
            legendCorrectedYieldsEta->AddEntry(h1CorrectedYield_Eta_Data, "13.6 TeV (Run 3)", "lp");
            legendCorrectedYieldsEta->AddEntry(hCorrectedYieldEtaRun2, "13 TeV (Run 2)", "lp");
            h1CorrectedYield_Eta_Data->GetYaxis()->SetRangeUser(8e-6, 2.);

            TObjArray* arrCorrectedYieldsEta = new TObjArray();
            arrCorrectedYieldsEta->Add(h1CorrectedYield_Eta_Data);
            arrCorrectedYieldsEta->Add(hCorrectedYieldEtaRun2);
            arrCorrectedYieldsEta->Add(legendCorrectedYieldsEta);
            TCanvas* canCorrectedYieldsEta = (TCanvas*)makeCanvas(arrCorrectedYieldsEta, 0, "CMYK|NoTime|LogX |LogY", 0, 0);
            canCorrectedYieldsEta->SaveAs(Form("Plots/%s/correctedYieldsEta.png", cutMode));
*/
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

void SetExponentialHisto(TH1D* hist, Int_t binColor, Int_t markerStyle){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->SetMarkerStyle(markerStyle);
    hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hist->SetYTitle("exponential");
    hist->GetXaxis()->SetRangeUser(0.4, 10.);
    hist->GetYaxis()->SetRangeUser(1e-3, 1e-1);  
    hist->GetXaxis()->SetTitleOffset(0.003);   
}

void SetSignificanceHisto(TH1D* hist, Int_t binColor, Int_t markerStyle){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->SetMarkerStyle(markerStyle);
    hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hist->SetYTitle("#it{S}^{#pi^{0} (#eta)}/d#it{p}_{T} (GeV/#it{c})^{-1}");
    hist->GetXaxis()->SetRangeUser(0.4, 10.);
    hist->GetYaxis()->SetRangeUser(5e-1, 1.8*hist->GetMaximum());  
    hist->GetXaxis()->SetTitleOffset(0.003);   
}

void PlotInvMassFinal(const char* inFileDataName, const char* inFileMCTrueName, vector<Double_t> pT, const char* meson, const char* cutMode){
    
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

        TH1D* hSignalTrue = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileMCTrueName, pT[i], pT[i+1]));
        if(!hSignalTrue) cout << "hSignalTrue not found" << endl;

        Int_t binMinIntegral = hSignal->FindBin(0.04+1e-4);
        Int_t binMaxIntegral = hSignal->FindBin(0.24-1e-4);

        // hSignal->Scale(1./hSignal->Integral(binMinIntegral, binMaxIntegral));
        // hBck->Scale(1./hBck->Integral(binMinIntegral, binMaxIntegral));
        // hSignalBck->Scale(1./hSignalBck->Integral(binMinIntegral, binMaxIntegral), "width");
        // hSignalTrue->Scale(1./hSignalTrue->Integral(binMinIntegral, binMaxIntegral), "width");

        TFile* f = TFile::Open(Form("%s",inFileDataName));
        TF1* fitSignal = (TF1*)f->Get(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));
        if(!fitSignal) cout << "fitSignal not found" << endl;
        double scaleFactor = (1./hSignal->GetBinWidth(5));  //constant bin width, so just take one
        fitSignal->SetParameter(0, fitSignal->GetParameter(0) * scaleFactor);
        fitSignal->SetParLimits(0, hSignal->GetMaximum() * 0.8, hSignal->GetMaximum() * 1.3);
        fitSignal->SetLineColor(kRed+1);


        TFile* fTrueMC = TFile::Open(Form("%s",inFileMCTrueName));
        TF1* fitSignalTrueMC = (TF1*)fTrueMC->Get(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));
        if(!fitSignalTrueMC) cout << "fitSignal not found" << endl;
        double scaleFactorTrueMC = (1./hSignalTrue->GetBinWidth(5));  //constant bin width, so just take one
        fitSignalTrueMC->SetParameter(0, fitSignalTrueMC->GetParameter(0) * scaleFactorTrueMC);
        fitSignalTrueMC->SetParLimits(0, hSignalTrue->GetMaximum() * 0.8, hSignalTrue->GetMaximum() * 1.3);
        fitSignalTrueMC->SetLineColor(kRed+1);

        if(strcmp(meson, "pi0") == 0){
            SetMassPeak(hSignal, 0, 8, "pi0");
            SetMassPeak(hSignalTrue, 2, 8, "pi0");
            SetMassPeak(hBck, 1, 4, "pi0");
            SetMassPeak(hSignalBck, 14, 8, "pi0");
        }
        else if(strcmp(meson, "eta") == 0){
            SetMassPeak(hSignal, 0, 8, "eta");
            SetMassPeak(hSignalTrue, 2, 8, "eta");
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
        legendMassPeak->AddEntry(hBck, "calculated BG", "lp");
        legendMassPeak->AddEntry(hSignal, "signal", "lp");
        legendMassPeak->AddEntry(fitSignal, "fit to signal", "lp");
        legendMassPeak->AddEntry(hSignalTrue, "true MC signal", "lp");

        legendMassPeak->SetTextSize(0.03);

        hBck->GetYaxis()->SetRangeUser(-0.05, hBck->GetMaximum()*1.2);
        arrMassPeak = new TObjArray();
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(hBck);
        arrMassPeak->Add(hSignal);
        arrMassPeak->Add(fitSignal);
        arrMassPeak->Add(hSignalTrue);
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(legendMassPeak);
        
        TCanvas* canMassPeak = (TCanvas*)makeCanvas(arrMassPeak, 0, "CMYK|NoTime|Quiet", 0, 0);
        canMassPeak->SaveAs(Form("Plots/%s/InvMassPeak_%s_%.2f_%.2f.png", cutMode, meson, pT[i], pT[i+1]));
    }
 

}




void PlotInvMass(const char* inFileDataName, const char* inFileMCTrueName, vector<Double_t> pT, vector<Double_t> pTMC, const char* meson, const char* cutMode){
    
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
        TH1D* hMggPt_MixedScaled1D = (TH1D*)extractHistogram(Form("%s:/hMggPt_MixedScaled1D_pT_%.2f_%.2f",inFileDataName, pT[i], pT[i+1])); 
        if(!hMggPt_MixedScaled1D) cout << "hMggPt_MixedScaled1D not found" << endl;

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
        double scaleFactor = (1./hSignal->GetBinWidth(5));  //constant bin width, so just take one
        fitSignal->SetParameter(0, fitSignal->GetParameter(0) * scaleFactor);
        fitSignal->SetParLimits(0, hSignal->GetMaximum() * 0.8, hSignal->GetMaximum() * 1.3);
        //fitSignal->Scale(scaleFactor);

        fitSignal->SetLineColor(kRed+1);

        //hSignal->Fit(fitSignal, "R");

        if(strcmp(meson, "pi0") == 0){
            SetMassPeak(hSignal, 0, 8, "pi0");
            if(!pTMC.empty()) SetMassPeak(hSignalTrue, 2, 8, "pi0");
            SetMassPeak(hBck, 1, 4, "pi0");
            SetMassPeak(hMggPt_MixedScaled1D, 5, 4, "pi0");
            SetMassPeak(hSignalBck, 14, 8, "pi0");
        }
        else if(strcmp(meson, "eta") == 0){
            SetMassPeak(hSignal, 0, 8, "eta");
            if(!pTMC.empty()) SetMassPeak(hSignalTrue, 2, 8, "eta");
            SetMassPeak(hBck, 1, 4, "eta");
            SetMassPeak(hMggPt_MixedScaled1D, 5, 4, "eta");
            SetMassPeak(hSignalBck, 14, 8, "eta");
        }

        TLegend* legendMassPeak = new TLegend(0.528249,0.672012,0.805085,0.87172);
        legendMassPeak->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
        legendMassPeak->AddEntry((TObject*)0x0,"PCMPCM", "");
        if(strcmp(meson, "pi0") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
        else if(strcmp(meson, "eta") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
        legendMassPeak->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", pT[i], pT[i+1]), "");
        legendMassPeak->AddEntry(hSignalBck, "signal+background", "lp");
        legendMassPeak->AddEntry(hBck, "calculated BG", "lp");
        legendMassPeak->AddEntry(hMggPt_MixedScaled1D, "mixed events + remaining BG", "lp");
        legendMassPeak->AddEntry(hSignal, "signal", "lp");
        legendMassPeak->AddEntry(fitSignal, "fit to signal", "lp");
        if(!pTMC.empty()) legendMassPeak->AddEntry(hSignalTrue, "true MC signal", "lp");

        legendMassPeak->SetTextSize(0.03);

        hBck->GetYaxis()->SetRangeUser(-0.05, hBck->GetMaximum()*1.2);
        arrMassPeak = new TObjArray();
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(hBck);
        arrMassPeak->Add(hSignal);
        arrMassPeak->Add(hMggPt_MixedScaled1D);
        arrMassPeak->Add(fitSignal);
        if(!pTMC.empty()) arrMassPeak->Add(hSignalTrue);
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(legendMassPeak);
        
        TCanvas* canMassPeak = (TCanvas*)makeCanvas(arrMassPeak, 0, "CMYK|NoTime|Quiet", 0, 0);
        canMassPeak->SaveAs(Form("Plots/%s/InvMassPeak_%s_%.2f_%.2f.png", cutMode, meson, pT[i], pT[i+1]));
    }
 

}



void PlotInvMass_TrueMC(const char* inFileMCDataName, const char* inFileMCTrueName, vector<Double_t> pT, const char* meson, const char* cutMode){
    
    TObjArray* arrMassPeak;
    for(int i = 0; i < pT.size()-1; i++){

        TH1D* hSignal = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileMCTrueName, pT[i], pT[i+1]));
        if(!hSignal) cout << "hSignal not found" << endl;
        //hSignal->Scale(1./hSignal->GetBinWidth(5));
        Double_t masswindowlow, masswindowhigh;
        if(strcmp(meson, "pi0") == 0){
            masswindowlow = 0.05;
            masswindowhigh = 0.25;
        }
        else if(strcmp(meson, "eta") == 0){
            masswindowlow = 0.35;
            masswindowhigh = 0.75;
        }
        //hSignal->Rebin(2);
        hSignal->Scale(1./hSignal->Integral(hSignal->FindBin(masswindowlow), hSignal->FindBin(masswindowhigh)));

        TFile* f = TFile::Open(Form("%s",inFileMCTrueName));
        TF1* fitSignal = (TF1*)f->Get(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));
        if(!fitSignal) cout << "fitSignal not found" << endl;
        

         double scaleFactor = (1./hSignal->Integral(hSignal->FindBin(masswindowlow), hSignal->FindBin(masswindowhigh))); 
        // fitSignal->SetParameter(0, fitSignal->GetParameter(0) * scaleFactor);
        // fitSignal->SetParLimits(0, hSignal->GetMaximum() * 0.8, hSignal->GetMaximum() * 1.3);
        fitSignal->SetParameter(0, fitSignal->GetParameter(0) * scaleFactor);
        fitSignal->SetLineColor(kRed-2);

        TH1D* hSignalData = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileMCDataName, pT[i], pT[i+1]));
        if(!hSignalData) cout << "hSignalData not found" << endl;
        //hSignalData->Scale(1./hSignalData->GetBinWidth(5));
        hSignalData->Scale(1./hSignalData->Integral(hSignalData->FindBin(masswindowlow), hSignalData->FindBin(masswindowhigh)));

        TFile* fData = TFile::Open(Form("%s",inFileMCDataName));
        TF1* fitSignalData = (TF1*)fData->Get(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));
        if(!fitSignalData) cout << "fitSignalData not found" << endl;
        double scaleFactorData = (1./hSignalData->Integral(hSignalData->FindBin(masswindowlow), hSignalData->FindBin(masswindowhigh))); 

        fitSignalData->SetParameter(0, fitSignalData->GetParameter(0) * scaleFactorData);
        fitSignalData->SetParLimits(0, hSignalData->GetMaximum() * 0.8, hSignalData->GetMaximum() * 1.3);
        fitSignalData->SetLineColor(kRed+1);


        if(strcmp(meson, "pi0") == 0){
            SetMassPeak(hSignalData, 0, 8, "pi0");
            SetMassPeak(hSignal, 0, 4, "pi0");
        }
        else if(strcmp(meson, "eta") == 0){
            SetMassPeak(hSignalData, 0, 8, "eta");
            SetMassPeak(hSignal, 0, 4, "eta");
        }

        TLegend* legendMassPeak = new TLegend(0.528249,0.672012,0.805085,0.87172);
        legendMassPeak->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
        legendMassPeak->AddEntry((TObject*)0x0,"PCMPCM", "");
        if(strcmp(meson, "pi0") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
        else if(strcmp(meson, "eta") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
        legendMassPeak->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", pT[i], pT[i+1]), "");
        legendMassPeak->AddEntry(hSignalData, "signal", "lp");
        legendMassPeak->AddEntry(fitSignalData, "fit to signal", "lp");

        legendMassPeak->AddEntry(hSignal, "signal true MC", "lp");
        legendMassPeak->AddEntry(fitSignal, "fit to signal true MC", "lp");

        legendMassPeak->SetTextSize(0.03);

        arrMassPeak = new TObjArray();
        arrMassPeak->Add(hSignal);
        //arrMassPeak->Add(fitSignal);
        arrMassPeak->Add(hSignalData);
        //arrMassPeak->Add(fitSignalData);
        arrMassPeak->Add(legendMassPeak);
        
        TCanvas* canMassPeak = (TCanvas*)makeCanvas(arrMassPeak, 0, "CMYK|NoTime|Quiet", 0, 0);
        canMassPeak->SaveAs(Form("Plots/%s/InvMassPeakTrueMC_%s_%.2f_%.2f.png", cutMode, meson, pT[i], pT[i+1]));
    }
 

}
