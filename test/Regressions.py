#!/usr/bin/python

from optparse import OptionParser
from stat_parse import *
import subprocess
import json
import sys
import os
import shutil
import re

parser = OptionParser()
parser.add_option("-b", "--build", type="string", help="RTSim build to test (e.g., *.fast, *.prof, *.debug)", default="fast")

(options, args) = parser.parse_args()

def get_tests():
    json_data = open('test/Tests.json')
    return json.load(json_data)

def find_bad_check(check_line, test_output):
    # Remove words from end of check string until match
    check_line = ' '.join(check_line.split(' ')[:-1])
    while (check_line):
        bad_line = [line for line in test_output if check_line in line]
        if len(bad_line) == 0:
            check_line = ' '.join(check_line.split(' ')[:-1])
            continue
        return bad_line[0]
    return ''

def run_test(test_data):
    exe_file = find_executable(test_data["exec"])
    command = [exe_file, test_data["config"], test_data["trace"], test_data["cycles"]]
    command.extend(test_data["overrides"].split(' '))

    try:
        result = subprocess.run(command, capture_output=True, text=True, timeout=60)
    except subprocess.CalledProcessError as e:
        print("Error!")
        sys.exit(1)
    except subprocess.TimeoutExpired as e:
        print("Timeout!")
        sys.exit(1)
    print("Done!")

    test_lines = result.stdout.split('\n')
    checks = [s in test_lines for s in test_data["checks"]]
    passed = all(checks)
    print("Passed {}/{} checks".format(sum(checks), len(test_data["checks"])))

    bad_checks = [i for i, check in enumerate(checks) if not check]

    for check_i in bad_checks:
        print("Expected: " + test_data["checks"][check_i])
        bad_line = find_bad_check(test_data["checks"][check_i], test_lines)
        if bad_line:
            print("Received: " + bad_line)
        else:
            print("Could not locate bad line...")
        print('--------------------')

    if not passed:
        return False
    return True

testdata = get_tests()

results = []
for test in testdata["tests"]:
    sys.stdout.write(test["name"] + "...")
    sys.stdout.flush()
    results.append(run_test(test))

if not all(results):
    sys.exit(-1)