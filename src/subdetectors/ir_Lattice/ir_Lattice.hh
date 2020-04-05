#ifndef G4E_IR_LATTICE_HH
#define G4E_IR_LATTICE_HH

#include <G4Material.hh>
#include <G4FieldManager.hh>

class ir_LatticeDesign {
public:

    int32_t fElectronBeamEnergy;
    int32_t fIonBeamEnergy;



    G4Material* fMaterial;
    G4VPhysicalVolume *fMotherPhysVolume;

    void SetMotherParams(G4VPhysicalVolume *physicalVolume, G4Material *material) {
        fMaterial = material;
        fMotherPhysVolume=physicalVolume;
    }

    void SetElectronBeamEnergy(int32_t electronBeamEnergy) {
        ir_LatticeDesign::fElectronBeamEnergy = electronBeamEnergy;
    }

    void SetIonBeamEnergy(int32_t ionBeamEnergy) {
        ir_LatticeDesign::fIonBeamEnergy = ionBeamEnergy;
    }

    G4Material *ffqsMaterial;
    G4VisAttributes *vb1, *vb1a, *vb1b, *vb1as;
    G4Material *ffqsMaterial_G;
    double fZ1;
    //---------------- BEAM QUADS -------------------------------------------
    G4Tubs *fSolid_QUADS_hd_v[100], *fSolid_QUADS_hd_ir[100], *fSolid_QUADS_hd_m[100];
    G4LogicalVolume *fLogic_QUADS_hd_v[100], *fLogic_QUADS_hd_ir[100], *fLogic_QUADS_hd_m[100];
    G4PVPlacement *fPhysics_QUADS_hd_v[100], *fPhysics_QUADS_hd_ir[100], *fPhysics_QUADS_hd_m[100];
    G4FieldManager *fieldMgr_QUADS_hd[100];
    G4RotationMatrix brm_hd[200];

    char fSolid_ffqsNAME[100][256];
    float fSolid_ffqsSizeZ[100], fSolid_ffqsRin[100], fSolid_ffqsRout[100];
    float fSolid_ffqsX[100], fSolid_ffqsY[100], fSolid_ffqsZ[100];


//---------------- BEAM ASOLENOID -------------------------------------------
    G4Tubs *fSolid_ASOLENOID_hd_v[40], *fSolid_ASOLENOID_hd_ir[40], *fSolid_ASOLENOID_hd_m[40];
    G4LogicalVolume *fLogic_ASOLENOID_hd_v[40], *fLogic_ASOLENOID_hd_ir[40], *fLogic_ASOLENOID_hd_m[40];
    G4PVPlacement *fPhysics_ASOLENOID_hd_v[40], *fPhysics_ASOLENOID_hd_ir[40], *fPhysics_ASOLENOID_hd_m[40];
    G4FieldManager *fieldMgr_ASOLENOID_hd[40];
    G4UniformMagField *fMagField_ASOLENOID[40];
    //  G4UniformMagField* fMagField_ASOLENOID[40];      //pointer to the magnetic field
    G4FieldManager *fieldMgrAS[40];
    G4RotationMatrix brm_as[40];

    //---------------- BEAM KICKERS/DIPOLES -------------------------------------------
    G4Tubs *fSolid_BigDi_v[100], *fSolid_BigDi_ir[100], *fSolid_BigDi_m[100];
    //  G4Box *fSolid_BigDi_v[100],*fSolid_BigDi_ir[100],*fSolid_BigDi_m[100];
    G4LogicalVolume *fLogic_BigDi_v[100], *fLogic_BigDi_ir[100], *fLogic_BigDi_m[100];
    G4PVPlacement *fPhysics_BigDi_v[100], *fPhysics_BigDi_ir[100], *fPhysics_BigDi_m[100];
    G4RotationMatrix brmBigDi[100];
    G4FieldManager *fieldMgr_BigDi[100];
    float fSolid_BigDi_ffqsX[100], fSolid_BigDi_ffqsY[100], fSolid_BigDi_ffqsZ[100];
    float fSolid_BigDi_ffqsSizeZDi[100], fSolid_BigDi_ffqsRinDi[100], fSolid_BigDi_ffqsRoutDi[100];
    char fSolid_BigDi_ffqsNAME[100][256];
    //----------------DIPOLES CHICANE -------------------------------------------
    G4Box *fSolid_Chicane_v[100], *fSolid_Chicane_ir[100], *fSolid_Chicane_m[100];
    G4LogicalVolume *fLogic_Chicane_v[100], *fLogic_Chicane_ir[100], *fLogic_Chicane_m[100];
    G4PVPlacement *fPhysics_Chicane_v[100], *fPhysics_Chicane_ir[100], *fPhysics_Chicane_m[100];
    G4RotationMatrix brmChicane[100];
    G4FieldManager *fieldMgr_Chicane[100];


    G4RotationMatrix brm_hk[500];

