#include "Plotting.h"
#include "extractHistogram.h"



class MesonSignalExtraction {

    private:
        const char* inFileDataName;
        const char* inFileMCName;
        const char* meson;
        const char* run;
        const char* period;
        const char* cutMode;
        TH2D* hMggPt_Same;
        TH2D* hMggPt_Mixed;
        TH1D* hCollisionCounter;
        const char* PathTo_hMggPtSame;
        const char* PathTo_hMggPtMixed;
        const char* PathTo_hCollisionCounter;
        const char* PathTo_hPtGen;
        const char* PathTo_hPtAcc;
        const char* Path_To_THnSparseSame;
        const char* Path_To_THnSparseMixed;
        TH1D* hMggPt_Same1D;
        TH1D* hMggPt_Mixed1D;
        TH1D* hMggPt_MixedScaled1D;

        Double_t mean = 0.0;
        Double_t sigma = 0.0;
        Double_t massWindowLow;
        Double_t massWindowHigh;
        Double_t theoMassMeson;
        Double_t FWHM = 0.0;
        Double_t FWHMErr = 0.0;


    public:
        std::vector<Double_t> pT;
        Double_t minBackgroundRange;
        Double_t maxBackgroundRange;
        Double_t minIntegrationRange;
        Double_t maxIntegrationRange;
        Double_t fBackgroundMultNumber;
        Bool_t isMC = false;
        Bool_t isTrueMC = false;
        Bool_t isData = false;

        TH1D* h1yield;
        TH1D* h1yieldUnNorm;
        TH1D* h1yieldTmp;
        TH1D* h1mean;
        TH1D* h1sigma;
        TH1D* h1exponential;
        TH1D* h1Significance;
        TH1D* hFWHM;
        TH1D* hCalculatedSignal;
        TH1D* hCalculatedBackground;
        TH1D* h1ratio;
        TH1D* h1yieldCorr;
        TH1D* mesonEff;
        TH1D* mesonAcc;
        TH1D* mesonAccXEff;
        TH1D* hpT_Rec;
        TH1D* hpT_Acc;
        TH1D* hpT_Gen;

        TFile* outputFile;
        const char* outputFileName;


        Bool_t isEmpty(const char* str) {
            return (str == nullptr || str[0] == '\0');
        }

        void SetPathToFile(const char* name){
            inFileDataName = name;
        }

        void SetMC(){
            isMC = true;
        }

        void SetTrueMC(){
            isTrueMC = true;
        }

        void SetData(){
            isData = true;
        }

        void SetPeriod(const char* thisPeriod){
            period = thisPeriod;
        }
        
        void SetCutMode(const char* thisCutMode){
            cutMode = thisCutMode;
        }

        void SetFitParameters(const char* thisPeriod){
            if(strcmp(period, "LHC22oPass6") == 0){

            }
        }

