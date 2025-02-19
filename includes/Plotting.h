#include "TH1D.h"
#include "TLatex.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TCanvas.h"

#include "TLegend.h"
#include "TPaveText.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TStyle.h"
#include "TH2.h"
#include "TF1.h"
#include "TRandom.h"
#include "TImage.h"
#include "TImage.h"
#include "TTimeStamp.h"
#include "TMath.h"


#include <iostream>

Double_t minXValue = 0.15;
Double_t maxXValue = 99.9;
Double_t maxYValue = -1;
Double_t minYValue = 1e20;
Double_t fPtPlotRange = 99.9;

//TCanvas *makeCanvas(TObjArray *histArray, TObjArray *ratioArray = 0,const char *controlstring="", Short_t *colorArray = 0, Short_t *markerArray = 0);
//TCanvas *makeCanvas4Pads(TObjArray *arrHist_TL, TObjArray *arrHist_TR,TObjArray *arrHist_BL, TObjArray *arrHist_BR,const char *controlstring="", Short_t *colorArray=0, Short_t *markerArray=0);
void *DefineOwnColors(const char *palette="std");
void SetHistogramProperties(TH1D* &h, TString YTitle, Int_t ColorIndex);
void SetHistogramProperties2dim(TH2* hist2d, TString XTitle, TString YTitle);

Int_t GetNiceColor(Int_t i)
{//
   Int_t niceColors[] = {kRed+1, kBlue+1, kCyan-2, kViolet-7, kGreen+3, kYellow+3, kGreen-3, kRed-1, kOrange+2, kOrange, kSpring+8, kAzure+4, kMagenta+3, kGray+2, kBlack};
  //Int_t niceColors[] = {kRed+1, kAzure+4, kBlue+1, kMagenta+3, kCyan-2, kViolet-7, kOrange+2, kYellow+3, kSpring+8, kGreen-3, kGray+2, kBlack};
 //Int_t niceColors[] = {kBlack, kRed-2, kPink-6, kPink+3, kMagenta+3, kViolet-7, kViolet+2, kBlue+3, kBlue-3, kAzure+3, kAzure+4, kCyan+1, kTeal-5, kTeal+3, kGreen-3, kSpring+8};

  return niceColors[i];
}

TH1D* GetPtSpectrumpp(TFile* inputfile, const char* hName, Int_t niceColor) //Used in SysUncertainties/pp/postproc.C
{
    TH1D* hist = (TH1D*)inputfile->Get(hName);
    if(!hist) printf("\n\nCould not load histogram %d\n\n", niceColor);
    SetHistogramProperties(hist, "1/N_{ev} 1/(2 #pi #it{p}_{T}) d#it{N}/d#it{p}_{T}", niceColor);
    hist->GetYaxis()->SetRangeUser(0.2*minYValue, 3*maxYValue);
    hist->GetXaxis()->SetRangeUser(0.15, 150);
    return (hist);
}


TH1D* GetPtSpectrumPbPb(TFile* inputfile, Int_t niceColor, const char* hname, const char* centClass) //Used in SysUncertainties/PbPb/postproc.C
{
    TList* list = (TList*)inputfile->Get(centClass);
    if(!list) printf("ERROR in ExtractHisto(): Could not load list\n");

    TH1D* hist = (TH1D*)list->FindObject(hname);
    if(!hist) printf("ERROR in ExtractHisto(): Could not load histogram\n");
    SetHistogramProperties(hist, "1/(N_{evt}) d^{2}N_{ch}/(d#eta dp_{T}) (GeV/c)^{-2}", niceColor);
    hist->GetYaxis()->SetRangeUser(0.2*minYValue, 3*maxYValue);
    hist->GetXaxis()->SetRangeUser(0.15, 150);

    return hist;
}

TH1D* GetRaa(TFile* inputfile, Int_t niceColor, const char* hname) //Used in SysUncertainties/Raa/postproc.C
{
    TH1D* hist = (TH1D*)inputfile->Get(hname);
    if(!hist) printf("\n\nCould not load histogram %d\n\n", niceColor);
    SetHistogramProperties(hist, "#it{R}_{PbPb}", niceColor);
    hist->GetYaxis()->SetRangeUser(-0.05, 1.15);
    hist->GetXaxis()->SetRangeUser(-0.5, 70);
    return (hist);
}



TH1D* MakeHistoSys_Unc(TH1D* ratio_variation1, TH1D* ratio_variation2)
{
  int firstBin = ratio_variation1->FindBin(0.15);
  int lastBin = ratio_variation1->FindBin(100.0-0.01);

  TH1D* hsys_unc = (TH1D*)ratio_variation1->Clone("hsys_unc");
	hsys_unc->Reset();
	hsys_unc->GetYaxis()->SetTitle("Uncertainty %");
	//SetHistogramProperties(hsys_unc, "Uncertainty %", 5);

    double BinContent_ratio_variation1;
    double BinContent_ratio_variation2;
    double sum_var1=0.0;
    double sum_var2=0.0;

    for(int i = firstBin; i< lastBin; ++i)
    {
      BinContent_ratio_variation1=ratio_variation1->GetBinContent(i);
      BinContent_ratio_variation2=ratio_variation2->GetBinContent(i);

      sum_var1=TMath::Abs(1.0 - BinContent_ratio_variation1) + sum_var1;
      sum_var2=TMath::Abs(1.0 - BinContent_ratio_variation2) + sum_var2;
    }

    if(sum_var1 >= sum_var2)
    {
        for(int i = firstBin; i<= lastBin; ++i)
        {
        BinContent_ratio_variation1=ratio_variation1->GetBinContent(i);
            if(  (BinContent_ratio_variation1 !=0.0))
            {
            hsys_unc->SetBinContent(i, 100.0*(TMath::Abs((BinContent_ratio_variation1-1.0))/1.0));
            }
        }
    }
    else
    {
        for(int i = firstBin; i<= lastBin; ++i)
        {
        BinContent_ratio_variation2=ratio_variation2->GetBinContent(i);
        if(  BinContent_ratio_variation2 !=0.0 )
            {
            hsys_unc->SetBinContent(i, 100.0*(TMath::Abs((BinContent_ratio_variation2-1.0))/1.0));
            }
        }
    }

    return(hsys_unc);
}

void UncertaintyPerCent(TH1D* h, int cutMode)
{
    int lastBin = h->FindBin(100.0-0.01);
    double maxunc=0.0;
    for(int i = 1; i< lastBin; i++){
        if(maxunc < h->GetBinContent(i)) maxunc = h->GetBinContent(i);
    }

    double minunc=100.0;
    for(int i = 1; i< lastBin; ++i){
        if(h->GetBinContent(i) > 0){
            if(minunc > h->GetBinContent(i)) minunc = h->GetBinContent(i);
        }
    }
    if(cutMode > 0 && cutMode != 119 ) {printf("\nCutMode: %d - %d, MinUncert in PerCent: %lf, MaxUncert in PerCent: %lf \n", cutMode, cutMode+1, minunc, maxunc);}
    if(cutMode == 119) {printf("\nCutMode: %d, MinUncert in PerCent: %lf, MaxUncert in PerCent: %lf \n", cutMode, minunc, maxunc);}
    if(cutMode == 0) {printf("\nCombined Uncertanties, MinUncert in PerCent: %lf, MaxUncert in PerCent: %lf \n", minunc, maxunc);}
}


TH1D* GetRelativeUncertainties(TH1D* h, Int_t niceColor){
  if(!h)
  {
    std::cout << "ERROR: Hist empty\n";
    return nullptr;
  }
  const char* hname = h->GetName();
  //std::string const _name{"uncerainties_"+hname};
  const char* _name = Form("uncerainties_%s", hname);
  TH1D* result = (TH1D*)h->Clone(_name);
  result->Reset();
  for(int ibin=0; ibin<=h->GetNbinsX();++ibin){
    auto const val = h->GetBinContent(ibin);
    auto const error = h->GetBinError(ibin);
    if(val == 0) continue;
    result->SetBinContent(ibin, 100*error/val);
  }
  SetHistogramProperties(result, "rel. Error", niceColor);

  return result;
}