    void Read_ERHIC_ion_beam_lattice() {
          FILE *rc;
        char buffer[512];
        char ffqtype[256];
        char mychar[40];
        char ffqnameDi[256];
        float ffqsRinDiG;
        float ffqsRinDi;
        float ffqsRoutDi;
        float ffqsSizeZDi;
        float qFIELDx;
        float qFIELDy;
        float qFIELQn;
        float qFIELQs;
        float qFIELSek;
        float qFIELSol;
        float ffqsZ;
        float ffqsY;
        float ffqsX;
        float ffqsTheta;
        float ffqsPhi;
        int iqmax_i;

        printf("DetectorConstruction::Read_ERHIC_ion_beam_lattice. %d \n",fIonBeamEnergy);
        // If G4E_HOME is set get a file from resources, otherwise try to open what we have

        std::string fileName = fmt::format("ion_ir_{}.txt",fIonBeamEnergy);
        spdlog::debug("DetectorConstruction::Read_ERHIC_ion_beam_lattice. Opening file: '{}'", fileName);

        const char* home_cstr = std::getenv("G4E_HOME");
        if(home_cstr){
            //--- USE_JLEIC      fileName = fmt::format("{}/resources/jleic/mdi/{}", home_cstr, fileName);
            //--- USE_RHIC
            fileName = fmt::format("{}/resources/erhic/mdi/{}", home_cstr, fileName);
        }

        spdlog::info("DetectorConstruction::Read_ERHIC_ion_beam_lattice. Opening file: '{}'", fileName);

        rc = fopen(fileName.c_str(), "r");
        if (rc == nullptr) {
            spdlog::warn("DetectorConstruction::Read_ERHIC_ion_beam_lattice. fopen returned NULLPTR on file: '{}'", fileName);
            return;
        }
        spdlog::debug("DetectorConstruction::Read_ERHIC_ion_beam_lattice. Opened file: '{}'", fileName);


        int iq=0,ik=0,is=0;
        while (fgets(buffer, 512, (FILE *) rc)) {

            spdlog::debug("*********************************************************************************\n");

            sscanf(buffer, "%s", mychar);

            if (mychar[0] != 'i') {

                continue;
            }
            if (buffer[0] == '#' || buffer[0] == '\n') {
                continue;
            }

            sscanf(buffer, "%s %s %f %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f", ffqnameDi, ffqtype, &ffqsSizeZDi, &ffqsRinDiG, &ffqsRinDi, &ffqsRoutDi, &qFIELDx, &qFIELDy, &qFIELQn,
                   &qFIELQs, &qFIELSek, &qFIELSol, &ffqsX, &ffqsY, &ffqsZ, &ffqsTheta, &ffqsPhi);

//            printf(" NUM: |%s| %s Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", ffqnameDi, ffqtype, ffqsSizeZDi, ffqsRinDiG,
//                   ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

            // ----------- create volumes for QUADRUPOLE----------
            if (strcmp(ffqtype, "QUADRUPOLE") == 0) {
//                printf(" found QUAD %s iq=%d \n", ffqtype, iq);
                CreateQuad(iq, ffqnameDi, ffqsSizeZDi, ffqsRinDiG*100, ffqsRinDi*100, ffqsRoutDi*100/2., qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                           ffqsPhi);
                iq++;
                iqmax_i = iq;
            }
         // ----------- skip multipoles at the moment----------
            if ((strcmp(ffqtype, "MULTIPOLE") == 0)) {
//                printf(" found  %s  ==> Skip it ! \n", ffqtype);
            }
            // ----------- create volumes for kickers and rbend----------
            if ((strcmp(ffqtype, "KICKER") == 0) || (strcmp(ffqtype, "RBEND") == 0) || (strcmp(ffqtype, "SBEND") == 0)) {
//                printf(" found  %s \n", ffqtype);
           //    ffqsRinDiG =1;  ffqsRinDi =1; ffqsRinDi=2.;  ffqsRoutDi=4.;

                CreateDipole(ik, ffqnameDi, ffqsSizeZDi, ffqsRinDiG*100, ffqsRinDi*100, ffqsRoutDi*100/2., qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                             ffqsPhi);
                ik++;
            }
            // ----------- create volumes for solenoid  ----------
            if ((strcmp(ffqtype, "SOLENOID") == 0) && ((strcmp(ffqnameDi, "iASDS") == 0) || (strcmp(ffqnameDi, "iASUS") == 0))) {
//                printf(" found SOLENOID %s \n", ffqtype);

                CreateASolenoid(is, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                                ffqsPhi);
                is++;
            }
        }


        fclose(rc);
    }

