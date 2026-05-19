//
// Created by Timotee on 07/05/2026.
//

#include "UserModuleTestBehaviour.h"

void UserModuleTestBehaviour::Update(const float _deltaTime)
{
    ObjectBehaviour::Update(_deltaTime);
    LOG_INFO("Update with dt: " + std::to_string(_deltaTime));
}
