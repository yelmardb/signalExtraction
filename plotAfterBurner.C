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
void SetMassPeak(TH1D* hist, Int_t binColor, Int_t markerStyle, const char* meson);
void PlotInvMass(const char* inFileDataName, const char* meson);
TH1D* makeCrossSection(TH1D* hist, Double_t etaRange, Double_t totXSection);
TH1D* computeCrossSection(TH1D* h1yield, TH1D* mesonAccXEffxBRTrue);

void plotAfterBurner(){


    //*******************************************************************************************************/

     for(auto cutMode: cutModes){



            const char* inFileName_Pi0_DATA = "inputAfterBurner/pi0Data.root";
            const char* inFileName_Pi0_MC = "inputAfterBurner/pi0MC.root";
            const char* inFileName_Pi0_DATA_wCorr = "inputAfterBurner/pi0Data_wCorr.root";
            const char* inFileName_Pi0_MC_wCorr = "inputAfterBurner/pi0MC_wCorr.root";

            const char* inFileName_Eta_DATA = "inputAfterBurner/etaData.root";
            const char* inFileName_Eta_MC = "inputAfterBurner/etaMC.root";
            const char* inFileName_Eta_DATA_wCorr = "inputAfterBurner/etaData_wCorr.root";   
            const char* inFileName_Eta_MC_wCorr = "inputAfterBurner/etaMC_wCorr.root"; 

            const char* hsigma = "histoSigma";
            const char* hmean = "histoMassMeson";
            const char* hexponential = "histoLambdaTail";
            //PlotInvMass(inFileName_Eta_DATA, "eta");

            ///*******************************************************************************************************///
            ///PEAK POSITIONS Pi0
            TH1D* h1mean_pi0_Data = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Pi0_DATA, hmean));
            TH1D* h1mean_pi0_MC = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Pi0_MC, hmean));
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

            ///PEAK POSITIONS Eta
            TH1D* h1mean_Eta_Data = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Eta_DATA, hmean));
            TH1D* h1mean_Eta_MC = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Eta_MC, hmean));
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

            ///*******************************************************************************************************///
            ///PEAK WIDTHS Pi0

            ///PEAK WIDTHS Eta
            TH1D* h1sigma_pi0_Data = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Pi0_DATA, hsigma));
            TH1D* h1sigma_pi0_MC = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Pi0_MC, hsigma));
            SetSigmaHisto(h1sigma_pi0_Data, 1, 8);
            SetSigmaHisto(h1sigma_pi0_MC, 1, 4);

            TH1D* h1sigma_Eta_Data = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Eta_DATA, hsigma));
            TH1D* h1sigma_Eta_MC = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Eta_MC, hsigma));
            SetSigmaHisto(h1sigma_Eta_Data, 0, 8);
            SetSigmaHisto(h1sigma_Eta_MC, 0, 4);

            TLegend* legendSigma = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendSigma->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendSigma->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendSigma->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
            legendSigma->AddEntry((TObject*)0x0,Form("cutMode: %s", cutMode), "");
            legendSigma->AddEntry(h1sigma_pi0_Data, "#pi^{0} Data", "lp");
            legendSigma->AddEntry(h1sigma_pi0_MC, "#pi^{0} MC", "lp");
            legendSigma->AddEntry(h1sigma_Eta_Data, "#eta Data", "lp");
            legendSigma->AddEntry(h1sigma_Eta_MC, "#eta MC", "lp");
            TObjArray* arrSigma = new TObjArray();
            arrSigma->Add(h1sigma_pi0_Data);
            arrSigma->Add(h1sigma_pi0_MC);
            arrSigma->Add(h1sigma_Eta_Data);
            arrSigma->Add(h1sigma_Eta_MC);
            arrSigma->Add(legendSigma);

    
            TCanvas* canSigma = (TCanvas*)makeCanvas(arrSigma, 0, "CMYK|NoTime", 0, 0);

            ///*******************************************************************************************************///
            ///Exponentials Pi0

            TH1D* h1exponential_pi0_Data = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Pi0_DATA, hexponential));
            TH1D* h1exponential_pi0_MC = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Pi0_MC, hexponential));
            SetExponentialHisto(h1exponential_pi0_Data, 0, 8);
            SetExponentialHisto(h1exponential_pi0_MC, 0, 4);

            TLegend* legendExponential = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendExponential->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendExponential->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendExponential->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
            legendExponential->AddEntry(h1exponential_pi0_Data, "#pi^{0} Data", "lp");
            legendExponential->AddEntry(h1exponential_pi0_MC, "#pi^{0} MC", "lp");
            TObjArray* arrExponential = new TObjArray();
            arrExponential->Add(h1exponential_pi0_Data);
            arrExponential->Add(h1exponential_pi0_MC);
            arrExponential->Add(legendExponential);
            
            TCanvas* canExponential = (TCanvas*)makeCanvas(arrExponential, 0, "CMYK|NoTime", 0, 0);

            ///Exponentials Eta
            TH1D* h1exponential_eta_Data = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Eta_DATA, hexponential));
            TH1D* h1exponential_eta_MC = (TH1D*)extractHistogram(Form("%s:/%s",inFileName_Eta_MC, hexponential));
            SetExponentialHisto(h1exponential_eta_Data, 0, 8);
            SetExponentialHisto(h1exponential_eta_MC, 0, 4);

            TLegend* legendExponentialEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendExponentialEta->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendExponentialEta->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendExponentialEta->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
            legendExponentialEta->AddEntry(h1exponential_eta_Data, "#eta Data", "lp");
            legendExponentialEta->AddEntry(h1exponential_eta_MC, "#eta MC", "lp");
            TObjArray* arrExponentialEta = new TObjArray();
            arrExponentialEta->Add(h1exponential_eta_Data);
            arrExponentialEta->Add(h1exponential_eta_MC);
            arrExponentialEta->Add(legendExponentialEta);
            
            TCanvas* canExponentialEta = (TCanvas*)makeCanvas(arrExponentialEta, 0, "CMYK|NoTime", 0, 0);
 
            ///*******************************************************************************************************///
        ///Mass peaks 
            std::vector<Double_t> dummy;
            //PlotInvMass(inFileName_Pi0_DATA, "pi0");
             //PlotInvMass(inFileName_Eta_DATA, "eta");

            ///*******************************************cross************************************************************///


            ///Efficiencies Eta
            Double_t branchingRatioPi0 = 0.98823;
            Double_t branchingRatioEta = 0.3941;

            TH1D* effEtaRun2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/EfficiencyEta_INT7"));
            SetHistogramPropertiesAlt(effEtaRun2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
            effEtaRun2->SetMarkerStyle(4);
            TH1D* mesonAccXEffxBRTrueEta = (TH1D*)extractHistogram(Form("%s:/TrueMesonEffiPt", inFileName_Eta_DATA_wCorr));
            if(!mesonAccXEffxBRTrueEta) cout << "no mesonAccXEffxBRTrueEta" << endl;
            SetHistogramPropertiesAlt(mesonAccXEffxBRTrueEta, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
            //mesonAccXEffxBRTrueEta->Scale(branchingRatioEta);



            TH1D* effPi0Run2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/EfficiencyPi0_INT7"));
            SetHistogramPropertiesAlt(effPi0Run2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
            effPi0Run2->SetMarkerStyle(4);
            TH1D* mesonAccXEffxBRTruePi0 = (TH1D*)extractHistogram(Form("%s:/TrueMesonEffiPt", inFileName_Pi0_DATA_wCorr));
            if(!mesonAccXEffxBRTruePi0) cout << "no mesonAccXEffxBRTruePi0" << endl;
            SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
            //mesonAccXEffxBRTruePi0->Scale(branchingRatioPi0);

            TLegend* legendEfficienciesPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendEfficienciesPi0->AddEntry((TObject*)0x0,"pp collisions", "");
            legendEfficienciesPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendEfficienciesPi0->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
            legendEfficienciesPi0->AddEntry(mesonAccXEffxBRTruePi0, "#pi^{0} 13.6 TeV", "lp");
            legendEfficienciesPi0->AddEntry(effPi0Run2, "#pi^{0} 13 TeV", "lp");
            legendEfficienciesPi0->AddEntry(mesonAccXEffxBRTrueEta, "#eta 13.6 TeV", "lp");
            legendEfficienciesPi0->AddEntry(effEtaRun2, "#eta 13 TeV", "lp");


            TObjArray* arrEfficienciesPi0 = new TObjArray();
            arrEfficienciesPi0->Add(effPi0Run2);
            arrEfficienciesPi0->Add(effEtaRun2);
            arrEfficienciesPi0->Add(mesonAccXEffxBRTruePi0);
            arrEfficienciesPi0->Add(mesonAccXEffxBRTrueEta);
            arrEfficienciesPi0->Add(legendEfficienciesPi0);
            TCanvas* canEffPi0 = (TCanvas*)makeCanvas(arrEfficienciesPi0, 0, "CMYK|NoTime|LogX|LogY", 0, 0);


            ///*******************************************************************************************************///
            //Raw yields
/*
            TH1D* h1RawYield_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Pi0_DATA));
            TH1D* h1RawYield_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Eta_DATA));
            TH1D* hRawYieldPi0Run2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/RAWYieldPerEventsPi0_INT7");
            TH1D* hRawYieldEtaRun2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/RAWYieldPerEventsEta_INT7");

            SetPtSpectrum(h1RawYield_pi0_Data, 0, 8);
            SetPtSpectrum(hRawYieldPi0Run2, 0, 4);
            SetPtSpectrum(h1RawYield_Eta_Data, 1, 8);
            SetPtSpectrum(hRawYieldEtaRun2, 1, 4);

            TLegend* legendRawYields = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendRawYields->AddEntry((TObject*)0x0,"This work pp collisions", "");
            legendRawYields->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendRawYields->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
            legendRawYields->AddEntry((TObject*)0x0,"Raw spectra", "");
            legendRawYields->AddEntry((TObject*)0x0,"Raw yields", "");
            legendRawYields->AddEntry(h1RawYield_pi0_Data, "#pi^{0}", "lp");
            legendRawYields->AddEntry(hRawYieldPi0Run2, "#pi^{0} Run 2", "lp");
            TLegend* legendRawYields2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendRawYields2->AddEntry(h1RawYield_Eta_Data, "#eta", "lp");
            legendRawYields2->AddEntry(hRawYieldEtaRun2, "#eta Run 2", "lp");

            TObjArray* arrRawYields = new TObjArray();
            arrRawYields->Add(h1RawYield_pi0_Data);
            arrRawYields->Add(hRawYieldPi0Run2);
            arrRawYields->Add(h1RawYield_Eta_Data);
            arrRawYields->Add(hRawYieldEtaRun2);
            arrRawYields->Add(legendRawYields);
            arrRawYields->Add(legendRawYields2);
            TCanvas* canRawYields = (TCanvas*)makeCanvas(arrRawYields, 0, "CMYK|NoTime|LogX |LogY", 0, 0);
            canRawYields->SaveAs(Form("Plots/%s/rawYields.png", cutMode));
*/
            ///*******************************************************************************************************///
            //Corrected Yields

            TH1D* h1CorrectedYield_pi0_Data = (TH1D*)extractHistogram(Form("%s:/CorrectedYieldTrueEff",inFileName_Pi0_DATA_wCorr));
            TH1D* h1CorrectedYield_Eta_Data = (TH1D*)extractHistogram(Form("%s:/CorrectedYieldTrueEff",inFileName_Eta_DATA_wCorr));
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


            ///*******************************************************************************************************///
            //Cross sections Pi0
            Double_t totXSection = 59.4*1e+9;
            TF1* TwoComponentModelFitPi0Unscaled = (TF1*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/TwoComponentModelFitPi0");
            double scale_factor = 1e+9*57.9;
            TF1 *TwoComponentModelFitPi0 = new TF1("TwoComponentModelFitPi0", [TwoComponentModelFitPi0Unscaled, scale_factor](double *x, double *p){
            return scale_factor * TwoComponentModelFitPi0Unscaled->Eval(x[0]); }, 0, 10, 0); // Create a new TF1 that scales the original function

            TwoComponentModelFitPi0->SetLineColor(kRed+2);

            cout << "debug, line: " << __LINE__ << endl;

            TH1D* h1UncorrectedYield_Pi0_Data =  (TH1D*)extractHistogram(Form("%s:/histoYieldMeson",inFileName_Pi0_DATA));
            TH1D* hCrossSectionPi0 = computeCrossSection(h1CorrectedYield_pi0_Data, mesonAccXEffxBRTruePi0);

            TH1D* hCrossSectionPi0Run2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/InvCrossSectionPi0");
            //TArrayD *binarrCrossSectionPi0 = (TArrayD*)hCrossSectionPi0->GetXaxis()->GetXbins();
            //TH1D* hCrossSectionPi0Run2 = RebinPtSpectrum(hCrossSectionPi0Run2Unrebinned, hCrossSectionPi0->GetNbinsX(), binarrCrossSectionPi0->GetArray());
                    cout << "debug, line: " << __LINE__ << endl;


            SetCrossSection(hCrossSectionPi0, 0, 8);
            SetCrossSection(hCrossSectionPi0Run2, 0, 4); 
            hCrossSectionPi0Run2->Fit(TwoComponentModelFitPi0, "R");
            //TwoComponentModelFitPi0->SetLineColor(hCrossSectionPi0Run2->GetMarkerColor());

            cout << "debug, line: " << __LINE__ << endl;

            TH1D* ratioCrossSection_Pi0 = (TH1D*)hCrossSectionPi0->Clone("ratioCrossSection_Pi0");
            ratioCrossSection_Pi0->Reset();
            ratioCrossSection_Pi0 = MakeRatioSpectra(hCrossSectionPi0, hCrossSectionPi0Run2, "quiet");
            ratioCrossSection_Pi0->GetYaxis()->SetRangeUser(0.5, 1.5);
            ratioCrossSection_Pi0->GetYaxis()->SetTitle("Run3/Run2");

            TLegend* legendCrossSectionPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendCrossSectionPi0->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendCrossSectionPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendCrossSectionPi0->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
            legendCrossSectionPi0->AddEntry(hCrossSectionPi0, "13.6 TeV (Run 3)", "lp");
            legendCrossSectionPi0->AddEntry(hCrossSectionPi0Run2, "13 TeV (Run 2, rebinned)", "lp");
            legendCrossSectionPi0->AddEntry(TwoComponentModelFitPi0, "TCM fit", "lp");
            cout << "debug, line: " << __LINE__ << endl;

            hCrossSectionPi0->GetYaxis()->SetRangeUser(1e3, 1e14);
            TObjArray* arrCrossSectionPi0 = new TObjArray();
            arrCrossSectionPi0->Add(hCrossSectionPi0);
            arrCrossSectionPi0->Add(hCrossSectionPi0Run2);
            arrCrossSectionPi0->Add(TwoComponentModelFitPi0);
            arrCrossSectionPi0->Add(legendCrossSectionPi0);
            TObjArray* arrRatioCrossSectionPi0 = new TObjArray();
            arrRatioCrossSectionPi0->Add(ratioCrossSection_Pi0);
    
            cout << "debug, line: " << __LINE__ << endl;

            ///Cross Section Eta
            TF1* TwoComponentModelFitEtaUnscaled = (TF1*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/TwoComponentModelFitEta");
            TF1 *TwoComponentModelFitEta = new TF1("TwoComponentModelFitEta", [TwoComponentModelFitEtaUnscaled, scale_factor](double *x, double *p){
            return scale_factor * TwoComponentModelFitEtaUnscaled->Eval(x[0]); }, 0, 10, 0); // Create a new TF1 that scales the original function

            cout << "debug, line: " << __LINE__ << endl;

            TH1D* h1UncorrectedYield_Eta_Data =  (TH1D*)extractHistogram(Form("%s:/histoYieldMeson",inFileName_Eta_DATA));
            TH1D* hCrossSectionEta = computeCrossSection(h1CorrectedYield_Eta_Data, mesonAccXEffxBRTrueEta);
            cout << "debug, line: " << __LINE__ << endl;

            TH1D* hCrossSectionEtaRun2 =  (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/InvCrossSectionEta");
            //TArrayD *binarrCrossSectionEta = (TArrayD*)hCrossSectionEta->GetXaxis()->GetXbins();
            //TH1D* hCrossSectionEtaRun2 = RebinPtSpectrum(hCrossSectionEtaRun2Unrebinned, hCrossSectionEta->GetNbinsX(), binarrCrossSectionEta->GetArray());
            cout << "debug, line: " << __LINE__ << endl;

            SetCrossSection(hCrossSectionEta, 1, 8);
            SetCrossSection(hCrossSectionEtaRun2, 1, 4); 
            hCrossSectionEta->Fit(TwoComponentModelFitEta, "R");
            TwoComponentModelFitEta->SetLineColor(hCrossSectionEta->GetMarkerColor());
            cout << "debug, line: " << __LINE__ << endl;

       
            TH1D* ratioCrossSection_Eta = (TH1D*)hCrossSectionEta->Clone("ratioCrossSection_Eta");
            ratioCrossSection_Eta->Reset();
            ratioCrossSection_Eta = MakeRatioSpectra(hCrossSectionEta, hCrossSectionEtaRun2, "quiet");
            ratioCrossSection_Eta->GetYaxis()->SetRangeUser(0.5, 1.5);
            ratioCrossSection_Eta->GetYaxis()->SetTitle("13.6 TeV/13 TeV");
            cout << "debug, line: " << __LINE__ << endl;

            TLegend* legendCrossSectionEta2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendCrossSectionEta2->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendCrossSectionEta2->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendCrossSectionEta2->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
            TLegend* legendCrossSectionEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendCrossSectionEta->AddEntry(hCrossSectionPi0, "#pi^{0} 13.6 TeV ", "lp");
            legendCrossSectionEta->AddEntry(hCrossSectionPi0Run2, "#pi^{0} 13 TeV", "lp");
            legendCrossSectionEta->AddEntry(hCrossSectionEta, "#eta 13.6 TeV ", "lp");
            legendCrossSectionEta->AddEntry(hCrossSectionEtaRun2, "#eta 13 TeV", "lp");
            TF1* TwoComponentModelFitDummy = (TF1*)TwoComponentModelFitPi0->Clone("TwoComponentModelFitDummy");
            TwoComponentModelFitDummy->SetLineColor(kBlack);
            legendCrossSectionEta->AddEntry(TwoComponentModelFitDummy, "TCM fit", "lp");

            hCrossSectionPi0->GetXaxis()->SetRangeUser(0.4, 10.);
            hCrossSectionPi0Run2->GetXaxis()->SetRangeUser(0.4, 10.);
            hCrossSectionEta->GetXaxis()->SetRangeUser(0.4, 10.);
            hCrossSectionEtaRun2->GetXaxis()->SetRangeUser(0.4, 10.);
            TwoComponentModelFitPi0->GetXaxis()->SetRangeUser(0.4, 10.);
            TwoComponentModelFitEta->GetXaxis()->SetRangeUser(0.4, 10.);



            hCrossSectionEta->GetYaxis()->SetRangeUser(1e1, 1e14);
            TObjArray* arrCrossSectionEta = new TObjArray();
            arrCrossSectionEta->Add(hCrossSectionPi0);
            arrCrossSectionEta->Add(hCrossSectionPi0Run2);
            arrCrossSectionEta->Add(TwoComponentModelFitPi0);
            arrCrossSectionEta->Add(hCrossSectionEtaRun2);
            arrCrossSectionEta->Add(hCrossSectionEta);
            arrCrossSectionEta->Add(TwoComponentModelFitEta);
            arrCrossSectionEta->Add(legendCrossSectionEta);
            arrCrossSectionEta->Add(legendCrossSectionEta2);


            TObjArray* arrRatioCrossSectionEta = new TObjArray();
            arrRatioCrossSectionEta->Add(ratioCrossSection_Eta);
            arrRatioCrossSectionEta->Add(ratioCrossSection_Pi0);
            
            TCanvas* canCrossSectionEta = (TCanvas*)makeCanvas(arrCrossSectionEta, arrRatioCrossSectionEta, "CMYK|NoTime| LogY LogX ", 0, 0);



            ///***********************************************************************************************************************************///
            ///Eta to Pi0 Ratio
            TH1D* eta2pi0Run2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/EtaToPi0YShiftedStatError");
            //TH1D* eta2pi0Run2 = (TH1D*)hCorrectedYieldEtaRun2->Clone("eta2pi0Run2");
            eta2pi0Run2 = MakeRatioSpectra(hCorrectedYieldEtaRun2, hCorrectedYieldPi0Run2, "quiet");

            TH1D* eta2pi0ratio = (TH1D*)h1CorrectedYield_Eta_Data->Clone("eta2pi0ratio");
            eta2pi0ratio->Reset();
            eta2pi0ratio = MakeRatioSpectra(h1CorrectedYield_Eta_Data, h1CorrectedYield_pi0_Data, "quiet");
            
            SetHistogramPropertiesAlt(eta2pi0Run2, "", "", 14);
            eta2pi0Run2->SetMarkerStyle(4);
            SetHistogramPropertiesAlt(eta2pi0ratio, "#it{p}_{T} (GeV/#it{c})", "#eta/#pi^{0}", 14);
            eta2pi0ratio->SetMarkerStyle(8);

            TLegend* legendEtaPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendEtaPi0->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendEtaPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendEtaPi0->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
            legendEtaPi0->AddEntry((TObject*)0x0,Form("cutMode: %s", cutMode), "");
            legendEtaPi0->AddEntry(eta2pi0ratio, "Run 3", "lp");
            legendEtaPi0->AddEntry(eta2pi0Run2, "Run 2", "lp");
    

            eta2pi0ratio->GetXaxis()->SetRangeUser(0.4, 5.);
            eta2pi0ratio->GetYaxis()->SetRangeUser(0., 1.);

            TObjArray* arrEtaPi0 = new TObjArray();
            arrEtaPi0->Add(eta2pi0ratio);
            arrEtaPi0->Add(eta2pi0Run2);
            arrEtaPi0->Add(legendEtaPi0);


            TCanvas* canEtaPi0 = (TCanvas*)makeCanvas(arrEtaPi0, 0, "CMYK|NoTime ", 0, 0);
            canEtaPi0->SaveAs(Form("Plots/%s/eta2pi0ratio.png", cutMode));
    }
}

TH1D* makeCrossSection(TH1D* hist, Double_t etaRange, Double_t totXSection){
    TH1D* hCrossSection = (TH1D*)hist->Clone("hCrossSection");
    hCrossSection->SetName("hCrossSection");
    FinalizeCrossSectionAfterburner(hCrossSection, etaRange); // cval = (etarange*nevents>0) ? (val)/(width * 2.0 * TMath::Pi() * etarange * nevents * pt) : 0;
    hCrossSection->Scale(totXSection);

    return hCrossSection;
}

TH1D* computeCrossSection(TH1D* h1yield, TH1D* mesonAccXEffxBRTrue){
    Double_t totXSection =  59.4*1e+9;

    Double_t etaRange;
    if(strcmp(period, "LHC22oPass6") == 0)  etaRange = 1.8;
    else etaRange = 1.6;

    const char* outputSignalExtractionData = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root", period, runNumberData));
    TH1D* hCollisionCounterData = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", outputSignalExtractionData));
    Double_t nEventsData;
    if(strcmp(period, "LHC22oPass6") == 0) nEventsData = hCollisionCounterData->GetBinContent(10);


 /*    TH1D* h1yieldCorr = (TH1D*)h1yield->Clone("h1yieldCorr");
   h1yieldCorr->Reset();
    
    for(int i = 2; i < h1yieldCorr->GetNbinsX(); i++){
        h1yieldCorr->SetBinContent(i, h1yield->GetBinContent(i)/mesonAccXEffxBRTrue->GetBinContent(i));
        cout <<  "h1yield->GetBinContent(i)/mesonAccXEffxBRTrue->GetBinContent(i): " << h1yield->GetBinContent(i)/mesonAccXEffxBRTrue->GetBinContent(i) << endl;

        Double_t contentYield = h1yield->GetBinContent(i);
        Double_t errorYield = h1yield->GetBinError(i);
        Double_t contentAccXEffxBR = mesonAccXEffxBRTrue->GetBinContent(i);
        Double_t errorAccXEffxBR = mesonAccXEffxBRTrue->GetBinError(i);
        Double_t error = TMath::Sqrt( TMath::Power(errorYield/contentAccXEffxBR, 2) + TMath::Power(contentYield*errorAccXEffxBR/contentAccXEffxBR*contentAccXEffxBR, 2));
        h1yieldCorr->SetBinError(i, error);
    }*/
    
    TH1D* hCrossSection = (TH1D*)h1yield->Clone("hCrossSection");
    hCrossSection->SetName("hCrossSection");
    FinalizeCrossSectionAfterburner(hCrossSection, etaRange); // cval = (etarange*nevents>0) ? (val)/(2.0 * TMath::Pi() * etarange * nevents * pt) : 0;
    hCrossSection->Scale(totXSection);

    return hCrossSection;
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

void PlotInvMass(const char* inFileDataName,  const char* meson){
        cout << "test, line: " << __LINE__ << endl;

    TObjArray* arrMassPeak;
    //cout << "pT.size(): " << pT.size() << endl;
    //cout << "pTMC.size(): " << pTMC.size() << endl;
    Int_t nBins = 0;
    if(strcmp(meson, "pi0") == 0) nBins = 61;
    else if(strcmp(meson, "eta") == 0) nBins = 15;
    cout << "test, line: " << __LINE__ << endl;

    for(int i = 3; i < 7 ; i++){
    cout << "test, line: " << __LINE__ << endl;

        TH1D* hSignal;
        TH1D* hBck;
        TH1D* hSignalBck;
    cout << "test, line: " << __LINE__ << endl;

        if(i > 9) hSignalBck = (TH1D*)extractHistogram(Form("%s:/Mapping_GG_InvMass_in_Pt_Bin%d",inFileDataName, i));
        else hSignalBck = (TH1D*)extractHistogram(Form("%s:/Mapping_GG_InvMass_in_Pt_Bin0%d",inFileDataName, i));
        if(!hSignalBck) cout << "hSignalBck not found" << endl;

        if(i > 9) hBck = (TH1D*)extractHistogram(Form("%s:/Mapping_BckNorm_InvMass_in_Pt_Bin%d",inFileDataName, i));
        else hBck = (TH1D*)extractHistogram(Form("%s:/Mapping_BckNorm_InvMass_in_Pt_Bin0%d",inFileDataName, i));
        if(!hBck) cout << "hBck not found" << endl;

        if(i > 9) hSignal = (TH1D*)extractHistogram(Form("%s:/fHistoMappingSignalInvMass_in_Pt_Bin%d",inFileDataName, i));
        else hSignal = (TH1D*)extractHistogram(Form("%s:/fHistoMappingSignalInvMass_in_Pt_Bin0%d",inFileDataName, i));
        if(!hSignal) cout << "hSignal not found" << endl;
        cout << "test, line: " << __LINE__ << endl;
    
        

        TFile* f = TFile::Open(Form("%s",inFileDataName));
        TF1* fitSignal;
        if(i > 9) fitSignal = (TF1*)f->Get(Form("Signal_InvMassFit_in_Pt_Bin%d", i));
        else fitSignal = (TF1*)f->Get(Form("Signal_InvMassFit_in_Pt_Bin0%d", i));
        if(!fitSignal) cout << "fitSignal not found" << endl;
        double scaleFactor = (1./hSignal->GetBinWidth(5));  //constant bin width, so just take one
        fitSignal->SetParameter(0, fitSignal->GetParameter(0) * scaleFactor);
        fitSignal->SetParLimits(0, hSignal->GetMaximum() * 0.8, hSignal->GetMaximum() * 1.3);
        fitSignal->SetLineColor(kRed+1);

    cout << "test, line: " << __LINE__ << endl;

        if(strcmp(meson, "pi0") == 0){
            SetMassPeak(hSignal, 0, 8, "pi0");
            SetMassPeak(hBck, 1, 4, "pi0");
                cout << "test, line: " << __LINE__ << endl;

            SetMassPeak(hSignalBck, 14, 8, "pi0");
        }
        else if(strcmp(meson, "eta") == 0){
            SetMassPeak(hSignal, 0, 8, "eta");
            SetMassPeak(hBck, 1, 4, "eta");
            SetMassPeak(hSignalBck, 14, 8, "eta");
        }
    cout << "test, line: " << __LINE__ << endl;

   std::vector<Double_t> fBinsPi013TeV_PCM_CombinedBinning {   // size: 61
         0.00, 0.20,  0.30,  0.40,  0.50,         0.60, 0.70,  0.80, 0.90,   1.00,  //10
	 1.10, 1.20,  1.30,  1.40,  1.50,         1.60, 1.70,  1.80,  1.90,  2.00,  //20
	 2.10, 2.20,  2.30,  2.40,  2.50,         2.60, 2.70,  2.80,  2.90,  3.00,  //30 
	 3.10, 3.20,  3.30,  3.40,  3.50,         3.60, 3.70,  3.80,  3.90,  4.00,  //40
	 4.20, 4.40,  4.60,  4.80,  5.00,         5.5,  6.00,  6.50,  7.00,  7.50,  //50
	 8.00, 8.50,  9.00,  9.50,  10.00,       10.50, 11.0,  11.5,  12.0,  15.0, // 60
	 20.0 ,22}; 

        std::vector<Double_t> fBinsEta13TeV_PCM_CombinedBinning {  //size 19
     0.00,  0.20, 0.40, 0.70, 1.00,     1.30,  1.60, 2.00, 2.50, 3.00,
     3.50,  4.0,  5.00, 6.00, 8.00,     10.0,  12.0, 16.0, 20.0};




        TLegend* legendMassPeak = new TLegend(0.528249,0.672012,0.805085,0.87172);
        legendMassPeak->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
        legendMassPeak->AddEntry((TObject*)0x0,"PCMPCM", "");
        if(strcmp(meson, "pi0") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
        else if(strcmp(meson, "eta") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
        if(strcmp(meson, "pi0") == 0) legendMassPeak->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", fBinsPi013TeV_PCM_CombinedBinning[i], fBinsPi013TeV_PCM_CombinedBinning[i+1]), "");
        else if(strcmp(meson, "eta") == 0) legendMassPeak->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", fBinsEta13TeV_PCM_CombinedBinning[i], fBinsEta13TeV_PCM_CombinedBinning[i+1]), "");
        legendMassPeak->AddEntry(hSignalBck, "signal+background", "lp");
        legendMassPeak->AddEntry(hBck, "mixed events", "lp");
        legendMassPeak->AddEntry(hSignal, "signal", "lp");
        legendMassPeak->AddEntry(fitSignal, "fit to signal", "lp");

        legendMassPeak->SetTextSize(0.03);

        hBck->GetYaxis()->SetRangeUser(-0.05, hBck->GetMaximum()*1.2);
        arrMassPeak = new TObjArray();
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(hBck);
        arrMassPeak->Add(hSignal);
        arrMassPeak->Add(fitSignal);
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(legendMassPeak);
        
        TCanvas* canMassPeak = (TCanvas*)makeCanvas(arrMassPeak, 0, "CMYK|NoTime|Quiet", 0, 0);
    }
 

}
