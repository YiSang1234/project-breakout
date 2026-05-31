#pragma once
#include <future>
#include <mutex>

class AsyncLoadManager {
private:
    std::future<void> ballFuture;
    bool ballLoading;
    bool ballComplete;

    std::future<void> brickFuture;
    bool brickLoading;
    bool brickComplete;

    std::mutex mtx;

public:
    AsyncLoadManager();

    // 小球
    void StartBallAsyncLoad();
    bool IsBallLoading() const { return ballLoading; }
    bool IsBallComplete() const { return ballComplete; }
    void UpdateBall();
    void ResetBall();

    // 砖块
    void StartBrickAsyncLoad();
    bool IsBrickLoading() const { return brickLoading; }
    bool IsBrickComplete() const { return brickComplete; }
    void UpdateBrick();
    void ResetBrick();

    // 统一更新
    void Update();
};