        void SetPathsToHistos(){ 
            // Initialize common paths
            Path_To_THnSparseSame = "";
            Path_To_THnSparseMixed = "";
            PathTo_hMggPtSame = "";
            PathTo_hMggPtMixed = "";
            PathTo_hPtGen = "";
            PathTo_hPtAcc = "";

            if(isData){
                if(strcmp(period, "LHC22oPass6") == 0){
                    PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-pcmpcm/Event/after/hCollisionCounter";
                    PathTo_hMggPtSame = ":/pi0eta-to-gammagamma-pcmpcm/Pair/same/hMggPt";
                    PathTo_hMggPtMixed = ":/pi0eta-to-gammagamma-pcmpcm/Pair/mix/hMggPt";
                    return;
                }  
                if(strcmp(period, "LHC22oPass7") == 0){
                    if(strcmp(cutMode, "") == 0){
                        PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-pcmpcm/Event/after/hCollisionCounter"; //pi0eta-to-gammagamma-pcmpcm_id15499 for LHC22oPass7 runNumberData = 255807 (no TPC only tracks in this train run)                     Path_To_THnSparseSame = ":/pi0eta-to-gammagamma-pcmpcm_id15499/Pair/same/hs"; //pi0eta-to-gammagamma-pcmpcm_id15499
                        Path_To_THnSparseSame = ":/pi0eta-to-gammagamma-pcmpcm/Pair/same/hs"; //pi0eta-to-gammagamma-pcmpcm_id15499
                        Path_To_THnSparseMixed = ":/pi0eta-to-gammagamma-pcmpcm/Pair/mix/hs"; //pi0eta-to-gammagamma-pcmpcm_id15499
                        cout << PathTo_hCollisionCounter << endl;
                        return;
                    }
                    else{
                        PathTo_hCollisionCounter = Form(":/pi0eta-to-gammagamma-pcmpcm_%s/Event/after/hCollisionCounter", cutMode); 
                        Path_To_THnSparseSame = Form(":/pi0eta-to-gammagamma-pcmpcm_%s/Pair/same/hs", cutMode); 
                        Path_To_THnSparseMixed = Form(":/pi0eta-to-gammagamma-pcmpcm_%s/Pair/mix/hs", cutMode); 
                        return;
                    }
                }
                else{
                    PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-pcmpcm/Event/after/hCollisionCounter";
                    Path_To_THnSparseSame = ":/pi0eta-to-gammagamma-pcmpcm/Pair/same/hs";
                    Path_To_THnSparseMixed = ":/pi0eta-to-gammagamma-pcmpcm/Pair/mix/hs";
                    return;
                }
            }
                else if(isTrueMC){
                    if(strcmp(period, "LHC22oPass6") == 0){
                    PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter";
                    PathTo_hMggPtSame = Form(":/pi0eta-to-gammagamma-mc-pcmpcm/Pair/%s/hMggPt_Primary", meson);
                    PathTo_hPtGen = Form(":/pi0eta-to-gammagamma-mc-pcmpcm/Generated/%s/hPt", meson);
                            cout << "test, line: " << __LINE__ << endl;

                        //DIRTY PATCH; FIX ASAP
                        Bool_t dirtyPatch = true;
                        if(dirtyPatch){
                            PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-mc/Event/PCMPCM/after/hCollisionCounter";
                            PathTo_hMggPtSame = Form(":/pi0eta-to-gammagamma-mc/Pair/PCMPCM/qc_qc/asym08/hMggPt_%s_Primary", meson);
                            PathTo_hPtGen = Form(":/pi0eta-to-gammagamma-mc/Generated/PCMPCM/hPt_%s", meson);
                            cout << "test, line: " << __LINE__ << endl;
                        } 
                    return;
                }
                else if(strcmp(period, "LHC22oPass7") == 0){
                    if(strcmp(cutMode, "") == 0){
                        PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter"; //pi0eta-to-gammagamma-pcmpcm_id15500 runNumberMC = 250643; (no TPC only tracks in this train run)
                        Path_To_THnSparseSame = Form(":/pi0eta-to-gammagamma-mc-pcmpcm/Pair/%s/hs_Primary", meson); //pi0eta-to-gammagamma-pcmpcm_id15500
                        PathTo_hPtGen = Form(":/pi0eta-to-gammagamma-mc-pcmpcm/Generated/%s/hPt", meson); //pi0eta-to-gammagamma-pcmpcm_id15500
                        return;
                    }
                    else{
                        PathTo_hCollisionCounter = Form(":/pi0eta-to-gammagamma-mc-pcmpcm_%s/Event/after/hCollisionCounter", cutMode); 
                        Path_To_THnSparseSame = Form(":/pi0eta-to-gammagamma-mc-pcmpcm_%s/Pair/%s/hs_Primary", cutMode, meson); 
                        PathTo_hPtGen = Form(":/pi0eta-to-gammagamma-mc-pcmpcm_%s/Generated/%s/hPt",cutMode, meson); 
                        return;
                    }

                }
                else{
                    PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-mc-pcmpcm/Event/after/hCollisionCounter";
                    Path_To_THnSparseSame = Form(":/pi0eta-to-gammagamma-mc-pcmpcm/Pair/%s/hs_Primary", meson);
                    PathTo_hPtGen = Form(":/pi0eta-to-gammagamma-mc-pcmpcm/Generated/%s/hPt", meson);
                    return;
                }
            }
            else if(isMC){
                if(strcmp(period, "LHC22oPass6") == 0){
                    PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma/Event/PCMPCM/after/hCollisionCounter";
                    PathTo_hMggPtSame = ":/pi0eta-to-gammagamma/Pair/PCMPCM/qc_qc/nocut/hMggPt_Same";
                    PathTo_hMggPtMixed =":/pi0eta-to-gammagamma/Pair/PCMPCM/qc_qc/nocut/hMggPt_Mixed";
                    return;
                }
                if(strcmp(period, "LHC22oPass7") == 0){
                    if(strcmp(cutMode, "") == 0){
                        PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-pcmpcm/Event/after/hCollisionCounter"; //pi0eta-to-gammagamma-pcmpcm_id15501 runNumberMC = 250643; (no TPC only tracks in this train run)
                        Path_To_THnSparseSame = ":/pi0eta-to-gammagamma-pcmpcm/Pair/same/hs";  //pi0eta-to-gammagamma-pcmpcm_id15501 
                        Path_To_THnSparseMixed = ":/pi0eta-to-gammagamma-pcmpcm/Pair/mix/hs";   //pi0eta-to-gammagamma-pcmpcm_id15501 
                        return;
                    }
                    else{
                        PathTo_hCollisionCounter = Form(":/pi0eta-to-gammagamma-pcmpcm_%s/Event/after/hCollisionCounter", cutMode); 
                        Path_To_THnSparseSame = Form(":/pi0eta-to-gammagamma-pcmpcm_%s/Pair/same/hs", cutMode);  
                        Path_To_THnSparseMixed = Form(":/pi0eta-to-gammagamma-pcmpcm_%s/Pair/mix/hs", cutMode);  
                        return;
                    }
                }
                else{
                    PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-pcmpcm/Event/after/hCollisionCounter";
                    Path_To_THnSparseSame = ":/pi0eta-to-gammagamma-pcmpcm/Pair/same/hs";
                    Path_To_THnSparseMixed = ":/pi0eta-to-gammagamma-pcmpcm/Pair/mix/hs";
                    return;
                }
            }
       
        }

        void SetOutputFileName(const char* name){
            outputFileName = name;
        }

        void SetPtBinning(std::vector<Double_t> thispT){
            pT = thispT;
        }

        void SetMeson(const char* thismeson){
            meson = thismeson;
            if(strcmp(thismeson, "Pi0")  == 0) theoMassMeson = 0.135;
            else if(strcmp(thismeson, "Eta")  == 0) theoMassMeson = 0.545;
        }

        void SetRun(const char* thisrun){
            if(strcmp(thisrun, "Run2") == 0){
                run = thisrun;
            }
            else if(strcmp(thisrun, "Run3") == 0){
                run = thisrun;
            }
            std::cout << "INFO: Running on " << period << " from " << run << "..." << std::endl;
        }


        void SetIntegrationRange(Double_t min, Double_t max){
            minIntegrationRange = min;
            maxIntegrationRange = max;
        }

