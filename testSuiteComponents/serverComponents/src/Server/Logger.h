#ifndef LOGGER_H
#define LOGGER_H


#include <fstream> 
#include <iostream.h>

class Logger {

private:
std::fstream LogFile; 
  char buffer[65536];
  bool logDetails;
  char loggedMessage[65536];
  char duff[20];
  
  
  
  
public:
	    Logger(){

		}
	   ~Logger(){}

	   void openFile(CString fileName){
		  LogFile.open(fileName, ios::out);
		}	

	   void writeFile(char *out){
		    time_t t;
			t = time(NULL);
			strftime(buffer, sizeof(buffer), "%y:%m:%d_%H:%M:%S ", localtime(&t));
			LogFile.write(buffer, strlen(buffer));
			LogFile.write(out, strlen(out)).put('\n');

			sprintf(loggedMessage,"%s",out);

		//	sprintf(loggedMessage,"%s%s",buffer,out);
		//	memset(buffer, 0, sizeof(buffer));
		//sprintf(buffer,"%s",out);
			
		//	LogFile.write(out, strlen(out));
		//	sprintf(duff,"\\\\");
		//	LogFile.write(duff, strlen(duff)).put('\n');
	   }
	   void closeFile(){
			
		   LogFile.close();
	   }

	   void setLogDetails(bool ld) { logDetails = ld; }
	   bool getLogDetails(){return logDetails;}
	   char * getLoggedMessage() {return loggedMessage;}

	  void replaceQuotes(char* in){
		int quoteCount = 0;
		for(int i=0; i<strlen(in); i++)
			if(in[i] == '"') in[i]='#';
		in[strlen(in)-2] = NULL;
	  }
  
};
#endif /* ifndef LOGGER_H */