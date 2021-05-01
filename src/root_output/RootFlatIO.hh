#ifndef G4E_ROOTFLATIO_HH
#define G4E_ROOTFLATIO_HH


#include <string>
#include <memory>
#include <mutex>

#include <TFile.h>
#include <TTree.h>

#include <G4Event.hh>

#include <spdlog/spdlog.h>
#include <UserEventInformation.hh>

#include "FlatIoHit.hh"
#include "FlatIoParticle.hh"
#include "FlatIoTrack.hh"
#include "FlatIoVertex.hh"
#include "FlatIoCe_EMCAL.hh"


namespace g4e
{
    class RootFlatIO {
    public:

        void Initialize(TFile *file, TTree *eventTree )
        {
            //--- Vector Branches -----
            mRootFile = file;
            mEventTree = eventTree;
            eventTree->SetTitle("g4e_api_v3");

            mEventTree->Branch("event_id", &mEventId, "event_id/l");
            mEventTree->Branch("evt_true_q2",      &mEventInfo.TrueDISInfo.Q2,   "evt_true_q2/D");
            mEventTree->Branch("evt_true_x",       &mEventInfo.TrueDISInfo.x ,   "evt_true_x/D");
            mEventTree->Branch("evt_true_y",       &mEventInfo.TrueDISInfo.y ,   "evt_true_y/D");
            mEventTree->Branch("evt_true_w2",      &mEventInfo.TrueDISInfo.w2,   "evt_true_w2/D");
            mEventTree->Branch("evt_true_nu",      &mEventInfo.TrueDISInfo.nu,   "evt_true_nu/D");
            mEventTree->Branch("evt_true_t_hat",   &mEventInfo.TrueDISInfo.tHat, "evt_true_t_hat/D");
            mEventTree->Branch("evt_has_dis_info", &mEventInfo.HasTrueDISInfo,   "evt_has_dis_info/B");
            mEventTree->Branch("evt_weight",       &mEventInfo.Weight,           "evt_weight/D");

            mHitIo.BindToTree(mEventTree);          // Bind/create branches for hits
            mTrackIo.BindToTree(mEventTree);        // Bind/create branches for tracks
            mParticleIo.BindToTree(mEventTree);     // Bind/create branches for primary particles
            mPrimeVertexIo.BindToTree(mEventTree);  // Bind/create branches for primary vertexes
            mCe_EMCALIo.BindToTree(mEventTree);      // Bind/create branches for Ce_EMCAL
        }

        void ClearForNewEvent()
        {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            mHitIo.Clear();
            mTrackIo.Clear();
            mParticleIo.Clear();
            mPrimeVertexIo.Clear();
            mCe_EMCALIo.Clear();

            // Index by id maps
            mTrackIndexById.clear();
            mVertexIndexById.clear();
        }

        void AddHit(
                uint64_t trackIndex,    // Index of a parent track inside tracks array
                uint64_t aTrackId,      // Track unique ID
                uint64_t ParentId,      // Parent track ID
                int64_t pdg,            // PDG code of the particle/track
                double aX,
                double aY,
                double aZ,
                double oX,
                double oY,
                double oZ,
                double aELoss,
                uint64_t aIRep,
                uint64_t aJRep,
                const std::string &aVolName,
                int type                // hit type. See g4e::HitTypes
                )
        {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);


            mHitIo.IdVect.push_back(mHitIo.IdVect.size());
            mHitIo.XPosVect.push_back(aX);
            mHitIo.YPosVect.push_back(aY);
            mHitIo.ZPosVect.push_back(aZ);
            mHitIo.OXPosVect.push_back(oX);
            mHitIo.OYPosVect.push_back(oY);
            mHitIo.OZPosVect.push_back(oZ);
            mHitIo.ELossVect.push_back(aELoss);
            mHitIo.TrackIdVect.push_back(aTrackId);
            mHitIo.TrackIndexVect.push_back(trackIndex);
            mHitIo.ParentTrackIdVect.push_back(ParentId);
            mHitIo.IRepVect.push_back(aIRep);
            mHitIo.JRepVect.push_back(aJRep);
            mHitIo.VolumeNameVect.push_back(aVolName);
            mHitIo.PdgVect.push_back(pdg);
            mHitIo.HitsCount = mHitIo.XPosVect.size();
            mHitIo.TypeVect.push_back(type);
        }

