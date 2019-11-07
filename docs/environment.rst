``G4E_HOME`` - **required** - should point to a place, where 'resources'
folder is located. 

This environment variable is required to find ``resources`` folder, which
contains static files such as magnetic fields, accelerator latice, etc.
If not set, g4e will look for resource files in the 
current working directory.


``G4E_MACRO_PATH`` - **optional** - allows to add aditional paths
where *.mac files are checked. The rule is the same as system PATH
(colon separated list of pahts)
