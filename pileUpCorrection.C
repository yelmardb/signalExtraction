#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>
#include "includes/inputData.h"




void pileUpCorrection(){
  //*******************************************************************************************************///
    ///Load 


    const char* inFileName_DATA = strdup(Form("../Run3Pi0/data/AnalysisResults2.root"));
    const char* inFileName_TrueMC = strdup(Form("../Run3Pi0/mc/AnalysisResults2.root"));
    std::vector<Double_t> fBinsPt= {0.2, 1.0, 2., 3.0, 4.0, 5.0, 6.0, 10.0}; 

    
    ///Number of Events
    TH1D* hCollisionCounter = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/Event/after/hCollisionCounter",inFileName_TrueMC));
    Double_t nEventsMC = hCollisionCounter->GetBinContent(10);
    cout << "nEventsMC: " << nEventsMC << endl;

    ///pT distributions

    ///DCAz distributions
    //
    THnSparseF* hDCAz_Pt_collType = (THnSparseF*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hDCAz_Pt_collType",inFileName_TrueMC));
    if(!hDCAz_Pt_collType) cout << "WARNING: hDCAz_Pt_collType in MC"<< endl;
    
    TH1D* hDCAz_V0_MC_Validated[fBinsPt.size()];
    TH1D* hDCAz_V0_MC_NotValidated[fBinsPt.size()];
    TH1D* hDCAz_V0_MC_All[fBinsPt.size()];
    TLegend* legend[fBinsPt.size()];
    TObjArray* arr[fBinsPt.size()];
    TCanvas* canDCAz[fBinsPt.size()];

  
    for(Int_t i = 0; i < fBinsPt.size() - 1; i++){
      hDCAz_Pt_collType->GetAxis(1)->SetRangeUser(fBinsPt[i], fBinsPt[i+1]);
      
      hDCAz_Pt_collType->GetAxis(2)->SetRangeUser(0.5, 1.5);
      hDCAz_V0_MC_Validated[i] = (TH1D*)hDCAz_Pt_collType->Projection(0);
      hDCAz_V0_MC_Validated[i]->SetName(Form("hDCAz_V0_MC_Validated_pT_%.2f_%.2f", fBinsPt[i], fBinsPt[i+1]));
      hDCAz_V0_MC_Validated[i]->Rebin(2);
      hDCAz_V0_MC_Validated[i]->Scale(1./nEventsMC);
      

      hDCAz_Pt_collType->GetAxis(2)->SetRangeUser(-0.5, 0.5);
      hDCAz_V0_MC_NotValidated[i] = (TH1D*)hDCAz_Pt_collType->Projection(0);
      hDCAz_V0_MC_NotValidated[i]->SetName(Form("hDCAz_V0_MC_NotValidated_pT_%.2f_%.2f", fBinsPt[i], fBinsPt[i+1]));
      hDCAz_V0_MC_NotValidated[i]->Rebin(2);
      hDCAz_V0_MC_NotValidated[i]->Scale(1./nEventsMC);
      
      hDCAz_Pt_collType->GetAxis(2)->SetRangeUser(-0.5, 1.5);
      hDCAz_V0_MC_All[i] = (TH1D*)hDCAz_Pt_collType->Projection(0);
      hDCAz_V0_MC_All[i]->SetName(Form("hDCAz_V0_MC_All_pT_%.2f_%.2f", fBinsPt[i], fBinsPt[i+1]));
      hDCAz_V0_MC_All[i]->Rebin(2);
      hDCAz_V0_MC_All[i]->Scale(1./nEventsMC);

      
      ///Plotting
      SetHistogramPropertiesAlt(hDCAz_V0_MC_Validated[i], "#it{DCA}_{z} (cm)", "norm. counts", 0);
      SetHistogramPropertiesAlt(hDCAz_V0_MC_NotValidated[i], "#it{DCA}_{z} (cm)", "norm. counts", 1);
      SetHistogramPropertiesAlt(hDCAz_V0_MC_All[i], "#it{DCA}_{z} (cm)", "norm. counts", 14);
      legend[i] = new TLegend(0.7, 0.7, 0.9, 0.9); 
      legend[i]->AddEntry((TObject*)0x0,Form("%.2f< #it{p}_{T} < %.2f GeV/c", fBinsPt[i], fBinsPt[i+1]), "");
      legend[i]->AddEntry(hDCAz_V0_MC_Validated[i], "Validated MC collisions", "lp");
      legend[i]->AddEntry(hDCAz_V0_MC_NotValidated[i], "Invalidated MC collisions", "lp");
      legend[i]->AddEntry(hDCAz_V0_MC_All[i], "All collisions", "lp");
      
      arr[i] = new TObjArray();
      arr[i]->Add(hDCAz_V0_MC_Validated[i]);
      arr[i]->Add(hDCAz_V0_MC_NotValidated[i]);
      arr[i]->Add(hDCAz_V0_MC_All[i]);
      arr[i]->Add(legend[i]); 
      
      canDCAz[i] = (TCanvas*)makeCanvas(arr[i], 0, "CMYK|NoTime LogY ", 0, 0);
      
    }

  }
    
    /*hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(-0.5, 0.5);
    TH1D* hDCAz_ValidatedCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(0);
    SetHistogramPropertiesAlt(hDCAz_ValidatedCollisions, "#it{DCA}_{z} (cm)", "norm. counts", 0);
    hDCAz_ValidatedCollisions->Rebin(2);
    hDCAz_ValidatedCollisions->Scale(1/hDCAz_ValidatedCollisions->Integral());
    hDCAz_ValidatedCollisions->GetXaxis()->SetRangeUser(-5.0, 5.0);

    hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(0.5, 1.5);
    TH1D* hDCAz_InvalidatedCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(0);
    SetHistogramPropertiesAlt(hDCAz_InvalidatedCollisions, "#it{DCA}_{z} (cm)", "norm. counts", 1);
    hDCAz_InvalidatedCollisions->Rebin(2);
    hDCAz_InvalidatedCollisions->Scale(1/hDCAz_InvalidatedCollisions->Integral());
    hDCAz_InvalidatedCollisions->GetXaxis()->SetRangeUser(-5.0, 5.0);

    hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(-0.5, 1.5);
    TH1D* hDCAz_AllCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(0);
    SetHistogramPropertiesAlt(hDCAz_AllCollisions, "#it{DCA}_{z} (cm)", "norm. counts", 2);
    hDCAz_AllCollisions->Rebin(2);
    hDCAz_AllCollisions->Scale(1/hDCAz_AllCollisions->Integral());




    TLegend* legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend2->AddEntry(hDCAz_ValidatedCollisions, "Validated collisions", "lp");
    legend2->AddEntry(hDCAz_InvalidatedCollisions, "Invalidated collisions", "lp");
    legend2->AddEntry(hDCAz_AllCollisions, "All collisions", "lp");

    TObjArray* arr2 = new TObjArray();
    arr2->Add(hDCAz_ValidatedCollisions);
    arr2->Add(hDCAz_InvalidatedCollisions);
    arr2->Add(hDCAz_AllCollisions);
    arr2->Add(legend2); 

    TCanvas* canDCAz2 = (TCanvas*)makeCanvas(arr2, 0, "CMYK|NoTime LogY ", 0, 0);


    TH2D* hCollisionCounter = (TH2D*)extractHistogram(Form("%s:/pcm-qc-mc/Event/after/hCollisionCounter",inFileName_TrueMC));
    Double_t nEvents = hCollisionCounter->GetBinContent(10);

  ///pT distributions
  hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(-0.5, 0.5);
  TH1D* hpT_ValidatedCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(1);
  SetHistogramPropertiesAlt(hpT_ValidatedCollisions, "p_{T} (GeV/c)", "norm. counts", 0);
  hpT_ValidatedCollisions->GetXaxis()->SetRangeUser(0.1, 10.0);
  hpT_ValidatedCollisions->Rebin(4);
  hpT_ValidatedCollisions->Scale(1./nEvents);


  hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(0.5, 1.5);
  TH1D* hpT_InvalidatedCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(1);
  SetHistogramPropertiesAlt(hpT_InvalidatedCollisions, "p_{T} (GeV/c)", "norm. counts", 1);
  hpT_InvalidatedCollisions->GetXaxis()->SetRangeUser(0.1, 10.0);
  hpT_InvalidatedCollisions->Rebin(4);
  hpT_InvalidatedCollisions->Scale(1./nEvents);
  

  hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(-0.5, 1.5); 
  TH1D* hpT_AllCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(1);
  SetHistogramPropertiesAlt(hpT_AllCollisions, "p_{T} (GeV/c)", "norm. counts", 2);
  hpT_AllCollisions->GetXaxis()->SetRangeUser(0.1, 10.0);
  hpT_AllCollisions->GetYaxis()->SetRangeUser(1e-3, 1.5*hpT_AllCollisions->GetMaximum());
  hpT_AllCollisions->Rebin(4);
  hpT_AllCollisions->Scale(1./nEvents);

  TH1D* hpT_ValidatedCollisionsRatioTo_AllCollisions = (TH1D*)hpT_ValidatedCollisions->Clone("hpT_ValidatedCollisionsRatioToAllCollisions");
  hpT_ValidatedCollisionsRatioTo_AllCollisions->Divide(hpT_AllCollisions);

  TH1D* hpT_InvalidatedCollisionsRatioTo_AllCollisions = (TH1D*)hpT_InvalidatedCollisions->Clone("hpT_InvalidatedCollisionsRatioToAllCollisions");
  hpT_InvalidatedCollisionsRatioTo_AllCollisions->Divide(hpT_AllCollisions);


  TLegend* legend3 = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend3->AddEntry(hpT_ValidatedCollisions, "Validated collisions", "lp");
  legend3->AddEntry(hpT_InvalidatedCollisions, "Invalidated collisions", "lp");
  legend3->AddEntry(hpT_AllCollisions, "All collisions", "lp");

  TObjArray* arr3 = new TObjArray();
  arr3->Add(hpT_AllCollisions);
  arr3->Add(hpT_ValidatedCollisions);
  arr3->Add(hpT_InvalidatedCollisions);
  arr3->Add(legend3); 

  TObjArray* arr3Ratio = new TObjArray();
  arr3Ratio->Add(hpT_ValidatedCollisionsRatioTo_AllCollisions);
  arr3Ratio->Add(hpT_InvalidatedCollisionsRatioTo_AllCollisions);

  TCanvas* canpT2 = (TCanvas*)makeCanvas(arr3, arr3Ratio, "CMYK|NoTime LogX LogY ", 0, 0);


  TH1D* correctionFactor = (TH1D*)hpT_ValidatedCollisionsRatioTo_AllCollisions->Clone("correctionFactor");
  correctionFactor->Reset();

  for(int i = 1; i <= hpT_ValidatedCollisionsRatioTo_AllCollisions->GetNbinsX(); i++){
    if(hpT_ValidatedCollisionsRatioTo_AllCollisions->GetBinContent(i) == 0){
      correctionFactor->SetBinContent(i, 1.0);
      correctionFactor->SetBinError(i, 0.0);
    }

    else{
      correctionFactor->SetBinContent(i, 1./hpT_ValidatedCollisionsRatioTo_AllCollisions->GetBinContent(i));
      correctionFactor->SetBinError(i, 1./hpT_ValidatedCollisionsRatioTo_AllCollisions->GetBinError(i));
    } 
    cout << "correctionFactor: in" << i << " is  " << correctionFactor->GetBinContent(i) << endl;
    
  }

  TObjArray* arr4 =  new TObjArray();
  arr4->Add(correctionFactor);
  TCanvas* canpT3 = (TCanvas*)makeCanvas(arr4, 0, "CMYK|NoTime LogX  ", 0, 0);


}

*/