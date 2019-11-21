#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include "catch.hh"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

struct TestHit {
    double x;
    double y;
};

class TestRootIo {
public:
    void Initialize(TFile *file)
    {
        //--- Vector Branches -----
        mRootFile = file;
        mEventTree = new TTree("events", "a Tree with vect");
        mEventTree->SetDirectory(file);
        auto b = mEventTree->Branch("event_id", &mEventId, "event_id/l");
       // b->BackFill()
        mEventTree->Branch("hit", &hits, 64000, 101);
    }

    void ClearForNewEvent()
    {
        trk_index_by_id.clear();
        hits.clear();
    }

    void FillEvent(uint64_t eventId)
    {
        mEventId = eventId;
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

    std::vector<TestHit> hits;

private:

    TFile *mRootFile;
    TTree *mEventTree;
    uint64_t mEventId;


    std::map<uint64_t, uint64_t> trk_index_by_id; // Track vector indexes by track id
};

TEST_CASE( "Factorials are computed", "[factorial]" ) {
REQUIRE( Factorial(1) == 1 );
REQUIRE( Factorial(2) == 2 );
REQUIRE( Factorial(3) == 6 );
REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE("Root write", "[root_io]") {

    auto file = new TFile("test.root", "RECREATE");
    TestRootIo test_io;
    test_io.Initialize(file);

    for(size_t i=0; i<10; i++)
    {
        test_io.ClearForNewEvent();
        test_io.hits.push_back({gRandom->Gaus(0,.02), gRandom->Gaus(0,.02)});
        test_io.hits.push_back({gRandom->Gaus(0,.02), gRandom->Gaus(0,.02)});
        test_io.FillEvent(i);
    }
}