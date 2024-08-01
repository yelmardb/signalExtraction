#include "TH1D.h"
#include "TFile.h"
#include "TH1.h"

#include <iostream>



struct pathManager{
  private:
    std::string delimiter = "/";
    std::string delimiterFile = ":/";

    std::string GetInternalPath(std::string tmp){
      size_t pos = tmp.find(delimiterFile);

      return tmp.substr(pos+2, tmp.length()-1);
    }

  public:
    std::string globalPath;
    std::string histName;
    std::string fileName;

    std::string GetFilePath(){
      std::string tmp = globalPath;
      size_t pos = tmp.find(delimiterFile);

      fileName = tmp.substr(0, pos);
      return fileName;
    }

    std::vector<std::string> GetDirectoryFileNames(){
        std::vector<string> sdirectoryFileNames;
        std::string tmp = GetInternalPath(globalPath);;
        size_t pos = 0;

        while ((pos = tmp.find(delimiter)) != std::string::npos){
           sdirectoryFileNames.push_back(tmp.substr(0, pos));
           tmp.erase(0, pos + delimiter.length());
        }

        return sdirectoryFileNames;
    }

    std::string GetHistName(){
        std::string tmp = GetInternalPath(globalPath);;
        size_t pos = 0;

        while ((pos = tmp.find(delimiter)) != std::string::npos){
          tmp.erase(0, pos + delimiter.length());
        }
        histName = tmp;
        return histName;
    }
}pathmanager;


template <typename I>
TDirectoryFile* getTDirectoryFile(I input, std::string const& inputName){
  if(!input){
    std::cout<<"WARNING: input for " << inputName << " is null pointer\n";
    return nullptr;
  }
  
  if(input->InheritsFrom("TDirectoryFile")) return (TDirectoryFile*)input->Get(inputName.c_str());
  else return (TDirectoryFile*)input->FindObject(inputName.c_str());
}
template <typename C>
TObject* getOtherContainer(C input, std::string const& inputName){
  if(!input){
    std::cout<<"WARNING: input for " << inputName << " is null pointer\n";
    return nullptr;
  }
  return (TObject*)input->FindObject(inputName.c_str());
}

TObject* extractHistogram(std::string path) {

    pathmanager.globalPath = path;

    TFile* inFile = TFile::Open(pathmanager.GetFilePath().c_str(), "READ");
    if(!inFile){
      std::cerr<< "ERROR: Could not open file \n";
      return NULL;
    }
    //std::cout <<  pathmanager.GetHistName().c_str() << std::endl;


    std::vector<TDirectoryFile*> dFiles;
    dFiles.push_back(inFile);
    auto const dFilesNames = pathmanager.GetDirectoryFileNames();

    Bool_t areThereDirectoryFiles = kFALSE;
    TObject *obj;
    TKey *key;
    TIter next( inFile->GetListOfKeys());
    while ((key = (TKey *) next())) {
       obj = inFile->Get(key->GetName());
       if (obj->InheritsFrom("TDirectoryFile") || obj->InheritsFrom("TList") 
       || obj->InheritsFrom("TDirectory")  || obj->InheritsFrom("TObjArray")  
       || obj->InheritsFrom("TFolder")|| obj->InheritsFrom("THashTable")){
          if(obj->GetName() == dFilesNames.front()){
             areThereDirectoryFiles =kTRUE;
            break;
          }
       }
    }

    if(areThereDirectoryFiles){
      for(int i=1; i<=dFilesNames.size(); ++i){
        auto _tmpDir = getTDirectoryFile(dFiles[i-1], dFilesNames[i-1].c_str());
        if (!_tmpDir){
          getOtherContainer(dFiles[i-1], dFilesNames[i-1].c_str());
        } 
        if(_tmpDir) dFiles.push_back(_tmpDir);
      }

      if(!dFiles.back()){
        std::cerr << "ERROR: File with name" << dFilesNames.back() << "does not exist!\n";
        return nullptr;
      }
    }


    TObject* hist;
    hist = (TObject*)dFiles.back()->Get( pathmanager.GetHistName().c_str());
    if(!hist) hist = (TObject*)dFiles.back()->FindObject( pathmanager.GetHistName().c_str());
    if(!hist) std::cerr << "ERROR: Histogram " << pathmanager.GetHistName().c_str() << " could not be found!\n";

    return hist;

}
//       if (obj->InheritsFrom("TDirectoryFile") || obj->InheritsFrom("TList") || obj->InheritsFrom("TDirectory")  || obj->InheritsFrom("TObjArray")  || obj->InheritsFrom("TFolder")|| obj->InheritsFrom("THashTable"))
