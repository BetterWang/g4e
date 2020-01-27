
#ifndef G4E_IR_LATTICE_HH
#define G4E_IR_LATTICE_HH

#include <sstream>
#include <string>
#include <G4Material.hh>
#include <G4FieldManager.hh>

class QMag {

public:

    std::string name, type;
    double LengthZ, Rin, Rin2, Rout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew;
    double SextupoleField, SolenoidField, Xcenter, Ycenter, Zcenter, MagTheta, MagPhi;
    G4RotationMatrix  mybrm;
    int USE_LINE;
    //---------------- BEAM DIPOLES -------------------------------------------
    G4Tubs *fSolid_BigDi_v, *fSolid_BigDi_ir, *fSolid_BigDi_m;
    //  G4Box *fSolid_BigDi_v[100],*fSolid_BigDi_ir[100],*fSolid_BigDi_m[100];
    G4LogicalVolume *fLogic_BigDi_v, *fLogic_BigDi_ir, *fLogic_BigDi_m;
    G4PVPlacement *fPhysics_BigDi_v, *fPhysics_BigDi_ir, *fPhysics_BigDi_m;
    G4FieldManager *fieldMgr_BigDi;

    QMag( std::string _name, std::string _type,double  _LengthZ,double _Rin, double _Rin2, double _Rout, double _DipoleFieldBx, double _DipoleFieldBy, double _QuadrupolFieldQnorm, double _QuadrupolFieldQskew,
          double _SextupoleField, double _SolenoidField, double _Xcenter, double _Ycenter, double _Zcenter, double _MagTheta, double _MagPhi, G4VPhysicalVolume *fMotherPhysVolume,int _USE_LINE)
    {
        using namespace spdlog;
        USE_LINE = _USE_LINE;

        Rin=_Rin; Rin2=_Rin2; Rout=_Rout;
        LengthZ=_LengthZ; DipoleFieldBx=_DipoleFieldBx; DipoleFieldBy=_DipoleFieldBy; QuadrupolFieldQnorm=_QuadrupolFieldQnorm; QuadrupolFieldQskew=_QuadrupolFieldQskew;
        SextupoleField=_SextupoleField; SolenoidField=_SolenoidField; Xcenter=_Xcenter; Ycenter=_Ycenter; Zcenter=_Zcenter; MagTheta=_MagTheta; MagPhi=_MagPhi;
        name=_name;
        type=_type;

        //  if (type == "SBEND" || type == "RBEND") {
        if (type == "QUADRUPOLE" ) {
            debug("Qmag:: found  {} name={} ", type.c_str(), name.c_str());
            CreateQuad(fMotherPhysVolume);
        }
        if (type == "SBEND" || type == "RBEND") {
            debug("QMag:: found  {} name={} ", type.c_str(), name.c_str());
            CreateDipole(fMotherPhysVolume);
        }

    }
    ~QMag() {
        //-- delete all new obj
    }


//==============================================================================================================
//                          DIPOLE
//==============================================================================================================

    void CreateDipole(G4VPhysicalVolume *fMotherPhysVolume) {
        using namespace spdlog;
        char abname[256];

        G4VisAttributes *vb1;

        /*----------Dipole --------------*/

        G4Material *Material_Ir = G4Material::GetMaterial("IronAll");
        G4Material *Material_G = G4Material::GetMaterial("G4_Galactic");

        mybrm.rotateY(MagTheta * rad);


        //-------------------------- Magnetic volume------------------------
        sprintf(abname, "Solid_DIPOLE_v_%s", name.c_str());
        fSolid_BigDi_v = new G4Tubs(abname, 0., (Rout + 0.1) * cm, (LengthZ / 2.+ 0.001) * m, 0., 360 * deg);

        // fSolid_BigDi_v[j] = new G4Box(abname, (double)(ffqsRoutDi+0.01)*cm, (double)(ffqsRoutDi+0.01)*cm,(double)(ffqsSizeZDi/2.)*m);
        sprintf(abname, "Logic_DIPOLE_v_%s", name.c_str());
        fLogic_BigDi_v = new G4LogicalVolume(fSolid_BigDi_v, Material_G, abname);
        sprintf(abname, "Physics_DIPOLE_v_%s", name.c_str());
        fPhysics_BigDi_v = new G4PVPlacement(G4Transform3D(mybrm, G4ThreeVector(Xcenter * m, Ycenter * m, Zcenter * m)), abname, fLogic_BigDi_v, fMotherPhysVolume, false, 0);
        trace(" Finish magnetic volume and start physics volume ");

        //-------------------------- Magnet iron------------------------
        sprintf(abname, "Solid_DIPOLE_i_%s", name.c_str());
        fSolid_BigDi_ir = new G4Tubs(abname, (Rin2+0.1) * cm, Rout * cm, (LengthZ / 2.) * m, 0., 360 * deg);
        // fSolid_BigDi_ir[j] = new G4Box(abname, (double) ffqsRoutDi*cm, (double)ffqsRoutDi*cm,((double)ffqsSizeZDi/2.)*m);
        sprintf(abname, "Logic_DIPOLE_i_%s", name.c_str());
        fLogic_BigDi_ir = new G4LogicalVolume(fSolid_BigDi_ir, Material_Ir, abname);
        sprintf(abname, "Physics_DIPOLE_i_%s", name.c_str());
        fPhysics_BigDi_ir = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_BigDi_ir, fPhysics_BigDi_v, false, 0);

