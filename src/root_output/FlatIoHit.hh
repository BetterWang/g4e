#ifndef G4E_FLATIOHIT_HH
#define G4E_FLATIOHIT_HH

namespace g4e {
    struct HitIo
    {
        void BindToTree(TTree *tree)
        {
            tree->Branch("hit_count", &HitsCount, "hit_count/l");
            tree->Branch("hit_id", &IdVect);
            tree->Branch("hit_trk_id", &TrackIdVect);
            tree->Branch("hit_ptr_id", &GenPartIdVect);
            tree->Branch("hit_vol_name", &VolumeNameVect);
            tree->Branch("hit_x", &XPosVect);
            tree->Branch("hit_y", &YPosVect);
            tree->Branch("hit_z", &ZPosVect);
            tree->Branch("hit_i_rep", &IRepVect);
            tree->Branch("hit_j_rep", &JRepVect);
            tree->Branch("hit_e_loss", &ELossVect);
        }

        void Clear()
        {
            HitsCount = 0;
            XPosVect.clear();
            YPosVect.clear();
            ZPosVect.clear();
            ELossVect.clear();
            IdVect.clear();
            TrackIdVect.clear();
            GenPartIdVect.clear();
            IRepVect.clear();
            JRepVect.clear();
            VolumeNameVect.clear();
        }

        uint64_t HitsCount;
        std::vector<uint64_t> IdVect;
        std::vector<uint64_t> TrackIdVect;
        std::vector<uint64_t> GenPartIdVect;
        std::vector<double> XPosVect;
        std::vector<double> YPosVect;
        std::vector<double> ZPosVect;
        std::vector<double> ELossVect;
        std::vector<uint64_t> IRepVect;
        std::vector<uint64_t> JRepVect;
        std::vector<std::string> VolumeNameVect;
    };
}
#endif //G4E_FLATIOHIT_HH
