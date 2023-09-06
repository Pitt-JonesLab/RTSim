import subprocess

# Build nvmain.fast
code = subprocess.run(["scons", "--build-type=fast"]).returncode

if code != 0:
    print("nvmain build failed!")
    exit(1)

# Run hello_world.nvt
code = subprocess.run(["./nvmain.fast", "Config/RM.config", "Tests/Traces/hello_world.nvt", "0"], capture_output=True, text=True)
output = code.stdout
code = code.returncode

if code != 0:
    print("Test run failed!")
    print(output)
    exit(1)

print(len(output))

expected_strings = ["i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.subArrayEnergy 929741nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.activeEnergy 7500.27nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.burstEnergy 5267.33nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.writeEnergy 913360nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.refreshEnergy 0nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.averagePausesPerRequest 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.measuredPauses 48275",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.averagePausedRequestProgress 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.measuredProgresses 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.reads 49675",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.writes 48275",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.shiftReqs 97950",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.shiftEnergy 3613.27nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.totalnumShifts 5929472",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.activates 93641",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.precharges 93640",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.refreshes 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.worstCaseEndurance 18446744073709551615",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.averageEndurance 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.actWaits 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.actWaitTotal 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.actWaitAverage -nan",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.averageWriteTime 4",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.subarray0.measuredWriteTimes 48275",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.bankEnergy 929741nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.activeEnergy 7500.27nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.burstEnergy 5267.33nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.refreshEnergy 0nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.bankPower 5.3677e+08W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.activePower 4.33015e+06W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.burstPower 3.04101e+06W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.refreshPower 0W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.bandwidth 28953.5MB/s",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.dataCycles 391800",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.powerCycles 1732102",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.utilization 0.226199",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.reads 49675",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.writes 48275",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.activates 93641",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.precharges 93640",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.refreshes 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.activeCycles 1577342",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.standbyCycles 154760",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.fastExitActiveCycles 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.fastExitPrechargeCycles 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.slowExitPrechargeCycles 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.actWaits 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.actWaitTotal 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.actWaitAverage -nan",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.averageEndurance 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.bank0.worstCaseEndurance 18446744073709551615",
"i0.defaultMemory.channel0.RTM.channel0.rank0.totalEnergy 1.09751e+06nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.backgroundEnergy 154999nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.activateEnergy 7500.27nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.burstEnergy 5267.33nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.refreshEnergy 0nJ",
"i0.defaultMemory.channel0.RTM.channel0.rank0.totalPower 0.193715W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.backgroundPower 0.178973W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.activatePower 0.00866031W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.burstPower 0.00608201W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.refreshPower 0W",
"i0.defaultMemory.channel0.RTM.channel0.rank0.reads 49675",
"i0.defaultMemory.channel0.RTM.channel0.rank0.writes 48275",
"i0.defaultMemory.channel0.RTM.channel0.rank0.activeCycles 1577342",
"i0.defaultMemory.channel0.RTM.channel0.rank0.standbyCycles 154760",
"i0.defaultMemory.channel0.RTM.channel0.rank0.fastExitActiveCycles 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.fastExitPrechargeCycles 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.slowExitCycles 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.actWaits 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.actWaitTotal 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.actWaitAverage -nan",
"i0.defaultMemory.channel0.RTM.channel0.rank0.rrdWaits 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.rrdWaitTotal 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.rrdWaitAverage -nan",
"i0.defaultMemory.channel0.RTM.channel0.rank0.fawWaits 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.fawWaitTotal 0",
"i0.defaultMemory.channel0.RTM.channel0.rank0.fawWaitAverage -nan",
"i0.defaultMemory.channel0.RTM.mem_reads 49675",
"i0.defaultMemory.channel0.RTM.mem_writes 48275",
"i0.defaultMemory.channel0.RTM.rb_hits 4309",
"i0.defaultMemory.channel0.RTM.rb_miss 93641",
"i0.defaultMemory.channel0.RTM.starvation_precharges 76",
"i0.defaultMemory.channel0.RTM.averageLatency 23.0075",
"i0.defaultMemory.channel0.RTM.averageQueueLatency 3.18591",
"i0.defaultMemory.channel0.RTM.averageTotalLatency 26.1934",
"i0.defaultMemory.channel0.RTM.measuredLatencies 97950",
"i0.defaultMemory.channel0.RTM.measuredQueueLatencies 97950",
"i0.defaultMemory.channel0.RTM.measuredTotalLatencies 97950",
"i0.defaultMemory.channel0.RTM.write_pauses 0",
"i0.defaultMemory.channel0.RTM.simulation_cycles 1732102",
"i0.defaultMemory.channel0.RTM.wakeupCount 524878",
"i0.defaultMemory.totalReadRequests 49675",
"i0.defaultMemory.totalWriteRequests 48275",
"i0.defaultMemory.successfulPrefetches 0",
"i0.defaultMemory.unsuccessfulPrefetches 0",
"Exiting at cycle 1732102 because simCycles 0 reached."]

checks = [s in output for s in expected_strings]
bad_lines = [i for i,val in enumerate(checks) if val is False]
test = all(checks)

if not test:
    print("Output not as expected!")
    for i in bad_lines:
        print("Expected: " + expected_strings[i])
    exit(1)