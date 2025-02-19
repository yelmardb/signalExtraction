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
void PlotInvMass_TrueMC(const char* inFileMCDataName, const char* inFileMCTrueName, const char* inFileDataName, vector<Double_t> pT, const char* meson);
void PlotInvMassFinal(const char* inFileDataName, const char* inFileMCTrueName, vector<Double_t> pT, const char* meson, const char* cutMode);

void newPlot(){


    //*******************************************************************************************************///
    ///Load 
    const char* inFileName_Pi0_DATA;
    const char* inFileName_Pi0_MC;
    const char* inFileName_Pi0_TrueMC;
    const char* inFileName_Pi0_DATA_Afterburner;
    const char* inFileName_Eta_DATA;
    const char* inFileName_Eta_MC;
    const char* inFileName_Eta_TrueMC;
    

     for(auto cutMode: cutModes){
            if(strcmp(cutMode, "BASE") == 0){
                inFileName_Pi0_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root", period, runNumberData));
                inFileName_Pi0_MC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_MC_%d.root", period, runNumberMC));
                inFileName_Pi0_TrueMC =  strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_TrueMC_%d.root", period, runNumberMC));
                
                inFileName_Eta_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_Data_%d.root", period, runNumberData));
                inFileName_Eta_MC =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_MC_%d.root", period, runNumberMC));
                inFileName_Eta_TrueMC =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_TrueMC_%d.root", period, runNumberTrueMC));
            }
            else{
                inFileName_Pi0_DATA = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_Data_%d.root", period, cutMode,runNumberData));
                inFileName_Pi0_MC = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_MC_%d.root", period, cutMode,runNumberMC));
                inFileName_Pi0_TrueMC = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_TrueMC_%d.root", period, cutMode,runNumberTrueMC));

                inFileName_Eta_DATA = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_Data_%d.root", period, cutMode,runNumberData));
                inFileName_Eta_MC = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_MC_%d.root", period, cutMode,runNumberMC));
                inFileName_Eta_TrueMC = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_TrueMC_%d.root", period, cutMode,runNumberTrueMC));  
            }

            cout << "///*******************************************************************************************************///WARNINGG line: " << __LINE__ << endl;
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
            canMean->SaveAs(Form("Plots/%s/means_Pi0.png", cutMode));

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
            canMeanEta->SaveAs(Form("Plots/%s/means_Eta.png", cutMode));

            ///*******************************************************************************************************///
            ///PEAK WIDTHS Pi0

            ///PEAK WIDTHS Eta
            TH1D* h1sigma_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Eta_DATA));
            TH1D* h1sigma_Eta_MC = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Eta_MC));
            SetSigmaHisto(h1sigma_Eta_Data, 1, 8);
            SetSigmaHisto(h1sigma_Eta_MC, 1, 4);
            
            TH1D* h1sigma_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Pi0_DATA));
            TH1D* h1sigma_pi0_MC = (TH1D*)extractHistogram(Form("%s:/h1sigma",inFileName_Pi0_MC));
            SetSigmaHisto(h1sigma_pi0_Data, 0, 8);
            SetSigmaHisto(h1sigma_pi0_MC, 0, 4);

            TLegend* legendSigma = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendSigma->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendSigma->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendSigma->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
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
            canSigma->SaveAs(Form("Plots/%s/sigma_Pi0.png", cutMode));

            ///*******************************************************************************************************///
            ///Exponentials Pi0

            TH1D* h1exponential_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1exponential",inFileName_Pi0_DATA));
            TH1D* h1exponential_pi0_MC = (TH1D*)extractHistogram(Form("%s:/h1exponential",inFileName_Pi0_MC));
            SetExponentialHisto(h1exponential_pi0_Data, 0, 8);
            SetExponentialHisto(h1exponential_pi0_MC, 0, 4);

            TLegend* legendExponential = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendExponential->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendExponential->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendExponential->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
            legendExponential->AddEntry(h1exponential_pi0_Data, "#pi^{0} Data", "lp");
            legendExponential->AddEntry(h1exponential_pi0_MC, "#pi^{0} MC", "lp");
            TObjArray* arrExponential = new TObjArray();
            arrExponential->Add(h1exponential_pi0_Data);
            arrExponential->Add(h1exponential_pi0_MC);
            arrExponential->Add(legendExponential);
            
            TCanvas* canExponential = (TCanvas*)makeCanvas(arrExponential, 0, "CMYK|NoTime", 0, 0);
            canExponential->SaveAs(Form("Plots/%s/exponential_Pi0.png", cutMode));

            ///Exponentials Eta
            TH1D* h1exponential_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1exponential",inFileName_Eta_DATA));
            TH1D* h1exponential_Eta_MC = (TH1D*)extractHistogram(Form("%s:/h1exponential",inFileName_Eta_MC));
            SetExponentialHisto(h1exponential_Eta_Data, 1, 8);
            SetExponentialHisto(h1exponential_Eta_MC, 1, 4);

            TLegend* legendExponentialEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendExponentialEta->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendExponentialEta->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendExponentialEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
            legendExponentialEta->AddEntry(h1exponential_Eta_Data, "#eta Data", "lp");
            legendExponentialEta->AddEntry(h1exponential_Eta_MC, "#eta MC", "lp");
            TObjArray* arrExponentialEta = new TObjArray();
            arrExponentialEta->Add(h1exponential_Eta_Data);
            arrExponentialEta->Add(h1exponential_Eta_MC);
            arrExponentialEta->Add(legendExponentialEta);
            
            TCanvas* canExponentialEta = (TCanvas*)makeCanvas(arrExponentialEta, 0, "CMYK|NoTime", 0, 0);
            canExponentialEta->SaveAs(Form("Plots/%s/exponential_Eta.png", cutMode));

            ///Significance
            //Pi0
            TH1D* h1Significance_Pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1Significance",inFileName_Pi0_DATA));
            TH1D* h1Significance_Pi0_MC = (TH1D*)extractHistogram(Form("%s:/h1Significance",inFileName_Pi0_MC));
            SetSignificanceHisto(h1Significance_Pi0_Data, 0, 8);
            SetSignificanceHisto(h1Significance_Pi0_MC, 0, 4);

            TLegend* legendSignificancePi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendSignificancePi0->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendSignificancePi0->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendSignificancePi0->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
            legendSignificancePi0->AddEntry(h1Significance_Pi0_Data, "#pi^{0} Data", "lp");
            legendSignificancePi0->AddEntry(h1Significance_Pi0_MC, "#pi^{0} MC", "lp");

            TObjArray* arrSignificancePi0 = new TObjArray();
            arrSignificancePi0->Add(h1Significance_Pi0_Data);
            arrSignificancePi0->Add(h1Significance_Pi0_MC);
            arrSignificancePi0->Add(legendSignificancePi0);

            TCanvas* canSignificancePi0 = (TCanvas*)makeCanvas(arrSignificancePi0, 0, "CMYK|NoTime LogY", 0, 0);
            canSignificancePi0->SaveAs(Form("Plots/%s/significance_Pi0.png", cutMode));


            ///Eta
            TH1D* h1Significance_Eta_Data = (TH1D*)extractHistogram(Form("%s:/h1Significance",inFileName_Eta_DATA));
            TH1D* h1Significance_Eta_MC = (TH1D*)extractHistogram(Form("%s:/h1Significance",inFileName_Eta_MC));
            SetSignificanceHisto(h1Significance_Eta_Data, 0, 8);
            SetSignificanceHisto(h1Significance_Eta_MC, 0, 4);


            TLegend* legendSignificanceEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendSignificanceEta->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendSignificanceEta->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendSignificanceEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
            legendSignificanceEta->AddEntry(h1Significance_Eta_Data, "#eta Data", "lp");
            legendSignificanceEta->AddEntry(h1Significance_Eta_MC, "#eta MC", "lp");

            TObjArray* arrSignificanceEta = new TObjArray();
            arrSignificanceEta->Add(h1Significance_Eta_Data);
            arrSignificanceEta->Add(h1Significance_Eta_MC);
            arrSignificanceEta->Add(legendSignificanceEta);

            TCanvas* canSignificanceEta = (TCanvas*)makeCanvas(arrSignificanceEta, 0, "CMYK|NoTime LogY", 0, 0);
            canSignificanceEta->SaveAs(Form("Plots/%s/significance_Eta.png", cutMode));



            ///*******************************************************************************************************///
        ///Mass peaks 
            std::vector<Double_t> dummy;
             //PlotInvMass(inFileName_Pi0_DATA, inFileName_Pi0_TrueMC, fBinsPi0PtMC, dummy,"pi0", cutMode);
            //PlotInvMass(inFileName_Pi0_MC, inFileName_Pi0_TrueMC, fBinsPi0PtMC, dummy,"pi0", cutMode);
            // PlotInvMass(inFileName_Eta_DATA, inFileName_Eta_TrueMC, fBinsEtaPt, dummy, "eta", cutMode);
            // //PlotInvMass(inFileName_Eta_MC, inFileName_Eta_TrueMC, fBinsEtaPt, dummy, "eta", cutMode);
             PlotInvMass_TrueMC(inFileName_Pi0_MC, inFileName_Pi0_TrueMC, inFileName_Pi0_DATA, fBinsPi0PtMC,"pi0");
            // PlotInvMass_TrueMC(inFileName_Eta_MC, inFileName_Eta_TrueMC, inFileName_Eta_DATA, fBinsEtaPt, "eta");

            //PlotInvMassFinal(inFileName_Pi0_DATA, inFileName_Pi0_TrueMC, fBinsPi0PtMC, "pi0", cutMode);
            

            ///*******************************************cross************************************************************///


            ///Efficiencies Eta      
              Double_t branchingRatio = 0.98823;
            Double_t branchingRatioEta = 0.3941;

            TH1D* effEtaRun2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/EfficiencyEta_INT7"));
            SetHistogramPropertiesAlt(effEtaRun2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
            effEtaRun2->SetMarkerStyle(4);
            effEtaRun2->Scale(branchingRatioEta);
            TH1D* mesonAccXEffxBRTrueEta = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", inFileName_Eta_DATA));
            SetHistogramPropertiesAlt(mesonAccXEffxBRTrueEta, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);

            TLegend* legendEfficienciesEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendEfficienciesEta->AddEntry((TObject*)0x0,"This work pp collisions", "");
            legendEfficienciesEta->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendEfficienciesEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
            legendEfficienciesEta->AddEntry(mesonAccXEffxBRTrueEta, "13.6 TeV (Run 3)", "lp");
            //legendEfficienciesEta->AddEntry(effEtaRun2, "13 TeV (Run 2)", "lp");

            TObjArray* arrEfficienciesEta = new TObjArray();
            //arrEfficienciesEta->Add(effEtaRun2);
            arrEfficienciesEta->Add(mesonAccXEffxBRTrueEta);
            arrEfficienciesEta->Add(legendEfficienciesEta);
            //TCanvas* canEffEta = (TCanvas*)makeCanvas(arrEfficienciesEta, 0, "CMYK|NoTime|LogX|LogY", 0, 0);
            //canEffEta->SaveAs(Form("Plots/efficienciesEta.png"));

            ///Efficiencies Pi0
            TH1D* effPi0Run2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/EfficiencyPi0_INT7"));
            SetHistogramPropertiesAlt(effPi0Run2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
            effPi0Run2->SetMarkerStyle(4);
            effPi0Run2->Scale(branchingRatio);
            TH1D* mesonAccXEffxBRTruePi0 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", inFileName_Pi0_DATA));
            SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);


            TLegend* legendEfficienciesPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendEfficienciesPi0->AddEntry((TObject*)0x0,"pp collisions", "");
            legendEfficienciesPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendEfficienciesPi0->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
            legendEfficienciesPi0->AddEntry(mesonAccXEffxBRTruePi0, "#pi^{0} 13.6 TeV", "lp");
            legendEfficienciesPi0->AddEntry(effPi0Run2, "#pi^{0} 13 TeV", "lp");
            legendEfficienciesPi0->AddEntry(mesonAccXEffxBRTrueEta, "#eta 13.6 TeV", "lp");
            legendEfficienciesPi0->AddEntry(effEtaRun2, "#eta 13 TeV", "lp");

            // Double_t branchingRatioPi0 = 0.98823;
            // effPi0Run2->Scale(branchingRatioPi0);
            // Double_t branchingRatioEta = 0.3941;
            // effEtaRun2->Scale(branchingRatioEta);

            TObjArray* arrEfficienciesPi0 = new TObjArray();
            arrEfficienciesPi0->Add(effPi0Run2);
            arrEfficienciesPi0->Add(effEtaRun2);
            arrEfficienciesPi0->Add(mesonAccXEffxBRTruePi0);
            arrEfficienciesPi0->Add(mesonAccXEffxBRTrueEta);
            arrEfficienciesPi0->Add(legendEfficienciesPi0);
            TCanvas* canEffPi0 = (TCanvas*)makeCanvas(arrEfficienciesPi0, 0, "CMYK|NoTime|LogX|LogY", 0, 0);
            canEffPi0->SaveAs(Form("Plots/efficienciesPi0.png"));


            ///*******************************************************************************************************///
            //Raw yields

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
            legendRawYields->AddEntry(h1RawYield_pi0_Data, "#pi^{0}", "lp");
            //legendRawYields->AddEntry(hRawYieldPi0Run2, "#pi^{0} Run 2", "lp");
            TLegend* legendRawYields2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendRawYields2->AddEntry(h1RawYield_Eta_Data, "#eta", "lp");
            //legendRawYields2->AddEntry(hRawYieldEtaRun2, "#eta Run 2", "lp");

            TObjArray* arrRawYields = new TObjArray();
            arrRawYields->Add(h1RawYield_pi0_Data);
            //arrRawYields->Add(hRawYieldPi0Run2);
            arrRawYields->Add(h1RawYield_Eta_Data);
            //arrRawYields->Add(hRawYieldEtaRun2);
            arrRawYields->Add(legendRawYields);
            arrRawYields->Add(legendRawYields2);
            TCanvas* canRawYields = (TCanvas*)makeCanvas(arrRawYields, 0, "CMYK|NoTime|LogX |LogY", 0, 0);
            canRawYields->SaveAs(Form("Plots/%s/rawYields.png", cutMode));

            ///*******************************************************************************************************///
            //Corrected Yields

            TH1D* h1CorrectedYield_pi0_Data = (TH1D*)extractHistogram(Form("%s:/h1yieldCorr",inFileName_Pi0_DATA));
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
            TF1* TwoComponentModelFitPi0Unscaled = (TF1*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/TwoComponentModelFitPi0");
            double scale_factor = 1e+9*57.9;
            TF1 *TwoComponentModelFitPi0 = new TF1("TwoComponentModelFitPi0", [TwoComponentModelFitPi0Unscaled, scale_factor](double *x, double *p){
            return scale_factor * TwoComponentModelFitPi0Unscaled->Eval(x[0]); }, 0, 10, 0); // Create a new TF1 that scales the original function

            TwoComponentModelFitPi0->SetLineColor(kRed+2);

            TH1D* hCrossSectionPi0 = (TH1D*)extractHistogram(Form("%s:/hCrossSection",inFileName_Pi0_DATA));
            TH1D* hCrossSectionPi0Run2Unrebinned = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/InvCrossSectionPi0");
            TArrayD *binarrCrossSectionPi0 = (TArrayD*)hCrossSectionPi0->GetXaxis()->GetXbins();
            TH1D* hCrossSectionPi0Run2 = RebinPtSpectrum(hCrossSectionPi0Run2Unrebinned, hCrossSectionPi0->GetNbinsX(), binarrCrossSectionPi0->GetArray());

            SetCrossSection(hCrossSectionPi0, 0, 8);
            SetCrossSection(hCrossSectionPi0Run2, 0, 4); 
            hCrossSectionPi0Run2->Fit(TwoComponentModelFitPi0, "R");
            //TwoComponentModelFitPi0->SetLineColor(hCrossSectionPi0Run2->GetMarkerColor());

            TH1D* ratioCrossSection_Pi0 = (TH1D*)hCrossSectionPi0->Clone("ratioCrossSection_Pi0");
            ratioCrossSection_Pi0->Reset();
            ratioCrossSection_Pi0->Divide(hCrossSectionPi0, hCrossSectionPi0Run2, 1, 1, "");
            //ratioCrossSection_Pi0 = MakeRatioSpectra(hCrossSectionPi0, hCrossSectionPi0Run2, "quiet");
            ratioCrossSection_Pi0->GetYaxis()->SetRangeUser(0.5, 1.5);
            ratioCrossSection_Pi0->GetYaxis()->SetTitle("Run3/Run2");

            TLegend* legendCrossSectionPi0 = new TLegend(0.528249,0.672012,0.805085,0.87172);
            legendCrossSectionPi0->AddEntry((TObject*)0x0,"This work pp collisions at #sqrt{s} = 13.6 TeV", "");
            legendCrossSectionPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
            legendCrossSectionPi0->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
            legendCrossSectionPi0->AddEntry(hCrossSectionPi0, "13.6 TeV (Run 3)", "lp");
            legendCrossSectionPi0->AddEntry(hCrossSectionPi0Run2, "13 TeV (Run 2, rebinned)", "lp");
            legendCrossSectionPi0->AddEntry(TwoComponentModelFitPi0, "TCM fit", "lp");
            hCrossSectionPi0->GetYaxis()->SetRangeUser(1e3, 1e14);
            TObjArray* arrCrossSectionPi0 = new TObjArray();
            arrCrossSectionPi0->Add(hCrossSectionPi0);
            arrCrossSectionPi0->Add(hCrossSectionPi0Run2);
            arrCrossSectionPi0->Add(TwoComponentModelFitPi0);
            arrCrossSectionPi0->Add(legendCrossSectionPi0);
            TObjArray* arrRatioCrossSectionPi0 = new TObjArray();
            arrRatioCrossSectionPi0->Add(ratioCrossSection_Pi0);
    
            //TCanvas* canCrossSection = (TCanvas*)makeCanvas(arrCrossSectionPi0, arrRatioCrossSectionPi0, "CMYK|NoTime| LogY LogX ", 0, 0);
            //canCrossSection->SaveAs(Form("Plots/%s/crossSectionPi0.png", cutMode));

            ///Cross Section Eta
            TF1* TwoComponentModelFitEtaUnscaled = (TF1*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/TwoComponentModelFitEta");
            TF1 *TwoComponentModelFitEta = new TF1("TwoComponentModelFitEta", [TwoComponentModelFitEtaUnscaled, scale_factor](double *x, double *p){
            return scale_factor * TwoComponentModelFitEtaUnscaled->Eval(x[0]); }, 0, 10, 0); // Create a new TF1 that scales the original function

            TH1D* hCrossSectionEta = (TH1D*)extractHistogram(Form("%s:/hCrossSection",inFileName_Eta_DATA));
            TH1D* hCrossSectionEtaRun2Unrebinned =  (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/InvCrossSectionEta");
            TArrayD *binarrCrossSectionEta = (TArrayD*)hCrossSectionEta->GetXaxis()->GetXbins();
            TH1D* hCrossSectionEtaRun2 = RebinPtSpectrum(hCrossSectionEtaRun2Unrebinned, hCrossSectionEta->GetNbinsX(), binarrCrossSectionEta->GetArray());

            SetCrossSection(hCrossSectionEta, 1, 8);
            SetCrossSection(hCrossSectionEtaRun2, 1, 4); 
            hCrossSectionEta->Fit(TwoComponentModelFitEta, "R");
            TwoComponentModelFitEta->SetLineColor(hCrossSectionEta->GetMarkerColor());

    
            TH1D* ratioCrossSection_Eta = (TH1D*)hCrossSectionEta->Clone("ratioCrossSection_Eta");
            ratioCrossSection_Eta->Reset();
            ratioCrossSection_Eta = MakeRatioSpectra(hCrossSectionEta, hCrossSectionEtaRun2, "quiet");
            ratioCrossSection_Eta->GetYaxis()->SetRangeUser(0.5, 1.5);
            ratioCrossSection_Eta->GetYaxis()->SetTitle("13.6 TeV/13 TeV");
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
            ratioCrossSection_Eta->GetXaxis()->SetRangeUser(0.4, 10.);
            ratioCrossSection_Pi0->GetXaxis()->SetRangeUser(0.4, 10.);
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
            canCrossSectionEta->SaveAs(Form("Plots/%s/crossSectionsEta.png", cutMode));


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


void SetMassPeak(TH1D* hist, Int_t binColor, Int_t markerStyle, const char* meson){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    //hist->Scale(1, "width"); 
    hist->GetXaxis()->SetTitle("#it{m}_{#gamma#gamma} (GeV/#it{c}^{2})");
    hist->SetYTitle("1/#it{N}_{ev} M_{#gamma#gamma}");
    hist->SetMarkerStyle(markerStyle);
    hist->GetYaxis()->SetRangeUser(0., hist->GetMaximum()*1.5);
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
    hist->GetYaxis()->SetRangeUser(1e-3, 0.03);  
    hist->GetXaxis()->SetTitleOffset(0.003);   
}

void SetSignificanceHisto(TH1D* hist, Int_t binColor, Int_t markerStyle){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->SetMarkerStyle(markerStyle);
    hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hist->SetYTitle("#it{S}^{#pi^{0} (#eta)}");
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

        TLegend* legendMassPeak = new TLegend(0.125, 0.665354, 0.402228, 0.864829);
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
            hSignalTrue = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileMCTrueName, pT[i], pT[i+1]));
            if(!hSignalTrue) cout << "hSignalTrue not found" << endl;
        
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
            SetMassPeak(hSignalTrue, 2, 8, "pi0");
            SetMassPeak(hBck, 1, 4, "pi0");
            SetMassPeak(hMggPt_MixedScaled1D, 5, 4, "pi0");
            SetMassPeak(hSignalBck, 14, 8, "pi0");
        }
        else if(strcmp(meson, "eta") == 0){
            SetMassPeak(hSignal, 0, 8, "eta");
            SetMassPeak(hSignalTrue, 2, 8, "eta");
            SetMassPeak(hBck, 1, 4, "eta");
            SetMassPeak(hMggPt_MixedScaled1D, 5, 4, "eta");
            SetMassPeak(hSignalBck, 14, 8, "eta");
        }
                fitSignal->SetLineColor(kRed+1);


        TLegend* legendMassPeak = new TLegend(0.482673,0.627297,0.759901,0.893701);
        legendMassPeak->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
        legendMassPeak->AddEntry((TObject*)0x0,"PCMPCM", "");
        if(strcmp(meson, "pi0") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
        else if(strcmp(meson, "eta") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
        legendMassPeak->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", pT[i], pT[i+1]), "");
        legendMassPeak->AddEntry(hSignalBck, "signal+background", "lp");
        legendMassPeak->AddEntry(hMggPt_MixedScaled1D, "mixed events + remaining BG", "lp");
        legendMassPeak->AddEntry(hBck, "calculated BG", "lp");
        legendMassPeak->AddEntry(hSignal, "signal", "lp");
        legendMassPeak->AddEntry(fitSignal, "fit to signal", "lp");
        //legendMassPeak->AddEntry(hSignalTrue, "true MC signal", "lp");

        legendMassPeak->SetTextSize(0.03);

        hBck->GetYaxis()->SetRangeUser(-0.05, hBck->GetMaximum()*1.2);
        arrMassPeak = new TObjArray();
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(hBck);
        arrMassPeak->Add(hSignal);
        arrMassPeak->Add(hMggPt_MixedScaled1D);
        arrMassPeak->Add(fitSignal);
        //arrMassPeak->Add(hSignalTrue);
        arrMassPeak->Add(hSignalBck);
        arrMassPeak->Add(legendMassPeak);
        
        TCanvas* canMassPeak = (TCanvas*)makeCanvas(arrMassPeak, 0, "CMYK|NoTime|Quiet", 0, 0);
        canMassPeak->SaveAs(Form("Plots/%s/InvMassPeak_%s_%.2f_%.2f.png", cutMode, meson, pT[i], pT[i+1]));
    }
 

}



void PlotInvMass_TrueMC(const char* inFileMCDataName, const char* inFileMCTrueName, const char* inFileDataName, vector<Double_t> pT, const char* meson){
    
    TH1D* hCollisionCounter_Data = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter",inFileDataName));
    Double_t nEventsData = hCollisionCounter_Data->GetBinContent(12);
    TH1D* hCollisionCounter_MC = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter",inFileMCTrueName));
    Double_t nEventsMC = hCollisionCounter_MC->GetBinContent(12);

    TObjArray* arrMassPeak;
    for(int i = 0; i < pT.size()-1; i++){
        TH1D* hSignalTrueMC = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileMCTrueName, pT[i], pT[i+1]));
        if(!hSignalTrueMC) cout << "hSignalTrueMC not found" << endl;
        // hSignalTrueMC->Scale(1./hSignalTrueMC->GetBinWidth(5));
        Double_t masswindowlow, masswindowhigh;
        if(strcmp(meson, "pi0") == 0){
            masswindowlow = 0.05;
            masswindowhigh = 0.3;
        }
        else if(strcmp(meson, "eta") == 0){
            masswindowlow = 0.3;
            masswindowhigh = 0.8;
        }
        // hSignalTrueMC->Scale(1./hSignalTrueMC->Integral(hSignalTrueMC->FindBin(masswindowlow), hSignalTrueMC->FindBin(masswindowhigh)));
        hSignalTrueMC->Scale(1./nEventsMC);


        TH1D* hSignalMC = (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileMCDataName, pT[i], pT[i+1]));
        if(!hSignalMC) cout << "hSignalMC not found" << endl;
        // hSignalMC->Scale(1./hSignalMC->GetBinWidth(5));
        // hSignalMC->Scale(1./hSignalMC->Integral(hSignalMC->FindBin(masswindowlow), hSignalMC->FindBin(masswindowhigh)));
        hSignalMC->Scale(1./nEventsMC);

        TH1D* hSignalData= (TH1D*)extractHistogram(Form("%s:/hCalculatedSignal_pT_%.2f_%.2f",inFileDataName, pT[i], pT[i+1]));
        if(!hSignalData) cout << "hSignalData not found" << endl;
        // hSignalData->Scale(1./hSignalData->GetBinWidth(5));
        // hSignalData->Scale(1./hSignalData->Integral(hSignalData->FindBin(masswindowlow), hSignalData->FindBin(masswindowhigh)));
        hSignalData->Scale(1./nEventsData);


        if(strcmp(meson, "pi0") == 0){
            SetMassPeak(hSignalMC, 2, 4, "pi0");
            SetMassPeak(hSignalTrueMC, 3, 4, "pi0");
            SetMassPeak(hSignalData, 0, 8, "pi0");
        }
        else if(strcmp(meson, "eta") == 0){
            SetMassPeak(hSignalMC, 2, 4, "eta");
            SetMassPeak(hSignalTrueMC, 3, 4, "eta");
            SetMassPeak(hSignalData, 0, 8, "eta");
        }

        TLegend* legendMassPeak = new TLegend(0.490099,0.568241,0.767327,0.872703);
        legendMassPeak->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
        legendMassPeak->AddEntry((TObject*)0x0,"PCMPCM", "");
        if(strcmp(meson, "pi0") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
        else if(strcmp(meson, "eta") == 0) legendMassPeak->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
        legendMassPeak->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", pT[i], pT[i+1]), "");
        legendMassPeak->AddEntry(hSignalData, "Extracted signal from data", "lp");
        legendMassPeak->AddEntry(hSignalMC, "Extracted signal from MC", "lp");
        legendMassPeak->AddEntry(hSignalTrueMC, "True MC signal", "lp");

        legendMassPeak->SetTextSize(0.03);

        arrMassPeak = new TObjArray();
        arrMassPeak->Add(hSignalData);
        arrMassPeak->Add(hSignalTrueMC);
        arrMassPeak->Add(hSignalMC);
        arrMassPeak->Add(legendMassPeak);
        
        TCanvas* canMassPeak = (TCanvas*)makeCanvas(arrMassPeak, 0, "CMYK|NoTime|Quiet", 0, 0);
        canMassPeak->SaveAs(Form("Plots/InvMassPeakTrueMC_%s_%.2f_%.2f.png", meson, pT[i], pT[i+1]));
    }
 

}
