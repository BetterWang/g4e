# Multi-threading

G4E supports multi threaded mode. By default it runs in single threaded 
mode. Adding flag `--threads` or `-t` enables multi-threaded mode:

```
g4e --threads=4
```


In terms of Geant4 this flag works like this:

```c++
    // Constructing the run manager...    
    if(args.ThreadsCount > 1) {
        // Multi-threaded run manager
        auto runManager = new G4MTRunManager;
        mtRunManager->SetNumberOfThreads(args.ThreadsCount);
    } else {
        // Single threaded mode
        runManager = new G4RunManager;
    }
```

Besides the Geant documentation, a very good explanation can be found here:

[Cern wiki QuickMigrationGuideForGeant4V10](https://twiki.cern.ch/twiki/bin/view/Geant4/QuickMigrationGuideForGeant4V10#ThreadSafety)