    void Read_ERHIC_electron_beam_lattice() {

        FILE *rc;
        char buffer[512];
        char ffqtype[256];
        char fname[256];
        char mychar[40];
        char ffqnameDi[256];
        float ffqsRinDiG;
        float ffqsRinDi;
        float ffqsRoutDi;
        float ffqsSizeZDi;
        float qFIELDx;
        float qFIELDy;
        float qFIELQn;
        float qFIELQs;
        float qFIELSek;
        float qFIELSol;
        float ffqsZ;
        float ffqsY;
        float ffqsX;
        float ffqsTheta;
        float ffqsPhi;


        printf("Initializing ERHIC_electron_beam_lattice. %d \n",fElectronBeamEnergy);
        // If G4E_HOME is set get a file from resources, otherwise try to open what we have

        std::string fileName = fmt::format("e_ir_{}.txt",fElectronBeamEnergy);
        spdlog::debug("DetectorConstruction::Read_ERHIC_electron_beam_lattice. Opening file: '{}'", fileName);

        // If G4E_HOME is set get a file from resources, otherwise try to open what we have
        //std::string fileName("e_ir.txt");
        const char* home_cstr = std::getenv("G4E_HOME");
        if(home_cstr){
            fileName = fmt::format("{}/resources/erhic/mdi/{}", home_cstr, fileName);
        }

        // Opening the file
        rc = fopen(fileName.c_str(), "r");
        if (rc == nullptr) {
            spdlog::warn("DetectorConstruction::Read_ERHIC_electron_beam_lattice. fopen returned NULLPTR on file: '{}'", fileName);
            return;
        }
        spdlog::debug("DetectorConstruction::Read_ERHIC_electron_beam_lattice. Opened file: '{}'", fileName);

        int iq=0,ik=0,is=0;
        while (fgets(buffer, 512, (FILE *) rc)) {

            //printf("Read_dE*********************************************************************************\n");
            //printf("Read_dE %s\n", buffer);
            sscanf(buffer, "%s", mychar);
            //printf("Read_dE:: mychar=%s\n ", mychar);
            if (buffer[0] == '#' || buffer[0] == '\n') {
                continue;
            }


            if (mychar[0] != 'e' || mychar[0] == '\n') {
                //printf("SKIP LINE %s\n", buffer);
                continue;
            }


            sscanf(buffer, "%s %s %f %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f", ffqnameDi, ffqtype, &ffqsSizeZDi, &ffqsRinDiG, &ffqsRinDi, &ffqsRoutDi, &qFIELDx, &qFIELDy, &qFIELQn,
                   &qFIELQs, &qFIELSek, &qFIELSol, &ffqsX, &ffqsY, &ffqsZ, &ffqsTheta, &ffqsPhi);

//            printf("Read_dE dE NUM: |%s| %s Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", ffqnameDi, ffqtype, ffqsSizeZDi, ffqsRinDiG,
//                   ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

            // ----------- create volumes for QUADRUPOLE----------
            if (strcmp(ffqtype, "QUADRUPOLE") == 0) {
                //printf("Read_dE found QUAD %s iq=%d\n", ffqtype, iq);
                CreateQuad(iq, ffqnameDi, ffqsSizeZDi, ffqsRinDiG*100, ffqsRinDi*100, ffqsRoutDi*100/2., qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                           ffqsPhi);
                iq++;
            }
            // ----------- create volumes for kickers and rbend----------
            if ((strcmp(ffqtype, "KICKER") == 0) || (strcmp(ffqtype, "RBEND") == 0)) {
                //printf("Read_dE found KICKER %s ik=%d \n", ffqtype, ik);
                CreateDipoleChicane(ik, ffqnameDi, ffqsSizeZDi, ffqsRinDiG*100, ffqsRinDi*100, ffqsRoutDi*100/2., qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ,
                                    ffqsTheta, ffqsPhi);
                ik++;
            }
            // ----------- create volumes for solenoid  ----------
            if ((strcmp(ffqtype, "SOLENOID") == 0) && ((strcmp(ffqnameDi, "eASDS") == 0) || (strcmp(ffqnameDi, "eASUS") == 0))) {
                //printf("Read_dE found SOLENOID %s \n", ffqtype);

                CreateASolenoid(is, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                                ffqsPhi);
                is++;
            }
        }

        fclose(rc);
        return;
    }


