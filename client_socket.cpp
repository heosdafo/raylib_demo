#include "client_socket.h"

client_socket::client_socket() : m_socket(INVALID_SOCKET), m_connected(false), m_receive_thread_handle(NULL), m_receive_thread_id(0), m_async_receiving(false), m_receive_callback(NULL) {
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa_data) != 0) {
		std::cerr << "WSAStartup failed" << std::endl;
	}
	
	m_receive_buffer.buf = new char[4096];
	m_receive_buffer.len = 4096;
}

client_socket::~client_socket() {
	close();
	delete[] m_receive_buffer.buf;
	WSACleanup();
}

client_socket& client_socket::instance() {
	static client_socket s_instance;
	return s_instance;
}

void client_socket::connect_to_server(const char* server_ip, int server_port) {
	if (!m_connected) {
		if (connect_to_server_internal(server_ip, server_port)) {
			m_connected = true;
			if (m_async_receiving) {
				async_receive();
			}
			else {
				receive_thread();
			}
		}
	}
}

bool client_socket::connect_to_server_internal(const char* server_ip, int server_port) {
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) {
		std::cerr << "socket failed" << std::endl;
		return false;
	}
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	if (connect(m_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		std::cerr << "connect failed" << std::endl;
		closesocket(m_socket);
		return false;
	}
	return true;
}

void client_socket::send_data(const char* data, int data_size) {
	if (m_connected) {
		send(m_socket, data, data_size, 0);
	}
}

void client_socket::close() {
	if (m_connected) {
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
		m_connected = false;
	}
}

bool client_socket::is_connected() const {
	return m_connected;
}

void client_socket::set_receive_callback(void (*callback)(const char*, int)) {
	m_receive_callback = callback;
}

void client_socket::receive_thread() {
	m_receive_thread_handle = CreateThread(NULL, 0, receive_thread_func, (LPVOID)this, 0, &m_receive_thread_id);
	if (m_receive_thread_handle == NULL) {
		std::cerr << "CreateThread failed" << std::endl;
		return;
	}
}

DWORD WINAPI client_socket::receive_thread_func(LPVOID lpParameter) {
	client_socket* pThis = (client_socket*)lpParameter;
	char buffer[4096];
	while (pThis->m_connected) {
		int bytes_received = recv(pThis->m_socket, buffer, sizeof(buffer), 0);
		if (bytes_received == SOCKET_ERROR) {
			std::cerr << "recv failed" << std::endl;
			pThis->close();
			break;
		}
		if (bytes_received == 0) {
			pThis->close();
			break;
		}
		if (pThis->m_receive_callback) {
			pThis->m_receive_callback(buffer, bytes_received);
		}
	}
	return 0;
}

void client_socket::async_receive() {
	memset(&m_receive_overlapped, 0, sizeof(m_receive_overlapped));
	m_receive_overlapped.hEvent = WSACreateEvent();
	DWORD flags = 0;
	DWORD bytes_received = 0;
	//WSARecv(m_socket, &m_receive_buffer[0], sizeof(m_receive_buffer), &bytes_received, &flags, &m_receive_overlapped, async_receive_callback);
	WSARecv(m_socket, &m_receive_buffer, 1, &bytes_received, &flags, &m_receive_overlapped, async_receive_callback);

	
	m_async_receiving = true;
}


void client_socket::async_receive_callback(DWORD error, DWORD bytes_transferred, LPWSAOVERLAPPED overlapped, DWORD flags) {
	client_socket* pThis = reinterpret_cast<client_socket*>(overlapped->hEvent);
	if (error == 0 && bytes_transferred > 0) {
		if (pThis->m_receive_callback) {
			char* receive_buffer = reinterpret_cast<char*>(pThis->m_receive_buffer.buf);
			pThis->m_receive_callback(receive_buffer, bytes_transferred);
		}

		DWORD bytes_received = 0;
		WSARecv(pThis->m_socket, &pThis->m_receive_buffer, 1, &bytes_received, &flags, &pThis->m_receive_overlapped, async_receive_callback);
	}
	else {
		std::cerr << "async_receive_callback error: " << error << std::endl;
		pThis->close();
	}
}
