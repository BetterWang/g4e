#ifndef G4E_GEOMETRYEXPORT_HH
#define G4E_GEOMETRYEXPORT_HH

/**
 * The class provides function to export Geometry to a number of files.
 * At this point it has only one function, which uses VGM to export geometry to various formats
 * Usage:
 *     GeometryExport::Export("jleic", worldVolume);
 * Result: created files:
 *    jleic.geo.root
 *    jleic.geo.gdml
 */

#include <string>

class G4VPhysicalVolume;
class TGeoNode;

namespace g4e {
    class GeometryExport
    {
    public:
        /** Exports geometry of motherVolume to a file <fileBaseName>.geo.<...> */
        static void Export(const std::string& fileBaseName, G4VPhysicalVolume *motherVolume);

    private:
        /** paint it black!
         *
         * Applies visual styles for the geometry
         *
         * @param topNode TGeoManager top node
         */
        static void EnhanceTGeoVisual(TGeoNode *topNode);
    };
}


#endif //G4E_GEOMETRYEXPORT_HH
