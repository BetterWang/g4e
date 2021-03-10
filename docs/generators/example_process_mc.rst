
.. code:: python

    #------------------------------------------------------------------------
    # Example macro. Process one of
    #------------------------------------------------------------------------

    # Include default setup
    # Use jleic.mac for headless mode or jleicvis.mac for GUI visualization
    /control/execute refdet.mac

    # Select Beagle generator
    # Options are: particleGun hepmcAscii pythiaAscii beagle
    /generator/select beagle

    # Open file and run some MC file
    /generator/beagle/open beagle_test_data.txt
    /generator/beagle/verbose 2

    /run/initialize
    /run/beamOn 1
    exit