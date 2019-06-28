# Geant 4 EIC = g4e

Jefferson Lab EIC Geant4 simulation
-------------------------------------------------------------------

     =========================================================
     Geant4 - an Object-Oriented Toolkit for Simulation in HEP
     =========================================================

                            JLEIC geant 4 
                            -------------


![Design picture](doc/JLEICgeant4-v1a.png)

[[_TOC_]] 

<!-- MarkdownTOC autolink="true" -->
items here


## Installation


*ejpm* - EIC Jana Packet Manager

```
ejpm install g4e
```

Manual installation



 
## INTRODUCTION

<!-- /MarkdownTOC -->

 3- GEOMETRY DEFINITION
 
  	
 4- AN EVENT : THE PRIMARY GENERATOR
  	
 5. DETECTOR RESPONSE

  				
 6 - PHYSICS DEMO
 
 	
 5- HOW TO START ?
 
	
 	- execute g4e in 'interactive' mode with visualization e.g.
 		% g4e
		....
		Idle> type your commands
		....
        - execute g4e in 'batch' mode  
                  ( set number of events in jleic.mac, example   /run/beamOn 100) 
                 % g4e jleic.mac
                 

		
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


