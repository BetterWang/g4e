#include "ce_EMCAL_DigiHit.hh"
#include "G4SystemOfUnits.hh"
#include "G4Poisson.hh"
#include "Randomize.hh"

// Using G4ThreadLocal external JLeicCe_emcalHitAllocator as was suggested in
//
G4ThreadLocal G4Allocator<ce_EMCAL_DigiHit>* JLeicCe_emcalDigiHitAllocator = nullptr;


ce_EMCAL_DigiHit::ce_EMCAL_DigiHit()
{

}

ce_EMCAL_DigiHit::~ce_EMCAL_DigiHit() = default;

//JLeicCe_emcalHit::~JLeicCe_emcalHit(){;}
/*
JLeicCe_emcalHit::JLeicCe_emcalHit(const JLeicCe_emcalHit& right)
{
  EdepAbs = right.EdepAbs; TrackLengthAbs = right.TrackLengthAbs;
  EdepGap = right.EdepGap; TrackLengthGap = right.TrackLengthGap;
}
*/


const ce_EMCAL_DigiHit& ce_EMCAL_DigiHit::operator=(const ce_EMCAL_DigiHit& right)
{

  return *this;
}

ce_EMCAL_DigiHit* ce_EMCAL_DigiHit::CreateHit(ce_EMCAL_Hit* aHit){

    ce_EMCAL_DigiHit *dHit = new ce_EMCAL_DigiHit()  ;
    double ADC_crs;

    //crystal parameters

    string name_det = aHit->GetDetName() ;
    double length_crs =2*aHit->GetZHalfLength(); //
    double Xside_crs=2*aHit->GetXHalfLength();
    double Yside_crs=2*aHit->GetYHalfLength();
    double p[6] = { 0., 0.00680, 0.64, 0.0334, 0.36, 0. }; // PbWO: fast component(in us), % fast, slow comp(in us), % slow
    double att_length_crs = 60000 * cm; // compatible with NO ATT Lenght
    double light_yield_crs = 240 * (1. / MeV);
    double redout_surface_crs = Xside_crs * Yside_crs;
    double birks_constant = 3.2e-3;


    //sensor parameters

    double integration_frac = 1.; //
    double optical_coupling = 0.9;
    double sensor_surface_crs = pow(1.2 * cm, 2);    //SipM 1.2x1.2cm2 with 25um pitch
    double PDE = 0.22;// consider only 25um sipm
    double sipm_pixel_area = pow(0.025*mm,2);
    int sipm_N_pixels = sensor_surface_crs/sipm_pixel_area;


    double veff_crs = 30 / 1.8 * cm / ns;                     // light velocity in crystal


    double etotL_crs = 0;
    double etotR_crs = 0;
    double timeL_crs = 0;
    double timeR_crs = 0;

    double Etot_crs = 0;
    double peR_crs = 0.;   //number of p.e.
    double peR_int_crs;

    double pe_crs = 0.;   //number of p.e.
    double pe_int_crs;

    int Nsamp_int = 250;  // 1.0us
    double smp_t = 4. / 1000.; // Assuming fADC sampling at 250 MHz 1sample every 4ns
    int Nch_digi = 800; //Number of channel for the digitizer

    double time_min_crs[4] = { 0, 0, 0, 0 };
    double* test;
    double tim=0;
    double tim_pbwo=0;
	double* test_pbwo;

    double tdc_conv_crs = 1. / ns;               // TDC conversion factor
    double T_offset_crs = 0 * ns;
    double TDCL_crs = 4096;
    double sigmaTR_crs = 0.;

    vector<G4ThreeVector> Lpos = aHit->GetLPos();
    vector<G4double> Edep = aHit->GetEdep();
    vector<G4double> Dx = aHit->GetDx();
    vector<G4double> times = aHit->GetTime();
    vector<int> charge = aHit->GetCharges();
    unsigned int nsteps = Edep.size();


    double light_coll_crs = sensor_surface_crs / redout_surface_crs;
   if (light_coll_crs > 1) light_coll_crs = 1.;

    for (unsigned int s = 0; s < nsteps; s++) {
        Etot_crs = Etot_crs + Edep[s];
         }


    double Etot_B_crs = 0;


    if (Etot_crs > 0) {
    for (unsigned int s = 0; s < nsteps; s++) {

    double dLeft_crs = length_crs / 2 + Lpos[s].z();            //Downstream (SIPM position )
    double dRight_crs = length_crs / 2 - Lpos[s].z();            //Upstream


    double Edep_B_crs = BirksAttenuation(Edep[s], Dx[s], charge[s], birks_constant);


    Etot_B_crs = Etot_B_crs + Edep_B_crs;

    etotL_crs = etotL_crs + Edep_B_crs / 2 * exp(-length_crs / att_length_crs);
    etotR_crs = etotR_crs + Edep_B_crs / 2 * exp(-length_crs / att_length_crs);

/*
    timeL_crs = timeL_crs + (times[s] + dLeft_crs / veff_crs) / nsteps;
    timeR_crs = timeR_crs + (times[s] + dRight_crs / veff_crs) / nsteps;

    if (etotL_crs > 0.) {
      if (s == 0 || (time_min_crs[0] > (times[s] + dLeft_crs / veff_crs))) time_min_crs[0] = times[s] + dLeft_crs / veff_crs;
                      }
     if (etotR_crs > 0.) {
       if (s == 0 || (time_min_crs[1] > (times[s] + dRight_crs / veff_crs))) time_min_crs[1] = times[s] + dRight_crs / veff_crs;
                       }

*/
    }



    //number of photons hit the sensor surface
    double n_photons = etotR_crs * light_yield_crs* optical_coupling * light_coll_crs;

    //number of fired cells taking into account SiPM saturation effect - NIM A 926 (2019) 16–35;

    double N_fired_cells = sipm_N_pixels * (1 - exp(- n_photons*PDE/sipm_N_pixels));

   //if you want Npe distribution as a function of the time (Waveform) uncommented the lines below
    //Waveform described as a convolution of the single p.e. response with the time distribution of the scintillation signal
    // I commented for the moment it becouse for high energy particle the computational time explodes

    /*
    test_pbwo = WaveFormPbwo(N_fired_cells, &tim_pbwo);
    test = test_pbwo;
    tim = tim_pbwo;

    pe_int_crs = 0.; // WF integration in 1us
     for (unsigned int s = 0; s < Nsamp_int; s++) {
            pe_int_crs = pe_int_crs + test[s];
                }

                */



    int mNpe = G4Poisson(N_fired_cells);
     pe_int_crs =mNpe;


     ADC_crs = - (sipm_N_pixels/PDE)*(log(1-(pe_int_crs/sipm_N_pixels)));


     ADC_crs = ADC_crs/(light_yield_crs* optical_coupling * light_coll_crs*0.5);


   // TDCL_crs = int(tim) + ((time_min_crs[0] + T_offset_crs + G4RandGauss::shoot(0., sigmaTR_crs)) * tdc_conv_crs);



    } //end if (etot_crs>0)


    dHit->SetDetName(name_det);
    dHit->SetEdep(Etot_crs);
    dHit->SetNpe(int(pe_int_crs));
     dHit->SetADC(ADC_crs);
    dHit->SetTDC(TDCL_crs);
  //  dHit->Setwaveform(test_pbwo);




    return dHit;
}



