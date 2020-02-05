#ifndef G4E_FLATIOVERTEX_HH
#define G4E_FLATIOVERTEX_HH

namespace g4e {
    struct VertexIo
    {
        void BindToTree(TTree *tree)
        {
            tree->Branch("gen_vtx_count", &VertexCount, "gen_vtx_count/l");
            tree->Branch("gen_vtx_id", &IdVect);
            tree->Branch("gen_vtx_part_count", &ParticleCountVect);
            tree->Branch("gen_vtx_x", &XVect);
            tree->Branch("gen_vtx_y", &YVect);
            tree->Branch("gen_vtx_z", &ZVect);
            tree->Branch("gen_vtx_time", &TimeVect);
            tree->Branch("gen_vtx_weight", &WeightVect);
        }

        void Clear()
        {
            VertexCount = 0;
            IdVect.clear();
            ParticleCountVect.clear();
            XVect.clear();
            YVect.clear();
            ZVect.clear();
            TimeVect.clear();
            WeightVect.clear();
        }

        uint64_t VertexCount;
        std::vector<uint64_t> IdVect;             // PrimaryVertexIndex;
        std::vector<uint64_t> ParticleCountVect;  // PrimaryVertex->GetNumberOfParticle();
        std::vector<double> XVect;              // PrimaryVertex->GetX0();
        std::vector<double> YVect;              // PrimaryVertex->GetY0();
        std::vector<double> ZVect;              // PrimaryVertex->GetZ0();
        std::vector<double> TimeVect;           // PrimaryVertex->GetT0();
        std::vector<double> WeightVect;         // PrimaryVertex->GetWeight();
    };
}

#endif //G4E_FLATIOVERTEX_HH