void PlotArray(TObjArray *arraytoplot, const char *controlstring ,Short_t *colorArray, Short_t *markerArray, Double_t relativeTextSize,Float_t lableSize,Float_t titleSize,  Int_t lableFont,Int_t titleFont,Float_t titleOffsetY,Float_t titleOffsetX,Float_t margin,Double_t xMin,Double_t xMax,Double_t yMin = 0,Double_t yMax = 0){

  TString control(controlstring);
  Int_t nbrEntries = arraytoplot->GetEntries();
  for(Int_t hh=0;hh<=nbrEntries;hh++){
    if(!arraytoplot->At(hh)){break;}

      if (arraytoplot->At(hh)->InheritsFrom("TH1") && !arraytoplot->At(hh)->InheritsFrom("TH2")){
      cout<<"| Drawing a TH1 "<<hh<<endl;
      TH1D *hist = (TH1D*) arraytoplot->At(hh);
      hist->SetStats(0);
     // hist->GetXaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist->GetXaxis()->SetTitleSize(23.);
      hist->GetXaxis()->SetLabelFont(lableFont);
      hist->GetXaxis()->SetTitleFont(titleFont);
      hist->GetXaxis()->SetTitleOffset(titleOffsetX);

     // hist->GetYaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist->GetYaxis()->SetTitleSize(23.);
      hist->GetYaxis()->SetLabelFont(lableFont);
      hist->GetYaxis()->SetTitleFont(titleFont);
   //   hist->GetYaxis()->SetTitleOffset(titleOffsetY);
      hist->SetTitle("");

      hist->SetAxisRange(xMin,xMax,"X");



      TString xTitle(hist->GetXaxis()->GetTitle());
      if (xTitle.Contains("pt")  || xTitle.Contains("p_{T} (GeV/c)")){
        hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
      }else if(xTitle.Contains("mcPt")){
        hist->GetXaxis()->SetTitle("#it{p}_{T}^{MC} (GeV/#it{c})");
      }


      // hist->SetMarkerStyle(markerArray[hh]);
      // hist->SetMarkerColor(colorArray[hh]);
      // hist->SetLineColor(colorArray[hh]);


      Double_t mrkSize = hist->GetMarkerSize();
      if(hist->GetMarkerStyle()== 33 || hist->GetMarkerStyle()== 27) hist->SetMarkerSize(1.4*mrkSize);
      if(hist->GetMarkerStyle()== 29 || hist->GetMarkerStyle()== 30) hist->SetMarkerSize(1.4*mrkSize);
      if(hist->GetMarkerStyle()== 5  || hist->GetMarkerStyle()== 5 ) hist->SetMarkerSize(1.4*mrkSize);
      if(hist->GetMarkerStyle()== 34 || hist->GetMarkerStyle()== 28) hist->SetMarkerSize(1.2*mrkSize);


      /// Set tick length independent of pad size
      Double_t pad_width  = gPad->XtoPixel(gPad->GetX2());
      Double_t pad_height = gPad->YtoPixel(gPad->GetY1());
      hist->SetTickLength(15/pad_height,"X");
      hist->SetTickLength(15/pad_width,"Y");



      if(control.Contains("thick")){
        hist->SetLineWidth(4);
      }else if(control.Contains("Thin")||control.Contains("thin")){
        hist->SetLineWidth(1);
      }else{
        hist->SetLineWidth(2);
      }

     if(markerArray[hh]==0){
        hist->SetLineWidth(0);
      }
      if(control.Contains("Lines")||control.Contains("lines")){
        cout<<"|         - Lines No errors"<<endl;
        if(markerArray){ hist->SetLineStyle(markerArray[hh]);}
        if(hh==0) { hist->Draw("hist");}
        else{hist->Draw("Same hist"); }
      }else if(markerArray[hh]==999){
        hist->SetFillColor(kGray);
        hist->SetMarkerSize(0);
        if(hh==0) {hist->Draw("E2");}
        else{hist->Draw("Same E2");}
      }else{
        if(hh==0) {hist->Draw("");}
        else{hist->Draw("Same");}
      }
    } else if (arraytoplot->At(hh)->InheritsFrom("TH2")){
      cout<<"| Drawing a TH2 "<<hh<<endl;
      TH2D *hist2 = (TH2D*) arraytoplot->At(hh);
      hist2->SetStats(0);
      hist2->GetXaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist2->GetXaxis()->SetTitleSize(titleSize*relativeTextSize);
      hist2->GetXaxis()->SetLabelFont(lableFont);
      hist2->GetXaxis()->SetTitleFont(titleFont);
      //hist2->GetXaxis()->SetTitleOffset(titleOffsetX);

      hist2->GetYaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist2->GetYaxis()->SetTitleSize(titleSize*relativeTextSize);
      hist2->GetYaxis()->SetLabelFont(lableFont);
      hist2->GetYaxis()->SetTitleFont(titleFont);
      //hist2->GetYaxis()->SetTitleOffset(titleOffsetY);
      hist2->SetTitle("");


      //hist2->SetAxisRange(xMin,xMax,"X");
      //hist2->SetAxisRange(yMin,yMax,"Y");


      TString xTitle(hist2->GetXaxis()->GetTitle());
      if (xTitle.Contains("pt")  || xTitle.Contains("p_{T} (GeV/c)")){
        hist2->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
      }else if(xTitle.Contains("mcPt")){
        hist2->GetXaxis()->SetTitle("#it{p}_{T}^{MC} (GeV/#it{c})");
      }


      /// Set tick length independent of pad size
      Double_t pad_width  = gPad->XtoPixel(gPad->GetX2());
      Double_t pad_height = gPad->YtoPixel(gPad->GetY1());
      hist2->SetTickLength(15/pad_height,"X");
      hist2->SetTickLength(15/pad_width,"Y");

      hist2->Draw("COLZ");

    }else if (arraytoplot->At(hh)->InheritsFrom("TLine")){
      cout<<"| Drawing line: "<<arraytoplot->At(hh)->GetName()<<endl;
      TLine *line = (TLine*) arraytoplot->At(hh);
      if (markerArray[hh]<11){line->SetLineStyle(markerArray[hh]);}
      line->Draw("Same");
    }else if (arraytoplot->At(hh)->InheritsFrom("TPaveText")){
      cout<<"| Drawing a TPaveText "<<hh<<endl;
      TPaveText *pt = (TPaveText*) arraytoplot->At(hh);
      if (! pt) cout<<"|Error with TPaveText"<<endl;
      cout<<"| Rel. text size: "<<relativeTextSize<<endl;
      pt->SetFillStyle(0);
      pt->SetFillColor(0);
      pt->SetBorderSize(0);
      pt->SetTextFont(titleFont);
      pt->SetTextSize(relativeTextSize);
      pt->Draw();
    }else if (arraytoplot->At(hh)->InheritsFrom("TLegend")){
      cout<<"| Drawing a Legend "<<hh<<endl;
      cout<<"| Rel. text size: "<<relativeTextSize<<endl;
      TLegend *leghh = (TLegend*) arraytoplot->At(hh);
      leghh->SetTextFont(titleFont);
      leghh->SetTextSize(23.);
      leghh->SetFillColor(kWhite);
      leghh->SetFillStyle(0);
      leghh->SetBorderSize(0);
      leghh->Draw("same");
    }else if (arraytoplot->At(hh)->InheritsFrom("TBox")){
      cout<<"| Drawing a TBox"<<endl;
      TBox *box = (TBox*) arraytoplot->At(hh);
      box->Draw();
    }else if (arraytoplot->At(hh)->InheritsFrom("TGraphAsymmErrors")){
      cout<<"| Drawing a TGraphAsymmErrors "<<hh<<endl;
      TGraphAsymmErrors *asym = (TGraphAsymmErrors*) arraytoplot->At(hh);

      asym->SetLineColor(colorArray[hh]);
      asym->SetLineStyle(markerArray[hh]);
      asym->SetLineWidth(4);
      asym->Draw("SAME");
    }else if (arraytoplot->At(hh)->InheritsFrom("TGraphErrors")){
      cout<<"| Drawing a TGraph "<<hh<<endl;
      TGraph *grph = (TGraphAsymmErrors*) arraytoplot->At(hh);
        cout<<"| - As sytematic error"<<endl;
        grph->Draw("SAME 5");
    }else if (arraytoplot->At(hh)->InheritsFrom("TGraph")){
      cout<<"| Drawing a TGraph "<<hh<<endl;
      TGraph *grph = (TGraphAsymmErrors*) arraytoplot->At(hh);

      if(markerArray[hh] == 999){
        cout<<"| - As sytematic error"<<endl;
        grph->SetFillColorAlpha(colorArray[hh],0.6);
        //         grph->Draw("SAME a3");
        //grph->Draw("SAME []");
                 grph->Draw("SAME 5");
                 printf("test\n");
      }else{
        grph->SetMarkerColor(colorArray[hh]);
        grph->SetMarkerStyle(markerArray[hh]);
        Double_t mrkSize = grph->GetMarkerSize();
        if(grph->GetMarkerStyle()== 33 || grph->GetMarkerStyle()== 27)    grph->SetMarkerSize(1.4*mrkSize);
        if(grph->GetMarkerStyle()== 29 || grph->GetMarkerStyle()== 30)    grph->SetMarkerSize(1.4*mrkSize);
        if(grph->GetMarkerStyle()== 34 || grph->GetMarkerStyle()== 28)    grph->SetMarkerSize(1.2*mrkSize);
        grph->Draw("SAME 5");
        printf("test\n");

      }
    }else if (arraytoplot->At(hh)->InheritsFrom("TF1")){
      cout<<"| Drawing a TF1 "<<hh<<endl;
      TF1 *fun = (TF1*) arraytoplot->At(hh);
      fun->SetLineColor(colorArray[hh]);
      if (markerArray[hh]< 10){
        fun->SetLineStyle(markerArray[hh]);
      }else{
        fun->SetLineStyle(1);
      }
      if(control.Contains("Thick")||control.Contains("thick")){
        fun->SetLineWidth(4);
      }else if(control.Contains("Thin")||control.Contains("thin")){
        fun->SetLineWidth(1);
      }else{
        fun->SetLineWidth(3);
      }
      fun->Draw("SAME L");
    }
    gPad->RedrawAxis();
  }
}

void PlotArrayTools(TObjArray *arraytoplot, const char *controlstring ,Short_t *colorArray, Short_t *markerArray, Double_t relativeTextSize,Float_t lableSize,Float_t titleSize,  Int_t lableFont,Int_t titleFont,Float_t titleOffsetY,Float_t titleOffsetX,Float_t margin,Double_t xMin,Double_t xMax,Double_t yMin = 0,Double_t yMax = 0){
  TString control(controlstring);
  if (control.Contains("CMYK")||control.Contains("cmyk")){gStyle->SetColorModelPS(1);}
  Int_t nbrEntries = arraytoplot->GetEntries();
  for(Int_t hh=0;hh<=nbrEntries;hh++){
    if(!arraytoplot->At(hh)){break;}

    if (arraytoplot->At(hh)->InheritsFrom("TH1") && !arraytoplot->At(hh)->InheritsFrom("TH2")){
      cout<<"| Drawing a TH1 "<<hh<<endl;
      TH1D *hist = (TH1D*) arraytoplot->At(hh);
      hist->SetStats(0);
      hist->GetXaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist->GetXaxis()->SetTitleSize(titleSize*relativeTextSize);
      hist->GetXaxis()->SetLabelFont(lableFont);
      hist->GetXaxis()->SetTitleFont(titleFont);
      hist->GetXaxis()->SetTitleOffset(titleOffsetX);

      hist->GetYaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist->GetYaxis()->SetTitleSize(titleSize*relativeTextSize);
      hist->GetYaxis()->SetLabelFont(lableFont);
      hist->GetYaxis()->SetTitleFont(titleFont);
      hist->GetYaxis()->SetTitleOffset(titleOffsetY);
      hist->SetTitle("");

      hist->SetAxisRange(xMin,xMax,"X");


      TString xTitle(hist->GetXaxis()->GetTitle());
      if (xTitle.Contains("pt")  || xTitle.Contains("p_{T} (GeV/c)")){
	hist->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
      }else if(xTitle.Contains("mcPt")){
	hist->GetXaxis()->SetTitle("#it{p}_{T}^{MC} (GeV/#it{c})");
      }



      //hist->SetMarkerStyle(markerArray[hh]);
   //   hist->SetMarkerColor(colorArray[hh]);
    //  hist->SetLineColor(colorArray[hh]);


      Double_t mrkSize = hist->GetMarkerSize();
      if(hist->GetMarkerStyle()== 33 || hist->GetMarkerStyle()== 27) hist->SetMarkerSize(1.4*mrkSize);
      if(hist->GetMarkerStyle()== 29 || hist->GetMarkerStyle()== 30) hist->SetMarkerSize(1.4*mrkSize);
      if(hist->GetMarkerStyle()== 5  || hist->GetMarkerStyle()== 5 ) hist->SetMarkerSize(1.4*mrkSize);
      if(hist->GetMarkerStyle()== 34 || hist->GetMarkerStyle()== 28) hist->SetMarkerSize(1.2*mrkSize);


      /// Set tick length independent of pad size
      Double_t pad_width  = gPad->XtoPixel(gPad->GetX2());
      Double_t pad_height = gPad->YtoPixel(gPad->GetY1());
      hist->SetTickLength(13/pad_height,"X");
      hist->SetTickLength(13/pad_width,"Y");

      if(control.Contains("Thick")||control.Contains("thick")){
	hist->SetLineWidth(4);
      }else if(control.Contains("Thin")||control.Contains("thin")){
	hist->SetLineWidth(1);
      }else{
	hist->SetLineWidth(2);
      }
      TString histName(hist->GetName());
     if(markerArray[hh]==0 || histName.Contains("dummy")){
	cout<<"|         - Don't draw dummy"<<endl;
        hist->SetLineWidth(0);
	hist->SetMarkerSize(0);
      }
      if(hist->GetMarkerStyle() <= 4 || control.Contains("Lines")||control.Contains("lines")){
	cout<<"|         - Lines No errors"<<endl;
	if(markerArray){ hist->SetLineStyle(markerArray[hh]);}
	if(control.Contains("Fill")){
          cout<<"|         - DrawFilled"<<endl;
          hist->SetLineColor(kBlack);
          hist->SetFillColor(colorArray[hh]);
          hist->SetFillStyle(1001);
        }
	if(hh==0) {hist->Draw("hist][");}
        else{hist->Draw("Same hist ][ ");}
      }else if(markerArray[hh]==999){
	hist->SetFillColor(kGray);
        hist->SetFillStyle(1001);
	hist->SetMarkerSize(0);
	if(hh==0) {hist->Draw("E2");}
	else{hist->Draw("Same E2");}
      }else if(histName.Contains("error")){
        if(hh==0) {hist->Draw("E3");}
	else{hist->Draw("Same E3");}
      }else{
	if(hh==0) {hist->Draw("");}
	else{hist->Draw("Same");}
      }
    }else if (arraytoplot->At(hh)->InheritsFrom("TH2")){
      cout<<"| Drawing a TH2 "<<hh<<endl;
      TH2D *hist2 = (TH2D*) arraytoplot->At(hh);
      hist2->SetStats(0);
      hist2->GetXaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist2->GetXaxis()->SetTitleSize(titleSize*relativeTextSize);
      hist2->GetXaxis()->SetLabelFont(lableFont);
      hist2->GetXaxis()->SetTitleFont(titleFont);
      hist2->GetXaxis()->SetTitleOffset(titleOffsetX);

      hist2->GetYaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist2->GetYaxis()->SetTitleSize(titleSize*relativeTextSize);
      hist2->GetYaxis()->SetLabelFont(lableFont);
      hist2->GetYaxis()->SetTitleFont(titleFont);
      hist2->GetYaxis()->SetTitleOffset(titleOffsetY);

      hist2->GetZaxis()->SetLabelSize(lableSize*relativeTextSize);
      hist2->GetZaxis()->SetTitleSize(titleSize*relativeTextSize);
      hist2->GetZaxis()->SetLabelFont(lableFont);
      hist2->GetZaxis()->SetTitleFont(titleFont);
      hist2->GetZaxis()->SetTitleOffset(titleOffsetY);
      hist2->SetTitle("");

      hist2->SetAxisRange(xMin,xMax,"X");
      hist2->SetAxisRange(yMin,yMax,"Y");


      TString xTitle(hist2->GetXaxis()->GetTitle());
      if (xTitle.Contains("pt")  || xTitle.Contains("p_{T} (GeV/c)")){
	hist2->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
      }else if(xTitle.Contains("mcPt")){
	hist2->GetXaxis()->SetTitle("#it{p}_{T}^{MC} (GeV/#it{c})");
      }

      /// Set tick length independent of pad size
      Double_t pad_width  = gPad->XtoPixel(gPad->GetX2());
      Double_t pad_height = gPad->YtoPixel(gPad->GetY1());
      hist2->SetTickLength(13/pad_height,"X");
      hist2->SetTickLength(13/pad_width,"Y");

      hist2->Draw("COLZ");

    }else if (arraytoplot->At(hh)->InheritsFrom("TLine")){
      cout<<"| Drawing line: "<<arraytoplot->At(hh)->GetName()<<endl;
      TLine *line = (TLine*) arraytoplot->At(hh);
      if (markerArray[hh]<11){line->SetLineStyle(markerArray[hh]);}
      line->Draw("Same");
    }else if (arraytoplot->At(hh)->InheritsFrom("TPaveText")){
      cout<<"| Drawing a TPaveText "<<hh<<endl;
      TPaveText *pt = (TPaveText*) arraytoplot->At(hh);
      if (! pt) cout<<"|Error with TPaveText"<<endl;
      cout<<"| Rel. text size: "<<relativeTextSize<<endl;
      pt->SetFillStyle(0);
      pt->SetFillColor(0);
      pt->SetBorderSize(0);
      pt->SetTextFont(titleFont);
      pt->SetTextSize(relativeTextSize);
      pt->Draw();
    }else if (arraytoplot->At(hh)->InheritsFrom("TLegend")){
      cout<<"| Drawing a Legend "<<hh<<endl;
      cout<<"| Rel. text size: "<<relativeTextSize<<endl;
      TLegend *leghh = (TLegend*) arraytoplot->At(hh);
      leghh->SetTextFont(titleFont);
      leghh->SetTextSize(relativeTextSize);
      leghh->SetFillColor(kWhite);
      leghh->SetFillStyle(0);
      leghh->SetBorderSize(0);
      leghh->Draw("same");
    }else if (arraytoplot->At(hh)->InheritsFrom("TBox")){
      cout<<"| Drawing a TBox"<<endl;
      TBox *box = (TBox*) arraytoplot->At(hh);
      box->Draw();
    }else if (arraytoplot->At(hh)->InheritsFrom("TGraphAsymmErrors")){
      cout<<"| Drawing a TGraphAsymmErrors "<<hh<<endl;
      TGraphAsymmErrors *asym = (TGraphAsymmErrors*) arraytoplot->At(hh);

      asym->SetLineColor(colorArray[hh]);
      asym->SetLineStyle(markerArray[hh]);
      asym->SetLineWidth(4);
      asym->Draw("SAME");
    }else if (arraytoplot->At(hh)->InheritsFrom("TGraph")){
      cout<<"| Drawing a TGraph "<<hh<<endl;
      TGraph *grph = (TGraphAsymmErrors*) arraytoplot->At(hh);

      if(markerArray[hh] == 999){
        cout<<"| - As sytematic error"<<endl;
        grph->SetFillColorAlpha(colorArray[hh],0.6);
//         grph->Draw("SAME a3");
        grph->Draw("SAME []");
//         grph->Draw("SAME 5");
      }else if(markerArray[hh] == 998){
        cout<<"| - Without markers"<<endl;
        grph->SetMarkerColor(colorArray[hh]);
        grph->SetMarkerSize(0);
        grph->Draw("SAME P");
      }else if (markerArray[hh] >= 3000 && markerArray[hh] <= 4000){
        cout<<"| - As band"<<endl;
        grph->SetFillColor(colorArray[hh]);
        grph->SetFillStyle(markerArray[hh]);
        grph->Draw("Same 3");
      }else{

        grph->SetMarkerColor(colorArray[hh]);
        grph->SetMarkerStyle(markerArray[hh]);
        Double_t mrkSize = grph->GetMarkerSize();
        if(grph->GetMarkerStyle()== 33 || grph->GetMarkerStyle()== 27)    grph->SetMarkerSize(1.4*mrkSize);
        if(grph->GetMarkerStyle()== 29 || grph->GetMarkerStyle()== 30)    grph->SetMarkerSize(1.4*mrkSize);
        if(grph->GetMarkerStyle()== 34 || grph->GetMarkerStyle()== 28)    grph->SetMarkerSize(1.2*mrkSize);
        grph->Draw("SAME P");


      }


    }else if (arraytoplot->At(hh)->InheritsFrom("TF1")){
      cout<<"| Drawing a TF1 "<<hh<<endl;
      TF1 *fun = (TF1*) arraytoplot->At(hh);
      fun->SetLineColor(colorArray[hh]);
      if (markerArray[hh]< 10){
	fun->SetLineStyle(markerArray[hh]);
      }else{
	fun->SetLineStyle(3);
      }
      if(control.Contains("Thick")||control.Contains("thick")){
	fun->SetLineWidth(4);
      }else if(control.Contains("Thin")||control.Contains("thin")){
	fun->SetLineWidth(1);
      }else{
	fun->SetLineWidth(2);
      }
      fun->Draw("SAME");
    }
    gPad->RedrawAxis();
  }
}