    //=================================================================================
    //=================================================================================
    void Read_JLEIC_ion_beam_lattice() {
        //Downstream elements
        // Element name	Type	   Length [m] Good_field half-aperture [cm]
        // Inner Half-A [cm]
        // Outer Radius [cm]
        // Dipole field [T]
        // Quadrupole field [T/m]
        // Sextupole [T/m^2]
        // Solenoid [T]
        // X center [m] Y center [m]
        // Z center [m]
        // Theta center [rad]	Phi [rad]
        //iDSDS	SOLENOID	2.4	2	160	210	0	0	0	0	0	-3.0	-0.060	0	1.199	-0.050	0
        //iBDS1a	RBEND   	0.85	4	35.2	45.2	1.30	2.47	0	0	0	0	-0.271	0	5.418	-0.051	0
        //iBDS1b	RBEND   	0.85	4	40.5	50.5	-1.09	2.24	0	0	0	0	-0.327	0	6.467	-0.054	0
        //iQDS1a	QUADRUPOLE	2.25	4	9.2	23.1	0	0	-37.24	0.63	0	0	-0.419	0	8.114	-0.056	0
        //iQDS1S	QUADRUPOLE	0.5	4	9.9	24.8	0	0	0	-0.50	0	0	-0.507	0	9.687	-0.056	0
        //iQDS1b	QUADRUPOLE	2.25	4	12.3	31.0	0	0	-37.24	0	0	0	-0.595	0	11.259	-0.056	0
        //iQDS2S	QUADRUPOLE	0.5	4	13.0	32.7	0	0	0	0.00078	0	0	-0.684	0	12.832	-0.056	0
        //iQDS2	QUADRUPOLE	4.5	4	17.7	44.4	0	0	25.96	0	0	0	-0.835	0	15.528	-0.056	0
        //iQDS3S	QUADRUPOLE	0.5	4	18.4	46.2	0	0	0	-1.24	0	0	-0.986	0	18.223	-0.056	0
        //iASDS	SOLENOID	1.2	4	19.8	49.7	0	0	0	0	0	6.0	-1.045	0	19.272	-0.056	0
        //iBDS2	RBEND   	8.00	4	40	90	0	-4.67	0	0	0	0	-1.296	0	25.766	-0.028	0
        //iBDS3	RBEND   	6.50	4	4	30	0	5.75	0	0	0	0	-1.499	0	44.560	-0.028	0
        //iQDS4	QUADRUPOLE	0.8	3	4	30	0	0	144.1	0	0	0	-1.875	0	52.890	-0.056	0
        FILE *rc;
        char buffer[512];
        char ffqtype[256];
        char mychar[40];
        char ffqnameDi[256];
        float ffqsRinDiG;
        float ffqsRinDi;
        float ffqsRoutDi;
        float ffqsSizeZDi;
        float qFIELDx;
        float qFIELDy;
        float qFIELQn;
        float qFIELQs;
        float qFIELSek;
        float qFIELSol;
        float ffqsZ;
        float ffqsY;
        float ffqsX;
        float ffqsTheta;
        float ffqsPhi;
        int iqmax_i;

        printf("Initializing ion_beam_lattice. %d \n",fIonBeamEnergy);
        // If G4E_HOME is set get a file from resources, otherwise try to open what we have

        std::string fileName = fmt::format("ion_ir_{}.txt",fIonBeamEnergy);
        spdlog::debug("DetectorConstruction::Read_ion_beam_lattice. Opening file: '{}'", fileName);

        const char* home_cstr = std::getenv("G4E_HOME");
        if(home_cstr){
       //--- USE_JLEIC      fileName = fmt::format("{}/resources/jleic/mdi/{}", home_cstr, fileName);
        //--- USE_RHIC
        fileName = fmt::format("{}/resources/erhic/mdi/{}", home_cstr, fileName);
        }

        spdlog::info("DetectorConstruction::Read_ion_beam_lattice. Opening file: '{}'", fileName);

        rc = fopen(fileName.c_str(), "r");
        if (rc == nullptr) {
            spdlog::warn("DetectorConstruction::Read_ion_beam_lattice. fopen returned NULLPTR on file: '{}'", fileName);
            return;
        }
        spdlog::debug("DetectorConstruction::Read_ion_beam_lattice. Opened file: '{}'", fileName);


        int iq=0,ik=0,is=0;
        while (fgets(buffer, 512, (FILE *) rc)) {

            spdlog::debug("*********************************************************************************\n");
            //printf("%s\n", buffer);
            sscanf(buffer, "%s", mychar);
            //printf("mychar=%s\n ", mychar);
            if (mychar[0] != 'i') {
                //printf("SKIP LINE %s\n", buffer);
                continue;
            }
            if (buffer[0] == '#' || buffer[0] == '\n') {
                continue;
            }

            sscanf(buffer, "%s %s %f %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f", ffqnameDi, ffqtype, &ffqsSizeZDi, &ffqsRinDiG, &ffqsRinDi, &ffqsRoutDi, &qFIELDx, &qFIELDy, &qFIELQn,
                   &qFIELQs, &qFIELSek, &qFIELSol, &ffqsX, &ffqsY, &ffqsZ, &ffqsTheta, &ffqsPhi);

//            printf(" NUM: |%s| %s Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", ffqnameDi, ffqtype, ffqsSizeZDi, ffqsRinDiG,
//                   ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

            // ----------- create volumes for QUADRUPOLE----------
            if (strcmp(ffqtype, "QUADRUPOLE") == 0) {
                //printf(" found QUAD %s iq=%d \n", ffqtype, iq);
                CreateQuad(iq, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                           ffqsPhi);
                iq++;
                iqmax_i = iq;
            }

            // ----------- create volumes for kickers and rbend----------
            if ((strcmp(ffqtype, "KICKER") == 0) || (strcmp(ffqtype, "RBEND") == 0)) {
                //printf(" found  %s \n", ffqtype);
                CreateDipole(ik, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                             ffqsPhi);
                ik++;
            }
            // ----------- create volumes for solenoid  ----------
            if ((strcmp(ffqtype, "SOLENOID") == 0) && ((strcmp(ffqnameDi, "iASDS") == 0) || (strcmp(ffqnameDi, "iASUS") == 0))) {
                //printf(" found SOLENOID %s \n", ffqtype);

                CreateASolenoid(is, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                                ffqsPhi);
                is++;
            }
        }


        fclose(rc);
    }