ce_EMCAL_DigiHit::ce_EMCAL_DigiHit(int aHit){


}




double ce_EMCAL_DigiHit::BirksAttenuation(double destep, double stepl, int charge, double birks) {
   //Example of Birk attenuation law in organic scintillators.
   //adapted from Geant3 PHYS337. See MIN 80 (1970) 239-244
   //
    // Taken from GEANT4 examples advanced/amsEcal and extended/electromagnetic/TestEm3
     //
    double response = destep;
    if (birks * destep * stepl * charge != 0.) {
       response = destep / (1. + birks * destep / stepl);
      }
      return response;
          }


double* ce_EMCAL_DigiHit::WaveFormPbwo(double npe, double* time_pbwo) {
    double c = exp(-2.);
   //    double Time;
   double t; // time in usec
   double WF;
   double y;
   double rr;
   int it;
   int Nch_digi = 800; //Number of channel for the digitizer
   static double WFsample[1000]; //Needs to be >  Nch_digi+size of the response to the single pe

   static int isFirst = 1;

   double smp_t = 4. / 1000.; // Assuming fADC sampling at 250 MHz 1sample every 4ns

   double p[6] = { 0., 0.00680, 0.64, 0.0334, 0.36, 0. }; // PbWO: fast component(in us), % fast, slow comp(in us), % slow

    double tau = 15.; // ampli response time constant (in ns)
    double t0 = 0.01; // t0 starting time (in ns)
    double area = (tau / c / 2.);
    double A = 1. / area; // amplitude at mnax (55.41 to have it normalized to integral=1, otherwise the max is at 1)
    //    double threshold=10.*1./area/smp_t/1000.; //time threshold in pe - 1/55.41/smp_t*1000. is the funct max -

    double t_spread = 1. * 0.000; // pream time spread in us
    double A_spread = 1. * 0.4 * A; // pream amp spread (in fraction of 1pe amplitude = A)
    double func = 0.;
    static double frac;	// fraction of pe in Nch_digi


    // Building the waveform
    for (unsigned int s = 0; s < 1000; s++) {
    WFsample[s] = 0;
    }

     // Building the response to a single pe (preamps response)
    static double AmpWF[80];
    if (isFirst) {
      for (unsigned int s = 0; s < 80; s++) {
            t = 1000. * s * smp_t;
            // parametrization of preamp out time is in ns (rise ~10ns decay~80ns) sampled in 160ns or 40 samples
            //func=1./411.5*((1-exp(p1[0]+p1[1]*t))*exp(p1[2]+p1[3]*t)+exp(p1[4]+p1[5]*t)));
            func = (t - t0) * (t - t0) * exp(-(t - t0) / tau) * A / (4 * tau * tau * c) * 0.5 * (abs(t - t0) / (t - t0) + 1);
     // spreading amplitude by apli noise
       AmpWF[s] = smp_t * 1000. * func;

         }
       frac = 1 - ((p[2] * exp(-smp_t * Nch_digi / p[1]) + p[4] * exp(-smp_t * Nch_digi / p[3])));	// fraction of pe in Nch_digi

        isFirst = 0;
         }



     int mNpe = G4Poisson(frac * npe);

     for (unsigned int s = 1; s <= mNpe; s++) {
       y = 1.;
       WF = 0.;
          while (y > WF) {

            rr = (rand() % 1000000 + 1) / 1000000.; // rnd number between 0-1

            t = Nch_digi * smp_t * rr; // extracting over 5000 samples range (5000x4ns=20us)

            WF = (p[2] / p[1] * exp(-t / p[1]) + p[4] / p[3] * exp(-t / p[3])) / (p[2] / p[1] + p[4] / p[3]);
            rr = (rand() % 10000000 + 1) / 10000000.; // rnd number between 0-1
            y = rr;

        }
          cout<< WF<<" "<<y<<endl;
        t = G4RandGauss::shoot(t, t_spread);
        if (t < 0.) t = 0.;
        it = t / smp_t;
        for (unsigned int s = 0; s < 80; s++) {
            t = 1000. * s * smp_t;
            func = AmpWF[s];
            func = G4RandGauss::shoot(func, A_spread);
            if ((s + it) < Nch_digi) WFsample[s + it] = WFsample[s + it] + func;
            }
     }

    // mimicking a CF discriminatorm at 1/3 of the max signal
    *time_pbwo = 0.;
    double time_max = -100;
    int s = 0;
    int s_time_max = 0;
   while (time_max < WFsample[s]) {
       time_max = 1 / 2. * (WFsample[s + 1] + WFsample[s]);
       s_time_max = s;
       *time_pbwo = 1000. * smp_t * s_time_max / 3.;
       s++;
      }

     return WFsample;

}





void ce_EMCAL_DigiHit::Print()
{;}


