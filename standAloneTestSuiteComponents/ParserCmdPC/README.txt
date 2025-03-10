This ParserCmdPC directory contains a PC-format stand-alone parser
for I++ command files. You can use it to check that commands are
parsable.

A Windows XP executable is bin\parserCmd.exe. Sample command files are
in test_files. The executable may be run either by choosing it in
START|RUN or by giving the command bin\parserCmd.exe in an MS-DOS
command window.

The executable will print:
turn stepping on? y/n
Enter y to turn stepping on, or n to turn it off.
When stepping is on, the executable parses one command at a time,
waiting for the user to press <Enter> before going on to the next
command. When stepping is off, the executable parses an entire file of
commands without pausing.

The executable will then print:
enter input file name or q to quit
Enter a file name, such as test_files\allCmdOK.prg
After the file has been parsed, the executable will prompt again.

The documentation of the parserCmd class in source\parserCmd.h
describes how the parser is intended to be used. The documentation
of main in source\parserCmd.cpp describes how main uses it.

Descriptions of the format of command strings and command string files
are in the users manual. Command string files have a ".prg" suffix.

The software will compile under Visual C++.

The test_files subdirectory of this directory contains four
test programs. More test files are available in other directories
in this distribution.

To test your own commands, put them in a file in the required format,
and use the parser as described above.

last updated 8/18/4
T. Kramer

