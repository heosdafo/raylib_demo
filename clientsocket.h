#ifndef _client_socket_h_
#define _client_socket_h_

#include <winsock2.h>
#include <iostream>

class client_socket {
public:
	static client_socket& instance();
	void connect_to_server(const char* server_ip, int server_port);
	void send_data(const char* data, int data_size);
	void close();
	bool is_connected() const;
	void set_receive_callback(void (*callback)(const char*, int));
	void receive_thread();
	void async_receive();
	
private:
	client_socket();
	~client_socket();
	client_socket(const client_socket&);
	client_socket& operator=(const client_socket&);
	bool connect_to_server_internal(const char* server_ip, int server_port);
	static DWORD WINAPI receive_thread_func(LPVOID lpParameter);
	static void async_receive_callback(DWORD error, DWORD bytes_transferred, LPWSAOVERLAPPED overlapped, DWORD flags);
	SOCKET m_socket;
	WSADATA m_wsa_data;
	bool m_connected;
	HANDLE m_receive_thread_handle;
	DWORD m_receive_thread_id;
	bool m_async_receiving;
	void (*m_receive_callback)(const char*, int);
	WSAOVERLAPPED m_receive_overlapped;
	//char m_receive_buffer[4096];
	WSABUF m_receive_buffer;
};

#endif
