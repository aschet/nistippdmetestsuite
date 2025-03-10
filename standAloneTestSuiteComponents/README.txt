This standAloneTestSuiteComponents directory has four stand-alone
utilities for helping develop clients and servers using the I++
DME Interface specification. The utilities implement version 1.4
of the specification.

The functions of the utilities are:
CheckerCmd -  checks that commands are parsable and executable in order.
CSYmover - modifies a command file to use a different coordinate system.
ParserCmd -  checks that commands are parsable.
ParserRes -  checks that responses are parsable.

Each utility has a simple text-based user interface. All the interfaces
are similar.

CheckerCmd uses ParserCmd, so any command that is OK with
CheckerCmd must be OK with ParserCmd.

The utilities are provided in PC format and in Unix format. Each
utility is in a separate subdirectory. The subdirectories are:

CheckerCmdPC
CheckerCmdUnix
CSYmoverPC
CSYmoverUnix
ParserCmdPC
ParserCmdUnix
ParserResPC
ParserResUnix

All files in the Unix subdirectories are in Unix format. The Unix
subdirectories include Sun Solaris executables. All files in the
PC subdirectories are in PC format. The PC subdirectories include
PC executables (built in Windows XP).

Each subdirectory is entirely self-contained and requires no source
code or other files from any other source. All source code for
compiling an executable is included in each subdirectory. The Unix
source compiles using the GNU C++ compiler. The PC source code
compiles using Visual C++. Any other modern C++ compiler should
also compile the source code.

Each subdirectory has a README file explaining what the utility is
used for and how to use it.

This directory also has an "artifacts" subdirectory containing
information about test parts.

last updated 8/18/4
T. Kramer
