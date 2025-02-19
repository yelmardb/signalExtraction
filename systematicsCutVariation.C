#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "includes/inputData.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>

void SetCrossSection(TH1D* hist, Int_t binColor, Int_t markerStyle);
void PlotCrossSecctions(TH1D* hCrossSectionPi0, TH1D* hCrossSectionEta);
std::pair<TH1D*, TH1D*> sysUncPi0AndEta(std::vector<const char*> cutModesVars);


void systematicsCutVariation(){


    std::vector<const char*> cutModeVar_max_alpha_ap = {"cutMode101", "cutMode102"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_max_alpha_ap = sysUncPi0AndEta(cutModeVar_max_alpha_ap);
    TH1D* sysUnc_max_alpha_ap_Pi0 = sysUncPi0Eta_max_alpha_ap.first;
    TH1D* sysUnc_max_alpha_ap_Eta = sysUncPi0Eta_max_alpha_ap.second;

    std::vector<const char*> cutModeVar_max_chi2its = {"cutMode103", "cutMode104"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_max_chi2its = sysUncPi0AndEta(cutModeVar_max_chi2its);
    TH1D* sysUnc_max_chi2its_Pi0 = sysUncPi0Eta_max_chi2its.first;
    TH1D* sysUnc_max_chi2its_Eta = sysUncPi0Eta_max_chi2its.second;

    std::vector<const char*> cutModeVar_max_chi2tpc = {"cutMode105", "cutMode106"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_max_chi2tpc = sysUncPi0AndEta(cutModeVar_max_chi2tpc);
    TH1D* sysUnc_max_chi2tpc_Pi0 = sysUncPi0Eta_max_chi2tpc.first;
    TH1D* sysUnc_max_chi2tpc_Eta = sysUncPi0Eta_max_chi2tpc.second;

    std::vector<const char*> cutModeVar_max_eta_v0 = {"cutMode107"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_max_eta_v0 = sysUncPi0AndEta(cutModeVar_max_eta_v0);
    TH1D* sysUnc_max_eta_v0_Pi0 = sysUncPi0Eta_max_eta_v0.first;
    TH1D* sysUnc_max_eta_v0_Eta = sysUncPi0Eta_max_eta_v0.second;
        
    std::vector<const char*> cutModeVar_max_pca = {"cutMode108", "cutMode109"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_max_pca = sysUncPi0AndEta(cutModeVar_max_pca);
    TH1D* sysUnc_max_pca_Pi0 = sysUncPi0Eta_max_pca.first;
    TH1D* sysUnc_max_pca_Eta = sysUncPi0Eta_max_pca.second;
    
    std::vector<const char*> cutModeVar_max_qt_ap = {"cutMode110", "cutMode111"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_max_qt_ap = sysUncPi0AndEta(cutModeVar_max_qt_ap);
    TH1D* sysUnc_max_qt_ap_Pi0 = sysUncPi0Eta_max_qt_ap.first;
    TH1D* sysUnc_max_qt_ap_Eta = sysUncPi0Eta_max_qt_ap.second;

    std::vector<const char*> cutModeVar_max_TPCNsigmaEl = {"cutMode112", "cutMode113"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_max_TPCNsigmaEl = sysUncPi0AndEta(cutModeVar_max_TPCNsigmaEl);
    TH1D* sysUnc_max_TPCNsigmaEl_Pi0 = sysUncPi0Eta_max_TPCNsigmaEl.first;
    TH1D* sysUnc_max_TPCNsigmaEl_Eta = sysUncPi0Eta_max_TPCNsigmaEl.second;


    std::vector<const char*> cutModeVar_max_max_v0radius = {"cutMode114", "cutMode115"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_max_max_v0radius = sysUncPi0AndEta(cutModeVar_max_max_v0radius);
    TH1D* sysUnc_max_max_v0radius_Pi0 = sysUncPi0Eta_max_max_v0radius.first;
    TH1D* sysUnc_max_max_v0radius_Eta = sysUncPi0Eta_max_max_v0radius.second;

    std::vector<const char*> cutModeVar_min_cospa= {"cutMode116", "cutMode117"};
    std::pair<TH1D*, TH1D*> sysUncPi0Eta_min_cospa = sysUncPi0AndEta(cutModeVar_min_cospa);
    TH1D* sysUnc_min_cospa_Pi0 = sysUncPi0Eta_min_cospa.first;
    TH1D* sysUnc_min_cospa_Eta = sysUncPi0Eta_min_cospa.second;

    // std::vector<const char*> cutModeVar_min_ncluster_tpc= {"cutMode118", "cutMode119"};
    // std::pair<TH1D*, TH1D*> sysUncPi0Eta_min_ncluster_tpc = sysUncPi0AndEta(cutModeVar_min_ncluster_tpc);
    // TH1D* sysUnc_min_ncluster_tpc_Pi0 = sysUncPi0Eta_min_ncluster_tpc.first;
    // TH1D* sysUnc_min_ncluster_tpc_Eta = sysUncPi0Eta_min_ncluster_tpc.second;

    // std::vector<const char*> cutModeVar_min_ncrossedrows= {"cutMode120", "cutMode121"};
    // std::pair<TH1D*, TH1D*> sysUncPi0Eta_min_ncrossedrows = sysUncPi0AndEta(cutModeVar_min_ncrossedrows);
    // TH1D* sysUnc_min_ncrossedrows_Pi0 = sysUncPi0Eta_min_ncrossedrows.first;
    // TH1D* sysUnc_min_ncrossedrows_Eta = sysUncPi0Eta_min_ncrossedrows.second;

    // std::vector<const char*> cutModeVar_min_TPCNsigmaEl = {"cutMode122", "cutMode123"};
    // std::pair<TH1D*, TH1D*> sysUncPi0Eta_min_TPCNsigmaEl = sysUncPi0AndEta(cutModeVar_min_TPCNsigmaEl);
    // TH1D* sysUnc_min_TPCNsigmaEl_Pi0 = sysUncPi0Eta_min_TPCNsigmaEl.first;
    // TH1D* sysUnc_min_TPCNsigmaEl_Eta = sysUncPi0Eta_min_TPCNsigmaEl.second;

    // std::vector<const char*> cutModeVar_min_v0radius = {"cutMode124", "cutMode125"};
    // // std::pair<TH1D*, TH1D*> sysUncPi0Eta_min_v0radius = sysUncPi0AndEta(cutModeVar_min_v0radius);
    // TH1D* sysUnc_min_v0radius_Pi0 = sysUncPi0Eta_min_v0radius.first;
    // TH1D* sysUnc_min_v0radius_Eta = sysUncPi0Eta_min_v0radius.second;


    TH1D* SysUnc_CombinedUncerPi0 = (TH1D*)sysUnc_min_cospa_Pi0->Clone("SysUnc_CombinedUncerPi0");
    TH1D* SysUnc_CombinedUncerEta = (TH1D*)sysUnc_min_cospa_Eta->Clone("SysUnc_CombinedUncerEta");
    
    for(int i = sysUnc_max_alpha_ap_Pi0->GetBin(0); i <= sysUnc_max_alpha_ap_Pi0->GetNbinsX(); ++i)
    {
        Double_t cont_max_alpha_ap_Pi0 = sysUnc_max_alpha_ap_Pi0->GetBinContent(i);
        Double_t cont_max_alpha_ap_Eta = sysUnc_max_alpha_ap_Eta->GetBinContent(i);
        Double_t cont_max_qt_ap_Pi0 = sysUnc_max_qt_ap_Pi0->GetBinContent(i);
        Double_t cont_max_qt_ap_Eta = sysUnc_max_qt_ap_Eta->GetBinContent(i);
        Double_t cont_max_TPCNsigmaEl_Pi0 = sysUnc_max_TPCNsigmaEl_Pi0->GetBinContent(i);
        Double_t cont_max_TPCNsigmaEl_Eta = sysUnc_max_TPCNsigmaEl_Eta->GetBinContent(i);
        Double_t cont_max_max_v0radius_Pi0 = sysUnc_max_max_v0radius_Pi0->GetBinContent(i);
        Double_t cont_max_max_v0radius_Eta = sysUnc_max_max_v0radius_Eta->GetBinContent(i);
        Double_t cont_min_cospa_Pi0 = sysUnc_min_cospa_Pi0->GetBinContent(i);
        Double_t cont_min_cospa_Eta = sysUnc_min_cospa_Eta->GetBinContent(i);
        // Double_t cont_min_ncluster_tpc_Pi0 = sysUnc_min_ncluster_tpc_Pi0->GetBinContent(i);
        // Double_t cont_min_ncluster_tpc_Eta = sysUnc_min_ncluster_tpc_Eta->GetBinContent(i);
        // Double_t cont_min_ncrossedrows_Pi0 = sysUnc_min_ncrossedrows_Pi0->GetBinContent(i);
        // Double_t cont_min_ncrossedrows_Eta = sysUnc_min_ncrossedrows_Eta->GetBinContent(i);
        // Double_t cont_min_TPCNsigmaEl_Pi0 = sysUnc_min_TPCNsigmaEl_Pi0->GetBinContent(i);
        // Double_t cont_min_TPCNsigmaEl_Eta = sysUnc_min_TPCNsigmaEl_Eta->GetBinContent(i);
        // Double_t cont_min_v0radius_Pi0 = sysUnc_min_v0radius_Pi0->GetBinContent(i);
        // Double_t cont_min_v0radius_Eta = sysUnc_min_v0radius_Eta->GetBinContent(i);

        Double_t cont_CombinedUncerPi0 = sqrt( pow(cont_max_alpha_ap_Pi0, 2.0) + pow(cont_max_qt_ap_Pi0, 2.0) + pow(cont_max_TPCNsigmaEl_Pi0, 2.0) 
                                            + pow(cont_max_max_v0radius_Pi0, 2.0) + pow(cont_min_cospa_Pi0, 2.0));
                                            //+ pow(cont_min_ncluster_tpc_Pi0, 2.0)
                                            //+ pow(cont_min_ncrossedrows_Pi0, 2.0) + pow(cont_min_TPCNsigmaEl_Pi0, 2.0) + pow(cont_min_v0radius_Pi0, 2.0));
        
        if(cont_CombinedUncerPi0 != 0.0) SysUnc_CombinedUncerPi0->SetBinContent(i, cont_CombinedUncerPi0);

        Double_t cont_CombinedUncerEta = sqrt( pow(cont_max_alpha_ap_Eta, 2.0) + pow(cont_max_qt_ap_Eta, 2.0) + pow(cont_max_TPCNsigmaEl_Eta, 2.0) 
                                            + pow(cont_max_max_v0radius_Eta, 2.0) + pow(cont_min_cospa_Eta, 2.0));
                                            //+ pow(cont_min_ncluster_tpc_Eta, 2.0)
                                            //+ pow(cont_min_ncrossedrows_Eta, 2.0) + pow(cont_min_TPCNsigmaEl_Eta, 2.0) + pow(cont_min_v0radius_Eta, 2.0));
        
        if(cont_CombinedUncerEta != 0.0) SysUnc_CombinedUncerEta->SetBinContent(i, cont_CombinedUncerEta);
    }



    SetHistogramPropertiesAlt(SysUnc_CombinedUncerPi0, "", "sys. Uncertainty (%)", 13); 
    SetHistogramPropertiesAlt(sysUnc_max_alpha_ap_Pi0, "", "sys. Uncertainty (%)", 0);
    SetHistogramPropertiesAlt(sysUnc_max_qt_ap_Pi0, "", "sys. Uncertainty (%)", 1);
    SetHistogramPropertiesAlt(sysUnc_max_TPCNsigmaEl_Pi0, "", "sys. Uncertainty (%)", 2);
    SetHistogramPropertiesAlt(sysUnc_max_max_v0radius_Pi0, "", "sys. Uncertainty (%)", 3);
    SetHistogramPropertiesAlt(sysUnc_min_cospa_Pi0, "", "sys. Uncertainty (%)", 4);
    // SetHistogramPropertiesAlt(sysUnc_min_ncluster_tpc_Pi0, "", "sys. Uncertainty (%)", 5);
    // SetHistogramPropertiesAlt(sysUnc_min_ncrossedrows_Pi0, "", "sys. Uncertainty (%)", 6);
    // SetHistogramPropertiesAlt(sysUnc_min_TPCNsigmaEl_Pi0, "", "sys. Uncertainty (%)", 7);
    // SetHistogramPropertiesAlt(sysUnc_min_v0radius_Pi0, "", "sys. Uncertainty (%)", 8);


    SetHistogramPropertiesAlt(SysUnc_CombinedUncerEta, "", "sys. Uncertainty (%)", 13); 
    SetHistogramPropertiesAlt(sysUnc_max_alpha_ap_Eta, "", "sys. Uncertainty (%)", 0);
    SetHistogramPropertiesAlt(sysUnc_max_qt_ap_Eta, "", "sys. Uncertainty (%)", 1);
    SetHistogramPropertiesAlt(sysUnc_max_TPCNsigmaEl_Eta, "", "sys. Uncertainty (%)", 2);
    SetHistogramPropertiesAlt(sysUnc_max_max_v0radius_Eta, "", "sys. Uncertainty (%)", 3);
    // SetHistogramPropertiesAlt(sysUnc_min_cospa_Eta, "", "sys. Uncertainty (%)", 4);
    // SetHistogramPropertiesAlt(sysUnc_min_ncluster_tpc_Eta, "", "sys. Uncertainty (%)", 5);
    // SetHistogramPropertiesAlt(sysUnc_min_ncrossedrows_Eta, "", "sys. Uncertainty (%)", 6);
    // SetHistogramPropertiesAlt(sysUnc_min_TPCNsigmaEl_Eta, "", "sys. Uncertainty (%)", 7);
    // SetHistogramPropertiesAlt(sysUnc_min_v0radius_Eta, "", "sys. Uncertainty (%)", 8);


    TLegend* legSysUncPi0 = new TLegend(0.457627,0.653001,0.707627,0.903367);
    legSysUncPi0->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legSysUncPi0->AddEntry((TObject*)0x0,"PCMPCM", "");
    legSysUncPi0->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legSysUncPi0->AddEntry(SysUnc_CombinedUncerPi0, "Combined uncertainty", "lp");
    legSysUncPi0->AddEntry(sysUnc_max_alpha_ap_Pi0, "max #alpha_{ap}", "lp");
    legSysUncPi0->AddEntry(sysUnc_max_qt_ap_Pi0, "max q_{T}^{ap}", "lp");
    legSysUncPi0->AddEntry(sysUnc_max_TPCNsigmaEl_Pi0, "max TPCNsigmaEl", "lp");
    legSysUncPi0->AddEntry(sysUnc_max_max_v0radius_Pi0, "max maxv0radius", "lp");
    legSysUncPi0->AddEntry(sysUnc_min_cospa_Pi0, "min cos#theta_{pa}", "lp");
    // legSysUncPi0->AddEntry(sysUnc_min_ncluster_tpc_Pi0, "min ncluster_{TPC}", "lp");
    // legSysUncPi0->AddEntry(sysUnc_min_ncrossedrows_Pi0, "min nCrossedRows_{TPC}", "lp");
    // legSysUncPi0->AddEntry(sysUnc_min_TPCNsigmaEl_Pi0, "min TPCNsigmaEl", "lp");
    // legSysUncPi0->AddEntry(sysUnc_min_v0radius_Pi0, "min v0radius", "lp");

    TLegend* legSysUncEta = new TLegend(0.457627,0.653001,0.707627,0.903367);
    legSysUncEta->AddEntry((TObject*)0x0,"pp collisions at #sqrt{s} = 13.6 TeV", "");
    legSysUncEta->AddEntry((TObject*)0x0,"PCMPCM", "");
    legSysUncEta->AddEntry((TObject*)0x0,"#eta #rightarrow #gamma + #gamma", "");
    legSysUncEta->AddEntry(SysUnc_CombinedUncerEta, "Combined uncertainty", "lp");
    legSysUncEta->AddEntry(sysUnc_max_alpha_ap_Eta, "max #alpha_{ap}", "lp");
    legSysUncEta->AddEntry(sysUnc_max_qt_ap_Eta, "max q_{T}^{ap}", "lp");
    legSysUncEta->AddEntry(sysUnc_max_TPCNsigmaEl_Eta, "max TPCNsigmaEl", "lp");
    legSysUncEta->AddEntry(sysUnc_max_max_v0radius_Eta, "max maxv0radius", "lp");
    legSysUncEta->AddEntry(sysUnc_min_cospa_Eta, "min cos#theta_{pa}", "lp");
    // legSysUncEta->AddEntry(sysUnc_min_ncluster_tpc_Eta, "min ncluster_{TPC}", "lp");
    // legSysUncEta->AddEntry(sysUnc_min_ncrossedrows_Eta, "min nCrossedRows_{TPC}", "lp");
    // legSysUncEta->AddEntry(sysUnc_min_TPCNsigmaEl_Eta, "min TPCNsigmaEl", "lp");
    // legSysUncEta->AddEntry(sysUnc_min_v0radius_Eta, "min v0radius", "lp");


    TObjArray* arrSysUnc_Pi0 = new TObjArray();
    arrSysUnc_Pi0->Add(SysUnc_CombinedUncerPi0);
    arrSysUnc_Pi0->Add(sysUnc_max_alpha_ap_Pi0);
    arrSysUnc_Pi0->Add(sysUnc_max_qt_ap_Pi0);
    arrSysUnc_Pi0->Add(sysUnc_max_TPCNsigmaEl_Pi0);
    arrSysUnc_Pi0->Add(sysUnc_max_max_v0radius_Pi0);
    arrSysUnc_Pi0->Add(sysUnc_min_cospa_Pi0);
    // arrSysUnc_Pi0->Add(sysUnc_min_ncluster_tpc_Pi0);
    // arrSysUnc_Pi0->Add(sysUnc_min_ncrossedrows_Pi0);
    // arrSysUnc_Pi0->Add(sysUnc_min_TPCNsigmaEl_Pi0);
    // arrSysUnc_Pi0->Add(sysUnc_min_v0radius_Pi0);
    arrSysUnc_Pi0->Add(legSysUncPi0);

    TObjArray* arrSysUnc_Eta = new TObjArray();
    arrSysUnc_Eta->Add(SysUnc_CombinedUncerEta);
    arrSysUnc_Eta->Add(sysUnc_max_alpha_ap_Eta);
    arrSysUnc_Eta->Add(sysUnc_max_qt_ap_Eta);
    arrSysUnc_Eta->Add(sysUnc_max_TPCNsigmaEl_Eta);
    arrSysUnc_Eta->Add(sysUnc_max_max_v0radius_Eta);
    arrSysUnc_Eta->Add(sysUnc_min_cospa_Eta);
    //arrSysUnc_Eta->Add(sysUnc_min_ncluster_tpc_Eta);
    // arrSysUnc_Eta->Add(sysUnc_min_ncrossedrows_Eta);
    // arrSysUnc_Eta->Add(sysUnc_min_TPCNsigmaEl_Eta);
    // arrSysUnc_Eta->Add(sysUnc_min_v0radius_Eta);
    arrSysUnc_Eta->Add(legSysUncEta);

    TCanvas* canSysUncPi0 = (TCanvas*)makeCanvas(arrSysUnc_Pi0, 0, "LogX|Square|CMYK|NoTime", 0, 0);
    TCanvas* canSysUncEta = (TCanvas*)makeCanvas(arrSysUnc_Eta, 0, "LogX|Square|CMYK|NoTime", 0, 0);
}


std::pair<TH1D*, TH1D*> sysUncPi0AndEta(std::vector<const char*> cutModesVars){

    const char* inFileName_Pi0_DATA_Base = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_Data_%d.root", period, runNumberData));
    const char* inFileName_Pi0_MC_Base = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_MC_%d.root", period, runNumberMC));
    const char* inFileName_Pi0_TrueMC_Base =  strdup(Form("outputSignalExtraction/%s/SignalExtraction_Pi0_TrueMC_%d.root", period, runNumberMC));

    const char* inFileName_Eta_DATA_Base = strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_Data_%d.root", period, runNumberData));
    const char* inFileName_Eta_MC_Base =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_MC_%d.root", period, runNumberMC));
    const char* inFileName_Eta_TrueMC_Base =strdup(Form("outputSignalExtraction/%s/SignalExtraction_Eta_TrueMC_%d.root", period, runNumberTrueMC));

    TH1D* hCrossSectionPi0Base =  (TH1D*)extractHistogram(Form("%s:/hCrossSection",inFileName_Pi0_DATA_Base));
    TH1D* hCrossSectionEtaBase =  (TH1D*)extractHistogram(Form("%s:/hCrossSection",inFileName_Eta_DATA_Base));

    SetCrossSection(hCrossSectionPi0Base, 0, 8);
    SetCrossSection(hCrossSectionEtaBase, 0, 4);     


    std::vector<TH1D*> vecRatiosCutVarsToBasePi0;
    std::vector<TH1D*> vecRatiosCutVarsToBaseEta;
    

    for(auto cutMode: cutModesVars){

        if(strcmp(cutMode, "BASE") == 0) continue;

        
        const char* inFileName_Pi0_DATA_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_Data_%d.root", period, cutMode,runNumberData));
        const char* inFileName_Pi0_MC_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_MC_%d.root", period, cutMode,runNumberMC));
        const char* inFileName_Pi0_TrueMC_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Pi0_TrueMC_%d.root", period, cutMode,runNumberTrueMC));

        const char* inFileName_Eta_DATA_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_Data_%d.root", period, cutMode,runNumberData));
        const char* inFileName_Eta_MC_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_MC_%d.root", period, cutMode,runNumberMC));
        const char* inFileName_Eta_TrueMC_cutVar = strdup(Form("outputSignalExtraction/%s/%s/SignalExtraction_Eta_TrueMC_%d.root", period, cutMode,runNumberTrueMC));  


        TH1D* hCrossSectionPi0_cutVar =  (TH1D*)extractHistogram(Form("%s:/hCrossSection",inFileName_Pi0_DATA_cutVar));
        TH1D* hCrossSectionEta_cutVar =  (TH1D*)extractHistogram(Form("%s:/hCrossSection",inFileName_Eta_DATA_cutVar));

        SetCrossSection(hCrossSectionPi0_cutVar, 0, 8);
        SetCrossSection(hCrossSectionEta_cutVar, 0, 4);     

        TH1D* ratioXSectionToBasePi0 = (TH1D*)MakeRatioSpectra(hCrossSectionPi0_cutVar, hCrossSectionPi0Base, "quiet|B");
        ratioXSectionToBasePi0->SetName(Form("ratioXSection_%sToBasePi0", cutMode));
        vecRatiosCutVarsToBasePi0.push_back(ratioXSectionToBasePi0);

        TH1D* ratioXSectionToBaseEta = (TH1D*)MakeRatioSpectra(hCrossSectionEta_cutVar, hCrossSectionEta_cutVar, "quiet|B");
        ratioXSectionToBaseEta->SetName(Form("ratioXSection_%sToBaseEta", cutMode));
        vecRatiosCutVarsToBaseEta.push_back(ratioXSectionToBaseEta);
        
    }

    TH1D* sysUncThisCutPi0;
    TH1D* sysUncThisCutEta;

    if(cutModesVars.size() == 0 || cutModesVars.size()  > 2) {
        cout << "ERROR: no cut var defined" << endl;
    }
    else if(cutModesVars.size() == 1){
        sysUncThisCutPi0 =  MakeHistoSys_Unc(vecRatiosCutVarsToBasePi0[0], vecRatiosCutVarsToBasePi0[0]);    
        sysUncThisCutEta =  MakeHistoSys_Unc(vecRatiosCutVarsToBaseEta[0], vecRatiosCutVarsToBaseEta[0]);    

    }
    else if(cutModesVars.size() == 2){
        sysUncThisCutPi0 =  MakeHistoSys_Unc(vecRatiosCutVarsToBasePi0[0], vecRatiosCutVarsToBasePi0[1]);   
        sysUncThisCutEta =  MakeHistoSys_Unc(vecRatiosCutVarsToBaseEta[0], vecRatiosCutVarsToBaseEta[1]);     
    }


    
    return std::make_pair(sysUncThisCutPi0, sysUncThisCutEta);


}

void SetCrossSection(TH1D* hist, Int_t binColor, Int_t markerStyle){
    SetHistogramPropertiesAlt(hist, "", "", binColor);
    hist->SetMarkerStyle(markerStyle);
    hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hist->SetYTitle("#it{E} #frac{d^{3}#it{#sigma}}{d#it{p}^{3}} (pb GeV^{#minus2} #it{c}^{3})");
    hist->GetXaxis()->SetRangeUser(0.4, 10.);
    hist->GetYaxis()->SetRangeUser(1e4, 2e13);  
    hist->GetXaxis()->SetTitleOffset(0.003);   
}




