import inspect
from os import path

from g4epy import Geant4Eic
import tempfile

this_file_dir = path.dirname(inspect.stack()[0][1]);

g4e = Geant4Eic()
g4e.config['output'] = 'test_run'
g4e.config['input_file'] = '/home/romanov/Downloads/beagle_eD.txt'
g4e.config['beamline'] = 'jleic'
g4e.config['detector'] = 'jleic'
g4e.config['is_batch'] = True
g4e.config['work_dir'] = path.join(this_file_dir, 'work')
g4e.config['executable_path'] = '/home/romanov/eic/g4e-dev/cmake-build-debug/g4e'
g4e.run()



#file = tempfile.NamedTemporaryFile(suffix='')
#
#file.write('')
#print(file.name)
