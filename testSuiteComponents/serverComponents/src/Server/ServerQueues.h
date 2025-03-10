#ifndef SERVERQUE_H
#define SERVERQUE_H

#include <queue>

using namespace std;

typedef vector <Response *> ResponseList; 


class ServerQueues
{
private:

  bool command_existent;
  int fastKey, slowKey, respKey;      // Keys for hanshaking
  vector <int> tagList;      // List of tag numbers in use
  vector <int>::iterator p;  // iterator for traversing tagList
  queue <char *> fastQue;             // Event commands go here
  queue <char *> slowQue;             // Command commands go here
  queue <char *> responseQue;         // All responses go here
  

public:

  ServerQueues(){ 
    tagList.reserve(50);
    fastKey=111;
    slowKey=222;
    respKey=333;
    command_existent=false;
  }
  ~ServerQueues(){}

  bool getCommandExistence() { return command_existent; }

  void setCommandExistence(bool cmd) { command_existent = cmd; }

  void addTag(int tag_num)
  {
	    tagList.push_back(tag_num);
  }


  // Remove a tag from the tag list
  void delTag(int tag_num)
  {
	for (p = tagList.begin(); p != tagList.end(); p++)
	{
	  if (*p == tag_num)
	  {
		tagList.erase(p);
		break;
	  }
	}
  }

  // Check to see if a tag number is currently in the tag list
  bool tagInList(int tag_num)
  {
	for (p = tagList.begin(); p != tagList.end(); p++)
	{
	  if (*p == tag_num)
	  break;
	} 
	return ((p == tagList.end()) ? false : true);   
  }


  // Check out the fast key
  int getFastKey()
  {
	int key;
	key = fastKey;
	fastKey = 0;
	return key;
  }

  // Check out the slow key
  int getSlowKey()
  {
	int key;
	key = slowKey;
	slowKey = 0;
	return key;
  }

  // Check out the response key
  int getRespKey()
  {
	int key;
	key = respKey;
	respKey = 0;
	return key;
  }

  // Put an event on the event queue
  bool enqFast(int key, char * a_string) 
  {
	if(key == 111){
      fastKey=0;
	  fastQue.push(a_string);
	  fastKey= 111;
	  return true;
	}
	else return false;
  }

  // Put a command on the command queue
  bool enqSlow(int key, char * a_string) 
  {
	if(key==222){
      slowKey=0;
      slowQue.push(a_string);
      slowKey=222;
      return true;
	}
	else return false;
  }

  // Put a response on the response queue
  bool enqResp(int key, char * a_string)
  {
	if(key == 333){
      respKey=0;
      responseQue.push(a_string);
      respKey=333;
      return true;
	}
	else return false;
  }

  // Remove an event from the event queue
  char * deqFast(int key) 
  {
	char * a_string;
	a_string=NULL;
	if(key == 111){	  
	  a_string = fastQue.front();
	  fastQue.pop();
	  fastKey=111;
	}
	return a_string;
  }

  // Remove a command from the command queue
  char * deqSlow(int key) 
  {
	  char * a_string;
	  a_string=NULL;
	  if(key==222){
		a_string = slowQue.front();
		slowQue.pop();
		slowKey=222;
	  }
	  return a_string;
  }

  // Remove a response from the response queue
  char * deqResp(int key) 
  {
	  char * a_string;
	  a_string=NULL;
	  if(key==333){
		a_string = responseQue.front();
		responseQue.pop();
		respKey=333;
	  }
	  return a_string;
  }


  // Check for data on a queue
  bool fastQueHasData() { return !fastQue.empty(); }
  bool slowQueHasData() { return !slowQue.empty(); }
  bool respQueHasData() { return !responseQue.empty(); }
  

  // Clear all queues in the case of an abort or a mid session client disconnect
  void clearAllQueues()
  {
	while (fastQue.empty() == false) { fastQue.pop(); }
	while (slowQue.empty() == false) { slowQue.pop(); }
	while (responseQue.empty() == false) { responseQue.pop(); }
	tagList.erase(tagList.begin(), tagList.end());
  }


};



#endif /* #ifndef SERVERQUE_H */
