import codecs
import inspect
import json
import argparse
import os
from Cheetah.Template import Template

#argument parser
parser = argparse.ArgumentParser()
parser.add_argument("json_file", help="File with configs")
parser.add_argument("output_directory", help="Directory for generated files")
args = parser.parse_args()

#open config file
config_file = codecs.open(args.json_file, encoding='cp1251')
configs = json.load(config_file, encoding='cp1251')
config_file.close()

this_folder = os.path.realpath(os.path.abspath(os.path.split(inspect.getfile(inspect.currentframe()))[0]))

#generate .h and .cpp code
for ext in ('h', 'cpp'):
    template_file = codecs.open(os.path.join(this_folder, ext + "_template.tmpl"), encoding='utf-8')
    template = Template(template_file.read(), searchList=[configs])
    output_filename = os.path.join(args.output_directory, configs['OutputFileName']+'.'+ext)
    output_file = codecs.open(output_filename, 'w', encoding='cp1251')
    output_file.write(template.respond())
    output_file.close()
    template_file.close()