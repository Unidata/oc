# This file is configured by CMake automatically as DartConfiguration.tcl
# If you choose not to use CMake, this file may be hand configured, by
# filling in the required variables.


# Configuration directories and files
SourceDirectory: /cygdrive/f/svn/oc2.0
BuildDirectory: /cygdrive/f/svn/oc2.0/tests

# Where to place the cost data store
CostDataFile: 

# Site is something like machine.domain, i.e. pragmatic.crd
Site: blackbird

# Build name is osname-revision-compiler, i.e. Linux-2.4.2-2smp-c++
BuildName: CYGWIN_NT-6.1-WOW64-1.7.16(0.262/5/3)-i686

# Submission information
IsCDash: 
CDashVersion: 
QueryCDashVersion: 
DropSite: 
DropLocation: 
DropSiteUser: 
DropSitePassword: 
DropSiteMode: 
DropMethod: http
TriggerSite: 
ScpCommand: /usr/bin/scp.exe

# Dashboard start time
NightlyStartTime: 00:00:00 EDT

# Commands for the build/test/submit cycle
ConfigureCommand: "/usr/bin/cmake.exe" "/cygdrive/f/svn/oc2.0"
MakeCommand: /usr/bin/make.exe -i
DefaultCTestConfigurationType: Release

# CVS options
# Default is "-d -P -A"
CVSCommand: /usr/bin/cvs.exe
CVSUpdateOptions: -d -A -P

# Subversion options
SVNCommand: /cygdrive/t/SlikSvn/bin/svn.exe
SVNUpdateOptions: 

# Git options
GITCommand: /cygdrive/t/Git/cmd/git.exe
GITUpdateOptions: 
GITUpdateCustom: 

# Generic update command
UpdateCommand: /cygdrive/t/SlikSvn/bin/svn.exe
UpdateOptions: 
UpdateType: svn

# Compiler info
Compiler: /usr/bin/c++.exe

# Dynamic analysis (MemCheck)
PurifyCommand: 
ValgrindCommand: 
ValgrindCommandOptions: 
MemoryCheckCommand: MEMORYCHECK_COMMAND-NOTFOUND
MemoryCheckCommandOptions: 
MemoryCheckSuppressionFile: 

# Coverage
CoverageCommand: /usr/bin/gcov.exe
CoverageExtraFlags: -l

# Cluster commands
SlurmBatchCommand: SLURM_SBATCH_COMMAND-NOTFOUND
SlurmRunCommand: SLURM_SRUN_COMMAND-NOTFOUND

# Testing options
# TimeOut is the amount of time in seconds to wait for processes
# to complete during testing.  After TimeOut seconds, the
# process will be summarily terminated.
# Currently set to 25 minutes
TimeOut: 1500

UseLaunchers: 
CurlOptions: 
# warning, if you add new options here that have to do with submit,
# you have to update cmCTestSubmitCommand.cxx

# For CTest submissions that timeout, these options
# specify behavior for retrying the submission
CTestSubmitRetryDelay: 5
CTestSubmitRetryCount: 3
