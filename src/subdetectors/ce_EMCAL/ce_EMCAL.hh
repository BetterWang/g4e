#ifndef G4E_CE_EMCAL_HH
#define G4E_CE_EMCAL_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include <spdlog/spdlog.h>
#include "main_detectors/DetectorConfig.hh"
#include <InitializationContext.hh>

struct ce_EMCAL_Config
{
    double RIn = 15 * cm;
    double ROut;
    double ROutshift = 5 * cm;
    double Thickness = 40 * cm;
    //double SizeZ;
    double PosZ;

    //............... EMCAL Crystals modules ......................
    double PWO_Thickness = 20. * cm;
    double PWO_Width = 2. * cm;
    double PWO_Gap = 0.01 * mm;
    double PWO_InnerR = 15. * cm;
    double PWO_OuterR = 82 * cm;
    double PWO_PosZ;

    //............... EMCAL Glass modules ......................
    double Glass_Thickness = 40. * cm;
    double Glass_Width = 4. * cm;
    double Glass_Gap = 0.01 * mm;
    double Glass_InnerR;
    double Glass_OuterR;
    double Glass_PosZ;

    // Messenger to control initialization properties from geant config file

    inline ce_EMCAL_Config() {
        static G4GenericMessenger *Messenger;

        // Create a global messenger that will be used
        if(!Messenger) {
            // Set geant options
            Messenger = new G4GenericMessenger(this, "/eic/ce_EMCAL/");
            Messenger->DeclareProperty("pwoThickness", PWO_Thickness, "Thikness (z direction dimention) of PWO crystals ");
            Messenger->DeclareProperty("pwoWidth", PWO_Width, "Width (and higth) of each PWO crystal");
            Messenger->DeclareProperty("pwoGap", PWO_Gap, "Gap between PWO crystals ");
            Messenger->DeclareProperty("pwoInnerR", PWO_InnerR, "Inner radius or beam hole for PWO block");
            Messenger->DeclareProperty("pwoOuterR", PWO_OuterR, "Outer radius of PWO block");
            Messenger->DeclareProperty("glassThickness", Glass_Thickness, "Thikness (z direction dimention) of Glass modules");
            Messenger->DeclareProperty("glassWidth", Glass_Width, "Width (and higth) of each Glass modules");
            Messenger->DeclareProperty("glassGap", Glass_Gap, "Gap between Glass modules ");
        }
    }

};

// Central region, electron end-cap EMCAL
class ce_EMCAL_Design
{
public:


    ce_EMCAL_Design(ce_EMCAL_Config& cfg, g4e::InitializationContext *init):
        ConstructionConfig(cfg)
    {
    }

