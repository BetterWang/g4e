# JLEICgeant4

Jefferson Lab EIC Geant4 simulation
-------------------------------------------------------------------

     =========================================================
     Geant4 - an Object-Oriented Toolkit for Simulation in HEP
     =========================================================

                            JLEIC geant 4 
                            -------------


![Design picture](doc/JLEICgeant4-v1a.png)

 0- INTRODUCTION
	

	
 1- GEOMETRY DEFINITION
 
  	
 2- AN EVENT : THE PRIMARY GENERATOR
  	
 3- DETECTOR RESPONSE

  				
 4- PHYSICS DEMO
 
 	
 5- HOW TO START ?
 
	
 	- execute JLEICgeant4 in 'interactive' mode with visualization e.g.
 		% JLEICgeant4
		....
		Idle> type your commands
		....
        - execute JLEICgeant4 in 'batch' mode  
                  ( set number of events in jleic.mac, example   /run/beamOn 100) 
                 % JLEICgeant4 jleic.mac
                 

		
 List of the built-in histograms
 -------------------------------

        1.   number of (tracking) steps/event
        2.   energy deposit distribution in the absorber (in MeV)
        3.   angle distribution of the primary particle at the exit
             of the absorber (deg)
        4.   distribution of the lateral displacement at exit(mm)
        5.   kinetic energy of the transmitted primaries (MeV)
        6.   angle distribution of the backscattered primaries (deg)
        7.   kinetic energy of the backscattered primary particles (MeV)
        8.   kinetic energy of the charged secondary particles (MeV)
        9.   z distribution of the secondary charged vertices (mm)
       10.   kinetic energy of the photons escaping the absorber (MeV)


 Using histograms
 ---------------- 

