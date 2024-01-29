#!/usr/bin/python

import subprocess
import json
import sys
import os

def find_executable(exec_name):
    exe_file = "bin" + os.sep + exec_name
    if not os.path.isfile(exe_file) or not os.access(exe_file, os.X_OK):
        print("Could not find executable: {}".format(exe_file))
        print("Exiting...")
        sys.exit(1)
    return exe_file

def find_check(check_tag, test_output):
    # Remove words from end of check string until match
    check_line = [line for line in test_output if check_tag in line]
    if len(check_line) == 0:
        print("Could not find tag " + check_tag)
        return ''
    return check_line[0]

def run_exec(run_data):
    print("Running " + run_data["name"])

    exe_file = find_executable(run_data["exec"])
    command = [exe_file, run_data["config"], run_data["trace"], run_data["cycles"]]
    command.extend(run_data["overrides"].split(' '))

    try:
        result = subprocess.run(command, capture_output=True, text=True, timeout=60)
    except subprocess.CalledProcessError as e:
        print("Error!")
        return ""
    except subprocess.TimeoutExpired as e:
        print("Timeout!")
        return ""
    return result.stdout