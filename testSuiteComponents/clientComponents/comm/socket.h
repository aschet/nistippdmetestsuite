// Socket class header file

#include <winsock.h>

class Socket {

public:

	Socket();
	Socket(char *hostName, int portNumber);
	~Socket();

	int setNonBlocking();
	int createSocket();
	int connectSocket();
	void closeSocket();
	int readSocket(char *replybuf);
	int writeSocket(char *sendbuf);
	int setHostName(char *hostName);
	void setPortNumber(int portNumber);


private:

	LPHOSTENT m_lpHostEntry;
	SOCKET	m_theSocket;
	SOCKADDR_IN m_saServer;
	int m_portNumber;


};

