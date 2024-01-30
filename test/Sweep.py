#!/usr/bin/python

from optparse import OptionParser
import subprocess
import json
import sys
import os
import shutil
import re
from stat_parse import *

def get_tests():
    json_data = open('test/Sweeps.json')
    return json.load(json_data)

def run_test(test_data):
    test_output = run_exec(test_data, timeout=3600)

    test_lines = test_output.split('\n')
    results = [find_check(tag, test_lines) for tag in test_data['checks']]
    results = [r if len(r) > 0 else test_data['checks'][i] + ' ???' for i,r in enumerate(results)]
    print('\n'.join(results))
    print()

    return all([len(r) > 0 for r in results])

def parse_results(test_data, test_output):
    print("Running " + test_data["name"])

    test_lines = test_output.split('\n')
    results = [find_check(tag, test_lines) for tag in test_data['checks']]
    results = [r if len(r) > 0 else test_data['checks'][i] + ' ???' for i,r in enumerate(results)]
    print('\n'.join(results))
    print()

    return all([len(r) > 0 for r in results])

testdata = get_tests()

outputs = run_parallel([t for t in testdata["tests"]])

results = []
for i, test in enumerate(testdata["tests"]):
    results.append(parse_results(test, outputs[i]))

if not all(results):
    sys.exit(-1)