        void SetBackgroundRange(Double_t min, Double_t max){
            minBackgroundRange = min;
            maxBackgroundRange = max;
        }


        void SetMassWindow(Double_t low, Double_t high){
            massWindowLow = low;
            massWindowHigh = high;
        }

        TH1D* ProjHistogramX(TH2D* h2d, Double_t low, Double_t high){
            Double_t tmp = 1e-6;
            Int_t binLow = h2d->GetYaxis()->FindBin(low + tmp);
            Int_t binHigh = h2d->GetYaxis()->FindBin(high - tmp);

            TH1D* h1d;
            if(high <= pT.back() && low > 0){
                h1d = (TH1D*)h2d->ProjectionX(Form("%s_proj_pT_%.2f_%.2f", h2d->GetName(), low, high), binLow, binHigh);
                return h1d;
            } 
            else{
                cout << "ERROR: Histogram out of range" << endl;
                return nullptr;
            }
        }
        
        TH1D* FillProjectionX (TH2* dummy2D,  Double_t low, Double_t high ){
            TH1D* dummy1D           = new TH1D(Form("%s_proj_pT_%.2f_%.2f", dummy2D->GetName(), low, high), "", dummy2D->GetNbinsX(), 0., dummy2D->GetXaxis()->GetBinUpEdge(dummy2D->GetNbinsX()));
            dummy1D->Sumw2();
            Int_t startBin          = dummy2D->GetYaxis()->FindBin(low+0.001);
            Int_t endBin            = dummy2D->GetYaxis()->FindBin(high-0.001);
            dummy2D->ProjectionX(Form("%s_proj_pT_%.2f_%.2f", dummy2D->GetName(), low, high),startBin,endBin,"e");
            return dummy1D;
        }



        
        void LoadHistograms(){
            cout << "INFO: Loading Histograms..." << endl;
            cout << PathTo_hCollisionCounter << endl;
            hCollisionCounter = (TH1D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hCollisionCounter)); 