    void Read_JLEIC_electron_beam_lattice() {
        //Downstream elements
        // Element name	Type	   Length [m] Good_field half-aperture [cm]
        // Inner Half-A [cm]
        // Outer Radius [cm]
        // Dipole field [T]
        // Quadrupole field [T/m]
        // Sextupole [T/m^2]
        // Solenoid [T]
        // X center [m] Y center [m]
        // Z center [m]
        // Theta center [rad]	Phi [rad]

        FILE *rc;
        char buffer[512];
        char ffqtype[256];
        char fname[256];
        char mychar[40];
        char ffqnameDi[256];
        float ffqsRinDiG;
        float ffqsRinDi;
        float ffqsRoutDi;
        float ffqsSizeZDi;
        float qFIELDx;
        float qFIELDy;
        float qFIELQn;
        float qFIELQs;
        float qFIELSek;
        float qFIELSol;
        float ffqsZ;
        float ffqsY;
        float ffqsX;
        float ffqsTheta;
        float ffqsPhi;


        //printf("DetectorConstruction::Read_electron_beam_lattice. %d \n",fElectronBeamEnergy);
        // If G4E_HOME is set get a file from resources, otherwise try to open what we have

        std::string fileName = fmt::format("e_ir_{}.txt",fElectronBeamEnergy);
        spdlog::debug("DetectorConstruction::Read_electron_beam_lattice. Opening file: '{}'", fileName);

        // If G4E_HOME is set get a file from resources, otherwise try to open what we have
        //std::string fileName("e_ir.txt");
        const char* home_cstr = std::getenv("G4E_HOME");
        if(home_cstr){
            fileName = fmt::format("{}/resources/jleic/mdi/{}", home_cstr, fileName);
        }

        // Opening the file
        rc = fopen(fileName.c_str(), "r");
        if (rc == nullptr) {
            spdlog::warn("DetectorConstruction::Read_electron_beam_lattice. fopen returned NULLPTR on file: '{}'", fileName);
            return;
        }
        spdlog::debug("DetectorConstruction::Read_electron_beam_lattice. Opened file: '{}'", fileName);

        int iq=0,ik=0,is=0;
        while (fgets(buffer, 512, (FILE *) rc)) {

            printf("Read_dE*********************************************************************************\n");
            //printf("Read_dE %s\n", buffer);
            sscanf(buffer, "%s", mychar);
            //printf("Read_dE:: mychar=%s\n ", mychar);
            if (buffer[0] == '#' || buffer[0] == '\n') {
                continue;
            }


            if (mychar[0] != 'e' || mychar[0] == '\n') {
                //printf("SKIP LINE %s\n", buffer);
                continue;
            }


            sscanf(buffer, "%s %s %f %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f", ffqnameDi, ffqtype, &ffqsSizeZDi, &ffqsRinDiG, &ffqsRinDi, &ffqsRoutDi, &qFIELDx, &qFIELDy, &qFIELQn,
                   &qFIELQs, &qFIELSek, &qFIELSol, &ffqsX, &ffqsY, &ffqsZ, &ffqsTheta, &ffqsPhi);

            printf("Read_dE dE NUM: |%s| %s Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", ffqnameDi, ffqtype, ffqsSizeZDi, ffqsRinDiG,
                   ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

            // ----------- create volumes for QUADRUPOLE----------
            if (strcmp(ffqtype, "QUADRUPOLE") == 0) {
                //printf("Read_dE found QUAD %s iq=%d\n", ffqtype, iq);
                CreateQuad(iq, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                           ffqsPhi);
                iq++;
            }
            // ----------- create volumes for kickers and rbend----------
            if ((strcmp(ffqtype, "KICKER") == 0) || (strcmp(ffqtype, "RBEND") == 0)) {
                //printf("Read_dE found KICKER %s ik=%d \n", ffqtype, ik);
                CreateDipoleChicane(ik, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ,
                                    ffqsTheta, ffqsPhi);
                ik++;
            }
            // ----------- create volumes for solenoid  ----------
            if ((strcmp(ffqtype, "SOLENOID") == 0) && ((strcmp(ffqnameDi, "eASDS") == 0) || (strcmp(ffqnameDi, "eASUS") == 0))) {
                //printf("Read_dE found SOLENOID %s \n", ffqtype);

                CreateASolenoid(is, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta,
                                ffqsPhi);
                is++;
            }
        }

        fclose(rc);
        return;
    }




//==============================================================================================================
//                          Quadrupole
//==============================================================================================================

    void
    CreateQuad(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi, float ffqsRoutDi, float qFIELDx, float qFIELDy, float qFIELQn,
                                          float qFIELQs, float qFIELSek, float qFIELSol, float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta, float ffqsPhi) {
        char abname[256];

//        printf("CreateQuad:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG,
//               ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

        //return;

        vb1 = new G4VisAttributes(G4Color(0.8, 0.3, 0.1, 0.9));
        vb1->SetForceSolid(true);

        ///printf("CreateQuad:: theta =%f rad=%f  deg=%f \n", ffqsTheta, ffqsTheta / rad, ffqsTheta / deg);
        brm_hd[j].rotateY(ffqsTheta * rad);
        // brm_hd.rotateY((0*180/3.1415)*deg);

        ffqsMaterial = G4Material::GetMaterial("IronAll");
        ffqsMaterial_G = G4Material::GetMaterial("G4_Galactic");

        //fZ1=ffqsZ1Di[j];
        // printf(" FFQs name %s, Z=%f, L=%f, R=%f \n",ffqnameDi[j], ffqsZ1Di[j],  ffqsSizeZDi[j], ffqsRoutDi[j]);

        sprintf(fSolid_ffqsNAME[j], "%s", ffqsNAME);
        fSolid_ffqsSizeZ[j] = ffqsSizeZDi;
        fSolid_ffqsRin[j] = ffqsRinDi;
        fSolid_ffqsRout[j] = ffqsRoutDi;
        fSolid_ffqsX[j] = ffqsX;
        fSolid_ffqsY[j] = ffqsY;
        fSolid_ffqsZ[j] = ffqsZ;

        //--------------------Volumes ---------
        sprintf(abname, "Solid_QUADS_hd_v_%s", ffqsNAME);
        fSolid_QUADS_hd_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi + 0.01) * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
        sprintf(abname, "Logic_QUADS_hd_v_%s", ffqsNAME);
        fLogic_QUADS_hd_v[j] = new G4LogicalVolume(fSolid_QUADS_hd_v[j], fMaterial, abname);
        //fLogic_QUADS_hd_v[j] = new G4LogicalVolume(fSolid_QUADS_hd_v[j], World_Material, abname,fieldMgr_QUADS_hd[j]);
        sprintf(abname, "Physics_QUADS_hd_v_%s", ffqsNAME);
        fPhysics_QUADS_hd_v[j] = new G4PVPlacement(G4Transform3D(brm_hd[j], G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m)), abname, fLogic_QUADS_hd_v[j], fMotherPhysVolume, false, 0);

        fLogic_QUADS_hd_v[j]->SetFieldManager(fieldMgr_QUADS_hd[j], true);
        //printf("create %s ");

