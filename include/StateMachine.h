#pragma once
#include "IState.h"
#include "GameStateType.h"
#include <unordered_map>
#include <memory>
#include <stdexcept>

class Game;

class StateMachine {
private:
    std::unordered_map<GameStateType, std::unique_ptr<IState>> states;
    IState* currentState = nullptr;
    Game* game;

public:
    explicit StateMachine(Game* g) : game(g) {}

    void RegisterState(GameStateType type, IState* state) {
        states[type] = std::unique_ptr<IState>(state);
    }

    void SwitchTo(GameStateType newType) {
        if (currentState) currentState->OnExit();
        auto it = states.find(newType);
        if (it == states.end()) throw std::runtime_error("State not registered");
        currentState = it->second.get();
        currentState->OnEnter();
    }

    void Update(float dt) { if (currentState) currentState->Update(dt); }
    void Draw() { if (currentState) currentState->Draw(); }
    IState* GetCurrentState() const { return currentState; }
};