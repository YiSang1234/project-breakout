#pragma once
#include <enet/enet.h>
#include <functional>
#include <cstdint>
#include <vector>

enum class NetRole { None, Host, Client };
enum class NetMessageType : uint8_t {
    Connect,           // 客户端连接请求
    PlayerInput,       // 客户端发送挡板位置
    GameState,         // 主机广播游戏状态
    StartGame,         // 主机通知开始游戏
    Disconnect
};

#pragma pack(push, 1)
struct PlayerInputMsg {
    float paddleX;      // 玩家2（客户端）的挡板X位置
};
struct GameStateMsg {
    float ballX, ballY;
    float ballVX, ballVY;
    float paddle1X, paddle2X;
    int score;
    int lives;
    int currentLevel;
    // 简化砖块状态（位图，最多支持256个砖块）
    uint8_t brickBits[32]; // 256 bits = 32 bytes
    uint32_t brickCount;
};
#pragma pack(pop)

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    bool Init();
    void Shutdown();

    // 创建主机
    bool CreateHost(uint16_t port = 12345);
    // 连接主机
    bool ConnectToHost(const char* ip, uint16_t port = 12345);
    void Disconnect();

    void Update();  // 轮询网络事件，调用回调

    void SendPlayerInput(float paddleX);
    void SendGameState(const GameStateMsg& state);
    void SendStartGame();

    void SetOnReceiveInput(std::function<void(float)> cb);
    void SetOnReceiveGameState(std::function<void(const GameStateMsg&)> cb);
    void SetOnClientConnected(std::function<void()> cb);
    void SetOnClientDisconnected(std::function<void()> cb);
    void SetOnStartGame(std::function<void()> cb);

    NetRole GetRole() const { return role; }
    bool IsConnected() const { return connected; }
private:
    void PollEvents();
    void SendPacket(ENetPeer* peer, const void* data, size_t size, uint8_t channel);

    ENetHost* host;
    ENetPeer* serverPeer; // 仅客户端使用
    NetRole role;
    bool connected;

    std::function<void(float)> onReceiveInput;
    std::function<void(const GameStateMsg&)> onReceiveGameState;
    std::function<void()> onClientConnected;
    std::function<void()> onClientDisconnected;
    std::function<void()> onStartGame;
};