        //--------------------Iron---------
        sprintf(abname, "Solid_QUADS_hd_ir_%s", ffqsNAME);
        fSolid_QUADS_hd_ir[j] = new G4Tubs(abname, ffqsRinDi * cm, (ffqsRoutDi + 0.005) * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
        sprintf(abname, "Logic_QUADS_hd_ir_%s", ffqsNAME);
        fLogic_QUADS_hd_ir[j] = new G4LogicalVolume(fSolid_QUADS_hd_ir[j], ffqsMaterial, abname);
        sprintf(abname, "Physics_QUADS_hd_ir_%s", ffqsNAME);
        fPhysics_QUADS_hd_ir[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_QUADS_hd_ir[j], fPhysics_QUADS_hd_v[j], false, 0);
        fLogic_QUADS_hd_ir[j]->SetVisAttributes(vb1);


        //----------------  magnetic field volume---------------
        sprintf(abname, "Solid_QUADS_hd_m_%s", ffqsNAME);
        fSolid_QUADS_hd_m[j] = new G4Tubs(abname, 0. * cm, ffqsRinDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
        sprintf(abname, "Logic_QUADS_hd_m_%s", ffqsNAME);
        fLogic_QUADS_hd_m[j] = new G4LogicalVolume(fSolid_QUADS_hd_m[j], ffqsMaterial_G, abname, fieldMgr_QUADS_hd[j]);
        sprintf(abname, "Physics_QUADS_hd_m_%s", ffqsNAME);
        fPhysics_QUADS_hd_m[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_QUADS_hd_m[j], fPhysics_QUADS_hd_v[j], false, 0);

        //---------------- create  magnetic field ---------------
        //printf("CreateQuad:: j=%d  FIELD = Dx %f  Dy %f --  Qn %f Qs %f -- Sek %f  Sol %f \n", j, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol);
        fieldMgr_QUADS_hd[j] = SetQMagField(qFIELQn, qFIELQs, ffqsTheta, G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m));   // gradient tesla/m;

        //    G4FieldManager* fieldMgr = SetQMagField(qFIELDx[j],qFIELDy[j]);   // gradient tesla/m;
        // fLogic_QUADSm[j]->SetFieldManager(fieldMgr,true);


        fLogic_QUADS_hd_m[j]->SetFieldManager(fieldMgr_QUADS_hd[j], true);

    }


//==============================================================================================================
//                          DIPOLE
//==============================================================================================================

