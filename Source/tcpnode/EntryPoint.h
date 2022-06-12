#include <string>
#include <vector>
#include <unordered_map>

#include "ImGui/ImLog.h"
#include "ImGui/IconsFontAwesome.h"
#include "ImGui/MyComponents.h"
#include "imgui.h"

#include "Application.h"
#include "TCPThread.h"

/* 需要在服务端、客户端程序中分别定义 */
extern void newTCPThread(std::unordered_map<std::string, Network::TCPThread*>* tabs);
extern bool newThread;
extern std::unordered_map<std::string, Network::TCPThread*> TCPThreads;
extern std::string name;


void UI();
int main();
