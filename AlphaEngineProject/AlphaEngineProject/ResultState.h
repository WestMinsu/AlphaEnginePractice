#pragma once
#include "IGameState.h"
#include <string>

class ResultState : public IGameState
{
public:
    void Enter(GameManager* gameManager) override;
    void Update(GameManager* gameManager, f32 dt) override;
    void Draw(GameManager* gameManager) override;
    void Exit(GameManager* gameManager) override;
    void SetWinMessage(const std::string& message);

private:
    std::string m_displayMessage;
};