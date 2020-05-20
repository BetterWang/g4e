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
            tree->Branch("hit_ptr_id", &GenParentTrackIdVect);
            tree->Branch("hit_parent_trk_id", &GenParentTrackIdVect);
            tree->Branch("hit_vol_name", &VolumeNameVect);
            tree->Branch("hit_x", &XPosVect);
            tree->Branch("hit_y", &YPosVect);
            tree->Branch("hit_z", &ZPosVect);
            tree->Branch("hit_ox", &OXPosVect);
            tree->Branch("hit_oy", &OYPosVect);
            tree->Branch("hit_oz", &OZPosVect);
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
            OXPosVect.clear();
            OYPosVect.clear();
            OZPosVect.clear();
            ELossVect.clear();
            IdVect.clear();
            TrackIdVect.clear();
            GenParentTrackIdVect.clear();
            IRepVect.clear();
            JRepVect.clear();
            VolumeNameVect.clear();
        }

        uint64_t HitsCount;
        std::vector<uint64_t> IdVect;
        std::vector<uint64_t> TrackIdVect;
        std::vector<uint64_t> GenParentTrackIdVect;
        std::vector<double> XPosVect;
        std::vector<double> YPosVect;
        std::vector<double> ZPosVect;
        std::vector<double> OXPosVect;
        std::vector<double> OYPosVect;
        std::vector<double> OZPosVect;
        std::vector<double> ELossVect;
        std::vector<uint64_t> IRepVect;
        std::vector<uint64_t> JRepVect;
        std::vector<std::string> VolumeNameVect;
    };
//
//    struct ZDCHitIo
//    {
//        void BindToTree(TTree *tree)
//        {
//            tree->Branch("ZDChit_count",  &HitsCount, "hit_count/l");
//            tree->Branch("ZDChit_id",     &IdVect);
//            tree->Branch("ZDChit_x",      &XPosVect);
//            tree->Branch("ZDChit_y",      &YPosVect);
//            tree->Branch("ZDChit_z",      &ZPosVect);
//            tree->Branch("ZDChit_e_loss", &ELossVect);
//            tree->Branch("ZDChit_charge", &Charge);
//            tree->Branch("ZDChit_copy",   &CopyNum);
//        }
//
//        void Clear()
//        {
//            HitsCount = 0;
//            IdVect.clear();
//            XPosVect.clear();
//            YPosVect.clear();
//            ZPosVect.clear();
//            ELossVect.clear();
//            Charge.clear();
//            CopyNum.clear();
//        }
//
//        uint64_t    HitsCount;
//        std:vector<uint64_t> IdVect;
//        std::vector<double> XPosVect;
//        std::vector<double> YPosVect;
//        std::vector<double> ZPosVect;
//        std::vector<double> ELossVect;
//        std::vector<int> Charge;
//        std::vector<int> CopyNum;
//    }
//
}
#endif //G4E_FLATIOHIT_HH