    void
    CreateDipole(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi, float ffqsRoutDi, float qFIELDx, float qFIELDy, float qFIELQn,
                                            float qFIELQs, float qFIELSek, float qFIELSol, float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta, float ffqsPhi) {
        char abname[256];
        //G4RotationMatrix brm_hk;

//        printf("CreateKicker:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG,
//               ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

        /*----------FFQs ions--------------*/

        ffqsMaterial = G4Material::GetMaterial("IronAll");
        ffqsMaterial_G = G4Material::GetMaterial("G4_Galactic");

        brm_hk[j].rotateY(ffqsTheta * rad);

        sprintf(fSolid_BigDi_ffqsNAME[j], "%s", ffqsNAME);
        fSolid_BigDi_ffqsSizeZDi[j] = ffqsSizeZDi;
        fSolid_BigDi_ffqsRinDi[j] = ffqsRinDi;
        fSolid_BigDi_ffqsRoutDi[j] = ffqsRoutDi;
        fSolid_BigDi_ffqsX[j] = ffqsX;
        fSolid_BigDi_ffqsY[j] = ffqsY;
        fSolid_BigDi_ffqsZ[j] = ffqsZ;

        //-------------------------- Magnetic volume------------------------
        sprintf(abname, "Solid_DIPOLE_v_%s", ffqsNAME);
        fSolid_BigDi_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi + 0.01) * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);

        // fSolid_BigDi_v[j] = new G4Box(abname, (double)(ffqsRoutDi+0.01)*cm, (double)(ffqsRoutDi+0.01)*cm,(double)(ffqsSizeZDi/2.)*m);
        sprintf(abname, "Logic_DIPOLE_v_%s", ffqsNAME);
        fLogic_BigDi_v[j] = new G4LogicalVolume(fSolid_BigDi_v[j], fMaterial, abname);
        sprintf(abname, "Physics_DIPOLE_v_%s", ffqsNAME);
        fPhysics_BigDi_v[j] = new G4PVPlacement(G4Transform3D(brm_hk[j], G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m)), abname, fLogic_BigDi_v[j], fMotherPhysVolume, false, 0);
        //printf(" Finish magnetic volume and start physics volume \n");

        //-------------------------- Magnet iron------------------------
        sprintf(abname, "Solid_DIPOLE_i_%s", ffqsNAME);
        fSolid_BigDi_ir[j] = new G4Tubs(abname, ffqsRinDi * cm, ffqsRoutDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
        // fSolid_BigDi_ir[j] = new G4Box(abname, (double) ffqsRoutDi*cm, (double)ffqsRoutDi*cm,((double)ffqsSizeZDi/2.)*m);
        sprintf(abname, "Logic_DIPOLE_i_%s", ffqsNAME);
        fLogic_BigDi_ir[j] = new G4LogicalVolume(fSolid_BigDi_ir[j], ffqsMaterial, abname);
        sprintf(abname, "Physics_DIPOLE_i_%s", ffqsNAME);
        fPhysics_BigDi_ir[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_BigDi_ir[j], fPhysics_BigDi_v[j], false, 0);

        vb1a = new G4VisAttributes(G4Color(0.2, 0.8, 0.2, 1.));
        vb1a->SetForceSolid(true);
        fLogic_BigDi_ir[j]->SetVisAttributes(vb1a);

        //-------------------------- Magnet field------------------------
        sprintf(abname, "Solid_DIPOLE_m_%s", ffqsNAME);
        fSolid_BigDi_m[j] = new G4Tubs(abname, 0., ffqsRinDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
        //  fSolid_BigDi_m[j] = new G4Box(abname,(double)ffqsRinDi*cm,(double)ffqsRinDi*cm,(double)(ffqsSizeZDi/2.)*m);
        sprintf(abname, "Logic_DIPOLE_m_%s", ffqsNAME);
        fLogic_BigDi_m[j] = new G4LogicalVolume(fSolid_BigDi_m[j], ffqsMaterial_G, abname);
        sprintf(abname, "Physics_DIPOLE_m_%s", ffqsNAME);
        fPhysics_BigDi_m[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_BigDi_m[j], fPhysics_BigDi_v[j], false, 0);

        vb1b = new G4VisAttributes(G4Color(1., 1., 0.8, 0.5));
        vb1b->SetForceSolid(true);
        fLogic_BigDi_m[j]->SetVisAttributes(vb1b);


        //printf(" Start assign magnet  \n");
        fieldMgr_BigDi[j] = SetDipoleMagField(qFIELDx, qFIELDy, 0., ffqsTheta);   // gradient tesla/m;
        fLogic_BigDi_m[j]->SetFieldManager(fieldMgr_BigDi[j], true);


    }

//==============================================================================================================
//                         Electron Chicane Dipoles
//==============================================================================================================


    void CreateDipoleChicane(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi, float ffqsRoutDi, float qFIELDx, float qFIELDy,
                                                        float qFIELQn, float qFIELQs, float qFIELSek, float qFIELSol, float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta,
                                                        float ffqsPhi) {
        char abname[256];
        //G4RotationMatrix brm_hk;

//        printf("CreateKicker:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG,
//               ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

        /*----------FFQs ions--------------*/

        ffqsMaterial = G4Material::GetMaterial("IronAll");
        ffqsMaterial_G = G4Material::GetMaterial("G4_Galactic");

        brm_hk[j].rotateY(ffqsTheta * rad);

        //-------------------------- Magnetic volume------------------------
        sprintf(abname, "Solid_v_%s", ffqsNAME);
        //   fSolid_Chicane_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi+0.01)*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);

        fSolid_Chicane_v[j] = new G4Box(abname, (double) (ffqsRoutDi + 0.01) * cm, (double) (ffqsRoutDi + 0.01) * cm, (double) (ffqsSizeZDi / 2.) * m);
        sprintf(abname, "Logic_Chicane_v_%s", ffqsNAME);
        fLogic_Chicane_v[j] = new G4LogicalVolume(fSolid_Chicane_v[j], fMaterial, abname);
        sprintf(abname, "Physics_Chicane_v_%s", ffqsNAME);
        fPhysics_Chicane_v[j] = new G4PVPlacement(G4Transform3D(brm_hk[j], G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m)), abname, fLogic_Chicane_v[j], fMotherPhysVolume, false, 0);
        printf(" Finish magnetic volume and start physics volume \n");

        //-------------------------- Magnet iron------------------------
        sprintf(abname, "Solid_i_%s", ffqsNAME);
        fSolid_Chicane_ir[j] = new G4Box(abname, (double) ffqsRoutDi * cm, (double) ffqsRoutDi * cm, ((double) ffqsSizeZDi / 2.) * m);
        sprintf(abname, "Logic_Chicane_i_%s", ffqsNAME);
        fLogic_Chicane_ir[j] = new G4LogicalVolume(fSolid_Chicane_ir[j], ffqsMaterial, abname);
        sprintf(abname, "Physics_Chicane_i_%s", ffqsNAME);
        fPhysics_Chicane_ir[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_Chicane_ir[j], fPhysics_Chicane_v[j], false, 0);

        vb1a = new G4VisAttributes(G4Color(0.2, 0.8, 0.2, 1.));
        vb1a->SetForceSolid(true);
        fLogic_Chicane_ir[j]->SetVisAttributes(vb1a);

        //-------------------------- Magnet field------------------------
        sprintf(abname, "Solid_m_%s", ffqsNAME);
        //    fSolid_Chicane_m[j] = new G4Tubs(abname, 0., ffqsRinDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);
        fSolid_Chicane_m[j] = new G4Box(abname, (double) ffqsRinDi * cm, (double) ffqsRinDi * cm, (double) (ffqsSizeZDi / 2.) * m);
        sprintf(abname, "Logic_Chicane_m_%s", ffqsNAME);
        fLogic_Chicane_m[j] = new G4LogicalVolume(fSolid_Chicane_m[j], ffqsMaterial_G, abname);
        sprintf(abname, "Physics_Chicane_m_%s", ffqsNAME);
        fPhysics_Chicane_m[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_Chicane_m[j], fPhysics_Chicane_v[j], false, 0);

        vb1b = new G4VisAttributes(G4Color(1., 1., 0.8, 0.5));
        vb1b->SetForceSolid(true);
        fLogic_Chicane_m[j]->SetVisAttributes(vb1b);


        //printf(" Start assign magnet  \n");
        fieldMgr_Chicane[j] = SetDipoleMagField(qFIELDx, qFIELDy, 0., ffqsTheta);   // gradient tesla/m;
        fLogic_Chicane_m[j]->SetFieldManager(fieldMgr_Chicane[j], true);


    }

