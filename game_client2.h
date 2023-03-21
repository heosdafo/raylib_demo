#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <winsock2.h>
#include <thread>
#include <mutex>

namespace game_client {
	
	class game_client {
	public:
		game_client(const game_client&) = delete;
		game_client& operator=(const game_client&) = delete;
		static game_client& get_instance();
		void connect(const char* server_ip, int port);
		void send(const char* data, int size);
		void close();
		
	private:
		game_client();
		~game_client();
		
		void init_winsock();
		void init_socket(const char* server_ip, int port);
		void run_connect_thread();
		void run_recv_thread();
		void handle_recv_data(const char* data, int size);
		void disconnect();
		static DWORD WINAPI recv_thread_func(LPVOID lpParam);
		
		static game_client* instance_;
		static std::mutex instance_mutex_;
		struct sockaddr_in server_addr;
		bool initialized_;
		SOCKET sock_;
		std::thread recv_thread_;
		bool stopped_;
	};
	
} // namespace game_client

#endif // GAME_CLIENT_H

