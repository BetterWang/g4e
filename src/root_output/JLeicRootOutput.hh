#ifndef G4E_JLEICROOTOUTPUT_HH
#define G4E_JLEICROOTOUTPUT_HH


#include <string>
#include <memory>

#include <TFile.h>
#include <TTree.h>

#include <spdlog/spdlog.h>

namespace g4e
{
    class JLeicRootOutput {

        struct HitIo {
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
            std::vector <uint64_t> IdVect;
            std::vector <uint64_t> TrackIdVect;
            std::vector <uint64_t> GenPartIdVect;
            std::vector <double> XPosVect;
            std::vector <double> YPosVect;
            std::vector <double> ZPosVect;
            std::vector <double> ELossVect;
            std::vector <uint64_t> IRepVect;
            std::vector <uint64_t> JRepVect;
            std::vector <std::string> VolumeNameVect;
        };

        struct TrackIo {

            void BindToTree(TTree* tree)
            {
                tree->Branch("trk_count", &TrackCount, "trk_count/l");
                tree->Branch("trk_id", &IdVect);
                tree->Branch("trk_pdg", &PdgVect);
                tree->Branch("trk_part_id", &ParentId);
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
            std::vector <uint64_t> IdVect;
            std::vector <uint64_t> ParentId;
            std::vector <uint64_t> PdgVect;
            std::vector <double> XVtxVect;
            std::vector <double> YVtxVect;
            std::vector <double> ZVtxVect;
            std::vector <double> XDirVtxVect;
            std::vector <double> YDirVtxVect;
            std::vector <double> ZDirVtxVect;
            std::vector <double> MomentumVect;
        };

        struct VertexIo {
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

        struct ParticleIo {

            void BindToTree(TTree *tree)
            {
                tree->Branch("gen_prt_count",     &ParticleCount, "gen_prt_count/l");
                tree->Branch("gen_prt_id",        &IdVect);
                tree->Branch("gen_prt_vtx_id",    &PrimeVtxIdVect);
                tree->Branch("gen_prt_pdg",       &PDGCodeVect);
                tree->Branch("gen_prt_trk_id",    &TrackIdVect);
                tree->Branch("gen_prt_charge",    &ChargeVect);
                tree->Branch("gen_prt_dir_x",     &MomDirXVect);
                tree->Branch("gen_prt_dir_y",     &MomDirYVect);
                tree->Branch("gen_prt_dir_z",     &MomDirZVect);
                tree->Branch("gen_prt_tot_mom",   &TotalMomentumVect);
                tree->Branch("gen_prt_tot_e",     &TotalEnergyVect);
                tree->Branch("gen_prt_time",      &ProperTimeVect);
                tree->Branch("gen_prt_polariz_x", &PolXVect);
                tree->Branch("gen_prt_polariz_y", &PolYVect);
                tree->Branch("gen_prt_polariz_z", &PolZVect);
            }