//==============================================================================================================
//                          ASOLENOID
//==============================================================================================================

    void CreateASolenoid(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi, float ffqsRoutDi, float qFIELDx, float qFIELDy,
                                                    float qFIELQn, float qFIELQs, float qFIELSek, float qFIELSol, float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta,
                                                    float ffqsPhi) {

        char abname[256];
        // G4RotationMatrix brm_as;

//        printf("CreateQuad:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG,
//               ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

        //return;

        vb1 = new G4VisAttributes(G4Color(0.8, 0.3, 0.1, 0.9));
        vb1->SetForceSolid(true);

        //printf("CreateQuad:: theta =%f %f %f \n", ffqsTheta, ffqsTheta * rad, (ffqsTheta * 180 / 3.1415) * deg);
        brm_as[j].rotateY(ffqsTheta * rad);

        ffqsMaterial = G4Material::GetMaterial("IronAll");
        ffqsMaterial_G = G4Material::GetMaterial("G4_Galactic");

        //fZ1=ffqsZ1Di[j];
        // printf(" FFQs name %s, Z=%f, L=%f, R=%f \n",ffqnameDi[j], ffqsZ1Di[j],  ffqsSizeZDi[j], ffqsRoutDi[j]);


        //--------------------Volumes ---------
        sprintf(abname, "Solid_ASOLENOID_hd_v_%s", ffqsNAME);
        fSolid_ASOLENOID_hd_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi + 0.01) * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
        sprintf(abname, "Logic_ASOLENOID_hd_v_%s", ffqsNAME);
        fLogic_ASOLENOID_hd_v[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_v[j], fMaterial, abname);
        sprintf(abname, "Physics_ASOLENOID_hd_v_%s", ffqsNAME);
        fPhysics_ASOLENOID_hd_v[j] = new G4PVPlacement(G4Transform3D(brm_as[j], G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m)), abname, fLogic_ASOLENOID_hd_v[j], fMotherPhysVolume, false,
                                                       0);
        //printf("create %s ");
        //--------------------Iron---------
        sprintf(abname, "Solid_ASOLENOID_hd_ir_%s", ffqsNAME);
        fSolid_ASOLENOID_hd_ir[j] = new G4Tubs(abname, ffqsRinDi * cm, ffqsRoutDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
        sprintf(abname, "Logic_ASOLENOID_hd_ir_%s", ffqsNAME);
        fLogic_ASOLENOID_hd_ir[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_ir[j], ffqsMaterial, abname);
        sprintf(abname, "Physics_ASOLENOID_hd_ir_%s", ffqsNAME);
        fPhysics_ASOLENOID_hd_ir[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_ASOLENOID_hd_ir[j], fPhysics_ASOLENOID_hd_v[j], false, 0);
        //  fLogic_ASOLENOID_hd_ir[j]->SetVisAttributes(vb1);

        //---------------- set magnetic field ---------------
        sprintf(abname, "Solid_ASOLENOID_hd_m_%s", ffqsNAME);
        fSolid_ASOLENOID_hd_m[j] = new G4Tubs(abname, 0. * cm, ffqsRinDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
        sprintf(abname, "Logic_ASOLENOID_hd_m_%s", ffqsNAME);
        fLogic_ASOLENOID_hd_m[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_m[j], ffqsMaterial_G, abname);
        sprintf(abname, "Physics_ASOLENOID_hd_m_%s", ffqsNAME);
        fPhysics_ASOLENOID_hd_m[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_ASOLENOID_hd_m[j], fPhysics_ASOLENOID_hd_v[j], false, 0);


        vb1as = new G4VisAttributes(G4Color(1., 0.5, 0.7, 1.));
        vb1as->SetForceSolid(true);
        fLogic_ASOLENOID_hd_ir[j]->SetVisAttributes(vb1as);


        //    G4FieldManager* fieldMgr = SetQMagField(qFIELDx[j],qFIELDy[j]);   // gradient tesla/m;
        // fLogic_ASOLENOIDm[j]->SetFieldManager(fieldMgr,true);

        // G4double fieldStrength = 3.0*tesla;  // 0.01*tesla; // field strength in pipe


        //JF just keep it for now! Need to move it back to nominal!
        qFIELSol = 0;

        G4double fieldStrength = qFIELSol * tesla;  // 0.01*tesla; // field strength in pipe
        G4double alphaB = 0. * degree;
        fMagField_ASOLENOID[j] = new G4UniformMagField(G4ThreeVector(fieldStrength * std::sin(alphaB), 0., fieldStrength * std::cos(alphaB)));

        fieldMgrAS[j] = new G4FieldManager(fMagField_ASOLENOID[j]);
        fieldMgrAS[j]->SetDetectorField(fMagField_ASOLENOID[j]);
        fieldMgrAS[j]->CreateChordFinder(fMagField_ASOLENOID[j]);
        fLogic_ASOLENOID_hd_m[j]->SetFieldManager(fieldMgrAS[j], true);
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

    G4FieldManager *SetQMagField(float field, float skew, float theta, G4ThreeVector fieldorigin)
    {
        G4RotationMatrix *qrm_f;
        //printf("SetQMagField:: got parameters %f %f \n", field, skew);

        //G4double fGradient = -223.*tesla/m;
        G4double fGradient = sqrt(field * field + skew * skew) * tesla / m;
        fGradient = copysign(fGradient, field);
        G4double angle = atan2(skew, fabs(field)); //--   atan2(y,x);
        //printf("SetQMagField():: angle=%f(%f) theta=%f(%f) rad(deg) \n", angle, angle / deg, theta, theta / deg);

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

#endif //G4E_IR_LATTICE_HH
