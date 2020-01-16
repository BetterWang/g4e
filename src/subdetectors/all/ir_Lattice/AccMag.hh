
#ifndef G4E_IR_LATTICE_HH
#define G4E_IR_LATTICE_HH

#include <sstream>
#include <string>
#include <G4Material.hh>
#include <G4FieldManager.hh>

class QMag {

public:

  std::string name, type;
  double LengthZ, Rin, Rout, Dout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew;
  double SextupoleField, SolenoidField, Xcenter, Ycenter, Zcenter, MagTheta, MagPhi;

  QMag( std::string name, std::string type,double  LengthZ,double Rin, double Rout, double Dout, double DipoleFieldBx, double DipoleFieldBy, double QuadrupolFieldQnorm, double QuadrupolFieldQskew,
	double SextupoleField, double SolenoidField, double Xcenter, double Ycenter, double Zcenter, double MagTheta, double MagPhi, G4VPhysicalVolume *fMotherPhysVolume)
  {

    if (type == "SBEND" || type == "RBEND") {

      printf("QMag:: found  %s name=%s \n", type.c_str(), name.c_str());
      CreateQuad(fMotherPhysVolume);
    }

  }
  ~QMag() {
    //-- delete all new obj
  }
  
  
  //==============================================================================================================
  //                          Quadrupole
  //==============================================================================================================
  
  void  CreateQuad (G4VPhysicalVolume *fMotherPhysVolume) {
    
    printf("CreateQuad:: \n");
    //return;
    G4VisAttributes *vb1;
    G4RotationMatrix  *mybrm;
    mybrm = new G4RotationMatrix();
    //---------------- BEAM QUADS -------------------------------------------
    G4Tubs *fSolid_QUADS_hd_v, *fSolid_QUADS_hd_ir, *fSolid_QUADS_hd_m;
    G4LogicalVolume *fLogic_QUADS_hd_v, *fLogic_QUADS_hd_ir, *fLogic_QUADS_hd_m;
    G4PVPlacement *fPhysics_QUADS_hd_v, *fPhysics_QUADS_hd_ir, *fPhysics_QUADS_hd_m;
    G4FieldManager *fieldMgr_QUADS_hd;


    vb1 = new G4VisAttributes(G4Color(0.8, 0.3, 0.1, 0.9));
    vb1->SetForceSolid(true);

    printf("CreateQuad:: theta =%f rad=%f  deg=%f \n", MagTheta, MagTheta / rad, MagTheta / deg);
    mybrm->rotateY(MagTheta * rad);
    // brm_hd.rotateY((0*180/3.1415)*deg);

    G4Material *Material_Ir = G4Material::GetMaterial("IronAll");
    G4Material *Material_G = G4Material::GetMaterial("G4_Galactic");
    char abname[256];
    //--------------------Volumes ---------
    sprintf(abname, "Solid_QUADS_hd_v_%s", name.c_str());
    fSolid_QUADS_hd_v= new G4Tubs(abname, 0., (Dout + 0.01) * cm, (LengthZ / 2.) * m, 0., 360 * deg);
    sprintf(abname, "Logic_QUADS_hd_v_%s", name.c_str());
    fLogic_QUADS_hd_v = new G4LogicalVolume(fSolid_QUADS_hd_v, Material_G, abname);
    //fLogic_QUADS_hd_v[j] = new G4LogicalVolume(fSolid_QUADS_hd_v[j], World_Material, abname,fieldMgr_QUADS_hd[j]);
    sprintf(abname, "Physics_QUADS_hd_v_%s", name.c_str());
    fPhysics_QUADS_hd_v = new G4PVPlacement(G4Transform3D(mybrm, G4ThreeVector(Xcenter * m, Ycenter * m, Zcenter * m)), abname, fLogic_QUADS_hd_v, fMotherPhysVolume, false, 0);

    fLogic_QUADS_hd_v->SetFieldManager(fieldMgr_QUADS_hd, true);
    //printf("create %s ");

    //--------------------Iron---------
    sprintf(abname, "Solid_QUADS_hd_ir_%s", name.c_str());
    fSolid_QUADS_hd_ir = new G4Tubs(abname, Rin * cm, (Dout + 0.005) * cm, (LengthZ / 2.) * m, 0., 360 * deg);
    sprintf(abname, "Logic_QUADS_hd_ir_%s", name.c_str());
    fLogic_QUADS_hd_ir = new G4LogicalVolume(fSolid_QUADS_hd_ir, Material_Ir, abname);
    sprintf(abname, "Physics_QUADS_hd_ir_%s", name.c_str());
    fPhysics_QUADS_hd_ir = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_QUADS_hd_ir, fPhysics_QUADS_hd_v, false, 0);
    fLogic_QUADS_hd_ir->SetVisAttributes(vb1);


