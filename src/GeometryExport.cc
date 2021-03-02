//
// Created by romanov on 1/20/20.
//

#include "GeometryExport.hh"

// TODO set definition from CMake
#define USE_TGEOM 1

#ifdef USE_TGEOM

//G4E
#include <StringHelpers.hh>

// VGM
#include "Geant4GM/volumes/Factory.h"
#include "RootGM/volumes/Factory.h"
#include "XmlVGM/GDMLExporter.h"

// ROOT
#include <TGeoManager.h>

#endif

#include <G4VPhysicalVolume.hh>

void g4e::GeometryExport::Export(const std::string& fileBaseName, G4VPhysicalVolume *motherVolume)
{
#ifdef USE_TGEOM

    auto rootFileName = fileBaseName + ".geo.root";
    auto gdmlFileName = fileBaseName + ".geo.gdml";

    // Export geometry in Root and save it in a file
    // Import Geant4 geometry to VGM
    Geant4GM::Factory g4Factory;

    g4Factory.SetDebug(0);
    g4Factory.Import(motherVolume);

    if(!gGeoManager) {
        gGeoManager = new TGeoManager(fileBaseName.c_str(), (fileBaseName+" geometry").c_str());
    }

    //
    // Export VGM geometry to Root
    RootGM::Factory rtFactory;
    rtFactory.SetDebug(0);
    g4Factory.Export(&rtFactory);

    //
    // Import Geant4 geometry to VGM
    Geant4GM::Factory factory;
    factory.Import(motherVolume);

    // Export VGM geometry in GDML (we now use TGeo GDML export)
    // XmlVGM::GDMLExporter gdmlExporter(&factory);
    // gdmlExporter.SetFileName(gdmlFileName);
    // gdmlExporter.GenerateXMLGeometry();
    //

    TGeoNode* topNode = gGeoManager->GetTopNode();
    EnhanceTGeoVisual(topNode);         // Give geometry nice look in TFile
    gGeoManager->CloseGeometry();
    gGeoManager->Export(rootFileName.c_str(), "detector_geo");
    gGeoManager->Export(gdmlFileName.c_str(), "detector");
#endif

}

void g4e::GeometryExport::EnhanceTGeoVisual(TGeoNode *topNode)
{
    using namespace g4e;
    assert(topNode != nullptr);

    // Just in case, color wheel:
    //https://root.cern/root/html524/TColor.html


    topNode->SetInvisible();

    //Set transparency & color of geometry
    TObjArray *volumes = gGeoManager->GetListOfVolumes();
    for (int i = 0; i < volumes->GetEntriesFast(); i++) {
        auto *volume = dynamic_cast<TGeoVolume *>(volumes->At(i));
        assert(volume != nullptr);

        std::string name(volume->GetName());

        // Default for all elements
        volume->SetLineColor(12);
        //volume->SetTransparency(50);

        // electron end-cup EMCalorimeter
        if(StartsWith(name, "World")) {
            volume->SetInvisible();
        }

        // electron end-cup EMCalorimeter
        if(StartsWith(name, "ce_EMCAL")) {
            volume->SetLineColor(kAzure + 1);
            //volume->SetTransparency(50);
        }

        // electron end-cup EMCalorimeter
        else if(name.find("HCAL") != std::string::npos) {
            volume->SetLineColor(kMagenta - 5);
            //volume->SetTransparency(50);
        }

            // electron end-cup EMCalorimeter
        else if(StartsWith(name, "cb_HCAL")) {
            volume->SetLineColor(kMagenta - 5);
            //volume->SetTransparency(50);
        }
        else if (name.find("DIPOLE") != std::string::npos || name.find("QUAD") != std::string::npos) {
            volume->SetLineColor(kGreen - 1);
        }
    }
}
