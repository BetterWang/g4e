#include "ConeGenerator.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

#include <iostream>
#include <fstream>



void ConeGenerator::Initialize() {

}




G4bool ConeGenerator::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
    G4Navigator* navigator= G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

    G4VPhysicalVolume* world = navigator->GetWorldVolume();
    G4VSolid* solid= world->GetLogicalVolume()->GetSolid();
    EInside qinside= solid->Inside(pos);

    if(qinside != kInside) return false;
    else return true;
}



void ConeGenerator::GeneratePrimaryVertex(G4Event* anEvent) {

    fParticlesPerEvent;
    for (int kp = 0; kp < fParticlesPerEvent; kp++) {
            auto particleDef = fParticleTable->GetParticle(pdg);

            //--- recalculate etot if PROTON !!!
            double proton_mass = 0.938272;
            //JF
            etot = sqrt(ptot * ptot + proton_mass * proton_mass);
            //  etot=sqrt(DMptot*DMptot+proton_mass*proton_mass);

            /*
                    Phi += dPhi;
                    double rpt = ptot * sin(Theta);
                    double rpx = rpt * cos(Phi);
                    double rpy = rpt * sin(Phi);
            */
            double rpz = dir.z();
            double rpx = dir.x();
            double rpy = dir.y();

            std::cout << "G4LorentzVector px= " << rpx << " py= " << rpy << " pz= " << rpz << std::endl;
            /*
            double rpt = sqrt(px*px + py*py);
            //double rphi = (0.5-G4UniformRand())*2. * 3.1415 / 2.;
            double rphi = (0.5-G4UniformRand())*2. * dCone;

            double rpt = sqrt(px*px + py*py);
            //double rphi = (0.5-G4UniformRand())*2. * 3.1415 / 2.;
            double rphi = (0.5-G4UniformRand())*2. * dCone;

            double rpx=rpt*cos(rphi);
            double rpy=rpt*sin(rphi);
            //double rpx  = (0.5-G4UniformRand())*2.*0.05 * px*GeV;
            //double rpy  = (0.5-G4UniformRand())*2.*0.1 * py*GeV;
            */

            //G4LorentzVector p(px*GeV+rpx,py*GeV+rpy,pz*GeV+rpz,etot*GeV);

            G4LorentzVector p(rpx * GeV, rpy * GeV, rpz * GeV, etot * GeV);
            double dVx = G4RandGauss::shoot(0,Vx);
            double dVy = G4RandGauss::shoot(0,Vy);
            double dVz = G4RandGauss::shoot(0,Vz);
            G4LorentzVector v(dVx * mm, dVy * mm, dVz * mm, 0);
            std::cout << "G4LorentzVector p= " << p << std::endl;

            ptrk.P = p;
            ptrk.V = v;
            pyEvt.push_back(ptrk);

            N++;

    }

    auto zeroVertex = new G4PrimaryVertex(0,0,0,0);
    bool useZeroVertex = Vx == 0 && Vy == 0 && Vz == 0;





    int Np = py->pyEvt.size(); // =py->N;

    for(int ip=0; ip<Np; ip++) {  // loop for particles ...

        // Create primary vertex
        G4PrimaryVertex* primaryVertex;
        if(useZeroVertex) {
            primaryVertex = zeroVertex;
        } else {
            primaryVertex = new G4PrimaryVertex(
                    G4RandGauss::shoot(0,Vx),
                    G4RandGauss::shoot(0,Vy),
                    G4RandGauss::shoot(0,Vz),
                    0);

            if (! CheckVertexInsideWorld(primaryVertex->Ve)) { printf("PyMC2G4: vtx outside world ip=%d\n",ip); continue;}
        }

        // Smear particle energy


        double deltaTheta = G4RandGauss::shoot(0, dCone);
        double conePhi = G4UniformRand() * 2. * pi; // --- flat in phi
        G4ThreeVector dir(std::sin(deltaTheta) * std::cos(conePhi), std::sin(deltaTheta) * std::sin(conePhi), std::cos(deltaTheta));
        dir.rotateY(Theta);
        dir.rotateZ(Phi);

        ptot = G4RandGauss::shoot(ptot, dmyMom);
        dir.setMag(ptot);   //- new ptot Gauss !!!










        //G4int pdgcode= py->K[2][ip];
        G4int pdgcode= py->pyEvt.at(ip).K[2];
        //G4LorentzVector p(py->P[0][ip],py->P[1][ip],py->P[2][ip],py->P[3][ip]);
        G4LorentzVector p(py->pyEvt.at(ip).P);

        G4PrimaryParticle* g4prim = new G4PrimaryParticle(pdgcode, p.x(), p.y(), p.z());

        printf("PyMC2G4:: PDG= %d vtx=(%f,%f,%f) mom=(%f,%f,%f) \n",pdgcode, xvtx.x()/mm, xvtx.y()/mm, xvtx.z()/mm, p.x()/GeV, p.y()/GeV, p.z()/GeV);

        primaryVertex-> SetPrimary(g4prim);
        g4event-> AddPrimaryVertex(primaryVertex);
    }
}