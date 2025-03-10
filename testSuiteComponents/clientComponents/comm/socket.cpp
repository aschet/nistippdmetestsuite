// Socket class implementation file
#include <afx.h>
#include "socket.h"

Socket::Socket() 
{
	// Set default port number
	m_portNumber = 1294;
}

Socket::Socket(char *hostName, int portNumber)
{  
	m_lpHostEntry = gethostbyname(hostName);
	m_portNumber = portNumber;
	m_saServer.sin_family = AF_INET;
	m_saServer.sin_addr = *((LPIN_ADDR)*m_lpHostEntry->h_addr_list);
										// ^ Server's address
	
	m_saServer.sin_port = htons(m_portNumber);	// Port number from command line
	
}

Socket::~Socket()
{  
	closesocket(m_theSocket);
}

int Socket::setNonBlocking()
{
	unsigned long arg = 1;
	if (ioctlsocket(m_theSocket, FIONBIO, &arg)<0)
		return WSAGetLastError();
	else 
		return 0;
	
}
int Socket::connectSocket()
{
	int nRet;

	nRet = connect(m_theSocket,				// Socket
				  (LPSOCKADDR)&m_saServer,	// Server address
				  sizeof(struct sockaddr));	// Length of server address structure
	

	if (nRet == SOCKET_ERROR)
		return WSAGetLastError();

	else
		return 0;
}

int Socket::readSocket(char *replybuf)
{
	int nRet;
	int x =0;
	char *read_character = NULL;
	int exit = 0;

	// Read the socket one character at a time until
	// a <CR> and <LF> are read
	//   '\r'     '\n'

	nRet = recv(m_theSocket,			// Connected socket
			replybuf,				// Receive buffer
			65535,
			//sizeof(replybuf),		// Size of receive buffer
			0);						// Flags

	if (nRet == SOCKET_ERROR)
		return WSAGetLastError();
		
	else 
		return 0;
	
}

int Socket::writeSocket(char *sendbuf)
{
	int nRet;
		
	//Write line out socket
	nRet = send(m_theSocket,			// Connected socket
				sendbuf,				// Data buffer
				strlen(sendbuf),		// Length of data
				0);						// Flags
	
	if (nRet == SOCKET_ERROR)
		return WSAGetLastError();
			
	else
		return 0;
}
	

int Socket::setHostName(char *hostName)
{
	m_lpHostEntry = gethostbyname(hostName);
	if (m_lpHostEntry == NULL)
		return -1;
		
	else {

	m_saServer.sin_family = AF_INET;
	m_saServer.sin_addr = *((LPIN_ADDR)*m_lpHostEntry->h_addr_list);
										// ^ Server's address
	return 0;
	}
}

void Socket::setPortNumber(int portNumber)
{

	m_portNumber = portNumber;
	m_saServer.sin_port = htons(m_portNumber);	

}

int Socket::createSocket()
{
m_theSocket = socket(AF_INET,			// Address family
					 SOCK_STREAM,		// Socket type
					 IPPROTO_TCP);		// Protocol

return m_theSocket;
}

void Socket::closeSocket()
{
	closesocket(m_theSocket);
}

