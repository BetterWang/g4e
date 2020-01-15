import os
import inspect
from setuptools import setup
from g4epy.version import version as ejpm_version

# The directory containing this file
this_script_dir = os.path.dirname(inspect.stack()[0][1])

# The text of the README file
with open(os.path.join(this_script_dir, "pip_readme.md"), 'r') as readme_file:
    readme = readme_file.read()

# This call to setup() does all the work
setup(
    name="g4epy",
    version=ejpm_version,
    description="Geant4Eic api and control through python",
    long_description=readme,
    long_description_content_type="text/markdown",
    url="https://gitlab.com/jlab-eic/g4e",
    author="Dmitry Romanov",
    author_email="romanov@jlab.org",
    license="MIT",
    classifiers=[
        # How mature is this project? Common values are
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        'Development Status :: 3 - Alpha',

        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
    ],
    packages=["g4epy"],
    include_package_data=True,
#    setup_requires=["click", "appdirs"],
#    install_requires=["click", "appdirs"],
#    entry_points={
#        "console_scripts": [
#            "janapy=janapy:ejpm_cli",
#        ]
#    },
)