TCanvas *makeCanvas(TObjArray *histArray, TObjArray *ratioArray = 0,const char *controlstring="", Short_t *colorArray = 0, Short_t *markerArray = 0){


  /// This fuction draws a canvas, which contains two panels. The upper one takes up the
  /// most space and is designed to show spectra.
  /// The lower panel is designed to show the ratio of the histograms shown in the upper
  /// panel.The histograms and the ratios as well as the ledgend are created befor and
  /// are part of the parameters.
  ///
  /// The function expects a <b>TObjArray</b> for the Historgrams and a <b>Short_t</b> Array for the
  /// colors
  ///
  /// \param TObjArray Array of histograms
  /// \param TObjArray Array of Ratios
  /// \param Char String ("LogY"&"LogX" to set Axis)
  /// \param Short_t Array of color integers
  /// \param Short_t Array of marker integers
  ///
  /// \return TCanvas with the drawn histograms
  ///
  /// <b>String posibilities:</b>
  /// \arg LogX     Set X-axis logarythmic
  /// \arg LogY     Set Y-axis logarythmic
  /// \arg Square   Produce a squared canvas
  ///
  ///
  ///
  cout<<" ____________________________________________________________________________"<<endl;
  cout<<"|"<<endl;
  cout<<"| Plots generated via makeCanvas"<<endl;

  // Define common colors for plots
  // Particle composition
  Int_t colKaon = kBlue-7;
  Int_t mkrKaon = 20;
  Int_t mkrKaonO = 24;
  Int_t colPion = kBlue+1;
  Int_t mkrPion = 21;
  Int_t mkrPionO = 25;
  Int_t colProt = kRed+1;
  Int_t mkrProt = 33;
  Int_t mkrProtO = 27;
  Int_t colSigp = kCyan+3;
  Int_t mkrSigm = 34;
  Int_t mkrSigmO = 28;
  Int_t colSigm = kGreen+3;
  Int_t mkrSigp = 29;
  Int_t mkrSigpO = 30;
  Int_t colRest = kGray +1;
  Int_t mkrRest = 5;
  Int_t mkrRestO = 5;

  Int_t colPrim = kCyan+3;
  Int_t mrkPrim = 33;
  Int_t mrkPrimO = 27;

  Int_t colSec  = kGreen+3;
  Int_t mrkSec = 34;
  Int_t mrkSecO = 28;

  // Centrality
  Int_t col05   = kRed+1;
  Int_t col510  = kRed-1;
  Int_t col1020 = kOrange-3;
  Int_t col2030 = kYellow-2;
  Int_t col3040 = kGreen-3;
  Int_t col4050 = kGreen+3;
  Int_t col5060 = kCyan+3;
  Int_t col6070 = kBlue-7;
  Int_t col7080 = kBlue+1;

  // Energies
  Int_t col276TeV = kBlue+1;
  Int_t mrk276TeV = 33;
  Int_t mrk276TeVO = 27;
  Int_t col502TeV = kGreen-2;
  Int_t mrk502TeV = 21;
  Int_t mrk502TeVO = 25;
  Int_t col544TeV = kOrange-3;
  Int_t mrk544TeV = 34;
  Int_t mrk544TeVO = 28;

  // Data MC comparisons
  Int_t colData = kBlue-7;
  Int_t mrkData = 20;
  Int_t mrkDataO= 24;
  Int_t colMC   = kRed+1;
  Int_t mrkMC   = 21;
  Int_t mrkMCO  = 25;

  gStyle->SetTextFont(43);
  TString control(controlstring);

  //   Short_t colorArray[]={kRed,kOrange-3,kGreen-3,kGreen+3,kCyan+3,kBlue};
    Short_t defaultColorArray[14] = {0};
  if(!colorArray) {
      Short_t copyArray[14]={kBlue-7,kRed+1,kGreen-3,6,8,9,11,12,13,14,15,16,17,18};
      for (int i =0; i<14; ++i) {
          defaultColorArray[i] = copyArray[i];
      }

  } /// \remark If no colors are specified, use default: blue, red, green, magenta, wierd green, wierd blue and various grey
    Short_t defaultMarkerArray[14] = {0};
  if(!markerArray) {
      Short_t copyArray[14]={20,21,22,23,24,25,26,27,28,29,30,20,20,20};
      for (int i =0; i<14; ++i) {
          defaultMarkerArray[i] = copyArray[i];
      }
  } /// \remark If no markers are specified, use default.
  // if(control.Contains("OwnColors")){//DefineOwnColors();
  //     Short_t copyArray[14]={4001,4002,4003,4004,4005,4006,4007,4008,4009,4010,4011,4012,17,18};
  //     for (int i =0; i<14; ++i) {
  //         defaultColorArray[i] = copyArray[i];
  //     }
  // }

  Int_t nHist = histArray->GetEntries();
  if(nHist == 0){cout<<"| ERROR: Arrays empty"<<endl; return nullptr;}

  Float_t lableSize = 2.0; // Factor the label will be larger than the relative textsize
  Float_t titleSize = 2.0; // Factor the title will be larger than the relative textsize
  Float_t textSizeFactor = 12000;
  Int_t lableFont= 43;
  Int_t titleFont= 43;

  Float_t canvasWidth;
  Float_t canvasHeight;
  Float_t padFraction;
  Float_t titleOffsetY;
  Float_t titleOffsetX;
  Float_t sideMargin;
  Float_t topMargin;
  Float_t lowMargin;

  if(control.Contains("square")||control.Contains("Square")||control.Contains("SQUARE")){
    cout<<"| - Square Canvas"<<endl;
    canvasWidth=700;
    canvasHeight=700;
    padFraction = 0.5;
    titleOffsetY=2.;
    titleOffsetX=1.5;
    sideMargin=0.15;
    topMargin=sideMargin-0.05;
    lowMargin=sideMargin+0.05;
    if(ratioArray){
      textSizeFactor =0.5 * 12000;
      sideMargin=0.13;
      topMargin = sideMargin/(padFraction)-0.05;
      lowMargin = sideMargin/(padFraction)+0.05;
      titleOffsetY=1.4;
      titleOffsetX=2.8;
    }
  }else if (control.Contains("horizontal")||control.Contains("Horizontal")||control.Contains("HORIZONTAL")){
    cout<<"| - Horizontal Canvas"<<endl;
    canvasWidth=600*1.414213562;
    canvasHeight=600;
    padFraction = 0.5;
    titleOffsetY=1.05;
    titleOffsetX=1.02;
    sideMargin = 0.09*1.414213562;
    topMargin=0.06;
    lowMargin=0.12;
    if (control.Contains("BinLabels")){
      titleOffsetY=1.05;
      titleOffsetX=2.1;
      lowMargin=0.19;
    }
    if(ratioArray){
    }
  }else if (control.Contains("A4")||control.Contains("a4")){
    cout<<"| - DIN-A4 shaped Canvas"<<endl;
    canvasWidth=600;
    canvasHeight=600*1.414213562;
    padFraction = 0.25;
    titleOffsetY=1.4;
    titleOffsetX=1.4;
    sideMargin = 0.11;
    topMargin=sideMargin * 1.414213562 - 0.05;
    lowMargin=sideMargin * 1.414213562 + 0.05;
    if(ratioArray){
    }
  }else if (control.Contains("OldAspect")||control.Contains("oldaspect")){
    cout<<"| - Improved aspect ratio Canvas"<<endl;
    canvasWidth=700;
    canvasHeight=800;
    padFraction = 0.28;
    titleOffsetY=1.7;
    titleOffsetX=3.5;
    sideMargin=0.12;
    topMargin=0.11;
    lowMargin=0.13;
    if(ratioArray){
      titleOffsetY=1.7;
      titleOffsetX=1.3;
      topMargin=0.08;
      lowMargin=0.30;
    }
  }else{
    cout<<"| - Improved aspect ratio Canvas"<<endl;
    textSizeFactor =1.8 * 12000;
    canvasWidth=800;
    canvasHeight=800*1.41421356;
    padFraction = 0.25;
    titleOffsetY=1.5;
    titleOffsetX=1.3;
    sideMargin = 1.54;
    topMargin = 0.12 * 1.414213562/2;
    lowMargin = 0.12 * 1.414213562/2;
    if(ratioArray){
      titleOffsetY=4;
      titleOffsetX=2.3;
      topMargin = (0.12 * 1.414213562/2)/(1-padFraction);
      lowMargin = (0.12 * 1.414213562/2)/(padFraction);
    }
  }
  titleOffsetY=5.5;
  titleOffsetX = 1.25;



  if (control.Contains("CMYK")||control.Contains("cmyk")){gStyle->SetColorModelPS(1);}

  TRandom *random = new TRandom(histArray->Hash());

    TString title("default");
    title += Form("%f",random->Rndm());
  /// Check if first element of th histogram array is indeed an Histogram.
    if(!histArray->At(0)) return nullptr;
    if (!histArray->At(0)->InheritsFrom("TH1")){cout<<"|ERROR: First entry has to be an Histogram"<<endl; return nullptr;}
    TH1D *hist0 = (TH1D*) histArray->At(0);
    if(!hist0){cout<<"| ERROR: Histogram empty"<<endl; return nullptr;}

    Double_t xMin = hist0->GetXaxis()->GetBinLowEdge(hist0->GetXaxis()->GetFirst())+0.0000001;
    Double_t xMax = hist0->GetXaxis()->GetBinUpEdge(hist0->GetXaxis()->GetLast())-0.0000001;
    cout<<"| - Axis range: "<<xMin<<"-"<<xMax<<endl;
    hist0->GetYaxis()->CenterTitle(0);
    if(control.Contains("CenterTitle")||control.Contains("centertitle")) hist0->GetYaxis()->CenterTitle(1);

  /// Check if the first Element of the ratio array is an histogram.
  if(ratioArray){
    if(ratioArray->At(0)){
      if (!ratioArray->At(0)->InheritsFrom("TH1")){cout<<"|ERROR: First entry has to be an Histogram"<<endl;}
      TH1D *ratio0 = (TH1D*) ratioArray->At(0);
      if(!ratio0){cout<<"| ERROR: Histogram empty"<<endl;}
      ratio0->GetXaxis()->SetTickLength(0.06);
      ratio0->GetYaxis()->SetNdivisions(506);
      ratio0->SetLineWidth(2);
      ratio0->GetYaxis()->CenterTitle(0);

      /// Automatic range + zoom
      if(control.Contains("Zoom")){
        Double_t ratio0Maximum = ratio0->GetBinContent(ratio0->GetMaximumBin());
        Double_t ratio0Minimum = 1;
        for(Int_t ii = 0; ii < ratio0->GetNbinsX(); ii++){
          Double_t val = ratio0->GetBinContent(ii);
          if (val > 0){if (val < ratio0Minimum){ratio0Minimum = val;}}
        }
        Double_t zoom = 0.05;
        if(control.Contains("Zoom"))     zoom = 0.15;
        if(control.Contains("Zoom+"))    zoom = 0.30;
        if(control.Contains("Zoom++"))   zoom = 0.45;
        if(control.Contains("Zoom+++"))  zoom = 0.60;
        if(control.Contains("Zoom++++")) zoom = 0.75;
        if(control.Contains("Zoom+++++"))zoom = 0.90;
        if ( ( 1 - ratio0Minimum) > ( ratio0Maximum - 1) ){zoom = zoom*(1- ratio0Minimum); ratio0->SetAxisRange(ratio0Minimum+zoom,2-ratio0Minimum-zoom,"Y");}
        else{zoom = zoom*(ratio0Maximum -1); if ( ratio0Maximum > 2){ratio0->SetAxisRange(zoom,ratio0Maximum - zoom,"Y");}else{ ratio0->SetAxisRange(1-(ratio0Maximum-1)+zoom,ratio0Maximum - zoom,"Y");}}
      }
    }
  }

  /// Create Canvas in a given size
  TCanvas *newCanvas = new TCanvas(title,title,10,10,canvasWidth+10,canvasHeight+10);
  if(!newCanvas) {printf("FATAL ERROR: could not create canvas"); return nullptr;}
  newCanvas->SetFillStyle(1001);
  newCanvas->SetFillColor(kWhite);
  newCanvas->cd();

    TPad *upperPad = nullptr;

  /// If ratioArray exist, split the Canvas in two and create a ratio panel
  if(ratioArray){
    /// Create upper pad for the histograms \remark upper pad has 72%
    upperPad = new TPad("upperPad","Distribution" ,0 ,padFraction ,1 ,1);
    TPad *lowerPad = new TPad("lowerPad","Ratio"        ,0 ,0           ,1 ,padFraction);

    upperPad->SetFillStyle(1001);
    upperPad->SetFillColor(kWhite);
    upperPad->SetTopMargin(topMargin);
    upperPad->SetLeftMargin(sideMargin);
    upperPad->SetRightMargin(sideMargin);
    upperPad->SetBottomMargin(0.0);
    upperPad->SetTickx(1);
    upperPad->SetTicky(1);
    upperPad->Draw();

    lowerPad->SetFillStyle(1001);
    lowerPad->SetFillColor(kWhite);
    lowerPad->SetTopMargin(0.0);
    lowerPad->SetLeftMargin(sideMargin);
    lowerPad->SetRightMargin(sideMargin);
    lowerPad->SetBottomMargin(lowMargin);
    lowerPad->SetTickx(1);
    lowerPad->SetTicky(1);
    lowerPad->Draw();


    if(control.Contains("logX")||control.Contains("logx")||control.Contains("LogX")||control.Contains("LOGX")){
      upperPad->SetLogx(1); lowerPad->SetLogx(1); cout<<"| - LogX"<<endl;
    }
    if(control.Contains("logY")||control.Contains("logy")||control.Contains("LogY")||control.Contains("LOGY")){
      upperPad->SetLogy(1);  cout<<"| - LogY"<<endl;
    }
    if(control.Contains("RatioGridY")){lowerPad->SetGridy(1); cout<<"| - Ratio: GridY"<<endl;}

    newCanvas->cd();
    upperPad->cd();

    Double_t relativeTextSize;
    Double_t pad_width;
    Double_t pad_height;

    /// Calculate the relative text size, independent of the size of the pad
    pad_width  = gPad->XtoPixel(gPad->GetX2());
    pad_height = gPad->YtoPixel(gPad->GetY1());
    if (pad_width < pad_height){relativeTextSize = 1/pad_width;}
    else{relativeTextSize = 1/pad_height;}
    relativeTextSize = textSizeFactor * relativeTextSize;

    /// Use the standardtised PlotArray function to loop over the histogram an to draw the elements.
    if(!markerArray && !colorArray){PlotArray(histArray,controlstring,defaultColorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else if(!markerArray){PlotArray(histArray,controlstring,colorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else if(!colorArray){ PlotArray(histArray,controlstring,defaultColorArray,markerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else{PlotArray(histArray,controlstring,colorArray,markerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}

    /// Go to the ratio pad and repeat the procedure
    lowerPad->cd();
    if(!markerArray && !colorArray){PlotArray(ratioArray,controlstring,defaultColorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else if(!markerArray){PlotArray(ratioArray,controlstring,colorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else if(!colorArray){PlotArray(ratioArray,controlstring,defaultColorArray,markerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else{PlotArray(ratioArray,controlstring,colorArray,markerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    upperPad->cd();

  }else{// If no ratios are given:

    newCanvas->SetLeftMargin(sideMargin);
    newCanvas->SetRightMargin(sideMargin);
    newCanvas->SetTopMargin(topMargin);
    newCanvas->SetBottomMargin(lowMargin);


    newCanvas->SetTickx(1);
    newCanvas->SetTicky(1);


    if(control.Contains("logX")||control.Contains("logx")||control.Contains("LogX")||control.Contains("LOGX")){
      newCanvas->SetLogx(1); cout<<"| - LogX"<<endl;}
    if(control.Contains("logY")||control.Contains("logy")||control.Contains("LogY")||control.Contains("LOGY")){
      newCanvas->SetLogy(1); cout<<"| - LogY"<<endl;}
    if(control.Contains("logZ")||control.Contains("logz")||control.Contains("LogZ")||control.Contains("LOGZ")){
        newCanvas->SetLogz(1); cout<<"| - LogZ"<<endl;}
    if(control.Contains("gridY")||control.Contains("gridy")||control.Contains("GridY")||control.Contains("GRIDY")){
      if(!control.Contains("atio") ) newCanvas->SetGridy(1);cout<<"| - Grid Y"<<endl;
    }

    Double_t relativeTextSize;
    Double_t pad_width  = gPad->XtoPixel(gPad->GetX2());
    Double_t pad_height = gPad->YtoPixel(gPad->GetY1());
    if (pad_width < pad_height){relativeTextSize = 1/pad_width;}
    else{relativeTextSize = 1/pad_height;}
    relativeTextSize = textSizeFactor * relativeTextSize;

    if(!markerArray && !colorArray){PlotArray(histArray,controlstring,defaultColorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else if(!markerArray){PlotArray(histArray,controlstring,colorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else if(!colorArray){PlotArray(histArray,controlstring,defaultColorArray,markerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
    else{PlotArray(histArray,controlstring,colorArray,markerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);}
  }

  /// Draw extras
  /// Timestamp
  if(!control.Contains("Notime")&&!control.Contains("NoTime")&&!control.Contains("notime")&&!control.Contains("NOTIME")){
    TTimeStamp time;
    TString stTime;
    stTime=Form("#color[16]{%i}",time.GetDate());
    TLatex *timeStamp = new TLatex();
    timeStamp->SetNDC();
    timeStamp->SetTextAlign(22);
    timeStamp->SetTextFont(63);
    timeStamp->SetTextSizePixels(10);
    timeStamp->DrawLatex(0.96,0.98,stTime);
  }else{
    cout<<"|         - No Timestamp"<<endl;
  }
  /// Text "This work"
  if(control.Contains("ThisWork")||control.Contains("Thiswork")||control.Contains("thiswork")||control.Contains("THISWORK")){
    TLatex *thsWrk = new TLatex();
    thsWrk->SetNDC();
    thsWrk->SetTextAlign(22);
    thsWrk->SetTextFont(43);
    thsWrk->SetTextSizePixels(20);
    thsWrk->DrawLatex(0.83,0.94,"#color[16]{this work}");
  }
  /// Unified or work logo
  if(control.Contains("Logo") ||control.Contains("logo") ||control.Contains("LOGO")){
    Double_t scale = 0.1;
    Double_t x1 = 0.7; Double_t y1 = 0.7;
    if(control.Contains("square")||control.Contains("Square")||control.Contains("SQUARE")){ x1 = 0.7;  y1 = 0.7;}
    else if (control.Contains("horizontal")||control.Contains("Horizontal")||control.Contains("HORIZONTAL")){ x1 = 0.75;  y1 = 0.8;}
    else{ x1 = 0.7;  y1 = 0.8;}
    Double_t x2 = x1+scale;
    Double_t y2 = y1+scale;

    const char* logopath = nullptr;
    if(control.Contains("Unified") ||control.Contains("unified") ||control.Contains("UNIFIED")){logopath = "./graphics/einhorn.pdf";}
    if(control.Contains("Work") ||control.Contains("work") ||control.Contains("WORK")){logopath = "./graphics/wrkprgs.png";}
    TImage* logo = TImage::Open(logopath);
    TPad *pad_logo = new TPad("pad_logo","pad_logo",x1,y1,x2,y2);
    pad_logo->SetMargin(0,0,0,0);
    newCanvas->cd();
    pad_logo->Draw();
    pad_logo->cd();
    logo->Draw();
  }

  if(ratioArray && upperPad){upperPad->cd();}
  cout<<"|_____________________________________________________________________________"<<endl;
  cout<<" "<<endl;
  return newCanvas;
}

TCanvas *makeCanvas4Pads(TObjArray *arrHist_TL, TObjArray *arrHist_TR,TObjArray *arrHist_BL, TObjArray *arrHist_BR,const char *controlstring="", Short_t *colorArray=0, Short_t *markerArray=0){

  cout<<" ____________________________________________________________________________"<<endl;
  cout<<"|"<<endl;
  cout<<"| Plots generated via makeCanvas for four"<<endl;

  gStyle->SetTextFont(43);
  TString control(controlstring);
  if(control.Contains("OwnColors")){DefineOwnColors();}

  Int_t nHist = arrHist_TL->GetEntries();
  if(nHist == 0){cout<<"| ERROR: Arrays empty"<<endl; return (TCanvas*)0x0;}

  Float_t lableSize = 1.2; // Factor the label will be larger than the relative textsize
  Float_t titleSize = 1.25; // Factor the title will be larger than the relative textsize
  Float_t textSizeFactor = 11000;
  Int_t lableFont= 43;
  Int_t titleFont= 43;

  Float_t canvasWidth;
  Float_t canvasHeight;
  Float_t padFraction;
  Float_t titleOffsetY;
  Float_t titleOffsetX;
  Float_t sideMargin;
  Float_t topMargin;
  Float_t lowMargin;

  if(control.Contains("square")||control.Contains("Square")||control.Contains("SQUARE")){
    cout<<"| - Square Canvas"<<endl;
    cout<<"|   Optimized for 0.49 * textwidth"<<endl;
    canvasWidth=700;
    canvasHeight=canvasWidth;
    titleOffsetY=3.2;
    titleOffsetX=2.7;
    sideMargin=0.24;
    padFraction = 0.5;
    topMargin = (sideMargin);
    lowMargin = (sideMargin);
    if(control.Contains("FullPage")||control.Contains("fullpage")){
      cout<<"|   Optimized for 0.99 * textwidth"<<endl;
      canvasWidth=2*700;
      textSizeFactor = 2 * textSizeFactor;
      canvasHeight=canvasWidth;
    }
    textSizeFactor = padFraction * textSizeFactor;
  }else{
    cout<<"| - Improved aspect ratio Canvas"<<endl;
    canvasWidth=700;
    canvasHeight=700*1.41421356;
    titleOffsetY=2;
    titleOffsetX=4.5;
    padFraction = 0.5;
    sideMargin = 0.24;
    topMargin = 0.24 * 1.414213562/2;
    lowMargin = 0.24 * 1.414213562/2;
    textSizeFactor = padFraction * textSizeFactor;
  }

  TRandom *random = new TRandom(arrHist_TL->Hash());
  Double_t xMin;
  Double_t xMax;

  TString title("");
  if(arrHist_TL->At(0)){
    if (!arrHist_TL->At(0)->InheritsFrom("TH1")){cout<<"|ERROR: First entry has to be an Histogram"<<endl;}
    TH1D *hist_TL_0 = (TH1D*) arrHist_TL->At(0);
    if(!hist_TL_0){cout<<"| ERROR: Histogram empty"<<endl;}
    title("default");
    title += Form("%f",random->Rndm());
    xMin = hist_TL_0->GetXaxis()->GetBinLowEdge(hist_TL_0->GetXaxis()->GetFirst())+0.0000001;
    xMax = hist_TL_0->GetXaxis()->GetBinUpEdge(hist_TL_0->GetXaxis()->GetLast())-0.0000001;
    cout<<"| - Axis range: "<<xMin<<"-"<<xMax<<endl;
    hist_TL_0->GetYaxis()->CenterTitle(0);
    if(control.Contains("CenterTitle")||control.Contains("centertitle")) hist_TL_0->GetYaxis()->CenterTitle(1);
  }
  if(arrHist_TR->At(0)){
    if (!arrHist_TR->At(0)->InheritsFrom("TH1")){cout<<"|ERROR: First entry has to be an Histogram"<<endl;}
    TH1D *hist_TR_0 = (TH1D*) arrHist_TR->At(0);
    if(!hist_TR_0){cout<<"| ERROR: Histogram empty"<<endl;}
    hist_TR_0->GetXaxis()->SetTickLength(0.06);
    hist_TR_0->GetYaxis()->SetNdivisions(506);
    hist_TR_0->SetLineWidth(2);
    hist_TR_0->GetYaxis()->CenterTitle(1);
  }
  if(arrHist_BL->At(0)){
    if (!arrHist_BL->At(0)->InheritsFrom("TH1")){cout<<"|ERROR: First entry has to be an Histogram"<<endl;}
    TH1D *hist_BL_0 = (TH1D*) arrHist_BL->At(0);
    if(!hist_BL_0){cout<<"| ERROR: Histogram empty"<<endl;}
    hist_BL_0->GetXaxis()->SetTickLength(0.06);
    hist_BL_0->GetYaxis()->SetNdivisions(506);
    hist_BL_0->SetLineWidth(2);
    hist_BL_0->GetYaxis()->CenterTitle(1);
  }
  if(arrHist_BR->At(0)){
    if (!arrHist_BR->At(0)->InheritsFrom("TH1")){cout<<"|ERROR: First entry has to be an Histogram"<<endl;}
    TH1D *hist_BR_0 = (TH1D*) arrHist_BR->At(0);
    if(!hist_BR_0){cout<<"| ERROR: Histogram empty"<<endl;}
    hist_BR_0->GetXaxis()->SetTickLength(0.06);
    hist_BR_0->GetYaxis()->SetNdivisions(506);
    hist_BR_0->SetLineWidth(2);
    hist_BR_0->GetYaxis()->CenterTitle(1);
  }




  /// Create Canvas in a given size
  TCanvas *newCanvas = new TCanvas(title,title,10,10,canvasWidth+10,canvasHeight+10);
  newCanvas->SetFillStyle(4000);
  newCanvas->cd();

  /// Create upper pad for the histograms \remark upper pad has 72%
  TPad *pad_TL = new TPad("pad_TL","Top Left"     ,0            ,padFraction ,padFraction ,1);
  TPad *pad_TR = new TPad("pad_TR","Top Right"    ,padFraction  ,padFraction ,1           ,1);
  TPad *pad_BL = new TPad("pad_BL","Bottom Left"  ,0            ,0           ,padFraction ,padFraction);
  TPad *pad_BR = new TPad("pad_BR","Bottom Right" ,padFraction  ,0           ,1           ,padFraction);

  pad_TL->SetFillStyle(4000);
  pad_TL->SetTopMargin(topMargin);
  pad_TL->SetLeftMargin(sideMargin);
  pad_TL->SetRightMargin(0.0);
  pad_TL->SetBottomMargin(0.0);
  pad_TL->SetTickx(1);
  pad_TL->SetTicky(1);
  pad_TL->Draw();

  pad_TR->SetFillStyle(4000);
  pad_TR->SetTopMargin(topMargin);
  pad_TR->SetLeftMargin(0.0);
  pad_TR->SetRightMargin(sideMargin);
  pad_TR->SetBottomMargin(0.0);
  pad_TR->SetTickx(1);
  pad_TR->SetTicky(1);
  pad_TR->Draw();


  pad_BL->SetFillStyle(4000);
  pad_BL->SetTopMargin(0.0);
  pad_BL->SetLeftMargin(sideMargin);
  pad_BL->SetRightMargin(0.0);
  pad_BL->SetBottomMargin(lowMargin);
  pad_BL->SetTickx(1);
  pad_BL->SetTicky(1);
  pad_BL->Draw();


  pad_BR->SetFillStyle(4000);
  pad_BR->SetTopMargin(0.0);
  pad_BR->SetLeftMargin(0.0);
  pad_BR->SetRightMargin(sideMargin);
  pad_BR->SetBottomMargin(lowMargin);
  pad_BR->SetTickx(1);
  pad_BR->SetTicky(1);
  pad_BR->Draw();

  if(control.Contains("logX")||control.Contains("logx")||control.Contains("LogX")||control.Contains("LOGX")){
    cout<<"| - LogX"<<endl;
    pad_TL->SetLogx(1);
    pad_TR->SetLogx(1);
    pad_BL->SetLogx(1);
    pad_BR->SetLogx(1);
  }
  if(control.Contains("logY")||control.Contains("logy")||control.Contains("LogY")||control.Contains("LOGY")){
    cout<<"| - LogY"<<endl;
    pad_TL->SetLogy(1);
    pad_TR->SetLogy(1);
    pad_BL->SetLogy(1);
    pad_BR->SetLogy(1);
  }

  newCanvas->cd();
  pad_TL->cd();

  Double_t relativeTextSize;
  Double_t pad_width;
  Double_t pad_height;

  Short_t defaultMarkerArray[14] = {0};
  if(!markerArray) {
      Short_t copyArray[14]={20,21,22,23,24,25,26,27,28,29,30,20,20,20};
      for (int i =0; i<14; ++i) {
          defaultMarkerArray[i] = copyArray[i];
      }
  }

  /// Calculate the relative text size, independent of the size of the pad
  pad_width  = gPad->XtoPixel(gPad->GetX2());
  pad_height = gPad->YtoPixel(gPad->GetY1());
  if (pad_width < pad_height){relativeTextSize = 1/pad_width;}
  else{relativeTextSize = 1/pad_height;}
  relativeTextSize = textSizeFactor * relativeTextSize;

  /// Use the standardtised PlotArrayTools function to loop over the histogram an to draw the elements.
  PlotArrayTools(arrHist_TL,controlstring,colorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);
  pad_TR->cd();
  PlotArrayTools(arrHist_TR,controlstring,colorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);

  pad_BL->cd();
  PlotArrayTools(arrHist_BL,controlstring,colorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);

  pad_BR->cd();
  PlotArrayTools(arrHist_BR,controlstring,colorArray,defaultMarkerArray,relativeTextSize,lableSize, titleSize, lableFont, titleFont, titleOffsetY, titleOffsetX, sideMargin, xMin,xMax);

  pad_TL->cd();
  cout<<"|_____________________________________________________________________________"<<endl;
  cout<<" "<<endl;
  return newCanvas;
}


void CanvasPartition(TCanvas *C,const Int_t Nx = 2,const Int_t Ny = 2,
                     Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05);

TCanvas* makeCanvasRaa(TH1D* Raa[9], TGraphErrors* SysUnc[9], TH1D* RaaToCompare[9], TGraphErrors* SysUncToCompare[9], TLegend* legend[11]) ///https://root.cern/doc/v610/canvas2_8C.html
{
    gStyle->SetOptStat(0);
    TCanvas *C = (TCanvas*) gROOT->FindObject("C");
    if (C) delete C;
    //C = new TCanvas("C","canvas",1024,640);
    C = new TCanvas("C","canvas",800,800);
    C->SetFillStyle(4000);
    // Number of PADS
    const Int_t Nx = 3;
    const Int_t Ny = 3;
    // Margins
    Float_t lMargin = 0.12;
    Float_t rMargin = 0.05;
    Float_t bMargin = 0.15;
    Float_t tMargin = 0.05;
    // Canvas setup
    CanvasPartition(C,Nx,Ny,lMargin,rMargin,bMargin,tMargin);

    TLine* l1 = new TLine(0.0, 1.0, 100.0, 1.0);
    l1->SetLineColor(kBlack);
    l1->SetLineWidth(2.);
    l1->SetLineStyle(8);

    TH1F *RaaFrame[9];
    int icount;
    int const centArray[] = {0,5,10,20,30,40,50,60,70,80};

    double pad_width, pad_height, relativeTextSize;
    double textSizeFactor = 12000;

    TPad *pad[Nx][Ny];
    for (Int_t i=0;i<Nx;i++) {
        for (Int_t j=0;j<Ny;j++) {
            if(i==0 && j==2) icount=0;
            if(i==1 && j==2) icount=1;
            else if(i==2 && j==2) icount=2;
            else if(i==0 && j==1) icount=3;
            else if(i==1 && j==1) icount=4;
            else if(i==2 && j==1) icount=5;
            else if(i==0 && j==0) icount=6;
            else if(i==1 && j==0) icount=7;
            else if (i==2 && j==0)(icount=8);

            C->cd(0);
            // Get the pads previously created.
            char pname[16];
            snprintf(pname, sizeof(pname), "pad_%i_%i", i, j);
            pad[i][j] = (TPad*) gROOT->FindObject(pname);
            pad[i][j]->Draw();
            pad[i][j]->SetFillStyle(4000);
            pad[i][j]->SetFrameFillStyle(4000);
            pad[i][j]->cd();
            // Size factors
            Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
            Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();
            Float_t normArea = 1e6*(yFactor*xFactor)/(800*800);
            char hname[16];
            snprintf(hname,sizeof(hname),"Raa_%i_%i",i,j);
            RaaFrame[icount] = (TH1F*) Raa[icount]->Clone(hname);
            // RaaFrame[icount] ->Reset();
            RaaFrame[icount] ->Draw("EX0");
            SysUnc[icount] ->Draw("same 5");
            if(RaaToCompare) RaaToCompare[icount] ->Draw("sameEX0");
            if(RaaToCompare && SysUncToCompare) SysUncToCompare[icount] ->Draw("same 5");
            l1->Draw("same");
            // y axis range
            //RaaFrame[icount] ->GetYaxis()->SetRangeUser(0.0001,1.2*Raa[icount]->GetMaximum());
            // Format for y axis
            RaaFrame[icount] ->GetYaxis()->SetLabelFont(43);
            RaaFrame[icount] ->GetYaxis()->SetLabelSize(18);
            RaaFrame[icount] ->GetYaxis()->SetLabelOffset(0.02);
            RaaFrame[icount] ->GetYaxis()->SetTitleFont(43);
            RaaFrame[icount] ->GetYaxis()->SetTitleSize(22);
            RaaFrame[icount] ->GetYaxis()->SetTitleOffset(2.8);
            //RaaFrame[icount] ->GetYaxis()->SetNdivisions(510, kFALSE);
            // TICKS Y Axis
            RaaFrame[icount] ->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
            // Format for x axis
            RaaFrame[icount] ->GetXaxis()->SetLabelFont(43);
            RaaFrame[icount] ->GetXaxis()->SetLabelSize(18);
            RaaFrame[icount] ->GetXaxis()->SetLabelOffset(0.02);
            RaaFrame[icount] ->GetXaxis()->SetTitleFont(43);
            RaaFrame[icount] ->GetXaxis()->SetTitleSize(22);
            RaaFrame[icount] ->GetXaxis()->SetTitleOffset(2.8);
            RaaFrame[icount]->GetXaxis()->SetRangeUser(-1, 51.);
            RaaFrame[icount] ->GetXaxis()->SetNdivisions(310);
            RaaFrame[icount]->GetXaxis()->SetRangeUser(-0.5, 70);

            //RaaFrame[icount] ->GetYaxis()->SetNdivisions(5);
            // TICKS X Axis
            RaaFrame[icount]->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
            if(icount>2) RaaFrame[icount]->GetYaxis()->SetTitle("");
            RaaFrame[icount]->Draw("same EX0");

            /// Calculate the relative text size, independent of the size of the pad
            pad_width  = gPad->XtoPixel(gPad->GetX2());
            pad_height = gPad->YtoPixel(gPad->GetY1());
            if (pad_width < pad_height){relativeTextSize = 1/pad_width;}
            else{relativeTextSize = 1/pad_height;}
            relativeTextSize = textSizeFactor * relativeTextSize;

            legend[icount]->SetBorderSize(0);
            legend[icount]->SetFillStyle(0);
            legend[icount]->SetTextSize(relativeTextSize/700);
            legend[icount]->Draw("same");
            legend[9]->SetBorderSize(0);
            legend[9]->SetFillStyle(0);
            legend[9]->SetTextSize(relativeTextSize/700);
            legend[10]->SetBorderSize(0);
            legend[10]->SetFillStyle(0);
            legend[10]->SetTextSize(relativeTextSize/700);

            if(icount==0) legend[10]->Draw("same");
            if(icount==1) legend[9]->Draw("same");

        }
   }
 C->cd();
 return C;
}

TCanvas* makeCanvasPtReso(TH1D* corrPtReso[9], TH1D* corrPtResoToCompare[9],TLegend* legend[10]) ///https://root.cern/doc/v610/canvas2_8C.html
{
    gStyle->SetOptStat(0);
    TCanvas *C = (TCanvas*) gROOT->FindObject("C");
    if (C) delete C;
    //C = new TCanvas("C","canvas",1024,640);
    C = new TCanvas("C","canvas",800,800);
    C->SetFillStyle(4000);
    // Number of PADS
    const Int_t Nx = 3;
    const Int_t Ny = 3;
    // Margins
    Float_t lMargin = 0.12;
    Float_t rMargin = 0.05;
    Float_t bMargin = 0.15;
    Float_t tMargin = 0.05;
    // Canvas setup
    CanvasPartition(C,Nx,Ny,lMargin,rMargin,bMargin,tMargin);

    TLine* l1 = new TLine(0.0, 1.0, 100.0, 1.0);
    l1->SetLineColor(kBlack);
    l1->SetLineWidth(2.);
    l1->SetLineStyle(8);

    TH1F *corrPtResoFrame[9];
    int icount;
    int const centArray[] = {0,5,10,20,30,40,50,60,70,80};

    double pad_width, pad_height, relativeTextSize;
    double textSizeFactor = 12000;

    TPad *pad[Nx][Ny];
    for (Int_t i=0;i<Nx;i++) {
        for (Int_t j=0;j<Ny;j++) {
            if(i==0 && j==2) icount=0;
            if(i==1 && j==2) icount=1;
            else if(i==2 && j==2) icount=2;
            else if(i==0 && j==1) icount=3;
            else if(i==1 && j==1) icount=4;
            else if(i==2 && j==1) icount=5;
            else if(i==0 && j==0) icount=6;
            else if(i==1 && j==0) icount=7;
            else if (i==2 && j==0)(icount=8);

            C->cd(0);
            // Get the pads previously created.
            char pname[16];
            snprintf(pname, sizeof(pname), "pad_%i_%i", i, j);
            pad[i][j] = (TPad*) gROOT->FindObject(pname);
            pad[i][j]->Draw();
            pad[i][j]->SetFillStyle(4000);
            pad[i][j]->SetFrameFillStyle(4000);
            pad[i][j]->cd();
            pad[i][j]->SetLogx();

            // Size factors
            Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
            Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();
            Float_t normArea = 1e6*(yFactor*xFactor)/(800*800);
            char hname[16];
            snprintf(hname,sizeof(hname),"Raa_%i_%i",i,j);
            corrPtResoFrame[icount] = (TH1F*) corrPtReso[icount]->Clone(hname);
            // corrPtResoFrame[icount] ->Reset();
            corrPtResoFrame[icount] ->Draw();
            if(corrPtResoToCompare) corrPtResoToCompare[icount] ->Draw("same");
            //l1->Draw("same");
            // y axis range
            //corrPtResoFrame[icount] ->GetYaxis()->SetRangeUser(0.0001,1.2*corrPtReso[icount]->GetMaximum());
            // Format for y axis
            corrPtResoFrame[icount] ->GetYaxis()->SetLabelFont(43);
            corrPtResoFrame[icount] ->GetYaxis()->SetLabelSize(18);
            corrPtResoFrame[icount] ->GetYaxis()->SetLabelOffset(0.02);
            corrPtResoFrame[icount] ->GetYaxis()->SetTitleFont(43);
            corrPtResoFrame[icount] ->GetYaxis()->SetTitleSize(22);
            corrPtResoFrame[icount] ->GetYaxis()->SetTitleOffset(2.8);
            //corrPtResoFrame[icount] ->GetYaxis()->SetNdivisions(510, kFALSE);
            // TICKS Y Axis
            corrPtResoFrame[icount] ->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
            // Format for x axis
            corrPtResoFrame[icount] ->GetXaxis()->SetLabelFont(43);
            corrPtResoFrame[icount] ->GetXaxis()->SetLabelSize(18);
            corrPtResoFrame[icount] ->GetXaxis()->SetLabelOffset(0.02);
            corrPtResoFrame[icount] ->GetXaxis()->SetTitleFont(43);
            corrPtResoFrame[icount] ->GetXaxis()->SetTitleSize(22);
            corrPtResoFrame[icount] ->GetXaxis()->SetTitleOffset(2.8);
            corrPtResoFrame[icount] ->GetXaxis()->SetNdivisions(310);
            corrPtResoFrame[icount]->GetXaxis()->SetRangeUser(7, 100.);
            corrPtResoFrame[icount]->GetYaxis()->SetRangeUser(0.978, 1.008);


            //corrPtResoFrame[icount] ->GetYaxis()->SetNdivisions(5);
            // TICKS X Axis
            corrPtResoFrame[icount]->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
            if(icount>2) corrPtResoFrame[icount]->GetYaxis()->SetTitle("");
            corrPtResoFrame[icount]->Draw("same");

            /// Calculate the relative text size, independent of the size of the pad
            pad_width  = gPad->XtoPixel(gPad->GetX2());
            pad_height = gPad->YtoPixel(gPad->GetY1());
            if (pad_width < pad_height){relativeTextSize = 1/pad_width;}
            else{relativeTextSize = 1/pad_height;}
            relativeTextSize = textSizeFactor * relativeTextSize;

            legend[icount]->SetBorderSize(0);
            legend[icount]->SetFillStyle(0);
            legend[icount]->SetTextSize(relativeTextSize/700);
            legend[icount]->Draw("same");
            legend[9]->SetBorderSize(0);
            legend[9]->SetFillStyle(0);
            legend[9]->SetTextSize(relativeTextSize/700);
            legend[9]->SetBorderSize(0);
            legend[9]->SetFillStyle(0);
            legend[9]->SetTextSize(relativeTextSize/700);

           if(icount==1) legend[9]->Draw("same");

        }
   }
 C->cd();
 C->SetLogx(1);
 return C;
}


TCanvas* multcanvas(TH1D* h[], int Ncanvas)
{
   gStyle->SetOptStat(0);
   TCanvas *C = (TCanvas*) gROOT->FindObject("C");
   if (C) delete C;
   C = new TCanvas("C","canvas",1024,640);
   C->SetFillStyle(4000);

   int Nx = 1;
   int Ny = 2;

   switch (Ncanvas) {
     case 6:
      Nx = 2;
      Ny = 3;
      break;
    case 9:
      Nx = 3;
      Ny = 3;
      break;
    case 12:
      Nx = 3;
      Ny = 4;
      break;
   }

   // Margins
   Float_t lMargin = 0.12;
   Float_t rMargin = 0.05;
   Float_t bMargin = 0.15;
   Float_t tMargin = 0.05;
   // Canvas setup
   CanvasPartition(C,Nx,Ny,lMargin,rMargin,bMargin,tMargin);
   // configuration of histogram


   TPad *pad[Nx][Ny];
   for(Int_t n = 0; n < 6; n++){
     for (Int_t i=0;i<Nx;i++) {
     for (Int_t j=0;j<Ny;j++) {
        C->cd(0);
        // Get the pads previously created.
        char pname[16];
        snprintf(pname, sizeof(pname), "pad_%i_%i", i, j);
        pad[i][j] = (TPad*) gROOT->FindObject(pname);
        pad[i][j]->Draw();
        pad[i][j]->SetFillStyle(4000);
        pad[i][j]->SetFrameFillStyle(4000);
        pad[i][j]->cd();
        // Size factors
        Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
        Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();
        char hname[16];
        snprintf(hname,sizeof(hname), "h_%i_%i",i,j);
          TH1F *hFrame = (TH1F*) h[n]->Clone(hname);
        hFrame->Reset();
        hFrame->Draw();
        // y axis range
        hFrame->GetYaxis()->SetRangeUser(0.0001,1.2*h[n]->GetMaximum());
        // Format for y axis
        hFrame->GetYaxis()->SetLabelFont(43);
        hFrame->GetYaxis()->SetLabelSize(16);
        hFrame->GetYaxis()->SetLabelOffset(0.02);
        hFrame->GetYaxis()->SetTitleFont(43);
        hFrame->GetYaxis()->SetTitleSize(16);
        hFrame->GetYaxis()->SetTitleOffset(5);
        hFrame->GetYaxis()->CenterTitle();
        hFrame->GetYaxis()->SetNdivisions(505);
        // TICKS Y Axis
        hFrame->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
        // Format for x axis
        hFrame->GetXaxis()->SetLabelFont(43);
        hFrame->GetXaxis()->SetLabelSize(16);
        hFrame->GetXaxis()->SetLabelOffset(0.02);
        hFrame->GetXaxis()->SetTitleFont(43);
        hFrame->GetXaxis()->SetTitleSize(16);
        hFrame->GetXaxis()->SetTitleOffset(5);
        hFrame->GetXaxis()->CenterTitle();
        hFrame->GetXaxis()->SetNdivisions(505);
        // TICKS X Axis
        hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
        h[n]->Draw("same");
     }
     }

   }

   C->cd();
   return C;
}

void CanvasPartition(TCanvas *C,const Int_t Nx,const Int_t Ny,
                     Float_t lMargin, Float_t rMargin,
                     Float_t bMargin, Float_t tMargin)
{
   if (!C) return;
   // Setup Pad layout:
   Float_t vSpacing = 0.0;
   Float_t vStep  = (1.- bMargin - tMargin - (Ny-1) * vSpacing) / Ny;
   Float_t hSpacing = 0.0;
   Float_t hStep  = (1.- lMargin - rMargin - (Nx-1) * hSpacing) / Nx;
   Float_t vposd,vposu,vmard,vmaru,vfactor;
   Float_t hposl,hposr,hmarl,hmarr,hfactor;
   for (Int_t i=0;i<Nx;i++) {
      if (i==0) {
         hposl = 0.0;
         hposr = lMargin + hStep;
         hfactor = hposr-hposl;
         hmarl = lMargin / hfactor;
         hmarr = 0.0;
      } else if (i == Nx-1) {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep + rMargin;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = rMargin / (hposr-hposl);
      } else {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = 0.0;
      }
      for (Int_t j=0;j<Ny;j++) {
         if (j==0) {
            vposd = 0.0;
            vposu = bMargin + vStep;
            vfactor = vposu-vposd;
            vmard = bMargin / vfactor;
            vmaru = 0.0;
         } else if (j == Ny-1) {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep + tMargin;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = tMargin / (vposu-vposd);
         } else {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = 0.0;
         }
         C->cd(0);
         char name[16];
         snprintf(name, sizeof(name), "pad_%i_%i", i, j);
         TPad *pad = (TPad*) gROOT->FindObject(name);
         if (pad) delete pad;
         pad = new TPad(name,"",hposl,vposd,hposr,vposu);
         pad->SetLeftMargin(hmarl);
         pad->SetRightMargin(hmarr);
         pad->SetBottomMargin(vmard);
         pad->SetTopMargin(vmaru);
         pad->SetFrameBorderMode(0);
         pad->SetBorderMode(0);
         pad->SetBorderSize(0);
         pad->Draw();
      }
   }
}

void *DefineOwnColors(const char *palette="std"){
  TString Palette(palette);
  if(Palette.Contains("TUD")){
    if(gROOT->GetColor(4202) && gROOT->GetColor(5202)){ return 0;}
    TColor * a1 =new TColor(4101,0.36,0.52,0.76);
    TColor * a2 =new TColor(4102,0.0,0.61,0.85);
    TColor * a3 =new TColor(4103,0.31,0.71,0.58);
    TColor * a4 =new TColor(4104,0.69,0.80,0.31);
    TColor * a5 =new TColor(4105,0.87,0.87,0.28);
    TColor * a6 =new TColor(4106,1.0,0.88,0.36);
    TColor * a7 =new TColor(4107,0.97,0.73,0.24);
    TColor * a8 =new TColor(4108,0.93,0.48,0.20);
    TColor * a9 =new TColor(4109,0.91,0.38,0.24);
    TColor * a10 =new TColor(4110,0.79,0.19,0.56);
    TColor * a11 =new TColor(4111,0.50,0.27,0.59);

    TColor * b1 =new TColor(4201,0.0,0.42,0.66);
    TColor * b2 =new TColor(4202,0.0,0.51,0.80);
    TColor * b2o =new TColor(5202,0.0,1.4*0.51,1);
    TColor * b3 =new TColor(4203,0.0,0.62,0.51);
    TColor * b4 =new TColor(4204,0.6,0.753,0.0);
    TColor * b4o =new TColor(5204,1.4*0.6,1,0.0);
    TColor * b5 =new TColor(4205,0.79,0.83,0.0);
    TColor * b6 =new TColor(4206,0.99,0.79,0.0);
    TColor * b7 =new TColor(4207,0.96,0.65,0.0);
    TColor * b8 =new TColor(4208,0.93,0.40,0.0);
    TColor * b9 =new TColor(4209,0.91,0.0,0.10);
    TColor * b90 =new TColor(5209,0.91,0.38,0.24);
    TColor * b10 =new TColor(4210,0.66,0.0,0.52);
    TColor * b11 =new TColor(4211,0.45,0.06,0.52);

    TColor * c1 =new TColor(4301,0.0,0.31,0.54);
    TColor * c2 =new TColor(4302,0.0,0.41,0.62);
    TColor * c3 =new TColor(4303,0.0,0.53,0.47);
    TColor * c4 =new TColor(4304,0.50,0.67,0.09);
    TColor * c5 =new TColor(4305,0.69,0.74,0.0);
    TColor * c6 =new TColor(4306,0.84,0.67,0.0);
    TColor * c7 =new TColor(4307,0.82,0.53,0.0);
    TColor * c8 =new TColor(4308,0.80,0.30,0.01);
    TColor * c9 =new TColor(4309,0.73,0.06,0.13);
    TColor * c10 =new TColor(4310,0.6,0.07,0.41);
    TColor * c11 =new TColor(4311,0.38,0.11,0.45);

    TColor * d1 =new TColor(4401,0.14,0.21,0.45);
    TColor * d2 =new TColor(4402,0.0,0.31,0.45);
    TColor * d3 =new TColor(4403,0.0,0.44,0.37);
    TColor * d4 =new TColor(4404,0.42,0.73,0.13);
    TColor * d5 =new TColor(4405,0.60,0.65,0.2);
    TColor * d6 =new TColor(4406,0.68,0.56,0.0);
    TColor * d7 =new TColor(4407,0.74,0.44,0.0);
    TColor * d8 =new TColor(4408,0.66,0.29,0.7);
    TColor * d9 =new TColor(4409,0.60,0.11,0.15);
    TColor * d10 =new TColor(4410,0.45,0.13,0.39);
    TColor * d11 =new TColor(4411,0.30,0.13,0.42);
  }

  else{
    if(gROOT->GetColor(4001)){
      cout<<"| Colors already defined"<<endl;
      return 0;
    }else{
      TColor * m01 =new TColor(4001,0.267, 0.467, 0.667);
      TColor * m02 =new TColor(4002,0.8, 0.4, 0.467);
      TColor * m03 =new TColor(4003,0.867, 0.8, 0.467);
      TColor * m04 =new TColor(4004,0.067, 0.467, 0.2);
      TColor * m05 =new TColor(4005,0.533, 0.8, 0.933);
      TColor * m06 =new TColor(4006,0.2, 0.133, 0.533);
      TColor * m07 =new TColor(4007,0.267, 0.667, 0.6);
      TColor * m08 =new TColor(4008,0.6, 0.6, 0.2);
      TColor * m09 =new TColor(4009,0.533, 0.133, 0.333);
      TColor * m10 =new TColor(4010,0.4, 0.067, 0);
      TColor * m11 =new TColor(4011,0.4, 0.6, 0.8);
      TColor * m12 =new TColor(4012,0.667, 0.267, 0.4);
    }
  }
  return 0;
}

void SetHistogramProperties(TH1D* &h, TString YTitle, Int_t ColorIndex)
{
  if(!h) {Printf("Can't read Histogram!"); return;}
  h->SetTitle("");
  h->SetLineWidth(2);
  h->SetTitleFont(43);
  h->SetTitleSize(0);
  h->SetXTitle("#it{p}_{T} (GeV/#it{c})");
  h->SetYTitle(YTitle);
  h->GetXaxis()->SetTitleOffset(1.3);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetTitleSize(22, "X");
  h->SetTitleSize(0.3f, "Y");
  h->SetTitleFont(43, "X");
  h->SetTitleFont(43, "Y");
  h->SetLabelFont(43, "X");
  h->SetLabelFont(43, "Y");
  h->SetLabelSize(24, "X");
  h->SetLabelSize(24, "Y");
  h->SetMarkerSize(1.0);
  h->SetStats(kFALSE);
  h->GetXaxis()->SetRangeUser(minXValue, fPtPlotRange);
  h->SetMarkerColor(GetNiceColor(ColorIndex));
  h->SetMarkerStyle(kFullCircle);
  h->SetLineColor(GetNiceColor(ColorIndex));
  Double_t minValue = 1e10;
  Double_t maxValue = -1;

  for(Int_t bin=1; bin<h->GetNbinsX()+1; bin++)
  {
    Double_t binContent = h->GetBinContent(bin);
    if(binContent<=0) continue; /// reject empty bins
    if(h->GetBinLowEdge(bin)<minXValue) continue; /// only in visable area
    if(h->GetBinLowEdge(bin)+h->GetBinWidth(bin)>maxXValue) continue;
    if(binContent>maxValue) maxValue = binContent;
    if(binContent<minValue) minValue = binContent;
  }
  h->GetYaxis()->SetRangeUser(0.95*minValue, 1.05*maxValue);
  if(minValue<minYValue) minYValue = minValue;
  if(maxValue>maxYValue) maxYValue = maxValue;
}

//SetHistogramProperties without x and y range
void SetHistogramPropertiesAlt(TH1D* &h, TString XTitle, TString YTitle, Int_t ColorIndex)
{
  if(!h) {Printf("Can't read Histogram!"); return;}
  h->SetTitle("");
  h->SetLineWidth(2);
  h->SetTitleFont(43);
  h->SetTitleSize(0);
  h->SetXTitle(XTitle);
  h->SetYTitle(YTitle);
  h->GetXaxis()->SetTitleOffset(1.3);
  h->GetYaxis()->SetTitleOffset(1.5);
  
  h->SetTitleSize(24, "X");
  h->SetTitleSize(24, "Y");
  h->SetTitleFont(43, "X");
  h->SetTitleFont(43, "Y");
  h->SetLabelFont(43, "X");
  h->SetLabelFont(43, "Y");
  h->SetLabelSize(24, "X");
  h->SetLabelSize(24, "Y");
  h->SetMarkerSize(1.3);
  h->SetStats(kFALSE);
  h->SetMarkerColor(GetNiceColor(ColorIndex));
  h->SetMarkerStyle(kFullCircle);
  h->SetLineColor(GetNiceColor(ColorIndex));
  TGaxis::SetMaxDigits(4);
}
void SetHistogramProperties2dim(TH2* hist2d, TString XTitle, TString YTitle)
{
  if(!hist2d) {Printf("Can't read Histogram!"); return;}
  hist2d->GetZaxis()->SetTitle("");
  hist2d->SetStats(0);
  hist2d->GetXaxis()->SetRange(0, hist2d->GetNbinsX());
  hist2d->GetYaxis()->SetRange(0, hist2d->GetNbinsY());
  hist2d->GetXaxis()->SetTitle(XTitle);
  hist2d->GetYaxis()->SetTitle(YTitle);
  TGaxis::SetMaxDigits(4);
}
Int_t GetLastFilledBin(TH1D* h)
{
    if(!h) return -1;

    Int_t iLastBin = 0;

    for(int i = 0; i<h->GetNbinsX(); i++)
    {
        //if(h->GetBinCenter(i)>350.) break;
        if(h->GetBinContent(i)>0.) iLastBin = i;
    }

    return iLastBin;
}

Int_t GetFirstFilledBin(TH1D* h)
    {
    int FirstBin=1;
    for(int i=1; i<h->GetNbinsX(); i++)
        {
        if(h->GetBinContent(i)>0.)
            {
            return(i);
            break;
            }
        }
    return 0; //Line added to avoid warning message
    }

double dMaxNch(TH1D* h)
    {
        return(h->GetBinCenter(GetLastFilledBin(h)));
    }

double GetIntegralFromBintoBin(TH1D* h, double lowBin, double highBin)
{
    if(!h) return 1;
    double integral = 0;
    for(int i = lowBin; i<= highBin; ++i)
    {
        integral+=h->GetBinContent(i);
    }
    return integral;
}

double GetIntegralFromTo(TH1D* h, double lowEnd, double highEnd)
{
    if(!h) return 1;
    int lowBin = h->FindBin(lowEnd);
    int highBin = h->FindBin(highEnd);
    double integral = 0;
    for(int i = lowBin; i<= highBin; ++i)
    {
        integral+=h->GetBinContent(i);
    }
    return integral;
}

TH1D* RebinPtSpectrum(const TH1D* h, Int_t nBins, const Double_t* xBins){
   if (!h) return 0;
   if (!nBins) return (TH1D*)h;
   if (!xBins) return (TH1D*)h;
   TH1D *h1 = (TH1D*) h->Clone("h");
   if(!h1->GetSumw2N()) h1->Sumw2();
   for (Int_t i = 1; i<=h1->GetNbinsX(); i++) {
     Double_t value = h1->GetBinContent(i);
     Double_t width = h1->GetBinWidth(i);
     Double_t center = h1->GetBinCenter(i);
     Double_t error = h1->GetBinError(i);
     h1->SetBinContent(i,value*center*width); //*center*width
     h1->SetBinError(i,error*center*width);
   }
   TH1D* h2 = (TH1D*)h1->Rebin(nBins,"hnew",xBins);
   for (Int_t i = 1; i<=h2->GetNbinsX(); i++) {
     Double_t value = h2->GetBinContent(i);
     Double_t width = h2->GetBinWidth(i);
     Double_t center = h2->GetBinCenter(i);
     Double_t error = h2->GetBinError(i);
     h2->SetBinContent(i,value/(center*width));
     h2->SetBinError(i,error/(center*width));
   }
  if (h1) delete h1;
  TString stName(h->GetName());
  TString stTitle(h->GetTitle());
  stName += "_rbn";
  h2->SetName(stName);
  h2->SetTitle(stTitle);
  return h2;
}

TH1D* RebinHistogram(const TH1D* h, Int_t nBins, const Double_t* xBins){
   if (!h) return 0;
   if (!nBins) return (TH1D*)h;
   if (!xBins) return (TH1D*)h;
   TH1D *h1 = (TH1D*) h->Clone("h");
   if(!h1->GetSumw2N()) h1->Sumw2();
   for (Int_t i = 1; i<h1->GetNbinsX(); i++) {
     Double_t value = h1->GetBinContent(i);
     Double_t error = h1->GetBinError(i);
     h1->SetBinContent(i,value);
     h1->SetBinError(i,error);
   }
   TH1D* h2 = (TH1D*)h1->Rebin(nBins,"hnew",xBins);
   for (Int_t i = 1; i<h2->GetNbinsX(); i++) {
     Double_t value = h2->GetBinContent(i);
     Double_t error = h2->GetBinError(i);
     h2->SetBinContent(i,value);
     h2->SetBinError(i,error);
   }
  if (h1) delete h1;
  TString stName(h->GetName());
  TString stTitle(h->GetTitle());
  stName += "_rbn";
  h2->SetName(stName);
  h2->SetTitle(stTitle);
  return h2;
}


TH1D* RebinHistogram4Eta2Pi0Ratio(const TH1D* h, Int_t nBins, const Double_t* xBins){
   if (!h) return 0;
   if (!nBins) return (TH1D*)h;
   if (!xBins) return (TH1D*)h;
   TH1D *h1 = (TH1D*) h->Clone("h");
   if(!h1->GetSumw2N()) h1->Sumw2();
   for (Int_t i = 1; i<h1->GetNbinsX(); i++) {
     Double_t value = h1->GetBinContent(i);
     Double_t error = h1->GetBinError(i);
     Double_t binWidth = h1->GetBinWidth(i);
     h1->SetBinContent(i,value/binWidth);
     h1->SetBinError(i,error/binWidth);
   }
   TH1D* h2 = (TH1D*)h1->Rebin(nBins,"hnew",xBins);
   for (Int_t i = 1; i<h2->GetNbinsX(); i++) {
     Double_t value = h2->GetBinContent(i);
     Double_t error = h2->GetBinError(i);
      Double_t binWidth = h1->GetBinWidth(i);
     h2->SetBinContent(i,value/binWidth);
     h2->SetBinError(i,error/binWidth);
   }
  if (h1) delete h1;
  TString stName(h->GetName());
  TString stTitle(h->GetTitle());
  stName += "_rbn";
  h2->SetName(stName);
  h2->SetTitle(stTitle);
  return h2;
}


TH1D* RebinHistAlternative(TH1D* hist, const std::vector<double>& bin_var) {
    // Create the new rebinned histogram
    int n_bins = bin_var.size() - 1;
    TH1D* new_hist = new TH1D("hrebinned", "", n_bins, bin_var.data());
    new_hist->Sumw2(); // Enable error handling

    // Loop through the new bins
    for (int bin = 0; bin < n_bins; ++bin) {
        double b1 = bin_var[bin];
        double b2 = bin_var[bin + 1];

        // Find the corresponding bin edges in the original histogram
        int bin_b1 = hist->GetXaxis()->FindBin(b1 + 1e-6);
        int bin_b2 = hist->GetXaxis()->FindBin(b2 - 1e-6);

        // Calculate the integral content and error
        double content = 0.0;
        double error = 0.0;

        for (int i = bin_b1; i <= bin_b2; ++i) {
            content += hist->GetBinContent(i);
        }

        error = TMath::Sqrt(content); // Assuming Poisson statistics

        // Bin properties
        double bin_center = (b1 + b2) / 2.0; // Center of the new bin
        double bin_width = b2 - b1;          // Width of the new bin

        // Normalize the content by the bin width and fill the new histogram
        double new_content = content / bin_width;
        new_hist->SetBinContent(new_hist->FindBin(bin_center), new_content);
        new_hist->SetBinError(new_hist->FindBin(bin_center), error);

        // Debug information
        std::cout << "Bin " << bin << ": [" << b1 << ", " << b2 << "]"
                  << ", Content: " << content
                  << ", Error: " << error << std::endl;
    }

    return new_hist;
}

TH1D* MakeRatioSpectra4Eta2Pi0Ratio(const TH1D *spec1, const TH1D*spec2, const char* option = ""){
  TString stOption(option);
  if(! stOption.Contains("quiet")){
    for(Int_t ii = 0; ii<30; ii++){
      cout<<"| Tools:  Only use MakeRatioSpectra for Spectra"<<endl;
    }
    cout<<"| Tools: Use the option \"quiet\" to mute this warning"<<endl;
  }
  Int_t nbins1 = spec1->GetXaxis()->GetNbins();
  Int_t nbins2 = spec2->GetXaxis()->GetNbins();
  cout<<"| MakeRatio: "<<spec1->GetName()<<"/"<<spec2->GetName()<<endl;
  cout<<"| Bins 1 :"<<nbins1<<endl;
  cout<<"| Bins 2 :"<<nbins2<<endl;
  

    TH1D *hratio = 0;

  if (nbins1 <= nbins2){
    TArrayD *binarr = (TArrayD*)spec1->GetXaxis()->GetXbins();
    TH1D *spec2rebin = RebinHistogram4Eta2Pi0Ratio(spec2,nbins1,binarr->GetArray());
    hratio = (TH1D*)spec1->Clone("ratio");
    hratio->GetYaxis()->SetTitle("Ratio");
    hratio -> Divide(spec1,spec2rebin,1,1,option);
  }else{
    TArrayD *binarr = (TArrayD*)spec2->GetXaxis()->GetXbins();
    TH1D *spec1rebin = RebinHistogram4Eta2Pi0Ratio(spec1,nbins2,binarr->GetArray());
    hratio = (TH1D*)spec1rebin->Clone("ratio");
    hratio->GetYaxis()->SetTitle("Ratio");
    hratio -> Divide(spec1rebin,spec2,1,1,option);
  }
  return hratio;
}

TH1D* MakeRatioSpectra(const TH1D *spec1, const TH1D*spec2, const char* option = ""){
  TString stOption(option);
  if(! stOption.Contains("quiet")){
    for(Int_t ii = 0; ii<30; ii++){
      cout<<"| Tools:  Only use MakeRatioSpectra for Spectra"<<endl;
    }
    cout<<"| Tools: Use the option \"quiet\" to mute this warning"<<endl;
  }
  Int_t nbins1 = spec1->GetXaxis()->GetNbins();
  Int_t nbins2 = spec2->GetXaxis()->GetNbins();
  cout<<"| MakeRatio: "<<spec1->GetName()<<"/"<<spec2->GetName()<<endl;
  cout<<"| Bins 1 :"<<nbins1<<endl;
  cout<<"| Bins 2 :"<<nbins2<<endl;
  

    TH1D *hratio = 0;

  if (nbins1 <= nbins2){
    TArrayD *binarr = (TArrayD*)spec1->GetXaxis()->GetXbins();
    TH1D *spec2rebin = RebinPtSpectrum(spec2,nbins1,binarr->GetArray());
    hratio = (TH1D*)spec1->Clone("ratio");
    hratio->GetYaxis()->SetTitle("Ratio");
    hratio -> Divide(spec1,spec2rebin,1,1,option);
  }else{
    TArrayD *binarr = (TArrayD*)spec2->GetXaxis()->GetXbins();
    TH1D *spec1rebin = RebinPtSpectrum(spec1,nbins2,binarr->GetArray());
    hratio = (TH1D*)spec1rebin->Clone("ratio");
    hratio->GetYaxis()->SetTitle("Ratio");
    hratio -> Divide(spec1rebin,spec2,1,1,option);
  }
  return hratio;
}


TH1D* MakeRatioSpectraWoNorm(const TH1D *spec1, const TH1D*spec2, const char* option = ""){
    TRandom *random = new TRandom(0);

    TString title("ratio");
    title += Form("%f",random->Rndm());

  TString stOption(option);
  if(! stOption.Contains("quiet")){
    for(Int_t ii = 0; ii<30; ii++){
      cout<<"| Tools:  Only use MakeRatioSpectra for Spectra"<<endl;
    }
    cout<<"| Tools: Use the option \"quiet\" to mute this warning"<<endl;
  }
  Int_t nbins1 = spec1->GetXaxis()->GetNbins();
  Int_t nbins2 = spec2->GetXaxis()->GetNbins();
  cout<<"| MakeRatio: "<<spec1->GetName()<<"/"<<spec2->GetName()<<endl;
  cout<<"| Bins 1 :"<<nbins1<<endl;
  cout<<"| Bins 2 :"<<nbins2<<endl;
  

    TH1D *hratio = 0;

  if (nbins1 <= nbins2){
    TArrayD *binarr = (TArrayD*)spec1->GetXaxis()->GetXbins();
    TH1D *spec2rebin = RebinHistogram(spec2,nbins1,binarr->GetArray());
    hratio = (TH1D*)spec1->Clone(title);
    hratio->GetYaxis()->SetTitle("Ratio");
    hratio -> Divide(spec1,spec2rebin,1,1,option);
  }else{
    TArrayD *binarr = (TArrayD*)spec2->GetXaxis()->GetXbins();
    TH1D *spec1rebin = RebinHistogram(spec1,nbins2,binarr->GetArray());
    hratio = (TH1D*)spec1->Clone(title);
    hratio = (TH1D*)spec1rebin->Clone("ratio");
    hratio->GetYaxis()->SetTitle("Ratio");
    hratio -> Divide(spec1rebin,spec2,1,1,option);
  }
  return hratio;
}



/// Function to scale pt-spectra
void FinalizePt(TH1D* h, Double_t nevents, Double_t etarange) //diff WQ
{
  for (int i=1; i <= h->GetNbinsX() ;i++) {
    Double_t pt = h->GetBinCenter(i);
    Double_t width = h->GetBinWidth(i);
    Double_t val = h->GetBinContent(i);
    Double_t err = h->GetBinError(i);
    Double_t cval = (etarange*nevents>0) ? (val)/(width * 2.0 * TMath::Pi() * etarange * nevents * pt) : 0;
    Double_t cerr = (etarange*nevents>0) ? (err)/(width * 2.0 * TMath::Pi() * etarange * nevents * pt) : 0;
    h->SetBinContent(i,cval);
    h->SetBinError(i,cerr);
  }
  h->GetYaxis()->SetTitle("1/(2#pi p_{T} N_{evt}) d^{2}N_{ch}/(d#eta dp_{T}) (GeV/c)^{-2}");
}

void FinalizeCrossSectionAfterburner(TH1D* h, Double_t etarange) //diff WQ
{
  for (int i= h->FindBin(0.4); i <= h->GetNbinsX() ;i++) {
    Double_t pt = h->GetBinCenter(i);
    Double_t val = h->GetBinContent(i);
    Double_t err = h->GetBinError(i);
    Double_t cval = (etarange>0) ? (val)/(2.0 * TMath::Pi() * etarange * pt) : 0;
    Double_t cerr = (etarange>0) ? (err)/(2.0 * TMath::Pi() * etarange * pt) : 0;
    h->SetBinContent(i,cval);
    h->SetBinError(i,cerr);
  }
  h->GetYaxis()->SetTitle("1/(2#pi p_{T} N_{evt}) d^{2}N_{ch}/(d#eta dp_{T}) (GeV/c)^{-2}");
}


/// Alternative function to scale pt-spectra without the 1/(2*pi*pT) term
void FinalizePt_b(TH1D* h, Double_t nevents, Double_t etarange) //invariant yield
{
  for (int i=1; i <= h->GetNbinsX() ;i++) {
    Double_t pt = h->GetBinCenter(i);
    Double_t width = h->GetBinWidth(i);
    Double_t val = h->GetBinContent(i);
    Double_t err = h->GetBinError(i);
    Double_t cval = (etarange*nevents>0) ? (val)/(width * etarange * nevents) : 0;
    Double_t cerr = (etarange*nevents>0) ? (err)/(width * etarange * nevents) : 0;
    h->SetBinContent(i,cval);
    h->SetBinError(i,cerr);
  }
  h->GetYaxis()->SetTitle("1/(N_{evt}) d^{2}N_{ch}/(d#eta dp_{T}) (GeV/c)^{-2}");
}


TH1D* FinalizedNineldPt(TH1D* h, Double_t nevents, Double_t etarange, Double_t verRecEff) // See section 8 from Edgar's thesis
{
    Double_t MBcrossSec = 50.87; //MB cross section for pp (2017). See section 8 from Edgar's thesis.
    Double_t INELcrossSec = 67.6; //inelastic cross section. See section 8 from Edgar's thesis.
    TH1D* hist = (TH1D*)h->Clone("hist");
    hist->Reset();

   for (int i=1; i <= h->GetNbinsX() ;i++) {
        Double_t pt = h->GetBinCenter(i);
        Double_t width = h->GetBinWidth(i);
        Double_t val = h->GetBinContent(i);
        Double_t err = h->GetBinError(i);
        Double_t cval = (etarange*nevents>0) ? (val*MBcrossSec*verRecEff)/(width* etarange * nevents) : 0;
        Double_t cerr = (etarange*nevents>0) ? (err*MBcrossSec*verRecEff)/(width * etarange * nevents) : 0;
        hist->SetBinContent(i,cval);
        hist->SetBinError(i,cerr);
    }

   hist->Scale(1.0/INELcrossSec);
   hist->GetYaxis()->SetTitle("#sigma_{MB}^{pp}/#sigma_{INEL} 1/(N_{ev}^{MB}) d^{2}N_{ch}/(d#eta dp_{T})(GeV/c)^{-2}");
   return hist;
}

TH1D* FinalizedSigmadPt(TH1D* h, Double_t nevents, Double_t etarange, Double_t verRecEff) // See section 8 from Edgar's thesis
{
    Double_t MBcrossSec =  50.87; //MB cross section for pp (2017). See section 8 from Edgar's thesis.
    TH1D* hist = (TH1D*)h->Clone("hist");
    hist->Reset();

   for (int i=1; i <= h->GetNbinsX() ;i++) {
        Double_t pt = h->GetBinCenter(i);
        Double_t width = h->GetBinWidth(i);
        Double_t val = h->GetBinContent(i);
        Double_t err = h->GetBinError(i);
        Double_t cval = (etarange*nevents>0) ? (val*MBcrossSec*verRecEff)/(width *etarange * nevents) : 0;
        Double_t cerr = (etarange*nevents>0) ? (err*verRecEff)/(width * etarange * nevents) : 0;
        hist->SetBinContent(i,cval);
        hist->SetBinError(i,cerr);
    }

   hist->GetYaxis()->SetTitle("#sigma_{MB}^{pp} 1/(N_{ev}^{MB}) d^{2}N_{ch}/(d#eta dp_{T}) mb (GeV/c)^{-2}");
   return hist;
}

/// Alternative function to scale pt-spectra
void FinalizePtSpectrum(TH1D* h, double etaRange, double Nev)
{
    h->Scale(1./(etaRange*Nev*2*TMath::Pi()), "width");
    for(int i = 0; i<h->GetNbinsX(); ++i)
    {
        double pt = h->GetBinCenter(i);
        h->SetBinContent(i, h->GetBinContent(i)/pt);
        h->SetBinError(i, h->GetBinError(i)/pt);
    }
  h->GetYaxis()->SetTitle("1/(2#pi p_{T} N_{evt}) d^{2}N_{ch}/(d#eta dp_{T}) (GeV/c)^{-2}");
}


///PCM Analysis:
void FinalizeInvariantYield(TH1D* h, double etaRange, double Nev)
{
  h->Scale(1./(etaRange*Nev*2*TMath::Pi()), "width");
  h->GetYaxis()->SetTitle("1/(2#pi N_{evt}) d^{2}N/(d#eta dp_{T}) GeV^{-1}/c^{3}");
}
