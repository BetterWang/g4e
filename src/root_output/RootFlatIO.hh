#ifndef G4E_ROOTFLATIO_HH
#define G4E_ROOTFLATIO_HH


#include <string>
#include <memory>
#include <mutex>

#include <TFile.h>
#include <TTree.h>

#include <spdlog/spdlog.h>

#include "FlatIoHit.hh"
#include "FlatIoParticle.hh"
#include "FlatIoTrack.hh"
#include "FlatIoVertex.hh"

namespace g4e
{
    class RootFlatIO {
    public:

        void Initialize(TFile *file, TTree *eventTree )
        {
            //--- Vector Branches -----
            mRootFile = file;
            mEventTree = eventTree;

            mEventTree->Branch("event_id", &mEventId, "event_id/l");

            mHitIo.BindToTree(mEventTree);          // Bind/create branches for hits
            mTrackIo.BindToTree(mEventTree);        // Bind/create branches for tracks
            mParticleIo.BindToTree(mEventTree);     // Bind/create branches for primary particles
            mPrimeVertexIo.BindToTree(mEventTree);  // Bind/create branches for primary vertexes
        }

        void ClearForNewEvent()
        {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            mHitIo.Clear();
            mTrackIo.Clear();
            mParticleIo.Clear();
            mPrimeVertexIo.Clear();
            trk_index_by_id.clear();
        }

        void AddHit(
                uint64_t aHitId,        // Hit unique ID
                uint64_t aTrackId,      // Track unique ID
                uint64_t ParentId,      // Parent track ID
                double aX,
                double aY,
                double aZ,
                double aELoss,
                uint64_t aIRep,
                uint64_t aJRep,
                const std::string &aVolName
                )
        {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);

            mHitIo.IdVect.push_back(aHitId);
            mHitIo.XPosVect.push_back(aX);
            mHitIo.YPosVect.push_back(aY);
            mHitIo.ZPosVect.push_back(aZ);
            mHitIo.ELossVect.push_back(aELoss);
            mHitIo.TrackIdVect.push_back(aTrackId);
            mHitIo.GenParentTrackIdVect.push_back(ParentId);
            mHitIo.IRepVect.push_back(aIRep);
            mHitIo.JRepVect.push_back(aJRep);
            mHitIo.VolumeNameVect.push_back(aVolName);
            mHitIo.HitsCount = mHitIo.XPosVect.size();
        }

        void AddTrack(
                uint64_t aTrackId,
                uint64_t aParentId,
                uint64_t aTrackPdg,
                int64_t creatorProc,
                double aXVertex,
                double aYVertex,
                double aZVertex,
                double aXMom,
                double aYMom,
                double aZMom,
                double aMom
        )
        {
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            if(trk_index_by_id.count(aTrackId)) {
                return;     // We already saved the track with this id. Nothing to do
            }

            mTrackIo.IdVect.push_back(aTrackId);
            mTrackIo.ParentId.push_back(aParentId);
            mTrackIo.PdgVect.push_back(aTrackPdg);
            mTrackIo.CreateProc.push_back(creatorProc);
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
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
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
            std::lock_guard<std::recursive_mutex> lk(io_mutex);
            mEventId = eventId;

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

        std::map<uint64_t, uint64_t> trk_index_by_id; // Track vector indexes by track id

        g4e::HitIo mHitIo;
        g4e::TrackIo mTrackIo;
        g4e::VertexIo mPrimeVertexIo;
        g4e::ParticleIo mParticleIo;
    };
}

#endif //G4E_ROOTFLATIO_HH
