#include "game_client1.h"

namespace game_client {
	
	game_client* game_client::instance_ = nullptr;
	std::mutex game_client::instance_mutex_;
	
	game_client& game_client::get_instance() {
		std::lock_guard<std::mutex> lock(instance_mutex_);
		if (!instance_) {
			instance_ = new game_client();
		}
		return *instance_;
	}
	
	game_client::game_client() : initialized_(false), sock_(INVALID_SOCKET), stopped_(false) {
		init_winsock();
	}
	
	game_client::~game_client() {
		close();
		WSACleanup();
	}
	
	void game_client::init_winsock() {
		WSADATA wsaData;
		int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (res != 0) {
			throw std::runtime_error("WSAStartup failed");
		}
	}
	
	void game_client::connect(const char* server_ip, int port) {
		if (initialized_) {
			throw std::runtime_error("already connected");
		}
		init_socket(server_ip, port);
		std::thread t(&game_client::run_connect_thread, this);
		t.detach();
	}
	
	void game_client::disconnect() {
		if (recv_thread_.joinable()) {
			stopped_ = true;
			recv_thread_.join();
		}
		if (sock_ != INVALID_SOCKET) {
			closesocket(sock_);
			sock_ = INVALID_SOCKET;
		}
		initialized_ = false;
	}
	
	void game_client::send(const char* data, int size) {
		if (!initialized_) {
			throw std::runtime_error("not connected");
		}
		int res = ::send(sock_, data, size, 0);
		if (res == SOCKET_ERROR) {
			throw std::runtime_error("send failed");
		}
	}
	
	void game_client::close() {
		if (recv_thread_.joinable()) {
			stopped_ = true;
			recv_thread_.join();
		}
		if (sock_ != INVALID_SOCKET) {
			closesocket(sock_);
			sock_ = INVALID_SOCKET;
		}
		initialized_ = false;
	}
	
	void game_client::init_socket(const char* server_ip, int port) {
		sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock_ == INVALID_SOCKET) {
			throw std::runtime_error("socket failed");
		}
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		//inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
		server_addr.sin_addr.s_addr = inet_addr(server_ip);
	}

	void game_client::run_connect_thread() {
		std::thread t([this]() {
			int res = ::connect(sock_, (sockaddr*)&server_addr, sizeof(server_addr));
			if (res == SOCKET_ERROR) {
				throw std::runtime_error("connect failed");
			}
			initialized_ = true;
			run_recv_thread();
		});
		t.detach();
	}
	
	void game_client::run_recv_thread() {
		recv_thread_ = std::thread(recv_thread_func, this);
	}
	
	void game_client::handle_recv_data(const char* data, int size) {
		// handle received data
	}
	
	DWORD WINAPI game_client::recv_thread_func(LPVOID lpParam) {
		game_client* client = reinterpret_cast<game_client*>(lpParam);
		while (!client->stopped_) {
			char buffer[1024];
			int res = recv(client->sock_, buffer, sizeof(buffer), 0);
			if (res == SOCKET_ERROR || res == 0) {
				client->close();
				break;
			}
			client->handle_recv_data(buffer, res);
		}
		return 0;
	}
	
} // namespace game_client

