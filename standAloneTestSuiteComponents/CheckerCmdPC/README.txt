This CheckerCmdPC directory contains a command context checker for
I++ command files. You can use it to check that the commands in a file
are parsable and are executable if executed in order. The checker
maintains a world model and updates it each time a command is executed.
Commands are checked against the world model.

A Windows XP executable is bin\checkerCmd.exe. Sample command files
are in test_files. The executable may be run either by choosing it
in START|RUN or by giving the command bin\checkerCmd.exe in an MS-DOS
command window.

The executable will print:
turn stepping on? y/n
Enter y to turn stepping on, or n to turn it off.
When stepping is on, the executable parses and checks one command at a
time, waiting for the user to press <Enter> before going on to the
next command. When stepping is off, the executable parses and checks
an entire file of commands without pausing.

The executable will then print:
enter input file name or q to quit
Enter a file name, such as test_files\allCmdOK.prg
After the file has been checked, the executable will prompt again.

The test_files\checkerCmdErrors.prg file runs without error in the
command parser, but it triggers all the errors the command checker
can detect.

We expect that the checker class, defined in source\checkerCmd.h, makes it
self-evident how the checker is intended to be used. The documentation
of main in source\checkerCmd.cpp describes how main uses it.

Descriptions of the format of command strings and command string files
are in the users manual. Command string files have a ".prg" suffix.

The software will compile under Visual C++.

The test_files subdirectory of this directory contains only two
test programs. More test files are available in other directories
in this distribution.

To test your own commands, put them in a file in the required format,
and use the checker as described above.


last updated 8/18/4
T. Kramer

