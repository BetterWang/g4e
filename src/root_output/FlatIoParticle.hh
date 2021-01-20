#ifndef G4E_ROOTFLATIOPARTICLE_HH
#define G4E_ROOTFLATIOPARTICLE_HH

#include <TTree.h>

namespace g4e {

    struct ParticleIo
    {

        void BindToTree(TTree *tree)
        {
            tree->Branch("gen_prt_count", &ParticleCount, "gen_prt_count/l");
            tree->Branch("gen_prt_id", &IdVect);
            tree->Branch("gen_prt_vtx_id", &PrimeVtxIdVect);
            tree->Branch("gen_prt_vtx_index", &PrimeVtxIdVect);
            tree->Branch("gen_prt_pdg", &PDGCodeVect);
            tree->Branch("gen_prt_trk_id", &TrackIdVect);
            tree->Branch("gen_prt_trk_index", &TrackIdVect);
            tree->Branch("gen_prt_charge", &ChargeVect);
            tree->Branch("gen_prt_dir_x", &MomDirXVect);
            tree->Branch("gen_prt_dir_y", &MomDirYVect);
            tree->Branch("gen_prt_dir_z", &MomDirZVect);
            tree->Branch("gen_prt_tot_mom", &TotalMomentumVect);
            tree->Branch("gen_prt_tot_e", &TotalEnergyVect);
            tree->Branch("gen_prt_time", &ProperTimeVect);
            tree->Branch("gen_prt_polariz_x", &PolXVect);
            tree->Branch("gen_prt_polariz_y", &PolYVect);
            tree->Branch("gen_prt_polariz_z", &PolZVect);
        }

        void Clear()
        {
            ParticleCount = 0;

            PrimeVtxIdVect.clear();
            IdVect.clear();
            PDGCodeVect.clear();
            TrackIdVect.clear();
            ChargeVect.clear();
            MomDirXVect.clear();
            MomDirYVect.clear();
            MomDirZVect.clear();
            TotalMomentumVect.clear();
            TotalEnergyVect.clear();
            ProperTimeVect.clear();
            PolXVect.clear();
            PolYVect.clear();
            PolZVect.clear();
        }

        uint64_t ParticleCount;
        std::vector<uint64_t> IdVect;             // PartInVtxIndexVect
        std::vector<uint64_t> PrimeVtxIdVect;     // primeVtxIndex
        std::vector<uint64_t> PDGCodeVect;        // particle->GetPDGcode();
        std::vector<uint64_t> TrackIdVect;        // particle->GetTrackID();
        std::vector<double> ChargeVect;         // particle->GetCharge();
        std::vector<double> MomDirXVect;        // particle->GetMomentumDirection().x();
        std::vector<double> MomDirYVect;        // particle->GetMomentumDirection().y();
        std::vector<double> MomDirZVect;        // particle->GetMomentumDirection().z();
        std::vector<double> TotalMomentumVect;  // particle->GetTotalMomentum()/GeV;
        std::vector<double> TotalEnergyVect;    // particle->GetTotalEnergy()/GeV;
        std::vector<double> ProperTimeVect;     // particle->GetProperTime()/ns;
        std::vector<double> PolXVect;           // particle->GetPolX();
        std::vector<double> PolYVect;           // particle->GetPolY();
        std::vector<double> PolZVect;           // particle->GetPolZ();
    };
}

#endif //G4E_ROOTFLATIOPARTICLE_HH
