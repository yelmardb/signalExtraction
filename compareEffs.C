#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>
#include "includes/inputData.h"




void compareEffs(){

  Int_t runnNumber_LHC24f3c = 348247;
  Int_t runnNumber_LHC24f3c_NoMCTunedOnData = 337539;
  Int_t runNumber_LHC24f3bFullStats = 300332; //pass7 with cut variations
  cout << "debugging, line" << __LINE__ << endl;
  const char* trainOutputMC_LHC24f3c = Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root", runnNumber_LHC24f3c);
  const char* trainOutputMC_LHC24f3c_NoMCTunedOnData =  Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);

  const char* signalExtractionMC_Pi0_LHC24f3c = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_MC_%d.root", runnNumber_LHC24f3c);
  const char* signalExtractionTrueMC_Pi0_LHC24f3c = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_TrueMC_%d.root", runnNumber_LHC24f3c);

  const char* signalExtractionMC_Eta_LHC24f3c = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_MC_%d.root", runnNumber_LHC24f3c);
  const char* signalExtractionTrueMC_Eta_LHC24f3c = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_TrueMC_%d.root", runnNumber_LHC24f3c);

  const char* signalExtractionMC_Pi0_LHC24f3c_NoMCTunedOnData = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_MC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);
  const char* signalExtractionTrueMC_Pi0_LHC24f3c_NoMCTunedOnData = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_TrueMC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);

  const char* signalExtractionMC_Eta_LHC24f3c_NoMCTunedOnData = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_MC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);
  const char* signalExtractionTrueMC_Eta_LHC24f3c_NoMCTunedOnData = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_TrueMC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);


  const char* signalExtractionMC_Pi0_LHC24f3bFullStats = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_MC_%d.root", runNumber_LHC24f3bFullStats);
  const char* signalExtractionTrueMC_Pi0_LHC24f3bFullStats = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_TrueMC_%d.root", runNumber_LHC24f3bFullStats);

  const char* signalExtractionMC_Eta_LHC24f3bFullStats = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_MC_%d.root", runNumber_LHC24f3bFullStats);
  const char* signalExtractionTrueMC_Eta_LHC24f3bFullStats = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_TrueMC_%d.root", runNumber_LHC24f3bFullStats);

  cout << "debugging, line" << __LINE__ << endl;
  ///Event selection
  TH1D* hCollisionCounter_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", signalExtractionMC_Pi0_LHC24f3c));
  SetHistogramPropertiesAlt(hCollisionCounter_LHC24f3c_NoMCTunedOnData, "", "events", 0);
  hCollisionCounter_LHC24f3c_NoMCTunedOnData->GetYaxis()->SetRangeUser(1e5, 1e10);
  hCollisionCounter_LHC24f3c_NoMCTunedOnData->SetLineWidth(4);
  cout << "debugging, line" << __LINE__ << endl;
  TH1D* hCollisionCounter_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", signalExtractionMC_Pi0_LHC24f3c_NoMCTunedOnData));
  SetHistogramPropertiesAlt(hCollisionCounter_LHC24f3c, "", "events", 1);
  hCollisionCounter_LHC24f3c->GetYaxis()->SetRangeUser(1e5, 1e10);
  hCollisionCounter_LHC24f3c->SetLineWidth(4);
  cout << "debugging, line" << __LINE__ << endl;
  TH1D* hCollisionCounter_LHC24f3bFullStats = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", signalExtractionMC_Pi0_LHC24f3bFullStats));
  cout << "debugging, line" << __LINE__ << endl;
  SetHistogramPropertiesAlt(hCollisionCounter_LHC24f3bFullStats, "", "events", 14);
  hCollisionCounter_LHC24f3bFullStats->GetYaxis()->SetRangeUser(1e5, 1e10);
  hCollisionCounter_LHC24f3bFullStats->SetLineWidth(4);
  cout << "debugging, line" << __LINE__ << endl;
  TLegend* legendEvents = new TLegend(0.528249,0.672012,0.805085,0.87172);
  legendEvents->AddEntry((TObject*)0x0,"pp collisions", "");
  legendEvents->AddEntry((TObject*)0x0,"PCMPCM", "");
  legendEvents->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
  legendEvents->AddEntry(hCollisionCounter_LHC24f3c, "LHC24f3c", "lp");
  legendEvents->AddEntry(hCollisionCounter_LHC24f3c_NoMCTunedOnData, "LHC24f3c_NoMCTunedOnData", "lp");
  legendEvents->AddEntry(hCollisionCounter_LHC24f3bFullStats, "LHC24f3b Full statistics", "lp");

  cout << "debugging, line" << __LINE__ << endl;
  TObjArray* arrEvents = new TObjArray();
  arrEvents->Add(hCollisionCounter_LHC24f3c_NoMCTunedOnData);
  arrEvents->Add(hCollisionCounter_LHC24f3c);
  arrEvents->Add(hCollisionCounter_LHC24f3bFullStats);
  arrEvents->Add(legendEvents);
  TCanvas* canEvents = (TCanvas*)makeCanvas(arrEvents, 0, "CMYK|NoTime|LogY|", 0, 0);
  
  cout << "debugging, line" << __LINE__ << endl;
  ///****************************************************************** Meson efficiencies  ************************************************************************************* */
  Double_t branchingRatioPi0 = 0.98823;
  Double_t branchingRatioEta = 0.3941;

  ///Run 2 efficiencies
  TH1D* effPi0Run2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/EfficiencyPi0_INT7"));
  SetHistogramPropertiesAlt(effPi0Run2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
  effPi0Run2->SetMarkerStyle(8);
  effPi0Run2->GetXaxis()->SetRangeUser(0.4, 10);
  effPi0Run2->Scale(branchingRatioPi0);
  TH1D* effEtaRun2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/EfficiencyEta_INT7"));
  SetHistogramPropertiesAlt(effEtaRun2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
  effEtaRun2->SetMarkerStyle(22);
  effEtaRun2->Scale(branchingRatioEta);
  cout << "debugging, line" << __LINE__ << endl;

  ///Run 3 efficiencies, 
  ///Pi0
  TH1D* mesonAccXEffxBRTrueMC_Pi0_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", signalExtractionTrueMC_Pi0_LHC24f3c));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
  mesonAccXEffxBRTrueMC_Pi0_LHC24f3c->SetMarkerStyle(20);

  TH1D* mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", signalExtractionTrueMC_Pi0_LHC24f3c_NoMCTunedOnData));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
  mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(4);

  TH1D* mesonAccXEffxBRTrueMC_Pi0_LHC24f3bFullStats = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", signalExtractionTrueMC_Pi0_LHC24f3bFullStats));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Pi0_LHC24f3bFullStats, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 14);
  mesonAccXEffxBRTrueMC_Pi0_LHC24f3bFullStats->SetMarkerStyle(20);



  ///Eta
  TH1D* mesonAccXEffxBRTrueMC_Eta_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", signalExtractionTrueMC_Eta_LHC24f3c));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Eta_LHC24f3c, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
  mesonAccXEffxBRTrueMC_Eta_LHC24f3c->SetMarkerStyle(22);


  TH1D* mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", signalExtractionTrueMC_Eta_LHC24f3c_NoMCTunedOnData));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
  mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(26);

  TH1D* mesonAccXEffxBRTrueMC_Eta_LHC24f3bFullStats = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", signalExtractionTrueMC_Eta_LHC24f3bFullStats));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Eta_LHC24f3bFullStats, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 14);
  mesonAccXEffxBRTrueMC_Eta_LHC24f3bFullStats->SetMarkerStyle(22);  

  ///Ratios
  TH1D* effRatios_Pi0_LHC24f3c = (TH1D*)mesonAccXEffxBRTrueMC_Pi0_LHC24f3c->Clone("effRatios_Pi0");
  effRatios_Pi0_LHC24f3c->Divide(mesonAccXEffxBRTrueMC_Pi0_LHC24f3bFullStats);
  effRatios_Pi0_LHC24f3c->GetYaxis()->SetTitle("ratio to LHC24f3b");
  effRatios_Pi0_LHC24f3c->GetYaxis()->SetRangeUser(0.9, 1.1);

  TH1D* effRatios_Pi0_LHC24f3c_NoMCTunedOnData = (TH1D*)mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData->Clone("effRatios_Pi0");
  effRatios_Pi0_LHC24f3c_NoMCTunedOnData->Divide(mesonAccXEffxBRTrueMC_Pi0_LHC24f3bFullStats);
  effRatios_Pi0_LHC24f3c_NoMCTunedOnData->GetYaxis()->SetTitle("ratio to LHC24f3c_NoMCTunedOnData");
  effRatios_Pi0_LHC24f3c_NoMCTunedOnData->GetYaxis()->SetRangeUser(0.9, 1.1);

  TH1D* effRatios_Eta_LHC24f3c = (TH1D*)mesonAccXEffxBRTrueMC_Eta_LHC24f3c->Clone("effRatios_Eta");
  effRatios_Eta_LHC24f3c->Divide(mesonAccXEffxBRTrueMC_Eta_LHC24f3bFullStats);
  effRatios_Eta_LHC24f3c->GetYaxis()->SetTitle("ratio to LHC24f3b");
  effRatios_Eta_LHC24f3c->GetYaxis()->SetRangeUser(0.9, 1.1);

  TH1D* effRatios_Eta_LHC24f3c_NoMCTunedOnData = (TH1D*)mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData->Clone("effRatios_Eta");
  effRatios_Eta_LHC24f3c_NoMCTunedOnData->Divide(mesonAccXEffxBRTrueMC_Eta_LHC24f3bFullStats);
  effRatios_Eta_LHC24f3c_NoMCTunedOnData->GetYaxis()->SetTitle("ratio to LHC24f3c_NoMCTunedOnData");
  effRatios_Eta_LHC24f3c_NoMCTunedOnData->GetYaxis()->SetRangeUser(0.9, 1.1);


  TLegend* legendEffs = new TLegend(0.528249,0.672012,0.805085,0.87172);
  legendEffs->AddEntry((TObject*)0x0,"pp collisions", "");
  legendEffs->AddEntry((TObject*)0x0,"PCMPCM", "");
  legendEffs->AddEntry((TObject*)0x0,"#pi^{0} / #eta #rightarrow #gamma + #gamma", "");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Pi0_LHC24f3bFullStats, "#pi^{0} LHC24f3b", "lp");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c, "#pi^{0} LHC24f3c", "lp");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData, "#pi^{0} LHC24f3c_NoMCTunedOnData", "lp");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Eta_LHC24f3bFullStats, "#eta LHC24f3b", "lp");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Eta_LHC24f3c, "#eta LHC24f3c", "lp");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData, "#eta LHC24f3c_NoMCTunedOnData", "lp");
  legendEffs->AddEntry(effPi0Run2, "#pi^{0} Run 2", "lp");
  legendEffs->AddEntry(effEtaRun2, "#eta Run 2", "lp");
  cout << "debugging, line" << __LINE__ << endl;

  TObjArray* arrEffs = new TObjArray();
  arrEffs->Add(effPi0Run2);
  arrEffs->Add(effEtaRun2);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Eta_LHC24f3bFullStats);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Eta_LHC24f3c);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Pi0_LHC24f3bFullStats);
  arrEffs->Add(legendEffs);

  TObjArray* arrRatioEffs = new TObjArray();
  arrRatioEffs->Add(effRatios_Pi0_LHC24f3c);
  arrRatioEffs->Add(effRatios_Pi0_LHC24f3c_NoMCTunedOnData);
  arrRatioEffs->Add(effRatios_Eta_LHC24f3c);
  arrRatioEffs->Add(effRatios_Eta_LHC24f3c_NoMCTunedOnData);
  
  TCanvas* canEffs = (TCanvas*)makeCanvas(arrEffs, arrRatioEffs, "CMYK|NoTime|LogX|LogY", 0, 0);

  cout << "debugging, line" << __LINE__ << endl;
}