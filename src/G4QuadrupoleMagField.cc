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
static int SFdebug=0;

G4QuadrupoleMagField::G4QuadrupoleMagField(G4double pGradient)
{
   fGradient = pGradient ;
   fOrigin   = G4ThreeVector( 0.0, 0.0, 0.0) ;
   fpMatrix  = &IdentityMatrix;

   if (SFdebug>0) {
     printf("SetQMagField:: pMatrix X: %f %f %f \n",fpMatrix->xx(),fpMatrix->xy(),fpMatrix->xz());
     printf("SetQMagField:: pMatrix Y: %f %f %f \n",fpMatrix->yx(),fpMatrix->yy(),fpMatrix->yz());
     printf("SetQMagField:: pMatrix Z: %f %f %f \n",fpMatrix->zx(),fpMatrix->zy(),fpMatrix->zz());
   }
}


/////////////////////////////////////////////////////////////////////////

G4QuadrupoleMagField::G4QuadrupoleMagField(G4double pGradient,
                                           G4ThreeVector pOrigin,
                                           G4RotationMatrix* pMatrix)
{
   fGradient    = pGradient ;
   fOrigin      = pOrigin ;
   fpMatrix     = pMatrix ;


   if (SFdebug>0) {
     printf("SetQMagField:: pMatrix X: %f %f %f \n",pMatrix->xx(),pMatrix->xy(),pMatrix->xz());
     printf("SetQMagField:: pMatrix Y: %f %f %f \n",pMatrix->yx(),pMatrix->yy(),pMatrix->yz());
     printf("SetQMagField:: pMatrix Z: %f %f %f \n",pMatrix->zx(),pMatrix->zy(),pMatrix->zz());
   }
}

G4Field* G4QuadrupoleMagField::Clone() const
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

void G4QuadrupoleMagField::GetFieldValue( const G4double y[7],
                                                G4double B[3]  ) const  
{
   G4ThreeVector r_global = G4ThreeVector(
        y[0] - fOrigin.x(), 
        y[1] - fOrigin.y(), 
        y[2] - fOrigin.z());

   if (SFdebug>2) {
     printf("SetQMagField::GetFieldValue: y(orig)= %f(%f) %f(%f) %f(%f) \n",y[0],fOrigin.x() ,y[1],fOrigin.y() ,y[2],fOrigin.z()   );
     printf("SetQMagField:: fpMatrix X: %f %f %f \n",fpMatrix->xx(),fpMatrix->xy(),fpMatrix->xz());
     printf("SetQMagField:: fpMatrix Y: %f %f %f \n",fpMatrix->yx(),fpMatrix->yy(),fpMatrix->yz());
     printf("SetQMagField:: fpMatrix Z: %f %f %f \n",fpMatrix->zx(),fpMatrix->zy(),fpMatrix->zz());
   }
   G4ThreeVector r_local = G4ThreeVector(
           fpMatrix->rowX() * r_global,
           fpMatrix->rowY() * r_global,
           fpMatrix->rowZ() * r_global);

   G4ThreeVector B_local = G4ThreeVector(
           fGradient * r_local.y(),
           fGradient * r_local.x(),
           0 );

   if (SFdebug>2) 
     printf("SetQMagField::GetFieldValue:B_local Gradient=%f B= %f %f %f \n",fGradient/(CLHEP::tesla/CLHEP::m), B_local.x()/(CLHEP::tesla/CLHEP::m), B_local.y()/(CLHEP::tesla/CLHEP::m) , B_local.z()/(CLHEP::tesla/CLHEP::m)  );

   G4ThreeVector B_global = G4ThreeVector(
           fpMatrix->inverse().rowX() * B_local,
           fpMatrix->inverse().rowY() * B_local,
           fpMatrix->inverse().rowZ() * B_local);

   G4ThreeVector B_global2 = fpMatrix->inverse() * B_local;
   
   if (SFdebug>2) {
     printf("SetQMagField:: fpMatrix inv X: %f %f %f \n",fpMatrix->inverse().xx(),fpMatrix->inverse().xy(),fpMatrix->inverse().xz());
     printf("SetQMagField:: fpMatrix inv Y: %f %f %f \n",fpMatrix->inverse().yx(),fpMatrix->inverse().yy(),fpMatrix->inverse().yz());
     printf("SetQMagField:: fpMatrix inv Z: %f %f %f \n",fpMatrix->inverse().zx(),fpMatrix->inverse().zy(),fpMatrix->inverse().zz());
   }   

   if (SFdebug>1) {
     printf("-->SetQMagField::GetFieldValue: B_local = %f %f %f \n", B_local.x(), B_local.y(), B_local.z() );
     printf("++>SetQMagField::GetFieldValue: B_global  = %f %f %f \n", B_global.x(), B_global.y(), B_global.z() );
     printf("++>SetQMagField::GetFieldValue: B_global2 = %f %f %f \n", B_global2.x(), B_global2.y(), B_global2.z() );
   }

   B[0] = B_global.x() ;
   B[1] = B_global.y() ;
   B[2] = B_global.z() ;


}