        vb1 = new G4VisAttributes(G4Color(0.2, 0.8, 0.2, 1.));
        vb1->SetForceSolid(true);
        fLogic_BigDi_ir->SetVisAttributes(vb1);

        //-------------------------- Magnet field------------------------
        sprintf(abname, "Solid_DIPOLE_m_%s", name.c_str());
        fSolid_BigDi_m = new G4Tubs(abname, 0., Rin2 * cm, (LengthZ / 2.) * m, 0., 360 * deg);
        //  fSolid_BigDi_m[j] = new G4Box(abname,(double)ffqsRinDi*cm,(double)ffqsRinDi*cm,(double)(ffqsSizeZDi/2.)*m);
        sprintf(abname, "Logic_DIPOLE_m_%s", name.c_str());
        fLogic_BigDi_m = new G4LogicalVolume(fSolid_BigDi_m, Material_G, abname);
        sprintf(abname, "Physics_DIPOLE_m_%s", name.c_str());
        fPhysics_BigDi_m = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_BigDi_m, fPhysics_BigDi_v, false, 0);

        vb1 = new G4VisAttributes(G4Color(1., 1., 0.8, 0.5));
        vb1->SetForceSolid(true);
        fLogic_BigDi_m->SetVisAttributes(vb1);


        trace(" Start assign magnet  ");
        fieldMgr_BigDi = SetDipoleMagField(DipoleFieldBx, DipoleFieldBy, 0., MagTheta);   // gradient tesla/m;
        fLogic_BigDi_m->SetFieldManager(fieldMgr_BigDi, true);

    }
    G4FieldManager *SetDipoleMagField(G4double fx, G4double fy, G4double fz, float theta) {
        // G4Box  *sMagField = new G4Bo "dipole_magfield", 10/2*cm, 10/2*cm, 10/2*cm);
        //  G4LogicalVolume  *lMagField = new G4LogicalVolume(sMagField, World_Material, "logical_magnet");

        double ffx, ffy, ffz;
        ffx = fx * cos(theta * rad) + fz * sin(theta * rad);
        ffy = fy;
        ffz = -fx * sin(theta * rad) + fz * cos(theta * rad);

        G4ThreeVector fieldValue = G4ThreeVector(ffx * tesla, ffy * tesla, ffz * tesla);
        G4UniformMagField *magField = new G4UniformMagField(fieldValue);
        G4FieldManager *fieldMgr = new G4FieldManager(magField);

        fieldMgr->SetDetectorField(magField);
        fieldMgr->CreateChordFinder(magField);
        //accuracy mag.
        G4double minEps = 1.0e-5; // Minimum & value for smallest steps
        G4double maxEps = 1.0e-4; // Maximum & value for largest steps
        fieldMgr->SetMinimumEpsilonStep(minEps);
        fieldMgr->SetMaximumEpsilonStep(maxEps);
        fieldMgr->SetDeltaOneStep(0.5 * um); // 0.5 micrometer
        return fieldMgr;
//  new G4PVPlacement(0, G4ThreeVector(0,0,ZPosVect*cm), lMagField, "Magnet",World_Logic, false, 0);
    }

    //==============================================================================================================
    //                          Quadrupole
    //==============================================================================================================

    void  CreateQuad (G4VPhysicalVolume *fMotherPhysVolume) {
        using namespace spdlog;
        debug("Initialize Quad. MotherVolume:'{}' \n",fMotherPhysVolume->GetName());
        //return;
        G4VisAttributes *vb1;
        //mybrm = new G4RotationMatrix();
        //---------------- BEAM QUADS -------------------------------------------
        G4Tubs *fSolid_QUADS_hd_v, *fSolid_QUADS_hd_ir, *fSolid_QUADS_hd_m;
        G4LogicalVolume *fLogic_QUADS_hd_v, *fLogic_QUADS_hd_ir, *fLogic_QUADS_hd_m;
        G4PVPlacement *fPhysics_QUADS_hd_v, *fPhysics_QUADS_hd_ir, *fPhysics_QUADS_hd_m;
        G4FieldManager *fieldMgr_QUADS_hd;


        vb1 = new G4VisAttributes(G4Color(0.8, 0.3, 0.1, 0.9));
        vb1->SetForceSolid(true);

        trace("CreateQuad:: theta =%f rad=%f  deg=%f \n", MagTheta, MagTheta / rad, MagTheta / deg);
        mybrm.rotateY(MagTheta * rad);
        // brm_hd.rotateY((0*180/3.1415)*deg);

        G4Material *Material_Ir = G4Material::GetMaterial("IronAll");
        G4Material *Material_G = G4Material::GetMaterial("G4_Galactic");
        char abname[256];

        //--------------------Volumes ---------
        sprintf(abname, "Solid_QUADS_hd_v_%s", name.c_str());
        fSolid_QUADS_hd_v= new G4Tubs(abname, 0., (Rout + 0.1) * cm, ((LengthZ + 0.01)/ 2.) * m, 0., 360 * deg);

        sprintf(abname, "Logic_QUADS_hd_v_%s", name.c_str());
        fLogic_QUADS_hd_v = new G4LogicalVolume(fSolid_QUADS_hd_v, Material_G, abname);
        sprintf(abname, "Physics_QUADS_hd_v_%s", name.c_str());
        fPhysics_QUADS_hd_v = new G4PVPlacement(G4Transform3D(mybrm, G4ThreeVector(Xcenter * m, Ycenter * m, Zcenter * m)), abname, fLogic_QUADS_hd_v, fMotherPhysVolume, false, 0);

        // fLogic_QUADS_hd_v->SetFieldManager(fieldMgr_QUADS_hd, true);
        //printf("create %s ");

        //--------------------Iron---------
        sprintf(abname, "Solid_QUADS_hd_ir_%s", name.c_str());
        fSolid_QUADS_hd_ir = new G4Tubs(abname, (Rin2+0.1) * cm, (Rout ) * cm, (LengthZ / 2.) * m, 0., 360 * deg);
        sprintf(abname, "Logic_QUADS_hd_ir_%s", name.c_str());
        fLogic_QUADS_hd_ir = new G4LogicalVolume(fSolid_QUADS_hd_ir, Material_Ir, abname);
        sprintf(abname, "Physics_QUADS_hd_ir_%s", name.c_str());
        fPhysics_QUADS_hd_ir = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_QUADS_hd_ir, fPhysics_QUADS_hd_v, false, 0);
        fLogic_QUADS_hd_ir->SetVisAttributes(vb1);


        //----------------  magnetic field volume---------------
        sprintf(abname, "Solid_QUADS_hd_m_%s", name.c_str());
        fSolid_QUADS_hd_m = new G4Tubs(abname, 0. * cm, Rin2 * cm, (LengthZ / 2.) * m, 0., 360 * deg);
        sprintf(abname, "Logic_QUADS_hd_m_%s", name.c_str());
        fLogic_QUADS_hd_m = new G4LogicalVolume(fSolid_QUADS_hd_m, Material_G, abname);
        sprintf(abname, "Physics_QUADS_hd_m_%s", name.c_str());
        fPhysics_QUADS_hd_m = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_QUADS_hd_m, fPhysics_QUADS_hd_v, false, 0);

        //---------------- create  magnetic field ---------------

        debug("CreateQuad:: name={}  FIELD = Dx {}  Dy {} --  Qn {} Qs {} -- Sek {}  Sol {} ", name.c_str(),  DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew, SextupoleField, SolenoidField);
        fieldMgr_QUADS_hd = SetQMagField(QuadrupolFieldQnorm, QuadrupolFieldQskew, MagTheta, G4ThreeVector(Xcenter * m, Ycenter * m, Zcenter * m));   // gradient tesla/m;

        //    G4FieldManager* fieldMgr = SetQMagField(qFIELDx[j],qFIELDy[j]);   // gradient tesla/m;
        // fLogic_QUADSm[j]->SetFieldManager(fieldMgr,true);

        fLogic_QUADS_hd_v->SetFieldManager(fieldMgr_QUADS_hd, true);

    }


    G4FieldManager *SetQMagField(float field, float skew, float theta, G4ThreeVector fieldorigin)
    {
        using namespace spdlog;
        G4RotationMatrix *qrm_f;
        trace("SetQMagField:: got parameters {} {} ", field, skew);

        //G4double fGradient = -223.*tesla/m;
        G4double fGradient = sqrt(field * field + skew * skew) * tesla / m;
        fGradient = copysign(fGradient, field);
        G4double angle = atan2(skew, fabs(field)); //--   atan2(y,x);
        trace("SetQMagField():: angle={}({}) theta={}({}) rad(deg) ", angle, angle / deg, theta, theta / deg);

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
        myChordFinder->SetVerbose(0);

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
    int32_t fElectronBeamEnergy;
    int32_t fIonBeamEnergy;

    int USE_LINE;
    void SetMotherParams(G4VPhysicalVolume *physicalVolume) {
        // fMaterial = material;
        fMotherPhysVolume=physicalVolume;
    }
    void SetElectronBeamEnergy(int32_t electronBeamEnergy) {
        AcceleratorMagnets::fElectronBeamEnergy = electronBeamEnergy;
    }

    void SetIonBeamEnergy(int32_t ionBeamEnergy) {
        AcceleratorMagnets::fIonBeamEnergy = ionBeamEnergy;
    }


    AcceleratorMagnets(std::string fname, G4VPhysicalVolume *physicalVolume, G4Material *material,int _USE_LINE)  {
        using namespace spdlog;
        fMotherPhysVolume=physicalVolume;
        info("Initializing AcceleratorMagnets:: MotherVolume: '{}'\n",fMotherPhysVolume->GetName());

        ifstream sourcefile;                    // build a read-Stream
        sourcefile.open(fname, ios_base::in);  // open data
        USE_LINE=_USE_LINE;

        if (!sourcefile) {                     // if it does not work
            cerr << "Can't open File with Lattice!\n";
        } else {
            debug("AcceleratorMagnets file opened {} \n",fname);
            for (std::string line; std::getline(sourcefile, line);) {

                std::istringstream in(line);      //make a stream for the line itself
                trace("AcceleratorMagnets:: read line = {} ",line);
                if (line[0] != 'e' && line[0] != 'i') { trace(" skip line {}",line.c_str()); continue; }
                parse_line(line,USE_LINE); //-- string--
            }
        }
    }
    //-----------------------------------------------------------
    void parse_line(std::string line,int USE_LINE)
    {using namespace spdlog;
        string name, type;
        double LengthZ, Rin, Rin2, Rout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew;
        double SextupoleField, SolenoidField, Xcenter, Ycenter, Zcenter, MagTheta, MagPhi;


        std::istringstream in(line);
        in >> name >> type >> LengthZ >> Rin >> Rin2 >> Rout >> DipoleFieldBx >> DipoleFieldBy >> QuadrupolFieldQnorm >> QuadrupolFieldQskew >> SextupoleField >> SolenoidField
           >> Xcenter >> Ycenter >> Zcenter >> MagTheta >> MagPhi;

        trace("AcceleratorMagnets::parse_line: Rin={}, Rin2={} Rout ={} ",Rin, Rin2,Rout );
        if(USE_LINE==1){Rin=Rin*100; Rin2=Rin2*100;  Rout=Rout*100/2.; } // different usints for ERHIC and JLEIC designs
        trace("AcceleratorMagnets::parse_line: Rin={}, Rin2={} Rout ={} ",Rin, Rin2,Rout );
        QMag *qmag = new QMag(name, type, LengthZ, Rin, Rin2, Rout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew, SextupoleField, SolenoidField, Xcenter,
                              Ycenter, Zcenter, MagTheta, MagPhi, fMotherPhysVolume,USE_LINE);

        allmagnets.push_back(qmag);

    }

};

///---- usage in Constructor ..

#endif //G4E_IR_LATTICE_HH


