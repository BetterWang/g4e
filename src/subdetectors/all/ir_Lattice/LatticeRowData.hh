#ifndef G4E_LATTICEROWDATA_HH
#define G4E_LATTICEROWDATA_HH

/**
 * JLeic lattice consists of a table which looks like this:
 *
 *   Element name	Type	Length [m]	Good field half-aperture [cm]	Inner Half-A [cm]	Outer Radius [cm]	Dipole field [T]		Quadrupole field [T/m]		Sextupole [T/m^2]	Solenoid [T]	X center [m]	Y center [m]	Z center [m]	Theta center [rad]	Phi [rad]
 *   Upstream elements						Bx	By	Normal	Skew
 *   iASUS	SOLENOID	1.6	3	4	12	0	0	0	0	0	2	0.640	0	-12.784	-0.05	0
 *   iQUS3S	QUADRUPOLE	0.5	3	4	12	0	0	0	0.98	0	0	0.567	0	-11.336	-0.05	0
 *   ...
 *
 *   This class holds data for a single row of this data file
 */


struct LatticeRowData {
    char *ffqsNAME; 
    float ffqsSizeZDi; 
    float ffqsRinDiG; 
    float ffqsRinDi; 
    float ffqsRoutDi; 
    float qFIELDx; 
    float qFIELDy;
    float qFIELQn; 
    float qFIELQs; 
    float qFIELSek; 
    float qFIELSol; 
    float ffqsX; 
    float ffqsY; 
    float ffqsZ; 
    float ffqsTheta;
    float ffqsPhi;
};

#endif //G4E_LATTICEROWDATA_HH
