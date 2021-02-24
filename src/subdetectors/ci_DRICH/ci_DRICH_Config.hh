#ifndef ci_DRICH_Config_HH
#define ci_DRICH_Config_HH

struct ci_DRICH_Config {
    // define here Global volume parameters
    double RIn ;
    double ROut  = 150 * cm;
    double ThicknessZ = 170 * cm;
    double PosZ ;
};


#endif //ci_DRICH_Config_HH
