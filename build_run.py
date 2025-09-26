#!/usr/bin/python3

# TODO:
# - only run if building succeeds

from argparse import ArgumentParser
from os import makedirs, listdir, path
from subprocess import run

FILE = path.basename(__file__)
EXE_NAME = 'Glen'

build_types = ['Debug', 'Release']

parser = ArgumentParser(FILE)
parser.add_argument(
    'build_type',
    help="Build configuration. Either 'Debug' or 'Release'",
    type=str,
    choices=build_types)
parser.add_argument(
    "-r", "--reconfigure", 
    action="store_true", 
    help='Force reconfiguration'
)

build_type = parser.parse_args().build_type
reconfigure = parser.parse_args().reconfigure

build_dir = './build/' + build_type

makedirs(build_dir, exist_ok=True)

# Check if the build type has been configured yet
if reconfigure == True or len(listdir(build_dir)) == 0:
    # If not, configure now
    print(f'{FILE}: Configuring...')
    run(['cmake', '../..', '-DCMAKE_BUILD_TYPE=' + build_type], cwd=build_dir)

# Build
print(f'{FILE}: Building...')
return_code = run(['cmake', '--build', build_dir]).returncode

if return_code == 0:
    # Run
    print(f'{FILE}: Running...')
    try:
        run([build_dir + '/' + EXE_NAME])
    except KeyboardInterrupt:
        print(f'{FILE}: Program was interrupted')

