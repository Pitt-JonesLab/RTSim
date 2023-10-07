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

def run_test(test_data, nvmainexec):
    command = [nvmainexec, test_data["config"], test_data["trace"], test_data["cycles"]]
    command.extend(test_data["overrides"].split(' '))

    try:
        result = subprocess.run(command, capture_output=True, text=True)
    except subprocess.CalledProcessError as e:
        print("Error!")
        return
    print("Done!")

    checks = [s in result.stdout for s in test_data["checks"]]
    passed = all(checks)
    print("Passed {}/{} checks".format(sum(checks), len(test_data["checks"])))
    if not passed:
        sys.exit(1)

nvmainexec = find_executable()
testdata = get_tests()

for test in testdata["tests"]:
    sys.stdout.write(test["name"] + " ... ")
    run_test(test, nvmainexec)