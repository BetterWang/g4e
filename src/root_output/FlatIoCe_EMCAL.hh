#ifndef G4E_FLATIOCe_EMCAL_HH
#define G4E_FLATIOCe_EMCAL_HH

namespace g4e {
    struct Ce_EMCALIo
    {
        void BindToTree(TTree *tree)
        {
         //   tree->Branch("Ce_emcal_count", &Ce_EMCALCount, "Ce_EMCALCount/l");
            tree->Branch("Ce_emcal_name", &Name);
            tree->Branch("Ce_emcal_Etot_dep", &Etot_dep);
            tree->Branch("Ce_emcal_Npe", &Npe);
            tree->Branch("Ce_emcal_ADC", &ADC);
            tree->Branch("Ce_emcal_TDC", &TDC);
         //   tree->Branch("Ce_emcal_waveform", &waveform);
          //  tree->Branch("Ce_emcal_waveform_time", &waveform_time);
            tree->Branch("Ce_emcal_xcrs", &xcrs);
            tree->Branch("Ce_emcal_ycrs", &ycrs);
            tree->Branch("Ce_emcal_zcrs", &zcrs);


        }

        void Clear()
        {
        //    Ce_EMCALCount=0;
            Name.clear();
            Etot_dep.clear();
            Npe.clear();
            ADC.clear();
            TDC.clear();
       //     waveform.clear();
            xcrs.clear();
            ycrs.clear();
            zcrs.clear();

        }

     //   uint64_t Ce_EMCALCount;
        std::vector<std::string> Name;              // PrimaryVertex->GetX0();
        std::vector<double> Etot_dep;              // PrimaryVertex->GetY0();
        std::vector<int> Npe;
        std::vector<double> ADC;              // PrimaryVertex->GetZ0();
        std::vector<double> TDC;           // PrimaryVertex->GetT0();
    //    std::vector<std::vector<double>> waveform ;         // PrimaryVertex->GetWeight();
      //   std::vector<std::vector<double>> waveform_time ;
         std::vector<double>xcrs;
         std::vector<double>ycrs;
         std::vector<double>zcrs;

    };
}

#endif //G4E_FLATIOCe_EMCAL_HH
