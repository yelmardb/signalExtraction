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
        Double_t cb_alpha = 0.0;
        Double_t cb_n = 0.0;
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
        Bool_t isMC = false;
        Bool_t isTrueMC = false;
        Bool_t isData = false;

        TH1D* h1yield;
        TH1D* h1yieldUnNorm;
        TH1D* h1yieldTmp;
        TH1D* h1mean;
        TH1D* h1sigma;
        TH1D* hFWHM;
        TH1D* h1ratio;
        TH1D* hCalculatedSignal;
        TH1D* hCalculatedBackground;
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
            hFWHM = new TH1D("hFWHM", "", pT.size()-1, pT.data());
            mesonEff = new TH1D("mesonEfficiency", "", pT.size()-1, pT.data());
            mesonAcc = new TH1D("mesonAcceptance", "", pT.size()-1, pT.data());
            mesonAccXEff = new TH1D("mesonAcceptanceXEfficiency", "", pT.size()-1, pT.data());
        }


         void SetFitParameters(){
            if(strcmp(meson, "Pi0") == 0){
                mean = 0.132;
                sigma = 0.005;
                cb_alpha = 0.6;
                cb_n = 12.0;
            }
            else if(strcmp(meson, "Eta") == 0){
                mean = 0.545;
                sigma = 0.012;
                cb_alpha = 0.6;
                cb_n = 12.0;
            }
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


        std::pair<double, double> calculate_raw_yield(TH1* histo, const std::vector<double>& params,  const std::vector<double>& params_err, Double_t yield_max) {
            // Find the integral bin range
            int integral_min = histo->FindBin(params[1] - 3.0 * params[2]); 
            int integral_max = histo->FindBin(params[1] + 3.0 * params[2]);

            /*if (pT < 1.0) {
                integral_min = histo->FindBin(params[1] - 0.04); 
                integral_max = histo->FindBin(params[1] + 0.04);
            } else if (pT < 2.0) {
                integral_min = histo->FindBin(params[1] - 0.035);
                integral_max = histo->FindBin(params[1] + 0.035);
            } else {
                integral_min = histo->FindBin(params[1] - 0.03);
                integral_max = histo->FindBin(params[1] + 0.03);
            }*/

            

            std::cout << "Integral range: ["  << histo->GetBinLowEdge(integral_min) << ", "  << histo->GetBinLowEdge(integral_max + 1) << "]" << std::endl;

            double error_integral = 0.0;
            double integral_histo = histo->IntegralAndError(integral_min, integral_max, error_integral);

            // // Subtract linear background
            // TF1 tf1_linear("tf1_linear", "[4]+[5]*x", 0, 1);
            // tf1_linear.SetParameter(5, params[5]);
            // tf1_linear.SetParameter(4, params[4]);


            // double lin_integral_min = params[1] - yield_max;
            // double lin_integral_max = params[1] + yield_max;
            double linear_integral = 0.0;
            double error_linear = 0.0;

            // if(!isTrueMC){
            //     linear_integral = params[4] * (lin_integral_max - lin_integral_min) + 0.5 * params[5] * (lin_integral_max * lin_integral_max - lin_integral_min * lin_integral_min);
            //     linear_integral = linear_integral / histo->GetBinWidth(params[1]);
            //      error_linear = std::sqrt(std::pow((lin_integral_max - lin_integral_min) * params_err[4], 2) + std::pow(0.5 * (lin_integral_max * lin_integral_max - lin_integral_min * lin_integral_min) * params_err[5], 2));
            //     error_linear = error_linear / histo->GetBinWidth(2);
                
            // }

            //cout << "integral = " << integral_histo << ",         linear integral = " << linear_integral << endl;
            double error = TMath::Sqrt(std::pow(error_integral, 2) + std::pow(error_linear, 2));

            double raw_yield = (integral_histo - linear_integral);

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


        void ExtractSignal(){ //This is the main function

            for(int i = 0; i < pT.size()-1; i++){
                cout << "pT: " << pT[i] << " - " << pT[i+1] << endl;
                hMggPt_Same1D = ProjHistogramX(hMggPt_Same, pT[i], pT[i+1]);
                hMggPt_Same1D->SetName(Form("hMggPt_Same1D_pT_%.2f_%.2f", pT[i], pT[i+1]));
                cout << "hMggPt_Same1D, integral: " << hMggPt_Same1D->Integral() << endl;

                hMggPt_Mixed1D = ProjHistogramX(hMggPt_Mixed, pT[i], pT[i+1]);
                hMggPt_Mixed1D->SetName(Form("hMggPt_Mixed1D_pT_%.2f_%.2f", pT[i], pT[i+1]));
                if(strcmp(meson, "Pi0")  == 0){
                    if((isMC || isTrueMC)){
                        hMggPt_Same1D->Rebin(4);
                        hMggPt_Mixed1D->Rebin(4);
                    }
                    else{
                         hMggPt_Same1D->Rebin(2);
                         hMggPt_Mixed1D->Rebin(2);
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

                Int_t binMinSame = hMggPt_Same1D->FindBin(massWindowLow);
                Int_t binMaxSame = hMggPt_Same1D->FindBin(massWindowHigh);
                Int_t binMinMixed = hMggPt_Mixed1D->FindBin(massWindowLow);
                Int_t binMaxMixed = hMggPt_Mixed1D->FindBin(massWindowHigh);
  
                Double_t integralSame = hMggPt_Same1D->Integral(binMinSame, binMaxSame);
                Double_t integralMixed = hMggPt_Mixed1D->Integral(binMinMixed, binMaxMixed);
                Double_t scalingFactor= integralSame/integralMixed;

                hMggPt_Mixed1D->Scale(scalingFactor);




                   

                SetHistogramPropertiesAlt(hMggPt_Same1D, "", "hMggPt_Same1D", 1); 
                SetHistogramPropertiesAlt(hMggPt_MixedScaled1D, "", "hMggPt_MixedScaled1D",1);
                h1ratio = (TH1D*)hMggPt_Same1D->Clone(Form("h1ratio_pT_%.2f_%.2f", pT[i], pT[i+1]));
                h1ratio->Reset();
                h1ratio->SetTitle(Form("h1ratio_pT_%.2f_%.2f", pT[i], pT[i+1]));
                if(!isTrueMC) h1ratio->Divide(hMggPt_Same1D, hMggPt_MixedScaled1D, 1, 1, "B");

                TF1* fitBackground = new TF1(Form("fitBackground_pT_%.2f_%.2f", pT[i], pT[i+1]), "pol1",massWindowLow, massWindowHigh);
                fitBackground->SetNpx(10000);
                if(!isTrueMC) h1ratio->Fit(fitBackground, "RQ", "", minBackgroundRange, maxBackgroundRange);
                
               
                hCalculatedSignal = (TH1D*)hMggPt_Same1D->Clone(Form("hCalculatedSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));


                TF1* fitSignal = new TF1(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]), "(x<[1]) * ([0]*(TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2)))+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2)))))+[4]+[5]*x)+\
               (x>=[1])*([0]*TMath::Exp(-0.5*(TMath::Power((x-[1])/[2],2)))+[4]+[5]*x)",massWindowLow, massWindowHigh);
               fitSignal->SetNpx(10000);

   
            if(!isTrueMC){
                hCalculatedBackground = (TH1D*)hMggPt_MixedScaled1D->Clone(Form("hCalculatedBackground_pT_%.2f_%.2f", pT[i], pT[i+1]));
                hCalculatedBackground->Multiply(fitBackground, 1.);
                hCalculatedSignal->Add(hCalculatedBackground, -1.);
            }
                
            
            if(strcmp(meson, "Pi0") == 0){
                Double_t height = 0.0;
                Double_t meanInitial = 0.135;
                Double_t sigmaInitial = 0.004;
                Double_t expInitial = 0.016;
                Double_t offsetInitial = fitBackground->GetParameter(0);
                Double_t linearInitial = fitBackground->GetParameter(1);


                height = GetAmplitudeInRange(hCalculatedSignal, 0.1, 0.15); //0.1, 0.15, eigene Funktion
                fitSignal->SetParameter(0, height); // amplitude
                fitSignal->SetParameter(1, meanInitial); // mean
                fitSignal->SetParameter(2, sigmaInitial); // sigma
                fitSignal->SetParameter(3, expInitial); // exponential 
                fitSignal->SetParameter(4, offsetInitial); // offset
                fitSignal->SetParameter(5, linearInitial); // linear
            
                fitSignal->SetParLimits(0, 0.9*height, 1.4*height);
                fitSignal->SetParLimits(1, 0.125, 0.138);
                fitSignal->SetParLimits(2, 0.8* sigmaInitial, 1.5*sigmaInitial);
                fitSignal->SetParLimits(3,0.8*expInitial, 1.2*expInitial);
            }
            else if(strcmp(meson, "Eta") == 0){
                Double_t height = 0.0;
                Double_t meanInitial = 0.548;
                Double_t sigmaInitial = 0.006;
                Double_t expInitial = 0.016;
                Double_t offsetInitial = fitBackground->GetParameter(0);
                Double_t linearInitial = fitBackground->GetParameter(1);

                height = GetAmplitudeInRange(hCalculatedSignal, 0.4, 0.6);
                fitSignal->SetParameter(0, height); // amplitude
                fitSignal->SetParameter(1, meanInitial); // mean
                fitSignal->SetParameter(2, sigmaInitial); // sigma
                fitSignal->SetParameter(3, expInitial); // exponential 
                fitSignal->SetParameter(4, offsetInitial); // offset
                fitSignal->SetParameter(5, linearInitial); // linear
            
            
                fitSignal->SetParLimits(0, 0.9*height, 1.4*height);
                fitSignal->SetParLimits(1, 0.54, 0.58);
                fitSignal->SetParLimits(2, 0.8*sigmaInitial, 1.5*sigmaInitial);
                fitSignal->SetParLimits(3,0.01, 0.02);
            }

            

            hCalculatedSignal->Fit(fitSignal, "RM0", "", massWindowLow, massWindowHigh);
            std::vector<double> params; 
            params.push_back(fitSignal->GetParameter(0));
            params.push_back(fitSignal->GetParameter(1));
            params.push_back(fitSignal->GetParameter(2));
            params.push_back(fitSignal->GetParameter(3));
            params.push_back(fitSignal->GetParameter(4));
            params.push_back(fitSignal->GetParameter(5));

            std::vector<double> params_err;
            params_err.push_back(fitSignal->GetParError(0));
            params_err.push_back(fitSignal->GetParError(1));
            params_err.push_back(fitSignal->GetParError(2));
            params_err.push_back(fitSignal->GetParError(3));
            params_err.push_back(fitSignal->GetParError(4));
            params_err.push_back(fitSignal->GetParError(5));

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


            std::pair<double, double> rawYieldAndError = calculate_raw_yield(hCalculatedSignal, params,  params_err, yield_max);
              
            std::pair<double, double> FWHMandError = calculateFWHM(params, params_err);


            h1mean->SetBinContent(i+1, params[1]);
            h1mean->SetBinError(i+1, params_err[1]);
            h1sigma->SetBinContent(i+1, params[2]);
            h1sigma->SetBinError(i+1, params_err[2]);
            hFWHM->SetBinContent(i+1, FWHMandError.first);
            hFWHM->SetBinError(i+1, FWHMandError.second);

            h1yield->SetBinContent(i+1, rawYieldAndError.first);
            h1yield->SetBinError(i+1, rawYieldAndError.second);

            SetHistogramPropertiesAlt(h1mean, "#it{p}_{T} GeV/#it{c}", "h1mean", 1);
            SetHistogramPropertiesAlt(h1sigma, "#it{p}_{T} GeV/#it{c}", "sigma", 1);
            SetHistogramPropertiesAlt(h1yield, "#it{p}_{T} GeV/#it{c}", "yield", 1);

            outputFile->cd();
            hMggPt_Same1D->Write();
            if(!isTrueMC) hMggPt_Mixed1D->Write();
            h1ratio->Write();
            hCalculatedSignal->Write();
            fitSignal->Write();
            if(!isTrueMC) hCalculatedBackground->Write();
                fitBackground->Write();
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
            cout << "nEvents: " << nEvents << endl;

            hMggPt_Same->Write();
            hMggPt_Mixed->Write();
            h1mean->Write();
            h1sigma->Write();
            hFWHM->Write();
            h1yield->Write();
            h1yieldUnNorm->Write();
            hCollisionCounter->Write();
            

            outputFile->Close();
            cout << "************* DONE! *************" << endl;
        }


};


