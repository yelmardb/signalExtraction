#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>



void compareSpectraUsedForEff(){

    ///*******************************************************************************************************///
    ///Efficiency study: take a look at pt spectra used to calculate efficiency
    const char* inFileName_Pi0_DATA_Pass6 = "outputSignalExtraction/LHC22oPass6/SignalExtraction_Pi0_Data_220661.root";
    const char* inFileName_Eta_DATA_Pass6 = "outputSignalExtraction/LHC22oPass6/SignalExtraction_Eta_Data_220661.root";
    const char* inFileName_Pi0_DATA_Pass7 = "outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_Data_298017.root";
    const char* inFileName_Eta_DATA_Pass7 = "outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_Data_298017.root";
    const char* inFileName_Eta_TrueMC_Pass7 = "outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_TrueMC_300332.root";
    const char* inFileName_Pi0_TrueMC_Pass7 = "outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_TrueMC_300332.root";
    const char* inFileName_Eta_TrueMC_Pass6 = "outputSignalExtraction/LHC22oPass6/SignalExtraction_Pi0_TrueMC_223972.root";
    const char* inFileName_Pi0_TrueMC_Pass6 = "outputSignalExtraction/LHC22oPass6/SignalExtraction_Eta_TrueMC_223972.root";

    //Number of events
    TH1D* hCollisionCounter_Pass6 = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter",inFileName_Pi0_TrueMC_Pass6));
    Double_t nEventsData_Pass6 = hCollisionCounter_Pass6->GetBinContent(10);

    TH1D* hCollisionCounter_Pass7 = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter",inFileName_Pi0_TrueMC_Pass7));
    Double_t nEventsData_Pass7 = hCollisionCounter_Pass7->GetBinContent(12);

    //Raw spectra
    TH1D* hyieldDataPi0_Pass6 = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Pi0_DATA_Pass6));
    hyieldDataPi0_Pass6->SetMarkerColor(kRed+2);
    hyieldDataPi0_Pass6->SetLineColor(kRed+2);
    hyieldDataPi0_Pass6->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}");
    TH1D* hyieldDataPi0_Pass7 = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Pi0_DATA_Pass7));
    hyieldDataPi0_Pass7->SetMarkerColor(kRed+2);
    hyieldDataPi0_Pass7->SetLineColor(kRed+2);
    hyieldDataPi0_Pass7->SetMarkerStyle(4);


    TH1D* hyieldDataEta_Pass6 = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Eta_DATA_Pass6));
    hyieldDataEta_Pass6->SetMarkerColor(kBlue+2);
    hyieldDataEta_Pass6->SetLineColor(kBlue+2);
    TH1D* hyieldDataEta_Pass7 = (TH1D*)extractHistogram(Form("%s:/h1yield",inFileName_Eta_DATA_Pass7));
    hyieldDataEta_Pass7->SetMarkerColor(kBlue+2);
    hyieldDataEta_Pass7->SetLineColor(kBlue+2);
    hyieldDataEta_Pass7->SetMarkerStyle(4);

    TLegend* legendRawYields = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendRawYields->AddEntry((TObject*)0x0,"This work pp collisions", "");
    legendRawYields->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendRawYields->AddEntry((TObject*)0x0,"#pi^{0}/#eta #rightarrow #gamma + #gamma", "");
    legendRawYields->AddEntry((TObject*)0x0,"Raw yields", "");
    legendRawYields->AddEntry(hyieldDataPi0_Pass6, "#pi^{0} pass6", "lp");
    legendRawYields->AddEntry(hyieldDataPi0_Pass7, "#pi^{0} pass7", "lp");
    TLegend* legendRawYields2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendRawYields2->AddEntry(hyieldDataEta_Pass6, "#eta pass6", "lp");
    legendRawYields2->AddEntry(hyieldDataEta_Pass7, "#eta pass7", "lp");

    TObjArray* arrRawYields = new TObjArray();
    arrRawYields->Add(hyieldDataPi0_Pass6);
    arrRawYields->Add(hyieldDataPi0_Pass7);
    arrRawYields->Add(hyieldDataEta_Pass6);
    arrRawYields->Add(hyieldDataEta_Pass7);
    arrRawYields->Add(legendRawYields);
    arrRawYields->Add(legendRawYields2);

    TCanvas* canRawYields = (TCanvas*)makeCanvas(arrRawYields, 0, "CMYK|NoTime|LogX |LogY", 0, 0);
    canRawYields->SaveAs("Plots/rawYieldsPassesComparison.png");


    //Efficiency pi0, pass6
    TH1D* hpTRecTruePi0_Pass6 = (TH1D*)extractHistogram(Form("%s:/hpTRecTrue_Pi0",inFileName_Pi0_DATA_Pass6));
    hpTRecTruePi0_Pass6->SetMarkerStyle(23);
    hpTRecTruePi0_Pass6->SetMarkerColor(kRed+2);
    hpTRecTruePi0_Pass6->SetLineColor(kRed+2);
    hpTRecTruePi0_Pass6->Scale(1./nEventsData_Pass6);
    hpTRecTruePi0_Pass6->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}");
    TH1D* hpTGenTrueRebinnedPi0_Pass6 = (TH1D*)extractHistogram(Form("%s:/hpTGenTrueRebinned_Pi0",inFileName_Pi0_DATA_Pass6));
    hpTGenTrueRebinnedPi0_Pass6->SetMarkerStyle(24);
    hpTGenTrueRebinnedPi0_Pass6->SetMarkerColor(kRed+2);
    hpTGenTrueRebinnedPi0_Pass6->SetLineColor(kRed+2);
    hpTGenTrueRebinnedPi0_Pass6->Scale(1./nEventsData_Pass6);


    ///Efficiency eta, pass6
    TH1D* hpTRecTrueEta_Pass6 = (TH1D*)extractHistogram(Form("%s:/hpTRecTrue_Eta",inFileName_Eta_DATA_Pass6));
    hpTRecTrueEta_Pass6->SetMarkerStyle(23);
    hpTRecTrueEta_Pass6->SetMarkerColor(kRed+2);
    hpTRecTrueEta_Pass6->SetLineColor(kRed+2);
    hpTRecTrueEta_Pass6->Scale(1./nEventsData_Pass6);
    hpTRecTrueEta_Pass6->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}");
    TH1D* hpTGenTrueRebinnedEta_Pass6 = (TH1D*)extractHistogram(Form("%s:/hpTGenTrueRebinned_Eta",inFileName_Eta_DATA_Pass6));
    hpTGenTrueRebinnedEta_Pass6->SetMarkerStyle(24);
    hpTGenTrueRebinnedEta_Pass6->SetMarkerColor(kRed+2);
    hpTGenTrueRebinnedEta_Pass6->SetLineColor(kRed+2);
    hpTGenTrueRebinnedEta_Pass6->Scale(1./nEventsData_Pass6);

    //Efficiency pi0, pass7
    TH1D* hpTRecTruePi0_Pass7 = (TH1D*)extractHistogram(Form("%s:/hpTRecTrue_Pi0",inFileName_Pi0_DATA_Pass7));
    hpTRecTruePi0_Pass7->SetMarkerStyle(23);
    hpTRecTruePi0_Pass7->SetMarkerColor(kBlue+2);
    hpTRecTruePi0_Pass7->SetLineColor(kBlue+2);
    hpTRecTruePi0_Pass7->Scale(1./nEventsData_Pass7);
    hpTRecTruePi0_Pass7->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}");
    TH1D* hpTGenTrueRebinnedPi0_Pass7 = (TH1D*)extractHistogram(Form("%s:/hpTGenTrueRebinned_Pi0",inFileName_Pi0_DATA_Pass7));
    hpTGenTrueRebinnedPi0_Pass7->SetMarkerStyle(24);
    hpTGenTrueRebinnedPi0_Pass7->SetMarkerColor(kBlue+2);
    hpTGenTrueRebinnedPi0_Pass7->SetLineColor(kBlue+2);
    hpTGenTrueRebinnedPi0_Pass7->Scale(1./nEventsData_Pass7);

    ///Efficiency eta, pass7
    TH1D* hpTRecTrueEta_Pass7 = (TH1D*)extractHistogram(Form("%s:/hpTRecTrue_Eta",inFileName_Eta_DATA_Pass7));
    hpTRecTrueEta_Pass7->SetMarkerStyle(23);
    hpTRecTrueEta_Pass7->SetMarkerColor(kBlue+2);
    hpTRecTrueEta_Pass7->SetLineColor(kBlue+2);
    hpTRecTrueEta_Pass7->Scale(1./nEventsData_Pass7);
    hpTRecTrueEta_Pass7->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}");
    TH1D* hpTGenTrueRebinnedEta_Pass7 = (TH1D*)extractHistogram(Form("%s:/hpTGenTrueRebinned_Eta",inFileName_Eta_DATA_Pass7));
    hpTGenTrueRebinnedEta_Pass7->SetMarkerStyle(24);
    hpTGenTrueRebinnedEta_Pass7->SetMarkerColor(kBlue+2);
    hpTGenTrueRebinnedEta_Pass7->SetLineColor(kBlue+2);
    hpTGenTrueRebinnedEta_Pass7->Scale(1./nEventsData_Pass7);


         
    TLegend* legendEffStudy = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEffStudy->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendEffStudy->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEffStudy->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legendEffStudy->AddEntry(hpTRecTruePi0_Pass6, "rec. true, PASS 6", "lp");
    legendEffStudy->AddEntry(hpTGenTrueRebinnedPi0_Pass6, "gen. true, PASS 6", "lp");
    legendEffStudy->AddEntry(hpTRecTruePi0_Pass7, "rec. true, PASS 7", "lp");
    legendEffStudy->AddEntry(hpTGenTrueRebinnedPi0_Pass7, "gen. true, PASS 7", "lp");
    TObjArray* arrEffStudy = new TObjArray();
        hpTRecTruePi0_Pass6->GetYaxis()->SetRangeUser(1e-7, 10.);
    hpTRecTruePi0_Pass6->GetXaxis()->SetRangeUser(0.4, 10.);
    arrEffStudy->Add(hpTRecTruePi0_Pass6);
    arrEffStudy->Add(hpTGenTrueRebinnedPi0_Pass6);
    arrEffStudy->Add(hpTRecTruePi0_Pass7);
    arrEffStudy->Add(hpTGenTrueRebinnedPi0_Pass7);
    arrEffStudy->Add(legendEffStudy);
    
    TCanvas* canEffStudy = (TCanvas*)makeCanvas(arrEffStudy, 0, "CMYK|NoTime|LogX|LogY", 0, 0);
    canEffStudy->SaveAs("Plots/EffStudyPi0.png");


    TLegend* legendEffStudyEta = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEffStudyEta->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legendEffStudyEta->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEffStudyEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legendEffStudyEta->AddEntry(hpTRecTrueEta_Pass6, "rec. true, PASS 6", "lp");
    legendEffStudyEta->AddEntry(hpTGenTrueRebinnedEta_Pass6, "gen. true, PASS 6", "lp");
    legendEffStudyEta->AddEntry(hpTRecTrueEta_Pass7, "rec. true, PASS 7", "lp");
    legendEffStudyEta->AddEntry(hpTGenTrueRebinnedEta_Pass7, "gen. true, PASS 7", "lp");
    TObjArray* arrEffStudyEta = new TObjArray();
    hpTRecTrueEta_Pass6->GetYaxis()->SetRangeUser(1e-7, 10.);
    hpTRecTrueEta_Pass6->GetXaxis()->SetRangeUser(0.4, 10.);
    arrEffStudyEta->Add(hpTRecTrueEta_Pass6);
    arrEffStudyEta->Add(hpTGenTrueRebinnedEta_Pass6);
    arrEffStudyEta->Add(hpTRecTrueEta_Pass7);
    arrEffStudyEta->Add(hpTGenTrueRebinnedEta_Pass7);
    arrEffStudyEta->Add(legendEffStudyEta);
    
    TCanvas* canEffStudyEta = (TCanvas*)makeCanvas(arrEffStudyEta, 0, "CMYK|NoTime|LogX|LogY", 0, 0);
    canEffStudyEta->SaveAs("Plots/EffStudyEta.png");

    ///Efficiencies pass6
    TH1D* effPi0Run2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/EfficiencyPi0_INT7"));
    SetHistogramPropertiesAlt(effPi0Run2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
    effPi0Run2->SetMarkerStyle(32);
    TH1D* mesonAccXEffxBRTruePi0_Pass6 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", inFileName_Pi0_DATA_Pass6));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0_Pass6, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
    mesonAccXEffxBRTruePi0_Pass6->SetMarkerStyle(4);

    TH1D* effEtaRun2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/EfficiencyEta_INT7"));
    SetHistogramPropertiesAlt(effEtaRun2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
    effEtaRun2->SetMarkerStyle(32);
    TH1D* mesonAccXEffxBRTrueEta_Pass6 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", inFileName_Eta_DATA_Pass6));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTrueEta_Pass6, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
    mesonAccXEffxBRTrueEta_Pass6->SetMarkerStyle(4);

    ///Efficiencies pass7
    TH1D* mesonAccXEffxBRTruePi0_Pass7 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", inFileName_Pi0_DATA_Pass7));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTruePi0_Pass7, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);

    TH1D* mesonAccXEffxBRTrueEta_Pass7 = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", inFileName_Eta_DATA_Pass7));
    SetHistogramPropertiesAlt(mesonAccXEffxBRTrueEta_Pass7, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);




    TLegend* legendEfficiencies = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legendEfficiencies->AddEntry((TObject*)0x0,"pp collisions", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"PCMPCM", "");
    legendEfficiencies->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTruePi0_Pass6, "#pi^{0} 13.6 TeV, pass6", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTrueEta_Pass6, "#eta 13.6 TeV, pass6", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTruePi0_Pass7, "#pi^{0} 13.6 TeV, pass7", "lp");
    legendEfficiencies->AddEntry(mesonAccXEffxBRTrueEta_Pass7, "#eta 13.6 TeV, pass7", "lp");
    legendEfficiencies->AddEntry(effPi0Run2, "#pi^{0} 13 TeV", "lp");
    legendEfficiencies->AddEntry(effEtaRun2, "#eta 13 TeV", "lp");


     TObjArray* arrEfficiencies = new TObjArray();
     arrEfficiencies->Add(effPi0Run2);
     arrEfficiencies->Add(effEtaRun2);
     arrEfficiencies->Add(mesonAccXEffxBRTruePi0_Pass6);
     arrEfficiencies->Add(mesonAccXEffxBRTrueEta_Pass6);
     arrEfficiencies->Add(mesonAccXEffxBRTruePi0_Pass7);
     arrEfficiencies->Add(mesonAccXEffxBRTrueEta_Pass7);

     arrEfficiencies->Add(legendEfficiencies);
     TCanvas* canEff = (TCanvas*)makeCanvas(arrEfficiencies, 0, "CMYK|NoTime|LogX|LogY", 0, 0);
     canEff->SaveAs("Plots/efficiencies.png");

    ///*************************************************


}