            void Clear()
            {
                ParticleCount=0;

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

    public:
        void Initialize(TFile *file)

        {
            printf("JLeicCalorimeterSD():: book Tree  for hitss \n");
            printf("JLeicCalorimeterSD():: SAVE Matrix file jleic_geant_hits.root \n");

            //--- Vector Branches -----
            mRootFile = file;
            mEventTree = new TTree("events", "a Tree with vect");
            mEventTree->SetDirectory(file);

            mEventTree->Branch("event_id", &mEventId, "event_id/l");

            mHitIo.BindToTree(mEventTree);          // Create branches for hits
            mTrackIo.BindToTree(mEventTree);        // Create branches for tracks
            mParticleIo.BindToTree(mEventTree);     // Create branches for primary particles
            mPrimeVertexIo.BindToTree(mEventTree);  // Create branches for primary vertexes
        }

        void ClearForNewEvent()
        {
            mHitIo.Clear();
            mTrackIo.Clear();
            mParticleIo.Clear();
            mPrimeVertexIo.Clear();
            trk_index_by_id.clear();
        }

        void AddHit(
                uint64_t aHitId,        // Hit unique ID
                uint64_t aTrackId,      // Track unique ID
                uint64_t aGenPartId,        // Generated particle ID
                double aX,
                double aY,
                double aZ,
                double aELoss,
                uint64_t aIRep,
                uint64_t aJRep,
                const std::string &aVolName
                )
        {

            mHitIo.IdVect.push_back(aHitId);
            mHitIo.XPosVect.push_back(aX);
            mHitIo.YPosVect.push_back(aY);
            mHitIo.ZPosVect.push_back(aZ);
            mHitIo.ELossVect.push_back(aELoss);
            mHitIo.TrackIdVect.push_back(aTrackId);
            mHitIo.GenPartIdVect.push_back(aGenPartId);
            mHitIo.IRepVect.push_back(aIRep);
            mHitIo.JRepVect.push_back(aJRep);
            mHitIo.VolumeNameVect.push_back(aVolName);
            mHitIo.HitsCount = mHitIo.XPosVect.size();
        }

        void AddTrack(
                uint64_t aTrackId,
                uint64_t aParentId,
                uint64_t aTrackPdg,
                double aXVertex,
                double aYVertex,
                double aZVertex,
                double aXMom,
                double aYMom,
                double aZMom,
                double aMom
        )
        {
            if(trk_index_by_id.count(aTrackId)) {
                return;     // We already saved the track with this id. Nothing to do
            }

            mTrackIo.IdVect.push_back(aTrackId);
            mTrackIo.ParentId.push_back(aParentId);
            mTrackIo.PdgVect.push_back(aTrackPdg);
            mTrackIo.XVtxVect.push_back(aXVertex);
            mTrackIo.YVtxVect.push_back(aYVertex);
            mTrackIo.ZVtxVect.push_back(aZVertex);
            mTrackIo.XDirVtxVect.push_back(aXMom);
            mTrackIo.YDirVtxVect.push_back(aYMom);
            mTrackIo.ZDirVtxVect.push_back(aZMom);
            mTrackIo.MomentumVect.push_back(aMom);
            trk_index_by_id[aTrackId] = 1;
            mTrackIo.TrackCount = mTrackIo.IdVect.size();
        }

        void AddPrimaryVertex(
                uint64_t aId,
                uint64_t aParticleCount,
                double aX,
                double aY,
                double aZ,
                double aTime,
                double aWeight
        )
        {
            mPrimeVertexIo.IdVect.push_back(aId);
            mPrimeVertexIo.ParticleCountVect.push_back(aParticleCount);
            mPrimeVertexIo.XVect.push_back(aX);
            mPrimeVertexIo.YVect.push_back(aY);
            mPrimeVertexIo.ZVect.push_back(aZ);
            mPrimeVertexIo.TimeVect.push_back(aTime);
            mPrimeVertexIo.WeightVect.push_back(aWeight);
            mPrimeVertexIo.VertexCount = mPrimeVertexIo.IdVect.size();
        }

        void AddPrimaryParticle (
                uint64_t aId,
                uint64_t aPrimeVtxId,
                uint64_t aPDGCode,
                uint64_t aTrackId,
                double aCharge,
                double aMomDirX,
                double aMomDirY,
                double aMomDirZ,
                double aTotalMomentum,
                double aTotalEnergy,
                double aProperTime,
                double aPolX,
                double aPolY,
                double aPolZ)
        {
            mParticleIo.IdVect.push_back(aId);
            mParticleIo.PrimeVtxIdVect.push_back(aPrimeVtxId);
            mParticleIo.PDGCodeVect.push_back(aPDGCode);
            mParticleIo.TrackIdVect.push_back(aTrackId);
            mParticleIo.ChargeVect.push_back(aCharge);
            mParticleIo.MomDirXVect.push_back(aMomDirX);
            mParticleIo.MomDirYVect.push_back(aMomDirY);
            mParticleIo.MomDirZVect.push_back(aMomDirZ);
            mParticleIo.TotalMomentumVect.push_back(aTotalMomentum);
            mParticleIo.TotalEnergyVect.push_back(aTotalEnergy);
            mParticleIo.ProperTimeVect.push_back(aProperTime);
            mParticleIo.PolXVect.push_back(aPolX);
            mParticleIo.PolYVect.push_back(aPolY);
            mParticleIo.PolZVect.push_back(aPolZ);
            mParticleIo.ParticleCount = mParticleIo.IdVect.size();
        }

        void FillEvent(uint64_t eventId)
        {
            mEventId = eventId;

            fmt::print("Fill hits tree ....ev={}  Nhits={}  Ntracks={} \n", mEventId, mHitIo.HitsCount, mTrackIo.TrackCount);
            mEventTree->Fill();
        }

        void Write()
        {
            if(mRootFile)
            {
                mRootFile->cd();
                mEventTree->SetDirectory(mRootFile);
                mEventTree->Write();
                mRootFile->Flush();
            }
        }



    private:

        TFile *mRootFile;
        TTree *mEventTree;
        uint64_t mEventId;

        std::map<uint64_t, uint64_t> trk_index_by_id; // Track vector indexes by track id

        HitIo mHitIo;
        TrackIo mTrackIo;
        VertexIo mPrimeVertexIo;
        ParticleIo mParticleIo;
    };
}

#endif //G4E_JLEICROOTOUTPUT_HH
