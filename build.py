#!/usr/bin/python3

from argparse import ArgumentParser
from os import makedirs, listdir, path
from subprocess import run

build_types = ['Debug', 'Release']

parser = ArgumentParser('build.py')
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
    print(f'{path.basename(__file__)}: Configuring...')
    run(['cmake', '../..', '-DCMAKE_BUILD_TYPE=' + build_type], cwd=build_dir)

# Build
print(f'{path.basename(__file__)}: Building...')
run(['cmake', '--build', build_dir])

