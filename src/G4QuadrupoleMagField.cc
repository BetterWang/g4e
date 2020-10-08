//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4QuadrupoleMagField.cc 96751 2016-05-04 09:39:38Z gcosmo $
//
// -------------------------------------------------------------------

#include "G4QuadrupoleMagField.hh"
#include "G4RotationMatrix.hh"

static G4RotationMatrix IdentityMatrix;
static int JFdebug = 0;

G4QuadrupoleMagField::G4QuadrupoleMagField(G4double pGradient)
{
    fGradient = pGradient;
    fOrigin = G4ThreeVector(0.0, 0.0, 0.0);
    fpMatrix = &IdentityMatrix;

    if (JFdebug > 0) {
        printf("SetQMagField:: pMatrix X: %f %f %f \n", fpMatrix->xx(), fpMatrix->xy(), fpMatrix->xz());
        printf("SetQMagField:: pMatrix Y: %f %f %f \n", fpMatrix->yx(), fpMatrix->yy(), fpMatrix->yz());
        printf("SetQMagField:: pMatrix Z: %f %f %f \n", fpMatrix->zx(), fpMatrix->zy(), fpMatrix->zz());
    }
}


/////////////////////////////////////////////////////////////////////////

G4QuadrupoleMagField::G4QuadrupoleMagField(G4double pGradient, G4ThreeVector pOrigin, G4RotationMatrix *pMatrix)
{
    fGradient = pGradient;
    fOrigin = pOrigin;
    fpMatrix = pMatrix;
    // fOrigin   = G4ThreeVector( 0.0, 0.0, 0.0) ;
    // fpMatrix  = &IdentityMatrix;

    if (JFdebug > 0) {
      double m = CLHEP::m;
      double tesla = CLHEP::tesla;

        printf("SetQMagField:: origin x=%f y=%f z=%f [m]=%f [tesla]=%f (CLHEP)\n",fOrigin.x()/m,fOrigin.y()/m,fOrigin.z()/m,m,tesla);
        printf("SetQMagField:: GetFieldValue:B_local Gradient=%f \n", fGradient / (CLHEP::tesla / CLHEP::m));
        printf("SetQMagField:: fpMatrix X: %f %f %f \n", fpMatrix->xx(), fpMatrix->xy(), fpMatrix->xz());
        printf("SetQMagField:: fpMatrix Y: %f %f %f \n", fpMatrix->yx(), fpMatrix->yy(), fpMatrix->yz());
        printf("SetQMagField:: fpMatrix Z: %f %f %f \n", fpMatrix->zx(), fpMatrix->zy(), fpMatrix->zz());

#if 0
        //JFdebug=0;
        printf("---------------- B_y MF table in tesla G4M::-----------------------------------------\n");
        printf("x= [m]       ");for (double x=-0.1 ; x<0.1; x+=0.01)  printf(" (%9.6f) ",x); printf("\n");
        G4double yy[7];
        G4double B[3];
        for (double y=-0.1 ; y<0.1; y+=0.01) {
          printf(" y=%9.6f ",y);
          for (double x=-0.1 ; x<0.1; x+=0.01) {
        yy[0]=x*CLHEP::m; yy[1]=y*CLHEP::m; yy[2]=0*CLHEP::m;
        GetFieldValue(yy,B);
        printf(" (%9.6f) ",B[1]/CLHEP::tesla);
          }
          printf("\n");
        }
        //JFdebug=3;
#endif
    }
}

G4Field *G4QuadrupoleMagField::Clone() const
{
    return new G4QuadrupoleMagField(fGradient, fOrigin, fpMatrix);
}

/////////////////////////////////////////////////////////////////////////

G4QuadrupoleMagField::~G4QuadrupoleMagField()
{
}

////////////////////////////////////////////////////////////////////////
//  Allow displaced origin and rotation 
//  Extensions by Björn Riese (GSI)