            if(isData){
                if(strcmp(period, "LHC22oPass6") == 0){
                    hMggPt_Same = (TH2D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hMggPtSame));
                    hMggPt_Same->SetName("hMggPt_Same");
                    hMggPt_Mixed = (TH2D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hMggPtMixed));
                    hMggPt_Mixed->SetName("hMggPt_Mixed");
                    cout << "INFO: Histograms loaded successfully!" << endl;
                    return;
                }  
                else{
                    auto *hsSame = (THnSparse*)extractHistogram(Form("%s%s",inFileDataName, Path_To_THnSparseSame)); 
                    auto *hsMixed = (THnSparse*)extractHistogram(Form("%s%s",inFileDataName, Path_To_THnSparseMixed)); 
                    hMggPt_Same = (TH2D*)hsSame->Projection(1,0);
                    hMggPt_Same->SetName("hMggPt_Same");
                    hMggPt_Mixed = (TH2D*)hsMixed->Projection(1,0);
                    hMggPt_Mixed->SetName("hMggPt_Mixed");
                    cout << "INFO: Histograms loaded successfully!" << endl;
                    return;
                }
            }
            else if(isTrueMC){
                if(strcmp(period, "LHC22oPass6") == 0){
                    hMggPt_Same = (TH2D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hMggPtSame));
                    hMggPt_Same->SetName("hMggPt_Same");
                    hMggPt_Mixed = (TH2D*)hMggPt_Same->Clone("hMggPt_Mixed");
                    hMggPt_Mixed->Reset(); //make dummy histogram
                    hpT_Gen = (TH1D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hPtGen));
                    hpT_Gen->SetName("hpT_Gen");
                    cout << "INFO: Histograms loaded successfully!" << endl;
                    return;
            }
                else{
                    auto *hsSame = (THnSparse*)extractHistogram(Form("%s%s",inFileDataName, Path_To_THnSparseSame)); 
                    hMggPt_Same = (TH2D*)hsSame->Projection(1,0);
                    hMggPt_Same->SetName("hMggPt_Same");
                    hpT_Gen = (TH1D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hPtGen));
                    hMggPt_Mixed = (TH2D*)hMggPt_Same->Clone("hMggPt_Mixed");
                    hMggPt_Mixed->Reset(); //make dummy histogram
                    cout << "INFO: Histograms loaded successfully!" << endl;
                    return;
                }
    
            }
            else if(isMC){
                if(strcmp(period, "LHC22oPass6") == 0){
                    hMggPt_Same = (TH2D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hMggPtSame));
                    hMggPt_Same->SetName("hMggPt_Same");
                    hMggPt_Mixed = (TH2D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hMggPtMixed));
                    hMggPt_Mixed->SetName("hMggPt_Mixed");
                    cout << "INFO: Histograms loaded successfully!" << endl;
                    return;

                }
                else{
                    auto *hsSame = (THnSparse*)extractHistogram(Form("%s%s",inFileDataName, Path_To_THnSparseSame)); 
                    auto *hsMixed = (THnSparse*)extractHistogram(Form("%s%s",inFileDataName, Path_To_THnSparseMixed)); 
                    hMggPt_Same = (TH2D*)hsSame->Projection(1,0);
                    hMggPt_Same->SetName("hMggPt_Same");
                    hMggPt_Mixed = (TH2D*)hsMixed->Projection(1,0);
                    hMggPt_Mixed->SetName("hMggPt_Mixed");
                    cout << "INFO: Histograms loaded successfully!" << endl;
                    return;
                
                }
            }
        }

        void InitHistograms(){
            outputFile = new TFile(Form("%s", outputFileName), "RECREATE");
            h1yield = new TH1D("h1yield", "", pT.size()-1, pT.data());
            h1yieldCorr = new TH1D("h1yieldCorr", "", pT.size()-1, pT.data());
            h1mean = new TH1D("h1mean", "", pT.size()-1, pT.data());
            h1sigma = new TH1D("h1sigma", "", pT.size()-1, pT.data());
            h1exponential = new TH1D("h1exponential", "", pT.size()-1, pT.data());
            hFWHM = new TH1D("hFWHM", "", pT.size()-1, pT.data());
            mesonEff = new TH1D("mesonEfficiency", "", pT.size()-1, pT.data());
            mesonAcc = new TH1D("mesonAcceptance", "", pT.size()-1, pT.data());
            mesonAccXEff = new TH1D("mesonAcceptanceXEfficiency", "", pT.size()-1, pT.data());
            h1Significance =  new TH1D("h1Significance", "", pT.size()-1, pT.data());
        }


        std::pair<double, double> computeSignificanceInPtBin(TH1D* h1sig, TH1D* h1bkg) {
       
            Double_t signalIntegral = h1sig->Integral(h1sig->FindBin(massWindowLow), h1sig->FindBin(massWindowHigh));
            Double_t backgroundIntegral = h1bkg->Integral(h1sig->FindBin(massWindowLow), h1sig->FindBin(massWindowHigh) );


            Double_t significance = 0.0;
            if(backgroundIntegral > 0.0) significance = signalIntegral/(TMath::Sqrt(signalIntegral + backgroundIntegral));

            Double_t signalError, backgroundError;
            for (int bin = h1sig->FindBin(massWindowLow); bin <= h1sig->FindBin(massWindowHigh); ++bin) {
                signalError += h1sig->GetBinError(bin) * h1sig->GetBinError(bin);
            }
            for (int bin = h1bkg->FindBin(massWindowLow); bin <= h1bkg->FindBin(massWindowHigh); ++bin) {
                backgroundError += h1bkg->GetBinError(bin) * h1bkg->GetBinError(bin);
            }
            signalError = TMath::Sqrt(signalError);
            backgroundError = TMath::Sqrt(backgroundError);

            Double_t dSignificance_dSignal = (TMath::Sqrt(signalIntegral + backgroundIntegral) - signalIntegral / (2 * TMath::Sqrt(signalIntegral + backgroundIntegral))) / (signalIntegral + backgroundIntegral);
            Double_t dSignificance_dBackground = (-signalIntegral / (2 * TMath::Sqrt(signalIntegral + backgroundIntegral))) / (signalIntegral + backgroundIntegral);

            Double_t significanceError = TMath::Sqrt( dSignificance_dSignal * dSignificance_dSignal * signalError * signalError +
                                          dSignificance_dBackground * dSignificance_dBackground * backgroundError * backgroundError);

            if(significance <= 0.) cout << "**************WARNING: Zero or negative significance!! **************" << endl;
            return std::make_pair(significance, significanceError);
    
        }


        std::pair<double, double>  calculateFWHM(const std::vector<double>& params, const std::vector<double>& paramsErr) {
            // Define the function with initial parameters
            TF1* tf1_fwhm = new TF1("tf1_fwhm", "(x<[1]) * ([0]*(TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))) + TMath::Exp((x-[1])/[3]) * (1. - TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))))) ) + \
            (x>=[1]) * ([0] * TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))))", 0, 1);
            
            for (int i = 0; i < 4; ++i) {
                tf1_fwhm->SetParameter(i, params[i]);
                tf1_fwhm->SetParError(i, paramsErr[i]);
            }
            
            tf1_fwhm->SetNpx(1000);
            
            double maximum = tf1_fwhm->GetMaximum();
            double maximum_x = tf1_fwhm->GetMaximumX();
            double half_maximum = maximum / 2.0;
            
            double left_x = tf1_fwhm->GetX(half_maximum, 0, maximum_x);
            double right_x = tf1_fwhm->GetX(half_maximum, maximum_x, 1);
            
            double FWHM = (right_x - left_x) / TMath::Sqrt(8 * TMath::Log(2)); // Convert to sigma equivalent
            
            // Calculate PLUS-error with (FWHM + FWHM_err)
            TF1* tf1_fwhm_plus_err = new TF1("tf1_fwhm_plus_err", "(x<[1]) * ([0]*(TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))) + TMath::Exp((x-[1])/[3]) * (1. - TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))))) ) + \
            (x>=[1]) * ([0] * TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))))", 0, 1);
            
            for (int i = 0; i < 4; ++i) {
                double param = params[i] + paramsErr[i];
                tf1_fwhm_plus_err->SetParameter(i, param);
            }
            
            double maximum_plus = tf1_fwhm_plus_err->GetMaximum();
            double maximum_x_plus = tf1_fwhm_plus_err->GetMaximumX();
            double half_maximum_plus = maximum_plus / 2.0;
            
            double left_x_plus = tf1_fwhm_plus_err->GetX(half_maximum_plus, 0, maximum_x_plus);
            double right_x_plus = tf1_fwhm_plus_err->GetX(half_maximum_plus, maximum_x_plus, 1);
            
            double FWHM_plus = (right_x_plus - left_x_plus) / TMath::Sqrt(8 * TMath::Log(2));
            
            // Calculate MINUS-error with (FWHM - FWHM_err)
            TF1* tf1_fwhm_minus_err = new TF1("tf1_fwhm_minus_err", "(x<[1]) * ([0]*(TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))) + TMath::Exp((x-[1])/[3]) * (1. - TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))))) ) + \
            (x>=[1]) * ([0] * TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))))", 0, 1);
            
            for (int i = 0; i < 4; ++i) {
                double param = params[i] - paramsErr[i];
                tf1_fwhm_minus_err->SetParameter(i, param);
            }
            
            double maximum_minus = tf1_fwhm_minus_err->GetMaximum();
            double maximum_x_minus = tf1_fwhm_minus_err->GetMaximumX();
            double half_maximum_minus = maximum_minus / 2.0;
            
            double left_x_minus = tf1_fwhm_minus_err->GetX(half_maximum_minus, 0, maximum_x_minus);
            double right_x_minus = tf1_fwhm_minus_err->GetX(half_maximum_minus, maximum_x_minus, 1);
            
            double FWHM_minus = (right_x_minus - left_x_minus) / TMath::Sqrt(8 * TMath::Log(2));
            
            // Calculate TOTAL error
            double FWHM_err = std::max(std::abs(FWHM_plus - FWHM), std::abs(FWHM_minus - FWHM)) / 2.0;
            
            // Output the results
            //cout << "FWHM calc: " << FWHM * 1e3 << " +/- " << FWHM_err * 1e3 << "; " << FWHM_plus * 1e3 << " " << FWHM_minus * 1e3 << endl;
            return std::make_pair(FWHM, FWHM_err);
        }


        std::pair<double, double> calculate_raw_yield(TH1* histo, const std::vector<double>& params,  const std::vector<double>& params_err, Double_t yield_max, Double_t yield_min) {
            // Find the integral bin range
            int integral_min = histo->FindBin(params[1] + minIntegrationRange); 
            int integral_max = histo->FindBin(params[1] + maxIntegrationRange);

            double error_integral = 0.0;
            double integral_histo = histo->IntegralAndError(integral_min, integral_max, error_integral);



            double linear_integral = 0.0;
            double error_linear = 0.0;

            TF1* f1 = new TF1("f1","[4]+[5]*x", 0,1);
            f1->SetParameter(5, params[5]);
            f1->SetParameter(4, params[4]);
            Double_t lin_integral_min = params[1] + minIntegrationRange;
            Double_t lin_integral_max = params[1] + maxIntegrationRange;

            if(!isTrueMC){
                linear_integral = params[4] * (lin_integral_max - lin_integral_min) + 0.5 * params[5] * (lin_integral_max * lin_integral_max - lin_integral_min * lin_integral_min);
                error_linear = std::sqrt(std::pow((lin_integral_max - lin_integral_min) * params_err[4], 2) + std::pow(0.5 * (lin_integral_max * lin_integral_max - lin_integral_min * lin_integral_min) * params_err[5], 2));            
            }

            double error = TMath::Sqrt(std::pow(error_integral, 2) + std::pow(error_linear, 2));

            double raw_yield = (integral_histo - 0);

            return std::make_pair(raw_yield, error);
        }


        Double_t GetAmplitudeInRange(TH1D* hist, double xMin, double xMax) {

            int binMin = hist->FindBin(xMin);
            int binMax = hist->FindBin(xMax);

            double maxAmplitude = -std::numeric_limits<double>::infinity();
            for (int bin = binMin; bin <= binMax; ++bin) {
                double binContent = hist->GetBinContent(bin);
                if (binContent > maxAmplitude) {
                    maxAmplitude = binContent;
                }
            }

            return maxAmplitude;
        }


        Double_t calculateScalingFactor(TH1D* hSame, TH1D* hMixed, Int_t mode = 0) { //0 for standard mode, 1 uses sidebands

            Double_t scalingFactor = 1.0;
            if(mode == 0){
                Int_t binMinSame = hMggPt_Same1D->FindBin(minBackgroundRange);
                Int_t binMaxSame = hMggPt_Same1D->FindBin(maxBackgroundRange);
                Int_t binMinMixed = hMggPt_Mixed1D->FindBin(minBackgroundRange);
                Int_t binMaxMixed = hMggPt_Mixed1D->FindBin(maxBackgroundRange);

                Double_t integralSame = hMggPt_Same1D->Integral(binMinSame, binMaxSame);
                Double_t integralMixed = hMggPt_Mixed1D->Integral(binMinMixed, binMaxMixed);
                cout << "integralSame: " << integralSame << "\t integralMixed: " << integralMixed << endl;


                if(!isTrueMC) scalingFactor = integralSame/integralMixed;

            }
            else if(mode == 1){
                 Double_t sidebandLowMin = 0.05;  // Lower sideband range (in GeV/c^2)
                 Double_t sidebandLowMax = 0.08;
                 Double_t sidebandHighMin = 0.17; // Upper sideband range (in GeV/c^2)
                 Double_t sidebandHighMax = 0.3;
                 
                if(strcmp(meson, "Eta") == 0){
                     sidebandLowMin = 0.35;
                     sidebandLowMax = 0.45;
                     sidebandHighMin = 0.58;
                     sidebandHighMax = 0.75;
                 }


                Int_t binSidebandLowMin = hSame->FindBin(sidebandLowMin);
                Int_t binSidebandLowMax = hSame->FindBin(sidebandLowMax);
                Int_t binSidebandHighMin = hSame->FindBin(sidebandHighMin);
                Int_t binSidebandHighMax = hSame->FindBin(sidebandHighMax);

                // Integrate over sidebands
                Double_t sidebandIntegralSame = hSame->Integral(binSidebandLowMin, binSidebandLowMax) + hSame->Integral(binSidebandHighMin, binSidebandHighMax);
                Double_t sidebandIntegralMixed = hMixed->Integral(binSidebandLowMin, binSidebandLowMax) + hMixed->Integral(binSidebandHighMin, binSidebandHighMax);

                if(!isTrueMC) scalingFactor = sidebandIntegralSame / sidebandIntegralMixed;
            }

            cout << "SCALING FACTOR: " << scalingFactor << endl;
            //if(scalingFactor < 1./80.) scalingFactor = 1./80.;
            return scalingFactor;
      
        }


        void ProcessEM(TH1D* hSame, TH1D* hMix, Double_t * fBGFitRangeEM) {
            fBackgroundMultNumber = 80;
            for (Int_t binx= 0; binx < hSame->GetNbinsX()+1; binx++){
                if(hSame->GetBinContent(binx) == 0){
                    hSame->SetBinError(binx,1.);
                    hSame->SetBinContent(binx,0.);
                }
            }
            
            TH1D* hMixNorm = (TH1D*)hMix->Clone("hMixNorm");


            Double_t    r       = hSame->Integral(hSame->GetXaxis()->FindBin(fBGFitRangeEM[0]),hSame->GetXaxis()->FindBin(fBGFitRangeEM[1]));
            Double_t    b       = hMix->Integral(hMix->GetXaxis()->FindBin(fBGFitRangeEM[0]),hMix->GetXaxis()->FindBin(fBGFitRangeEM[1]));
            Double_t    norm    = 1;
            if(b != 0) norm     = r/b;
            if ( norm>20./fBackgroundMultNumber )  norm=1./fBackgroundMultNumber;
            cout << "SCALING FACTOR: " << norm << endl;
            hMixNorm->Scale(norm);

            Int_t numberOfZeros = 0;
            for (Int_t i = 1; i < hMixNorm->GetNbinsX()+1; i++){
                if (hMixNorm->GetBinContent(i) == 0){
                    numberOfZeros++;
                    if (norm > 1.){
                        hMixNorm->SetBinError(i,1.);
                        hMixNorm->SetBinContent(i,0.);
                    }
                }
            }
            TH1D* fSignal  = (TH1D*)hSame->Clone("fSignal");
            if ((Double_t)numberOfZeros/hMix->GetNbinsX()< 0.25 && !isTrueMC) fSignal->Add(hMixNorm,-1.);
            
            TH1D* fMesonFullPtBackground = (TH1D*)hMixNorm->Clone("test");
            fMesonFullPtBackground->Reset();
            fMesonFullPtBackground->Add(hMixNorm,norm);

            hCalculatedSignal = fSignal;
            hCalculatedBackground = fMesonFullPtBackground;
            hMggPt_MixedScaled1D = hMixNorm;
        }

  
  
  

        void ExtractSignal(){ //This is the main function
            for(int i = 0; i < pT.size()-1; i++){
                cout << "pT: " << pT[i] << " - " << pT[i+1] << endl;

                hMggPt_Same1D = FillProjectionX(hMggPt_Same, pT[i], pT[i+1]);
                hMggPt_Same1D->SetName(Form("hMggPt_Same1D_pT_%.2f_%.2f", pT[i], pT[i+1]));

                hMggPt_Mixed1D = FillProjectionX(hMggPt_Mixed, pT[i], pT[i+1]);
                hMggPt_Mixed1D->SetName(Form("hMggPt_Mixed1D_pT_%.2f_%.2f", pT[i], pT[i+1]));

                if(strcmp(meson, "Pi0")  == 0){
                    if((isMC || isTrueMC)){
                        hMggPt_Same1D->Rebin(4);
                        hMggPt_Mixed1D->Rebin(4);
                    }
                    else{
                         hMggPt_Same1D->Rebin(4);
                         hMggPt_Mixed1D->Rebin(4);
                    }
                 
                }
                else if(strcmp(meson, "Eta")  == 0){
                    if((isMC || isTrueMC)){
                        hMggPt_Same1D->Rebin(4);
                        hMggPt_Mixed1D->Rebin(4);
                    }
                   else{
                        hMggPt_Same1D->Rebin(4);
                        hMggPt_Mixed1D->Rebin(4);
                   }
                }
                hMggPt_MixedScaled1D = (TH1D*)hMggPt_Mixed1D->Clone(Form("hMggPt_MixedScaled1D_pT_%.2f_%.2f", pT[i], pT[i+1]));
                hMggPt_MixedScaled1D->SetTitle(Form("hMggPt_MixedScaled1D_pT_%.2f_%.2f", pT[i], pT[i+1]));                
                SetHistogramPropertiesAlt(hMggPt_Same1D, "", "hMggPt_Same1D", 1); 
                SetHistogramPropertiesAlt(hMggPt_Mixed1D, "", "hMggPt_MixedScaled1D",1);
                SetHistogramPropertiesAlt(hMggPt_MixedScaled1D, "", "hMggPt_MixedScaled1D",1);

                Double_t fBGFitRangeEM[2] = {minBackgroundRange, maxBackgroundRange };
    
                cout << "line: " << __LINE__ << endl;
                Double_t scalingFactor = calculateScalingFactor(hMggPt_Same1D, hMggPt_Mixed1D, 0);
                if(scalingFactor > 0.) hMggPt_MixedScaled1D->Scale(scalingFactor);
                hCalculatedSignal = (TH1D*)hMggPt_Same1D->Clone(Form("hCalculatedSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));
                if(!isTrueMC) hCalculatedSignal->Add(hMggPt_MixedScaled1D, -1);

                //ProcessEM(hMggPt_Same1D, hMggPt_Mixed1D, fBGFitRangeEM);
                hCalculatedBackground = (TH1D*)hMggPt_MixedScaled1D->Clone(Form("hCalculatedBackground_pT_%.2f_%.2f", pT[i], pT[i+1]));


                TF1* fitSignal;
                if(!isTrueMC) fitSignal = new TF1(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]), "(x<[1]) * ([0]*(TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2)))+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2)))))+[4]+[5]*x)+\
                (x>=[1])*([0]*TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2)))+[4]+[5]*x)",massWindowLow, massWindowHigh);
                else fitSignal = new TF1(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]), "(x<[1]) * ([0]*(TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2)))+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))))))+\
                (x>=[1])*([0]*TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2))))",massWindowLow, massWindowHigh);
                fitSignal->SetNpx(10000);


                TF1* fitBackground = new TF1(Form("fitBackground_pT_%.2f_%.2f", pT[i], pT[i+1]), "pol1",massWindowLow, massWindowHigh);
                fitBackground->SetNpx(10000);
                hCalculatedBackground->Fit(fitBackground, "RQ", "", massWindowLow, massWindowHigh);

            
                
                if(strcmp(meson, "Pi0") == 0){
                    Double_t height = 0.0;
                    Double_t meanInitial = 0.135;
                    Double_t sigmaInitial = 0.008;
                    Double_t expInitial = 0.016;
                    Double_t offsetInitial = 1e6;
                    Double_t linearInitial = 0.0; 
                    
                    Double_t heighMinWeight = 0.98;
                    Double_t heighMaxWeight = 1.05;
                    Double_t sigmaMinWeight = 0.5;
                    Double_t sigmaMaxWeight = 1.8;
                    Double_t expMinWeight = 0.5;
                    Double_t expMaxWeight = 1.6;

                    if(!isTrueMC){
                        //offsetInitial = fitBackground->GetParameter(0);
                        linearInitial = fitBackground->GetParameter(1);
                        //quadraticInitial = fitBackground->GetParameter(2);
                    }
                    else if(isTrueMC){
                        expInitial = 0.01;
                        expMaxWeight = 1.2;
                        offsetInitial = 0;
                        linearInitial = 0;
                        sigmaMinWeight = 0.7;
                        sigmaMaxWeight = 1.4;
                    }
                

                    TH1D* hCalculatedSignaltmp = (TH1D*)hCalculatedSignal->Clone("hCalculatedSignaltmp");
                    hCalculatedSignaltmp->GetXaxis()->SetRangeUser(0.1, 0.14);

                    //height = GetAmplitudeInRange(hCalculatedSignal, 0.125, 0.138); //0.1, 0.15, eigene Funktion
                    height =  hCalculatedSignaltmp->GetMaximum();
                    fitSignal->SetParameter(0, height); // amplitude
                    fitSignal->SetParameter(1, meanInitial); // mean
                    fitSignal->SetParameter(2, sigmaInitial); // sigma
                    fitSignal->SetParameter(3, expInitial); // exponential 
                    if(!isTrueMC){
                        fitSignal->SetParameter(4, offsetInitial); // offset
                        fitSignal->SetParameter(5, linearInitial); // linear
                    }
                
                    fitSignal->SetParLimits(0, heighMinWeight*height, heighMaxWeight*height);
                    fitSignal->SetParLimits(1, 0.125, 0.138);
                    fitSignal->SetParLimits(2, sigmaMinWeight*sigmaInitial, sigmaMaxWeight*sigmaInitial);
                    fitSignal->SetParLimits(3, expMinWeight*expInitial, expMaxWeight*expInitial);
                }
                else if(strcmp(meson, "Eta") == 0){
                    Double_t height, meanInitial, sigmaInitial, expInitial, offsetInitial, linearInitial, mesonAmplitudeMin, mesonAmplitudeMax;


                    TH1D* hCalculatedSignaltmp = (TH1D*)hCalculatedSignal->Clone("hCalculatedSignaltmp");
                    hCalculatedSignaltmp->GetXaxis()->SetRangeUser(0.45, 0.62);


                    height =  hCalculatedSignaltmp->GetMaximum();
                    mesonAmplitudeMin = height*0.8;
                    mesonAmplitudeMax = height;
                    meanInitial = 0.548;
                    sigmaInitial = 0.008;
                    expInitial = 0.016;
                    offsetInitial = 4.e5;
                    linearInitial = fitBackground->GetParameter(1);

                    Double_t sigmaMinWeight = 0.4;
                    Double_t sigmaMaxWeight = 1.5;
                    Double_t expMinWeight = 0.5;
                    Double_t expMaxWeight = 1.6;


                    if(isTrueMC){
                        if(pT[i] >= 1.4 && pT[i] < 3.) {
                            // sigmaInitial = 0.015;
                            // expInitial = 0.02;
                            // mesonAmplitudeMax = height*1.5;
                        }
                    }

                    fitSignal->SetParameter(0, height); // amplitude
                    fitSignal->SetParameter(1, meanInitial); // mean
                    fitSignal->SetParameter(2, sigmaInitial); // sigma
                    fitSignal->SetParameter(3, expInitial); // exponential 
                    fitSignal->SetParameter(4, offsetInitial); // offset
                    fitSignal->SetParameter(5, linearInitial); // linear
                
                
                    fitSignal->SetParLimits(0,mesonAmplitudeMin, mesonAmplitudeMax);
                    fitSignal->SetParLimits(1, 0.54, 0.57);
                    fitSignal->SetParLimits(2, 0.005, 0.018);
                    fitSignal->SetParLimits(3,0.01, 0.02);
                    if(isTrueMC){
                        fitSignal->FixParameter(4, offsetInitial);
                        fitSignal->FixParameter(5, linearInitial);
                    }

            }


                hCalculatedSignal->Fit(fitSignal, "RMBE0S", "", massWindowLow, massWindowHigh);
                std::vector<double> params; 
                params.push_back(fitSignal->GetParameter(0));
                params.push_back(fitSignal->GetParameter(1));
                params.push_back(fitSignal->GetParameter(2));
                params.push_back(fitSignal->GetParameter(3));
                
                if(!isTrueMC){
                    params.push_back(fitSignal->GetParameter(4));
                    params.push_back(fitSignal->GetParameter(5));
                }
                std::vector<double> params_err;
                params_err.push_back(fitSignal->GetParError(0));
                params_err.push_back(fitSignal->GetParError(1));
                params_err.push_back(fitSignal->GetParError(2));
                params_err.push_back(fitSignal->GetParError(3));
                if(!isTrueMC){
                    params_err.push_back(fitSignal->GetParError(4));
                    params_err.push_back(fitSignal->GetParError(5));
                }
                
                TF1* f1 = new TF1("f1","[4]+[5]*x", massWindowLow,massWindowHigh);
                f1->SetParameter(5, params[5]);
                f1->SetParameter(4, params[4]);
                f1->SetParError(5, params_err[5]);
                f1->SetParError(4, params_err[4]);

                if(!isTrueMC){
                    hCalculatedSignal->Add(f1, -1.);
                    hCalculatedBackground->Add(f1, 1.);
                } 


                Double_t yield_min = 0.035;
                Double_t yield_max= 0.02;
                if(strcmp(meson, "Pi0")  == 0){
                    yield_min = 0.035;
                    yield_max= 0.02;
                }
                else if(strcmp(meson, "Eta")  == 0){
                    yield_min = 0.065;
                    yield_max= 0.03;
                }

                Double_t meanCalc = fitSignal->GetParameter(1);
                Double_t meanErrCalc  = fitSignal->GetParError(1);
                Double_t sigmaCalc = fitSignal->GetParameter(2);
                Double_t sigmaErrCalc  = fitSignal->GetParError(2);

                std::pair<double, double> rawYieldAndError = calculate_raw_yield(hCalculatedSignal, params,  params_err, yield_max, yield_min);
                std::pair<double, double> FWHMandError = calculateFWHM(params, params_err);
                h1mean->SetBinContent(i+1, params[1]);
                h1mean->SetBinError(i+1, params_err[1]);
                h1sigma->SetBinContent(i+1, params[2]);
                h1sigma->SetBinError(i+1, params_err[2]);
                h1exponential->SetBinContent(i+1, params[3]);
                h1exponential->SetBinError(i+1, params_err[3]);
                hFWHM->SetBinContent(i+1, FWHMandError.first);
                hFWHM->SetBinError(i+1, FWHMandError.second);
                h1yield->SetBinContent(i+1, rawYieldAndError.first);
                h1yield->SetBinError(i+1, rawYieldAndError.second);

   
                if(!isTrueMC){
                    std::pair<double, double>  significanceInPtBin = computeSignificanceInPtBin(hCalculatedSignal, hCalculatedBackground);
             
                    h1Significance->SetBinContent(i+1, significanceInPtBin.first);
                    h1Significance->SetBinError(i+1, significanceInPtBin.second);
                } 
                
                
                SetHistogramPropertiesAlt(h1Significance, "#it{p}_{T} GeV/#it{c}", "#it{S}^{#pi^{0} (#eta)}",1);
                SetHistogramPropertiesAlt(h1mean, "#it{p}_{T} GeV/#it{c}", "h1mean", 1);
                SetHistogramPropertiesAlt(h1sigma, "#it{p}_{T} GeV/#it{c}", "sigma", 1);
                SetHistogramPropertiesAlt(h1exponential, "#it{p}_{T} GeV/#it{c}", "exponential", 1);
                SetHistogramPropertiesAlt(h1yield, "#it{p}_{T} GeV/#it{c}", "yield", 1);
                SetHistogramPropertiesAlt(hFWHM, "#it{p}_{T} GeV/#it{c}", "FWHM", 1);
                

                outputFile->cd();
                                

                hMggPt_Same1D->Write();
                if(!isTrueMC){
                    hMggPt_Mixed1D->Write();
                    hMggPt_MixedScaled1D->Write();
                    hCalculatedBackground->Write();
                    fitBackground->Write();
                } 
                hCalculatedSignal->Write();
                fitSignal->Write();
            }

            Double_t nEvents;

            if(strcmp(period, "LHC22oPass6") == 0){
                if(isData) nEvents = hCollisionCounter->GetBinContent(10);
                else if(isMC) nEvents = hCollisionCounter->GetBinContent(7);
                else if(isTrueMC) nEvents = hCollisionCounter->GetBinContent(10);
            }
            else nEvents = hCollisionCounter->GetBinContent(12);

        
            h1yieldUnNorm = (TH1D*)h1yield->Clone("h1yieldUnNorm");
            h1yield->Scale(1/nEvents); 
            h1yieldTmp = (TH1D*)h1yield->Clone("h1yieldTmp");
            h1yield->Scale(1., "width");
            h1Significance->Scale(1., "width");
            cout << "nEvents: " << nEvents << endl;
                

            hMggPt_Same->Write();
            hMggPt_Mixed->Write();
            hMggPt_MixedScaled1D->Write();
            h1mean->Write();
            h1sigma->Write();
            h1exponential->Write();
            h1Significance->Write();
            hFWHM->Write();
            h1yield->Write();
            h1yieldUnNorm->Write();
            hCollisionCounter->Write();
                            


            outputFile->Close();
            cout << "************* DONE! *************" << endl;
        }



};


       