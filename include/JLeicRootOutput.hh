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

        struct Hit {


        };

        struct Track {

        };

        struct VertexIo
        {
            void BindToTree(TTree *tree)
            {
                tree->Branch("prime_vtx_count", &VertexCount, "prime_vtx_count/l");
                tree->Branch("prime_vtx_id", &IdVect);
                tree->Branch("prime_vtx_particle_count", &ParticleCountVect);
                tree->Branch("prime_vtx_x", &XVect);
                tree->Branch("prime_vtx_y", &YVect);
                tree->Branch("prime_vtx_z", &ZVect);
                tree->Branch("prime_vtx_time", &TimeVect);
                tree->Branch("prime_vtx_weight", &WeightVect);
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
                tree->Branch("prime_part_count",     &ParticleCount, "prime_part_count/l");
                tree->Branch("prime_part_id",        &IdVect);
                tree->Branch("prime_part_vtx_id",    &PrimeVtxIdVect);
                tree->Branch("prime_part_pdg",       &PDGCodeVect);
                tree->Branch("prime_part_trk_id",    &TrackIdVect);
                tree->Branch("prime_part_charge",    &ChargeVect);
                tree->Branch("prime_part_dir_x",     &MomDirXVect);
                tree->Branch("prime_part_dir_y",     &MomDirYVect);
                tree->Branch("prime_part_dir_z",     &MomDirZVect);
                tree->Branch("prime_part_tot_mom",   &TotalMomentumVect);
                tree->Branch("prime_part_tot_e",     &TotalEnergyVect);
                tree->Branch("prime_part_time",      &ProperTimeVect);
                tree->Branch("prime_part_polariz_x", &PolXVect);
                tree->Branch("prime_part_polariz_y", &PolYVect);
                tree->Branch("prime_part_polariz_z", &PolZVect);

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
        void Initialize(const std::string &fileName)
        {
            printf("JLeicCalorimeterSD():: book Tree  for hitss \n");
            printf("JLeicCalorimeterSD():: SAVE Matrix file jleic_geant_hits.root \n");
            fhits = new TFile(fileName.c_str(), "RECREATE");

            //--- Vector Branches -----

            EVENT_VECT = new TTree("event", "a Tree with vect");

            EVENT_VECT->Branch("event_num", &event_num, "event_num/I");
            EVENT_VECT->Branch("hit_size", &hit_size, "hit_size/I");
            EVENT_VECT->Branch("trackID", &trackID);
            EVENT_VECT->Branch("DetectorName", &DetectorName);
            EVENT_VECT->Branch("xpos", &xpos);
            EVENT_VECT->Branch("ypos", &ypos);
            EVENT_VECT->Branch("zpos", &zpos);
            EVENT_VECT->Branch("ipos", &ipos);
            EVENT_VECT->Branch("jpos", &jpos);
            EVENT_VECT->Branch("dedx", &dedx);

            EVENT_VECT->Branch("track_size", &track_size, "track_size/I");
            EVENT_VECT->Branch("trkID", &trkID);
            EVENT_VECT->Branch("trkPDG", &trkPDG);
            EVENT_VECT->Branch("parID", &parID);
            EVENT_VECT->Branch("xvtx", &xvtx);
            EVENT_VECT->Branch("yvtx", &yvtx);
            EVENT_VECT->Branch("zvtx", &zvtx);
            EVENT_VECT->Branch("pxvtx", &pxvtx);
            EVENT_VECT->Branch("pyvtx", &pyvtx);
            EVENT_VECT->Branch("pzvtx", &pzvtx);
            EVENT_VECT->Branch("trkMom", &trkMom);

            mParticleIo.BindToTree(EVENT_VECT);
            fVertexIo.BindToTree(EVENT_VECT);
        }

        ~RootOutput()
        {



        }

        void AddHit(
                double aX,
                double aY,
                double aZ,
                double adedx,
                int aTrackId,
                int aipos,
                int ajpos,
                const std::string &aDetectorName
                )
        {

            xpos.push_back(aX);
            ypos.push_back(aY);
            zpos.push_back(aZ);
            dedx.push_back(adedx);
            trackID.push_back(aTrackId);
            ipos.push_back(aipos);
            jpos.push_back(ajpos);
            DetectorName.push_back(aDetectorName);

            hit_size = xpos.size();
        }

        void UpdateTrack(
                int aTrackId,
                int aParentId,
                int aTrackPdg,
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

            trkID.push_back(aTrackId);
            parID.push_back(aParentId);
            trkPDG.push_back(aTrackPdg);
            xvtx.push_back(aXVertex);
            yvtx.push_back(aYVertex);
            zvtx.push_back(aZVertex);
            pxvtx.push_back(aXMom);
            pyvtx.push_back(aYMom);
            pzvtx.push_back(aZMom);
            trkMom.push_back(aMom);
            trk_index_by_id[aTrackId] = track_size;
            track_size = trkID.size();
        }

        void Clear()
        {
            hit_size = 0;
            track_size = 0;

            xpos.clear();
            ypos.clear();
            zpos.clear();
            dedx.clear();
            trackID.clear();
            ipos.clear();
            jpos.clear();
            DetectorName.clear();

            trkID.clear();
            parID.clear();
            trkPDG.clear();
            xvtx.clear();
            yvtx.clear();
            zvtx.clear();
            pxvtx.clear();
            pyvtx.clear();
            pzvtx.clear();
            trkMom.clear();
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
            fVertexIo.IdVect.push_back(aId);
            fVertexIo.ParticleCountVect.push_back(aParticleCount);
            fVertexIo.XVect.push_back(aX);
            fVertexIo.YVect.push_back(aY);
            fVertexIo.ZVect.push_back(aZ);
            fVertexIo.TimeVect.push_back(aTime);
            fVertexIo.WeightVect.push_back(aWeight);
            fVertexIo.VertexCount = fVertexIo.IdVect.size();
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

        void FillEvent(int eventId)
        {
            event_num = eventId;

            printf("Fill hits tree ....ev=%d  Nhits=%d  Ntracks=%d \n", event_num, hit_size, track_size);
            EVENT_VECT->Fill();

        }

        void Close()
        {
            EVENT_VECT->SetDirectory(fhits);
            EVENT_VECT->Write();
            fhits->Close();
            delete fhits;
        }

    private:

        TFile *fhits;

        TTree *EVENT_VECT;

        int event_num;
        size_t hit_size;
        std::vector <double> xpos;
        std::vector <double> ypos;
        std::vector <double> zpos;
        std::vector <double> dedx;
        std::vector <int> trackID;
        std::vector <int> ipos;
        std::vector <int> jpos;
        std::vector <std::string> DetectorName;

        size_t track_size;
        std::vector <int> trkID;
        std::vector <int> parID;
        std::vector <int> trkPDG;
        std::vector <double> xvtx;
        std::vector <double> yvtx;
        std::vector <double> zvtx;
        std::vector <double> pxvtx;
        std::vector <double> pyvtx;
        std::vector <double> pzvtx;
        std::vector <double> trkMom;
        std::map<uint64_t, size_t> trk_index_by_id; // Track vector indexes by track id

        VertexIo fVertexIo;
        ParticleIo mParticleIo;
    };
}

#endif //G4E_JLEICROOTOUTPUT_HH
