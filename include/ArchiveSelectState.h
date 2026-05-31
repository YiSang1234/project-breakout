#pragma once
#include "IState.h"

class ArchiveSelectState : public IState {
private:
    int selectedSlot;
    bool isSaveMode;
public:
    explicit ArchiveSelectState(Game* game);
    void SetMode(bool saveMode) { isSaveMode = saveMode; }
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};