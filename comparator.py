#!/usr/bin/env python3

import sys
import argparse

def read_tc(file):
    path = next(file).strip()
    mem = int(next(file).strip())
    order = list(map(int, next(file).strip().split(' ')))
    return path, mem, order

def read_file(path):
    file = open(path, 'r')
    s = {}
    try:
      while(True):
          path, mem, order = read_tc(file)
          s[path] = (mem, order)
    except StopIteration:
        pass
    return s

def get_or_default(dict, key, default):
    try:
        return dict[key]
    except KeyError:
        return default
    return default

def main():
    # Parse args
    parser = argparse.ArgumentParser(description='Read and compare two output files then print in Markdown')
    parser.add_argument("file1", help='first file path to be compared')
    parser.add_argument("file2", help='second file path to be compared')
    args = parser.parse_args()

    # Read data
    data1 = read_file(args.file1)
    data2 = read_file(args.file1)

    # Print header
    OUTPUT_FMT = '| {:20} | {:>8} | {:>8} | {:>8} | {:>8} |'
    print(OUTPUT_FMT.format('TC', 'result1', 'result2', 'diff', 'diff%'))
    print('|----------------------|---------:|---------:|---------:|---------:|')

    # Print content
    union_keys = set(data1.keys()) | set(data2.keys())
    union_keys = sorted(list(union_keys))
    for key in union_keys:
        v1 = get_or_default(data1, key, (-1, []))[0]
        v2 = get_or_default(data2, key, (-1, []))[0]
        v1 = v1 if v1 != -1 else '.'
        v2 = v2 if v2 != -1 else '.'
        diff, diffp = (v2 - v1, '{:>6.1f}'.format(v2 * 100.0 / v1)) if isinstance(v1, int) and isinstance(v2, int) else ('.', '.')
        print(OUTPUT_FMT.format(key, v1, v2, diff, diffp))


if __name__ == "__main__":
    main()