    inline void Construct(G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        auto cfg = ConstructionConfig;
        spdlog::debug("Construct ce_EMCAL");

        Solid = new G4Tubs("ce_EMCAL_GVol_Solid", cfg.RIn, cfg.ROut, cfg.Thickness / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ce_EMCAL_GVol_Logic");
        OuterVolumeVisAttr = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
        OuterVolumeVisAttr->SetLineWidth(1);
        OuterVolumeVisAttr->SetForceSolid(false);
        Logic->SetVisAttributes(OuterVolumeVisAttr);

        //   my_z= 0*cm;
        Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, cfg.PosZ), "ce_EMCAL_GVol_Phys", Logic, motherVolume, false, 0);
    }

    /// CE EMCAL module Crystals
    inline void ConstructCrystals()
    {
        static char abname[256];
        auto cfg = ConstructionConfig;

        cfg.PWO_PosZ = cfg.Thickness / 2 - cfg.PWO_Thickness / 2;

        ce_EMCAL_detPWO_Material = fMat->GetMaterial("PbWO4");
        ce_EMCAL_detPWO_Solid = new G4Box("ce_EMCAL_detPWO_Solid", cfg.PWO_Width * 0.5, cfg.PWO_Width * 0.5, cfg.PWO_Thickness * 0.5);
        ce_EMCAL_detPWO_Logic = new G4LogicalVolume(ce_EMCAL_detPWO_Solid, ce_EMCAL_detPWO_Material, "ce_EMCAL_detPWO_Logic");

        attr_ce_EMCAL_detPWO = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 1.));
        attr_ce_EMCAL_detPWO->SetLineWidth(1);
        attr_ce_EMCAL_detPWO->SetForceSolid(true);
        ce_EMCAL_detPWO_Logic->SetVisAttributes(attr_ce_EMCAL_detPWO);

        // Crystals
        double y_C = 0;
        double x_C;
        G4int k = -1;

        fmt::print("\nCE EMCAL PWO START\n");
        fmt::print("PWO_Thickness = {} cm;\n", cfg.PWO_Thickness / cm);
        fmt::print("PWO_Width     = {} cm;\n", cfg.PWO_Width / cm);
        fmt::print("PWO_Gap       = {} cm;\n", cfg.PWO_Gap / cm);
        fmt::print("PWO_InnerR    = {} cm;\n", cfg.PWO_InnerR / cm);
        fmt::print("PWO_OuterR    = {} cm;\n", cfg.PWO_OuterR / cm);
        fmt::print("PWO_PosZ      = {} cm;\n", cfg.PWO_PosZ / cm);

        //============  For sectors =====
        for (int j = 0; j < 50; j++) {
            y_C = j==0 ? cfg.PWO_Gap : y_C - cfg.PWO_Width + cfg.PWO_Gap;
            x_C = (cfg.PWO_Width + cfg.PWO_Gap) * 0.5;

            for (int i = 0; i < 50; i++) {
                double R = sqrt(x_C * x_C + y_C * y_C);

                //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f ce_EMCAL_detPWO_InnerR=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);
                if (R < cfg.PWO_OuterR - cfg.PWO_Width + cfg.PWO_Gap && R > cfg.PWO_InnerR) {

                    double x,y;
                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), abname, ce_EMCAL_detPWO_Logic, Phys, false, k);
                    fmt::print("({:<10}, {:<10}, {:<10}, {:<10.4f}, {:<10.4f}, '{}'),\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=-x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), abname, ce_EMCAL_detPWO_Logic, Phys, false, k);
                    fmt::print("({:<10}, {:<10}, {:<10}, {:<10.4f}, {:<10.4f}, '{}'),\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), abname, ce_EMCAL_detPWO_Logic, Phys, false, k);
                    fmt::print("({:<10}, {:<10}, {:<10}, {:<10.4f}, {:<10.4f}, '{}'),\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=-x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), abname, ce_EMCAL_detPWO_Logic, Phys, false, k);
                    fmt::print("({:<10}, {:<10}, {:<10}, {:<10.4f}, {:<10.4f}, '{}'),\n", k, i, j, x / cm, y / cm, abname);
                    //  printf("ce_EMCAL_detPWO::k=%d  j=%d i =%d x=%f, y=%f  R=%f ce_EMCAL_detPWO_InnerR=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);

                }
                x_C += cfg.PWO_Width + cfg.PWO_Gap;
            }
        }
        fmt::print("CE EMCAL PWO END\n\n");
    }

    inline void ConstructGlass()
    {
        static char abname[256];
        auto cfg = ConstructionConfig;

        //------------------------------------------------------------------
        // Ecal module GLASS
        //-------------------------------------------------------------------
        cfg.Glass_OuterR = cfg.ROut - 1 * cm ;
        cfg.Glass_InnerR = cfg.PWO_OuterR + 3 * cm;
        cfg.Glass_PosZ = cfg.Thickness / 2. - cfg.Glass_Thickness / 2.;

        ce_EMCAL_detGLASS_Material = fMat->GetMaterial("DSBCe");
        //   ce_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
        // ce_EMCAL_detGLASS_Solid = new G4Box("ce_EMCAL_detGLASS_Solid", cfg.Glass_Width * 0.5, cfg.Glass_Width * 0.5, cfg.Glass_Thickness * 0.5);
        ce_EMCAL_detGLASS_Logic = new G4LogicalVolume(ce_EMCAL_detGLASS_Solid, ce_EMCAL_detGLASS_Material, "ce_EMCAL_detGLASS_Logic");

        attr_ce_EMCAL_detGLASS = new G4VisAttributes(G4Color(0.3, 0.4, 1., 0.5));
        attr_ce_EMCAL_detGLASS->SetLineWidth(1);
        attr_ce_EMCAL_detGLASS->SetForceSolid(true);
        ce_EMCAL_detGLASS_Logic->SetVisAttributes(attr_ce_EMCAL_detGLASS);

        // GLASS
        double y_C = 0;
        double x_C = 0.;
        int k = -1;

        fmt::print("\nCE EMCAL GLASS START\n");
        fmt::print("Glass_Thickness = {} cm;\n", cfg.Glass_Thickness / cm);
        fmt::print("Glass_Width     = {} cm;\n", cfg.Glass_Width / cm);
        fmt::print("Glass_Gap       = {} cm;\n", cfg.Glass_Gap / cm);
        fmt::print("Glass_InnerR    = {} cm;\n", cfg.Glass_InnerR / cm);
        fmt::print("Glass_OuterR    = {} cm;\n", cfg.Glass_OuterR / cm);
        fmt::print("Glass_PosZ      = {} cm;\n", cfg.Glass_PosZ / cm);

        //============  For sectors =====
        for (int j = 0; j < 50; j++) {
            if (j == 0) { y_C = cfg.Glass_Gap; }
            else { y_C -= cfg.Glass_Width + cfg.Glass_Gap; }
            x_C = (cfg.Glass_Width + cfg.Glass_Gap) * 0.5;

            for (int i = 0; i < 50; i++) {
                double R = sqrt(x_C * x_C + y_C * y_C);

                //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);

                if (R < cfg.Glass_OuterR - cfg.Glass_Width + cfg.Glass_Gap && R > cfg.Glass_InnerR) {
                    double x,y;
                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), abname, ce_EMCAL_detGLASS_Logic, Phys, false, k);
                    fmt::print("{:<10} {:<10} {:<10} {:<10.4f} {:<10.4f} {}\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=-x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), abname, ce_EMCAL_detGLASS_Logic, Phys, false, k);
                    fmt::print("{:<10} {:<10} {:<10} {:<10.4f} {:<10.4f} {}\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), abname, ce_EMCAL_detGLASS_Logic, Phys, false, k);
                    fmt::print("{:<10} {:<10} {:<10} {:<10.4f} {:<10.4f} {}\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=-x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), abname, ce_EMCAL_detGLASS_Logic, Phys, false, k);
                    fmt::print("{:<10} {:<10} {:<10} {:<10.4f} {:<10.4f} {}\n", k, i, j, x / cm, y / cm, abname);
                }
                x_C += cfg.Glass_Width + cfg.Glass_Gap;

            }
        }

        fmt::print("CE EMCAL GLASS END\n\n");
    }

    /// CE EMCAL module Crystals
    inline void ConstructCrystalsSquare()
    {
        auto cfg = ConstructionConfig;

        cfg.PWO_PosZ = cfg.Thickness / 2 - cfg.PWO_Thickness / 2;

        ce_EMCAL_detPWO_Material = fMat->GetMaterial("PbWO4");
        ce_EMCAL_detPWO_Solid = new G4Box("ce_EMCAL_detPWO_Solid", cfg.PWO_Width * 0.5, cfg.PWO_Width * 0.5, cfg.PWO_Thickness * 0.5);
        ce_EMCAL_detPWO_Logic = new G4LogicalVolume(ce_EMCAL_detPWO_Solid, ce_EMCAL_detPWO_Material, "ce_EMCAL_detPWO_Logic");

        attr_ce_EMCAL_detPWO = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 1.));
        attr_ce_EMCAL_detPWO->SetLineWidth(1);
        attr_ce_EMCAL_detPWO->SetForceSolid(true);
        ce_EMCAL_detPWO_Logic->SetVisAttributes(attr_ce_EMCAL_detPWO);

        fmt::print("\nCE EMCAL PWO START\n");

        // CRYSTAL
        double diameter = 2 * cfg.PWO_OuterR;

        // How many towers do we have per row/column?
        // Add a gap + diameter as if we have N towers, we have N-1 gaps;
        int towersInRow = std::ceil((diameter + cfg.PWO_Gap) /  (cfg.PWO_Width + cfg.PWO_Gap));

        // Is it odd or even number of towersInRow
        double leftTowerPos, topTowerPos;
        if(towersInRow%2) {
            //             |
            //      [ ][ ][ ][ ][ ]
            //       ^     |
            int towersInHalfRow = std::ceil(towersInRow/2.0);
            leftTowerPos = topTowerPos = -towersInHalfRow * (cfg.PWO_Width + cfg.PWO_Gap);
        } else {
            //               |
            //      [ ][ ][ ][ ][ ][ ]
            //       ^      |
            int towersInHalfRow = towersInRow/2;
            leftTowerPos = topTowerPos = -(towersInHalfRow - 0.5) * (cfg.PWO_Width + cfg.PWO_Gap);
        }

        fmt::print("\nCE EMCAL PWO SQUARE START\n");
        fmt::print("PWO_Thickness = {} cm;\n", cfg.PWO_Thickness / cm);
        fmt::print("PWO_Width     = {} cm;\n", cfg.PWO_Width / cm);
        fmt::print("PWO_Gap       = {} cm;\n", cfg.PWO_Gap / cm);
        fmt::print("PWO_InnerR    = {} cm;\n", cfg.PWO_InnerR / cm);
        fmt::print("PWO_OuterR    = {} cm;\n", cfg.PWO_OuterR / cm);
        fmt::print("PWO_PosZ      = {} cm;\n", cfg.PWO_PosZ / cm);
        fmt::print("Towers in Row/Col   = {};\n", towersInRow);
        fmt::print("Top left tower pos  = {:<10} {:<10} cm;\n", -leftTowerPos / cm, topTowerPos / cm);

        // fmt::print("#Towers info:\n");
        // fmt::print("#{:<5} {:<6} {:<3} {:<3} {:>10} {:>10}   {}\n", "idx",  "code", "col", "row", "x", "y", "name");
        int towerIndex = 0;
        for(int colIndex=0; colIndex < towersInRow; colIndex++) {
            for(int rowIndex=0; rowIndex < towersInRow; rowIndex++) {
                double x = leftTowerPos + colIndex * (cfg.PWO_Width + cfg.PWO_Gap);
                double y = topTowerPos + rowIndex * (cfg.PWO_Width + cfg.PWO_Gap);

                if ((std::abs(y) < cfg.PWO_OuterR && std::abs(x) < cfg.PWO_OuterR) &&
                    (std::abs(y) > cfg.PWO_InnerR || std::abs(x) > cfg.PWO_InnerR))
                {
                    int code = 1000 * rowIndex + colIndex;
                    std::string name(fmt::format("ce_EMCAL_pwo_phys_{}", code));
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), name, ce_EMCAL_detPWO_Logic, Phys, false, code);
                    // fmt::print(" {:<5} {:<6} {:<3} {:<3} {:>10.4f} {:>10.4f}   {}\n", towerIndex, code, colIndex, rowIndex, x / cm, y / cm, name);
                    towerIndex++;

                }
            }
        }
        fmt::print("Total PWO modules: {}\n", towerIndex);
        fmt::print("\nCE EMCAL PWO END\n");
    }


    inline void ConstructGlassSquare()
    {
        using namespace std;
        auto cfg = ConstructionConfig;

        //------------------------------------------------------------------
        // Ecal module GLASS
        //------------------------------------------------------------------
        cfg.Glass_OuterR = cfg.ROut - 1 * cm ;
        cfg.Glass_InnerR = cfg.PWO_OuterR + 3 * cm;
        cfg.Glass_PosZ = cfg.Thickness / 2. - cfg.Glass_Thickness / 2.;

        ce_EMCAL_detGLASS_Material = fMat->GetMaterial("DSBCe");
        ce_EMCAL_detGLASS_Solid = new G4Box("ce_EMCAL_detGLASS_Solid", cfg.Glass_Width * 0.5, cfg.Glass_Width * 0.5, cfg.Glass_Thickness * 0.5);
        ce_EMCAL_detGLASS_Logic = new G4LogicalVolume(ce_EMCAL_detGLASS_Solid, ce_EMCAL_detGLASS_Material, "ce_EMCAL_detGLASS_Logic");

        attr_ce_EMCAL_detGLASS = new G4VisAttributes(G4Color(0.3, 0.4, 1., 0.5));
        attr_ce_EMCAL_detGLASS->SetLineWidth(1);
        attr_ce_EMCAL_detGLASS->SetForceSolid(true);
        ce_EMCAL_detGLASS_Logic->SetVisAttributes(attr_ce_EMCAL_detGLASS);

        // GLASS
        double diameter = 2 * cfg.Glass_OuterR;

        // How many towers do we have per row/columnt?
        // Add a gap + diameter as if we have N towers, we have N-1 gaps;
        int towersInRow = std::ceil((diameter + cfg.Glass_Gap) /  (cfg.Glass_Width + cfg.Glass_Gap));

        // Is it odd or even number of towersInRow
        double leftTowerPos, topTowerPos;
        if(towersInRow%2) {
            //             |
            //      [ ][ ][ ][ ][ ]
            //       ^     |
            int towersInHalfRow = std::ceil(towersInRow/2.0);
            topTowerPos = leftTowerPos = -towersInHalfRow * (cfg.Glass_Width + cfg.Glass_Gap);

        } else {
            //               |
            //      [ ][ ][ ][ ][ ][ ]
            //       ^      |
            int towersInHalfRow = towersInRow/2;
            topTowerPos = leftTowerPos = -(towersInHalfRow - 0.5) * (cfg.Glass_Width + cfg.Glass_Gap);
        }

        int moduleIndex = 0;

        fmt::print("\nCE EMCAL GLASS SQUARE START\n");
        fmt::print("Glass_Thickness = {} cm;\n", cfg.Glass_Thickness / cm);
        fmt::print("Glass_Width     = {} cm;\n", cfg.Glass_Width / cm);
        fmt::print("Glass_Gap       = {} cm;\n", cfg.Glass_Gap / cm);
        fmt::print("Glass_InnerR    = {} cm;\n", cfg.Glass_InnerR / cm);
        fmt::print("Glass_OuterR    = {} cm;\n", cfg.Glass_OuterR / cm);
        fmt::print("Glass_PosZ      = {} cm;\n", cfg.Glass_PosZ / cm);
        fmt::print("Towers in Row/Col   = {} cm;\n", cfg.Glass_PosZ / cm);
        fmt::print("Top left tower pos  = {:<10} {:<10} cm;\n", -leftTowerPos / cm, topTowerPos / cm);

        // fmt::print("#Towers info:\n");
        // fmt::print("#{:<5} {:<6} {:<3} {:<3} {:>10} {:>10}   {}\n", "idx",  "code", "col", "row", "x", "y", "name");
        int towerIndex = 0;
        for(int rowIndex=0; rowIndex < towersInRow; rowIndex++) {
            for(int colIndex=0; colIndex < towersInRow; colIndex++) {
                double x = leftTowerPos + colIndex * (cfg.Glass_Width + cfg.Glass_Gap);
                double y = topTowerPos + rowIndex * (cfg.Glass_Width + cfg.Glass_Gap);
                double r = sqrt(x * x + y * y);


                if (r < cfg.Glass_OuterR && (std::abs(x) > cfg.Glass_InnerR || std::abs(y) > cfg.Glass_InnerR)) {
                    int code = 1000 * rowIndex + colIndex;
                    std::string name = fmt::format("ce_EMCAL_glass_phys_{}", code);
                    auto placement = new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), name, ce_EMCAL_detGLASS_Logic, Phys, false, 1000000 + code);
                    // fmt::print(" {:<5} {:<6} {:<3} {:<3} {:>10.4f} {:>10.4f}   {}\n", towerIndex, code, colIndex, rowIndex, x / cm, y / cm, name);
                    towerIndex++;
                }
            }
        }
        fmt::print("Total Glass modules: {}\n", towerIndex);
        fmt::print("CE EMCAL GLASS END\n\n");
    }

    G4Tubs *Solid;              // pointer to the solid
    G4LogicalVolume *Logic;     // pointer to the logical
    G4VPhysicalVolume *Phys;    // pointer to the physical

    G4VisAttributes *OuterVolumeVisAttr;

    /// Parameters that was used in the moment of construction
    ce_EMCAL_Config& ConstructionConfig;

    //--- crystals
    G4Material *ce_EMCAL_detPWO_Material;
    G4VisAttributes *attr_ce_EMCAL_detPWO;
    G4Box *ce_EMCAL_detPWO_Solid;
    G4LogicalVolume *ce_EMCAL_detPWO_Logic;

    //---glass
    G4Material *ce_EMCAL_detGLASS_Material;
    G4VisAttributes *attr_ce_EMCAL_detGLASS;
    G4Box *ce_EMCAL_detGLASS_Solid;
    G4LogicalVolume *ce_EMCAL_detGLASS_Logic;


private:
    g4e::Materials *fMat;
};

#endif //G4E_CE_EMCAL_HH
