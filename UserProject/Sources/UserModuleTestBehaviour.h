//
// Created by Timotee on 07/05/2026.
//

#ifndef MOONLIT_USERMODULETESTBEHAVIOUR_H
#define MOONLIT_USERMODULETESTBEHAVIOUR_H
#include <Engine/Component/ObjectBehaviour.h>


class UserModuleTestBehaviour
    : public Moonlit::ObjectBehaviour
{
public:
    UserModuleTestBehaviour(Moonlit::GameObject* _owner)
        : Moonlit::ObjectBehaviour(_owner) {};

    void Update(const float _deltaTime) override;
private:
};


#endif //MOONLIT_USERMODULETESTBEHAVIOUR_H