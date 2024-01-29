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
    test_output = run_exec(test_data)

    test_lines = test_output.split('\n')
    results = [find_check(tag, test_lines) for tag in test_data['checks']]
    results = [r if len(r) > 0 else test_data['checks'][i] + ' ???' for i,r in enumerate(results)]
    print('\n'.join(results))
    print()

    return all([len(r) > 0 for r in results])

testdata = get_tests()

results = []
for test in testdata["tests"]:
    results.append(run_test(test))

if not all(results):
    sys.exit(-1)