        int64_t AddTrack(
                uint64_t aTrackId,
                uint64_t aParentId,
                int64_t  aTrackPdg,
                double   aTrackCharge,
                int64_t  creatorProc,
                uint64_t ancestryLevel,
                double   aXVertex,
                double   aYVertex,
                double   aZVertex,
                double   aXMom,
                double   aYMom,
                double   aZMom,
                double   aMom
        )
        {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            if(mTrackIndexById.count(aTrackId)) {
                return mTrackIndexById[aTrackId];     // We already saved the track with this id. Nothing to do
            }
            uint64_t thisTrackIndex = mTrackIo.IdVect.size();
 
            mTrackIo.IdVect.push_back(aTrackId);
            mTrackIo.ParentId.push_back(aParentId);
            mTrackIo.PdgVect.push_back(aTrackPdg);
            mTrackIo.Charge.push_back(aTrackCharge);
            mTrackIo.CreateProc.push_back(creatorProc);
            mTrackIo.AncestryLevel.push_back(ancestryLevel);
            mTrackIo.XVtxVect.push_back(aXVertex);
            mTrackIo.YVtxVect.push_back(aYVertex);
            mTrackIo.ZVtxVect.push_back(aZVertex);
            mTrackIo.XDirVtxVect.push_back(aXMom);
            mTrackIo.YDirVtxVect.push_back(aYMom);
            mTrackIo.ZDirVtxVect.push_back(aZMom);
            mTrackIo.MomentumVect.push_back(aMom);
            mTrackIndexById[aTrackId] = thisTrackIndex;
            mTrackIo.TrackCount = mTrackIo.IdVect.size();
            return thisTrackIndex;
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
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            mVertexIndexById[aId] = mPrimeVertexIo.IdVect.size();
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
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            mParticleIo.IdVect.push_back(aId);
            mParticleIo.VertexIdVect.push_back(aPrimeVtxId);
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

        void FillCe_EMCAL(Ce_EMCALIoData data) {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            mCe_EMCALIo.Fill(data);
        }



        void FillEvent(const G4Event *evt)
        {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            mEventId = (uint64_t)evt->GetEventID();
            auto userEventData = dynamic_cast<g4e::UserEventInformation*>(evt->GetUserInformation());

            // Copy event data values
            if(userEventData) {
                mEventInfo = userEventData->GetEventData();
            }

            // Set track indexes for parent tracks
            for(int i=0; i<mTrackIo.TrackCount; i++) {
                mTrackIo.ParentIndex.push_back(mTrackIndexById[mTrackIo.ParentId[i]]);
            }

            // Set track indexes for generated particles
            for(int i=0; i<mParticleIo.ParticleCount; i++) {
                mParticleIo.TrackIndexVect.push_back(mTrackIndexById[mParticleIo.TrackIdVect[i]]);
            }

            // Set vertex indexes for vertexes
            for(int i=0; i<mParticleIo.ParticleCount; i++) {
                mParticleIo.PrimeVtxIndexVect.push_back(mVertexIndexById[mParticleIo.VertexIdVect[i]]);
            }

            fmt::print("RooIO: {:<4} hits:{:<7} tracks:{:<7} genpart:{:<4} \n", mEventId+1, mHitIo.HitsCount, mTrackIo.TrackCount, mParticleIo.ParticleCount);
            mEventTree->Fill();
        }

        void Write()
        {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            if(mRootFile)
            {
                mRootFile->cd();
                mEventTree->SetDirectory(mRootFile);
                mEventTree->Write();
                mRootFile->Flush();
            }
        }

    private:

        // TODO there should be a FlatIO for each worker thread
        std::recursive_mutex io_mutex;

        TFile *mRootFile;
        TTree *mEventTree;
        uint64_t mEventId;

        std::map<uint64_t, uint64_t> mTrackIndexById; // Track vector indexes by track id
        std::map<uint64_t, uint64_t> mVertexIndexById; // Track vector indexes by track id

        g4e::HitIo mHitIo;
        g4e::TrackIo mTrackIo;
        g4e::VertexIo mPrimeVertexIo;
        g4e::ParticleIo mParticleIo;
        g4e::EventData mEventInfo;
        g4e::Ce_EMCALIo mCe_EMCALIo;
    };
}

#endif //G4E_ROOTFLATIO_HH
