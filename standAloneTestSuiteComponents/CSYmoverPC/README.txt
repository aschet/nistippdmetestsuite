This CSYmoverPC directory contains a coordinate system mover for
I++ command files. You can use it with some (NOT ALL!) command files.
It reads a command file written for some specific work volume and
coordinate system orientation and writes a copy of the command file
with a few lines added near the beginning. The output file is
usable for some other specific work volume and coordinate system
orientation. The user provides input to the mover describing this
second coordinate system.

The two commands (four lines) added to the file are of the form:

09997 SetCsyTransformation(PartCsy, 10.000, 11.000, 12.000, 0, 0, 270)
\\
09998 SetCoordSystem(PartCsy)
\\

The arguments to the SetCsyTransformation command above vary according
to the input provided by the user.

A Windows XP executable is bin\CSYmover. A sample command file
is in test_files. The executable may be run either by choosing it in
START|RUN or by giving the command bin\CSYmover in an MS-DOS
command window.

The executable will print:

enter a number:
1. write output file
2. change input file name (currently no input file)
3. change output file name: (currently no output file)
4. change axis pointing right (currently +X)
5. change axis pointing up (currently +Z)
6. change coordinates of middle of table (currently(0.000, 0.000, 0.000))
7. quit
enter choice => 

The user should then enter a number between 1 and 7. The executable
engages the user in a brief dialog if any of the items 2 through 6
is chosen.

Before the executable will write an output file (item 1) the user
must specify the names of the input file and output file (items 2 and 3).

The executable will not write an output file if the axis pointing
right (item 4) is not perpendicular to the axis pointing up (item 5).

The allowed axis names are: +X +Y +Z -X -Y -Z

For item 6, the coordinates of a point should be entered in the
format (x, y, z). I.e., there should be three numbers in parentheses,
separated by two commas.

The executable exits only when item 7 (quit) is chosen. In particular, it
does not exit automatically after writing an output file (item 1).

The software will compile under Visual C++. 

The test_files subdirectory of this directory contains only one
test program. More test files are available in other directories
in this distribution.

The CSYmover utility is intended to be used with I++ command files that
are written so that:
1. Only machine coordinates are used.
2. All coordinate values are between -500 and +500.
3. The positive Z axis points up from the table of the CMM.
4. The positive X axis is parallel to the front of the table
   and points to the right.

The CSYmover utility should not be used on any command file that includes:
1. a SetCoordSystem(XXX) command, where XXX is not PartCsy, or
2. a SetCsyTransformation(PartCsy...) command. 
Such commands will undo what the utility does.

last updated 8/18/2004
T. Kramer

