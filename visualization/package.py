#!/usr/bin/env python3

import argparse
import pathlib

import numpy as np


def normalize(s):
    for char in ('-', ' ', '+', '-'):
        s = s.replace(char, '_')
    return s


def make_key(label, identifier):
    return normalize(label) + "_" + normalize(identifier)


def gather_data(path):
    data = {}

    path = pathlib.Path(path)
    for subdir in path.iterdir():
        if not subdir.is_dir():
            continue
        for filename in subdir.glob('**/*.swc'):
            p = filename.relative_to(path)
            identifier = p.with_suffix('').stem
            label = ' '.join(x.name for x in reversed(p.parents)).strip()
            key = make_key(label, identifier)

            data[key] = np.loadtxt(str(filename))

            print('Packaging {}...'.format(key))

    return data


if __name__ == '__main__':
    description = (
        "Pre-processes and gathers *.swc files into a single archive for use "
        "with neuronplot.py."
        )

    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('path', help='Path to the data directory.')
    parser.add_argument('destination', help='Name of output file.')
    args = parser.parse_args()

    data = gather_data(args.path)
    np.savez(args.destination, **data)
