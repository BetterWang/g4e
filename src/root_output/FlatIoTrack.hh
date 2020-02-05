#ifndef G4E_FLATIOTRACK_HH
#define G4E_FLATIOTRACK_HH

namespace g4e {
    struct TrackIo
    {

        void BindToTree(TTree *tree)
        {
            tree->Branch("trk_count", &TrackCount, "trk_count/l");
            tree->Branch("trk_id", &IdVect);
            tree->Branch("trk_pdg", &PdgVect);
            tree->Branch("trk_parent_id", &ParentId);
            tree->Branch("trk_vtx_x", &XVtxVect);
            tree->Branch("trk_vtx_y", &YVtxVect);
            tree->Branch("trk_vtx_z", &ZVtxVect);
            tree->Branch("trk_vtx_dir_x", &XDirVtxVect);
            tree->Branch("trk_vtx_dir_y", &YDirVtxVect);
            tree->Branch("trk_vtx_dir_z", &ZDirVtxVect);
            tree->Branch("trk_mom", &MomentumVect);
        }

        void Clear()
        {
            TrackCount = 0;
            IdVect.clear();
            ParentId.clear();
            PdgVect.clear();
            XVtxVect.clear();
            YVtxVect.clear();
            ZVtxVect.clear();
            XDirVtxVect.clear();
            YDirVtxVect.clear();
            ZDirVtxVect.clear();
            MomentumVect.clear();
        }

        uint64_t TrackCount;
        std::vector<uint64_t> IdVect;
        std::vector<uint64_t> ParentId;
        std::vector<uint64_t> PdgVect;
        std::vector<double> XVtxVect;
        std::vector<double> YVtxVect;
        std::vector<double> ZVtxVect;
        std::vector<double> XDirVtxVect;
        std::vector<double> YDirVtxVect;
        std::vector<double> ZDirVtxVect;
        std::vector<double> MomentumVect;
    };
}
#endif //G4E_FLATIOTRACK_HH
