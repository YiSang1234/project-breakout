#include "AsyncLoadManager.h"
#include <thread>
#include <chrono>

AsyncLoadManager::AsyncLoadManager()
    : ballLoading(false), ballComplete(false),
      brickLoading(false), brickComplete(false) {}

// 小球实现
void AsyncLoadManager::StartBallAsyncLoad() {
    if (ballLoading) return;
    ballLoading = true;
    ballComplete = false;
    ballFuture = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    });
}

void AsyncLoadManager::UpdateBall() {
    if (!ballLoading) return;
    if (ballFuture.valid() && ballFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        ballFuture.get();
        ballLoading = false;
        ballComplete = true;
    }
}

void AsyncLoadManager::ResetBall() {
    ballLoading = false;
    ballComplete = false;
}

// 砖块实现
void AsyncLoadManager::StartBrickAsyncLoad() {
    if (brickLoading) return;
    brickLoading = true;
    brickComplete = false;
    brickFuture = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    });
}

void AsyncLoadManager::UpdateBrick() {
    if (!brickLoading) return;
    if (brickFuture.valid() && brickFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        brickFuture.get();
        brickLoading = false;
        brickComplete = true;
    }
}

void AsyncLoadManager::ResetBrick() {
    brickLoading = false;
    brickComplete = false;
}

// 统一更新
void AsyncLoadManager::Update() {
    UpdateBall();
    UpdateBrick();
}