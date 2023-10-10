#!/usr/bin/python

from optparse import OptionParser
import subprocess
import json
import sys
import os
import shutil
import re

parser = OptionParser()
parser.add_option("-b", "--build", type="string", help="RTSim build to test (e.g., *.fast, *.prof, *.debug)", default="fast")

(options, args) = parser.parse_args()

def find_executable():
    nvmainexec = "bin" + os.sep + "nvmain." + options.build
    if not os.path.isfile(nvmainexec) or not os.access(nvmainexec, os.X_OK):
        print("Could not find Nvmain executable: {}".format(nvmainexec))
        print("Exiting...")
        sys.exit(1)
    return nvmainexec

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

def run_test(test_data, nvmainexec):
    command = [nvmainexec, test_data["config"], test_data["trace"], test_data["cycles"]]
    command.extend(test_data["overrides"].split(' '))

    try:
        result = subprocess.run(command, capture_output=True, text=True)
    except subprocess.CalledProcessError as e:
        print("Error!")
        return
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
        sys.exit(1)

nvmainexec = find_executable()
testdata = get_tests()

for test in testdata["tests"]:
    sys.stdout.write(test["name"] + " ... ")
    run_test(test, nvmainexec)