    //----------------  magnetic field volume---------------
    sprintf(abname, "Solid_QUADS_hd_m_%s", name.c_str());
    fSolid_QUADS_hd_m = new G4Tubs(abname, 0. * cm, Rin * cm, (LengthZ / 2.) * m, 0., 360 * deg);
    sprintf(abname, "Logic_QUADS_hd_m_%s", name.c_str());
    fLogic_QUADS_hd_m = new G4LogicalVolume(fSolid_QUADS_hd_m, Material_G, abname, fieldMgr_QUADS_hd);
    sprintf(abname, "Physics_QUADS_hd_m_%s", name.c_str());
    fPhysics_QUADS_hd_m = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_QUADS_hd_m, fPhysics_QUADS_hd_v, false, 0);

    //---------------- create  magnetic field ---------------

    printf("CreateQuad:: name=%s  FIELD = Dx %f  Dy %f --  Qn %f Qs %f -- Sek %f  Sol %f \n", name.c_str(),  DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew, SextupoleField, SolenoidField);
    fieldMgr_QUADS_hd = SetQMagField(QuadrupolFieldQnorm, QuadrupolFieldQskew, MagTheta, G4ThreeVector(Xcenter * m, Ycenter * m, Zcenter * m));   // gradient tesla/m;

    //    G4FieldManager* fieldMgr = SetQMagField(qFIELDx[j],qFIELDy[j]);   // gradient tesla/m;
    // fLogic_QUADSm[j]->SetFieldManager(fieldMgr,true);

    fLogic_QUADS_hd_m->SetFieldManager(fieldMgr_QUADS_hd, true);

  }


  G4FieldManager *SetQMagField(float field, float skew, float theta, G4ThreeVector fieldorigin)
  {
    G4RotationMatrix *qrm_f;
    printf("SetQMagField:: got parameters %f %f \n", field, skew);

    //G4double fGradient = -223.*tesla/m;
    G4double fGradient = sqrt(field * field + skew * skew) * tesla / m;
    fGradient = copysign(fGradient, field);
    G4double angle = atan2(skew, fabs(field)); //--   atan2(y,x);
    printf("SetQMagField():: angle=%f(%f) theta=%f(%f) rad(deg) \n", angle, angle / deg, theta, theta / deg);

    qrm_f = new G4RotationMatrix();
    qrm_f->rotateZ(angle * rad);
    qrm_f->rotateY(-theta * rad);
    //--------------------------

    //G4ThreeVector fieldValue = G4ThreeVector( fGradient_x,fGradient_y,fGradient_z);
    // old  G4QuadrupoleMagField* pipeField = new G4QuadrupoleMagField(fGradient);

    // G4ThreeVector fieldorigin = G4ThreeVector(0,0,0);

    G4QuadrupoleMagField *pipeField = new G4QuadrupoleMagField(fGradient, fieldorigin, qrm_f);


#if 1

    G4Mag_UsualEqRhs *myEquation = new G4Mag_UsualEqRhs(pipeField);
    G4MagIntegratorStepper *myStepper = new G4ClassicalRK4(myEquation);
    G4ChordFinder *myChordFinder = new G4ChordFinder(pipeField, 0.1 * mm, myStepper);

    G4FieldManager *fieldMgr = new G4FieldManager(pipeField, myChordFinder, false);

#else
    G4FieldManager *fieldMgr = new G4FieldManager(pipeField);
    fieldMgr->SetDetectorField(pipeField);
    fieldMgr->CreateChordFinder(pipeField);
    //G4double minEps = 1.0e-5; // Minimum & value for smallest steps
    //G4double maxEps = 1.0e-4; // Maximum & value for largest steps
    //fieldMgr->SetMinimumEpsilonStep(minEps);
    //fieldMgr->SetMaximumEpsilonStep(maxEps);
    //fieldMgr->SetDeltaOneStep(0.5 * um); // 0.5 micrometer
#endif

    //logicSpace_Pipe->SetFieldManager(fieldMgr,true);

    /*
      G4Box *solidB = new G4Bo"solidB", 1*m, 1*m, 1*m);
      G4LogicalVolume* logicB = new G4LogicalVolume(solidB, Mat("G4_Galactic"), "logicB",fieldMgrq);
      new G4PVPlacement(0, G4ThreeVector(5*m,5*m,-7*m), logicB,
      "physiB", world->getLogic(), false, 0);
    */
    //delete qrm_f;
    return fieldMgr;
  }


};
//==============================================================================================
//==  
//==============================================================================================

class AcceleratorMagnets {

public:
  std::vector<QMag *> allmagnets;
 G4VPhysicalVolume *fMotherPhysVolume;

    void SetMotherParams(G4VPhysicalVolume *physicalVolume, G4Material *material) {
       // fMaterial = material;
        fMotherPhysVolume=physicalVolume;
    }

  AcceleratorMagnets(std::string fname)  {

    ifstream sourcefile;                    // build a read-Stream
    sourcefile.open(fname, ios_base::in);  // open data

    if (!sourcefile) {                     // if it does not work
      cerr << "Can't open File with Lattice!\n";
    } else {    
      printf("AcceleratorMagnets file opened %s \n",fname.c_str());
      for (std::string line; std::getline(sourcefile, line);) {

	std::istringstream in(line);      //make a stream for the line itself
	std::cout << "AcceleratorMagnets:: read line = " << line << std::endl;
	if (line[0] != 'e' && line[0] != 'i') { printf(" skip line %s\n",line.c_str()); continue; }
	parce_line(line); //-- string--
      }
    }
  }
  //-----------------------------------------------------------
  void parce_line(std::string line)
  {

    string name, type;
    double LengthZ, Rin, Rout, Dout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew;
    double SextupoleField, SolenoidField, Xcenter, Ycenter, Zcenter, MagTheta, MagPhi;


    std::istringstream in(line);
    in >> name >> type >> LengthZ >> Rin >> Rout >> Dout >> DipoleFieldBx >> DipoleFieldBy >> QuadrupolFieldQnorm >> QuadrupolFieldQskew >> SextupoleField >> SolenoidField
       >> Xcenter >> Ycenter >> Zcenter >> MagTheta >> MagPhi;

    printf("AcceleratorMagnets::parce_line: Rin=%f, Rout=%f \n",Rin, Rout );

    QMag *qmag = new QMag(name, type, LengthZ, Rin, Rout, Dout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew, SextupoleField, SolenoidField, Xcenter,
			  Ycenter, Zcenter, MagTheta, MagPhi, fMotherPhysVolume);
    allmagnets.push_back(qmag);

  }

};

///---- usage in Constructor ..

#endif //G4E_IR_LATTICE_HH