void G4QuadrupoleMagField::GetFieldValue(const G4double y[7], G4double B[3]) const
{
    G4ThreeVector r_global = G4ThreeVector(y[0] - fOrigin.x(), y[1] - fOrigin.y(), y[2] - fOrigin.z());

    double tpm = (CLHEP::tesla / CLHEP::m);
    double m = CLHEP::m;

    if (JFdebug > 2) {
        printf("SetQMagField::GetFieldValue:  y (orig) [m] = %f(%f) %f(%f) %f(%f) \n", y[0] / m, fOrigin.x() / m, y[1] / m, fOrigin.y() / m, y[2] / m, fOrigin.z() / m);
        printf("SetQMagField::GetFieldValue:  y-loc [m] = %f %f %f \n", (y[0] - fOrigin.x()) / m, (y[1] - fOrigin.y()) / m, (y[2] - fOrigin.z()) / m);
        //printf("SetQMagField:: fpMatrix X: %f %f %f \n",fpMatrix->xx(),fpMatrix->xy(),fpMatrix->xz());
        //printf("SetQMagField:: fpMatrix Y: %f %f %f \n",fpMatrix->yx(),fpMatrix->yy(),fpMatrix->yz());
        //printf("SetQMagField:: fpMatrix Z: %f %f %f \n",fpMatrix->zx(),fpMatrix->zy(),fpMatrix->zz());
    }
    //--- JF should this be swapped ?

    G4ThreeVector r_local = G4ThreeVector(fpMatrix->rowX() * r_global, fpMatrix->rowY() * r_global, fpMatrix->rowZ() * r_global);


    //  JF :: this is how in original Geant4
    G4ThreeVector r_local_0 = G4ThreeVector(fpMatrix->colX() * r_global, fpMatrix->colY() * r_global, fpMatrix->colZ() * r_global);


    if (JFdebug > 2) {
        printf("SetQMagField::GetFieldValue:  y (loc) [m] = %f(%f) %f(%f) %f(%f) \n", y[0] / m, r_local.x() / m, y[1] / m, r_local.y() / m, y[2] / m, r_local.z() / m);
        std::cout << "SetQMagField::GetFieldValue:  fpMatrix->colX()= " << fpMatrix->colX() << std::endl;
        std::cout << "SetQMagField::GetFieldValue:  fpMatrix->colY()= " << fpMatrix->colY() << std::endl;
        std::cout << "SetQMagField::GetFieldValue:  fpMatrix->colZ()= " << fpMatrix->colZ() << std::endl;

        std::cout << "SetQMagField::GetFieldValue:  fpMatrix->rowX()= " << fpMatrix->rowX() << std::endl;
        std::cout << "SetQMagField::GetFieldValue:  fpMatrix->rowY()= " << fpMatrix->rowY() << std::endl;
        std::cout << "SetQMagField::GetFieldValue:  fpMatrix->rowZ()= " << fpMatrix->rowZ() << std::endl;
    }


// JF  this is original

    G4ThreeVector B_local = G4ThreeVector(fGradient * r_local.y(), fGradient * r_local.x(), 0);

    G4ThreeVector B_local_0 = G4ThreeVector(fGradient * r_local_0.y(), fGradient * r_local_0.x(), 0);


    if (JFdebug > 2) {
        printf("SetQMagField::GetFieldValue:B_local Gradient=%f B= %f %f %f \n", fGradient / (CLHEP::tesla / CLHEP::m), B_local.x() / (CLHEP::tesla / CLHEP::m),
               B_local.y() / (CLHEP::tesla / CLHEP::m), B_local.z() / (CLHEP::tesla / CLHEP::m));
        printf("SetQMagField::GetFieldValue:B_local_0 Gradient=%f B= %f %f %f \n", fGradient / tpm, B_local_0.x() / tpm, B_local_0.y() / tpm, B_local_0.z() / tpm);
    }
    G4ThreeVector B_global = G4ThreeVector(fpMatrix->inverse().rowX() * B_local, fpMatrix->inverse().rowY() * B_local, fpMatrix->inverse().rowZ() * B_local);

    // JF --- invert field?

    G4ThreeVector B_global2 = fpMatrix->inverse() * B_local;

    if (JFdebug > 20) {
        printf("SetQMagField:: fpMatrix inv X: %f %f %f \n", fpMatrix->inverse().xx(), fpMatrix->inverse().xy(), fpMatrix->inverse().xz());
        printf("SetQMagField:: fpMatrix inv Y: %f %f %f \n", fpMatrix->inverse().yx(), fpMatrix->inverse().yy(), fpMatrix->inverse().yz());
        printf("SetQMagField:: fpMatrix inv Z: %f %f %f \n", fpMatrix->inverse().zx(), fpMatrix->inverse().zy(), fpMatrix->inverse().zz());
    }

    if (JFdebug > 1) {
        printf("-->SetQMagField::GetFieldValue: B_local = %f %f %f \n", B_local.x() / tpm, B_local.y() / tpm, B_local.z() / tpm);
        printf("++>SetQMagField::GetFieldValue: B_global  = %f %f %f \n", B_global.x() / tpm, B_global.y() / tpm, B_global.z() / tpm);
        printf("++>SetQMagField::GetFieldValue: B_global2 = %f %f %f \n", B_global2.x() / tpm, B_global2.y() / tpm, B_global2.z() / tpm);
    }
//---- JF this is how in original

    B[0] = B_global.x();
    B[1] = B_global.y();
    B[2] = B_global.z();
    if (JFdebug > 1) { printf("SetQMagField::  === >Exit  <=== \n"); }
/*
  B[0] = B_global2.x() ;
  B[1] = B_global2.y() ;
  B[2] = B_global2.z() ;
*/

}
