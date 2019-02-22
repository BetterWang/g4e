#include "ConstField.h"
#include "Exception.h"
#include "FieldManager.h"
#include "KalmanFitterRefTrack.h"
#include "DAF.h"
#include "StateOnPlane.h"
#include "Track.h"
#include "TrackPoint.h"

#include <MaterialEffects.h>
#include <RKTrackRep.h>
#include <TGeoMaterialInterface.h>

#include <EventDisplay.h>

#include <HelixTrackModel.h>
#include <MeasurementCreator.h>
#include <WireMeasurement.h>
#include "PlanarMeasurement.h"

#include <TDatabasePDG.h>
#include <TEveManager.h>
#include <TGeoManager.h>
#include <TRandom.h>
#include <TVector3.h>
#include <vector>

#include "TDatabasePDG.h"
#include <TMath.h>
#include "CLAS12HitsEvent.h"
#include "RCGeometry.h"
#include "RHGeometry.h"
#include "RecoilChamberParticleHit.h"
#include "SiVertexTrackerGeometry.h"


void kalman_test1(int runnumber=11, int eventnumber = 22) {

   using namespace clas12::hits;
   using namespace clas12::sim;
   using namespace clas12::geo;
   using namespace eic::geo;

   RunConfiguration  * run_config  = nullptr;
   CLAS12HitsEvent   * event       = nullptr;
   RecoilScintEvent  * scint_event = nullptr;
   RecoilScintEvent  * pm1_event  = nullptr;
   RecoilScintEvent  * pm2_event  = nullptr;
   RecoilScintEvent  * pm3_event  = nullptr;
   ThrownEvent       * thrown      = nullptr;
   SiVertexTrackerGeometry  svt_geometry;

   std::string fname = std::string("data/rootfiles/eic_test")  
      + std::to_string(runnumber) 
      + std::string(".root");

   TFile * f = new TFile(fname.c_str(),"READ");
   f->cd();

   run_config = (RunConfiguration*)f->Get("Run_Config");
   run_config->Print();

   TTree * t = (TTree*)f->Get("clasdigi_hits");
   if(!t) {
      std::cout << "tree not found\n";  return;
   }
   t->SetBranchAddress("HitsEvent",&event);
   t->SetBranchAddress("ScintTile_particleHits",&scint_event);
   t->SetBranchAddress("PrimaryEvent",&thrown);
   t->SetBranchAddress("PM3_SiPM_Hits",&pm3_event);

   //------------------------------------------------

   new TGeoManager("Geometry", "Geane geometry");
   TGeoManager::Import("geometry/eic_svt.gdml");

   genfit::FieldManager::getInstance()->init(new genfit::ConstField(0.,0., -50.)); // 15 kGauss
   genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());

   genfit::EventDisplay* display = genfit::EventDisplay::getInstance();
   display->setOptions("X");

   TGeoNode*       node1 = gGeoManager->GetTopNode();
   TEveGeoTopNode* its   = new TEveGeoTopNode(gGeoManager, node1);
   //gEve->AddGlobalElement(its);

   //------------------------------------------------

   //genfit::AbsKalmanFitter* fitter = new genfit::KalmanFitterRefTrack();
   genfit::AbsKalmanFitter* fitter = new genfit::DAF();

  //// init the factory
  //int myDetId(0);
  //genfit::MeasurementFactory<genfit::AbsMeasurement> factory;
  //genfit::MeasurementProducer<genfit::mySpacepointDetectorHit, genfit::WireMeasurement> myProducer(&myDetectorHitArray);
  //factory.addProducer(myDetId, &myProducer);

   //------------------------------------------------

   int nevents = t->GetEntries();
   //for(int ievent = 0; ievent < 10; ievent++) {
   ievent = eventnumber;
   t->GetEntry(ievent);

   TVector3 thrown_momentum = {0,0,1};
   TParticle * part =  thrown->GetParticle(0);
   if(part) {
      TVector3 dir  = {part->Px(), part->Py(), part->Pz()};
      thrown_momentum = dir;
      dir.SetMag(2.0);
      TVector3 vert = {part->Vx(), part->Vy(),part->Vz()};
      vert.Print();
      TEveArrow* a1 = new TEveArrow(
            dir.X()  , dir.Y()  , dir.Z(), 
            part->Vx(), part->Vy(), part->Vz());
      a1->SetMainColor(kGreen);
      a1->SetTubeR(0.01);
      a1->SetPickable(kTRUE);
      gEve->AddElement(a1);
   }

   //------------------------------------------------

   // helix track model
   const int pdg = 11;//1000020040;//2212;               // particle pdg code
   //const double charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge()/(3.);
   //genfit::HelixTrackModel* helix = new genfit::HelixTrackModel(pos, mom, charge);

   ParticleHit * first_hit = event->fSVTEvent.GetParticleHit(0);
   TVector3 mom_init = first_hit->fGlobalPosition.Vect();
   mom_init.SetMag(1.0);
   TVector3 pos_init(0, 0, 0);
   mom_init.Print();
   std::cout << "derP" << std::endl;

   // approximate covariance
   TMatrixDSym covM(6);
   double resolution = 0.01;
   for (int i = 0; i < 3; ++i)
      covM(i,i) = resolution*resolution;
   for (int i = 3; i < 6; ++i)
      covM(i,i) = pow(resolution/9.0/sqrt(3.0), 2);

   // trackrep
   genfit::AbsTrackRep* rep = new genfit::RKTrackRep(pdg);

   // smeared start state
   genfit::MeasuredStateOnPlane stateSmeared(rep);
   stateSmeared.setPosMomCov(pos_init, mom_init, covM);

   // create track
   TVectorD    seedState(6);
   TMatrixDSym seedCov(6);
   stateSmeared.get6DStateCov(seedState, seedCov);

   genfit::Track fitTrack(rep, seedState, seedCov);


   // --------------------------------------------
   // add wire for beam at target

   TVector3 targ1  = {0,0,-15.0};
   TVector3 targ2  = {0,0,15.0};

   TVectorD hitCoordsTarget(7);
   hitCoordsTarget(0) = targ1.X();//(point-wirePerp-currentWireDir).X();
   hitCoordsTarget(1) = targ1.Y();//(point-wirePerp-currentWireDir).Y();
   hitCoordsTarget(2) = targ1.Z();//(point-wirePerp-currentWireDir).Z();

   hitCoordsTarget(3) = targ2.X();//(point-wirePerp+currentWireDir).X();
   hitCoordsTarget(4) = targ2.Y();//(point-wirePerp+currentWireDir).Y();
   hitCoordsTarget(5) = targ2.Z();//(point-wirePerp+currentWireDir).Z();

   TMatrixDSym hitCovTarget(7);
   hitCovTarget(6,6) = 0.01*0.01;
   hitCoordsTarget(6) = 0.0;

   genfit::AbsMeasurement* target_wire_meas = new genfit::WireMeasurement(hitCoordsTarget, hitCovTarget, 0, 0,nullptr);
   fitTrack.insertPoint(new genfit::TrackPoint(target_wire_meas, &fitTrack));

   // --------------------------------------------
   // Add pixel hits
   int n_scint_hits = 0;
   std::map<int,int>  channel_hits;
   channel_hits.clear();
   int nparticle_hits = event->fSVTEvent.fNParticleHits;
   for(int i_hit=0; i_hit<nparticle_hits; i_hit++  ) {

      //RecoilChamberParticleHit * a_hit = event->fRCEvent.GetParticleHit(i_hit);
      ParticleHit * a_hit = event->fSVTEvent.GetParticleHit(i_hit);
      int chan = a_hit->fChannel;

      //if( channel_hits.count(chan) != 0 ) continue;
      //channel_hits[chan] = 1;

      TVector3 point     = clas12::geo::Convert(svt_geometry.GetChannelPosition(chan)/CLHEP::cm);
      TVector3 zdir      = {0,0,1};
      TVector3 planeNorm = clas12::geo::Convert(svt_geometry.GetChannelNorm(chan)/CLHEP::cm);

      std::cout << " hit " << i_hit << std::endl;
      point.Print();
      a_hit->fGlobalPosition.Vect().Print();
      point = a_hit->fGlobalPosition.Vect();
      //planeNorm.Print();

      genfit::SharedPlanePtr plane(new genfit::DetPlane(point, zdir.Cross(planeNorm) , zdir) );
      //double r_chan       = svt_geometry.GetChannelRadius(chan);
      double resolution_z = 0.050*CLHEP::mm;//svt_geometry.fDeltaZ/2.0/CLHEP::cm;
      double resolution_x = 0.050*CLHEP::mm;//r_chan*TMath::Tan(rh_geometry.fDeltaPhi/2.0/CLHEP::cm);

      TVectorD hitCoordsScint(2);
      hitCoordsScint(0) = 0.0;
      hitCoordsScint(1) = 0.0;

      TMatrixDSym hitCovScint(2);
      hitCovScint(0,0) = resolution_z*resolution_z;
      hitCovScint(1,1) = resolution_x*resolution_x;

      genfit::AbsMeasurement* scint_measurement = new genfit::PlanarMeasurement(hitCoordsScint, hitCovScint, 1, n_scint_hits, nullptr);
      static_cast<genfit::PlanarMeasurement*>(scint_measurement)->setPlane(plane, n_scint_hits);

      fitTrack.insertPoint(new genfit::TrackPoint(scint_measurement, &fitTrack));

      n_scint_hits++;
      //break;
   }
   svt_geometry.Print();

   //check
   assert(fitTrack.checkConsistency());

   // do the fit
   fitter->processTrack(&fitTrack);

   //check
   assert(fitTrack.checkConsistency());


   if (ievent < 1000) {
      // add track to event display
   }

   if(fitTrack.getFitStatus()->isFitConverged()){
      TVector3 vertexPos, vertexMom;
      TMatrixDSym vertexCov;
      genfit::MeasuredStateOnPlane state = fitTrack.getFittedState(); // copy
      TVector3 vertex(0,0,0);
      state.extrapolateToPoint(vertex);
      //// or alternatively
      //TVector3 axis(0,0,1);
      //state.extrapolateToLine(vertex, axis);
      state.getPosMomCov(vertexPos, vertexMom, vertexCov);
      std::cout << "result: \n";
      std::cout << "vertex   : "; vertexPos.Print();
      std::cout << "momentum : "; vertexMom.Print();
      std::cout << "thrown   : "; thrown_momentum.Print();
      std::cout << "P0-P     : "; (thrown_momentum-vertexMom).Print();
      std::cout << "|P0|-|P| : " << (thrown_momentum.Mag()- vertexMom.Mag()) << std::endl;

   }
   display->addEvent(&fitTrack);



   //}// end loop over events

   //delete fitter;

   // open event display
   display->open();

}


