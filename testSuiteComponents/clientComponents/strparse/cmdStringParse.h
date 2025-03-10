#include <iostream.h>
#include <fstream.h>

int readWholeNum(char *line);

double readRealNum(char *line);

void find_delimiter(char delimiter, char *buffer, 
					char *word, char *out_buffer);

void logError(int c_id, char severity, int code, 
			  fstream Logfile);

bool check_message(char *buffer, char *word, 
				   char *out_buffer);

void writeLogFile(char *buffer, fstream Logfile);
