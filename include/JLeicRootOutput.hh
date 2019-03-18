#include <memory>

//
// Created by romanov on 3/14/19.
//

#ifndef G4E_JLEICROOTOUTPUT_HH
#define G4E_JLEICROOTOUTPUT_HH

#include <string>

#include <TFile.h>
#include <TTree.h>

namespace g4e
{
    class RootOutput {

        struct HitIo {
            void BindToTree(TTree *tree)
            {
                tree->Branch("hit_count", &HitsCount, "hit_count/l");
                tree->Branch("hit_id", &IdVect);
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
                IRepVect.clear();
                JRepVect.clear();
                VolumeNameVect.clear();
            }

            uint64_t HitsCount;
            std::vector <uint64_t> IdVect;
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
                tree->Branch("trk_part_id", &ParticleIdVect);
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
                ParticleIdVect.clear();
                PdgVect.clear();
                XVtxVect.clear();
                YVtxVect.clear();
                ZVtxVect.clear();
                XDirVtxVect.clear();
                YDirVtxVect.clear();
                ZDirVtxVect.clear();
                MomentumVect.clear();
            }

            size_t TrackCount;
            std::vector <uint64_t> IdVect;
            std::vector <uint64_t> ParticleIdVect;
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
                tree->Branch("o_vtx_count", &VertexCount, "o_vtx_count/l");
                tree->Branch("o_vtx_id", &IdVect);
                tree->Branch("o_vtx_part_count", &ParticleCountVect);
                tree->Branch("o_vtx_x", &XVect);
                tree->Branch("o_vtx_y", &YVect);
                tree->Branch("o_vtx_z", &ZVect);
                tree->Branch("o_vtx_time", &TimeVect);
                tree->Branch("o_vtx_weight", &WeightVect);
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

            size_t VertexCount;
            std::vector<size_t> IdVect;             // PrimaryVertexIndex;
            std::vector<size_t> ParticleCountVect;  // PrimaryVertex->GetNumberOfParticle();
            std::vector<double> XVect;              // PrimaryVertex->GetX0();
            std::vector<double> YVect;              // PrimaryVertex->GetY0();
            std::vector<double> ZVect;              // PrimaryVertex->GetZ0();
            std::vector<double> TimeVect;           // PrimaryVertex->GetT0();
            std::vector<double> WeightVect;         // PrimaryVertex->GetWeight();
        };

        struct ParticleIo {

            void BindToTree(TTree *tree)
            {
                tree->Branch("o_part_count",     &ParticleCount, "o_part_count/l");
                tree->Branch("o_part_id",        &IdVect);
                tree->Branch("o_part_vtx_id",    &PrimeVtxIdVect);
                tree->Branch("o_part_pdg",       &PDGCodeVect);
                tree->Branch("o_part_trk_id",    &TrackIdVect);
                tree->Branch("o_part_charge",    &ChargeVect);
                tree->Branch("o_part_dir_x",     &MomDirXVect);
                tree->Branch("o_part_dir_y",     &MomDirYVect);
                tree->Branch("o_part_dir_z",     &MomDirZVect);
                tree->Branch("o_part_tot_mom",   &TotalMomentumVect);
                tree->Branch("o_part_tot_e",     &TotalEnergyVect);
                tree->Branch("o_part_time",      &ProperTimeVect);
                tree->Branch("o_part_polariz_x", &PolXVect);
                tree->Branch("o_part_polariz_y", &PolYVect);
                tree->Branch("o_part_polariz_z", &PolZVect);
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

            size_t ParticleCount;
            std::vector<size_t> IdVect;             // PartInVtxIndexVect
            std::vector<size_t> PrimeVtxIdVect;     // primeVtxIndex
            std::vector<size_t> PDGCodeVect;        // particle->GetPDGcode();
            std::vector<size_t> TrackIdVect;        // particle->GetTrackID();
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
            mEventTree = new TTree("event", "a Tree with vect");
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
        }

        void AddHit(
                uint64_t aTrackId,
                double aX,
                double aY,
                double aZ,
                double aELoss,
                uint64_t aIRep,
                uint64_t aJRep,
                const std::string &aVolName
                )
        {
            mHitIo.XPosVect.push_back(aX);
            mHitIo.YPosVect.push_back(aY);
            mHitIo.ZPosVect.push_back(aZ);
            mHitIo.ELossVect.push_back(aELoss);
            mHitIo.IdVect.push_back(aTrackId);
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
            mTrackIo.ParticleIdVect.push_back(aParentId);
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
                size_t aId,
                size_t aParticleCount,
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
                size_t aId,
                size_t aPrimeVtxId,
                size_t aPDGCode,
                size_t aTrackId,
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

            printf("Fill hits tree ....ev=%d  Nhits=%d  Ntracks=%d \n", mEventId, mHitIo.HitsCount, mTrackIo.TrackCount);
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

        std::map<uint64_t, size_t> trk_index_by_id; // Track vector indexes by track id

        HitIo mHitIo;
        TrackIo mTrackIo;
        VertexIo mPrimeVertexIo;
        ParticleIo mParticleIo;
    };
}

#endif //G4E_JLEICROOTOUTPUT_HH
