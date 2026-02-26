#ifndef MOONLIT_BEHAVIOURBANK_H
#define MOONLIT_BEHAVIOURBANK_H

#include "ObjectBehaviour.h"

constexpr int DEFAULT_BEHAVIOUR_BANK_SIZE = 256;

template <Moonlit::IsBehaviour BEHAVIOUR_TYPE>
class BehaviourBank {
public:
    BehaviourBank() {
        m_behaviours.reserve(DEFAULT_BEHAVIOUR_BANK_SIZE);
    };
    ~BehaviourBank() = default;

    static BehaviourBank Get() {
        static BehaviourBank instance;
        return instance;
    }

    BEHAVIOUR_TYPE& AddBehaviour();
    BEHAVIOUR_TYPE& AddBehaviour(const BEHAVIOUR_TYPE& _behaviour);
    template <typename... Args>
    BEHAVIOUR_TYPE& AddBehaviour(Args&&... args);

    void DeleteBehaviour(const Moonlit::GameObject& _obj);
    void DeleteBehaviour(const BEHAVIOUR_TYPE& _behaviour);
    BEHAVIOUR_TYPE* GetBehaviour(const Moonlit::GameObject& _obj);
protected:
    void DeleteBehaviour(std::vector<BEHAVIOUR_TYPE>::iterator _it);

    std::vector<BEHAVIOUR_TYPE> m_behaviours;
};

template <Moonlit::IsBehaviour BEHAVIOUR_TYPE>
BEHAVIOUR_TYPE& BehaviourBank<BEHAVIOUR_TYPE>::AddBehaviour()
{
    m_behaviours.emplace_back(BEHAVIOUR_TYPE());
    return m_behaviours.back();
}

template <Moonlit::IsBehaviour BEHAVIOUR_TYPE>
BEHAVIOUR_TYPE& BehaviourBank<BEHAVIOUR_TYPE>::AddBehaviour(const BEHAVIOUR_TYPE& _behaviour) {
    m_behaviours.push_back(_behaviour);
    return m_behaviours.back();
}

template <Moonlit::IsBehaviour BEHAVIOUR_TYPE>
template <typename... Args>
BEHAVIOUR_TYPE& BehaviourBank<BEHAVIOUR_TYPE>::AddBehaviour(Args&&... args) {
    m_behaviours.emplace_back(std::forward<Args>(args)...);
    return m_behaviours.back();
}

template <Moonlit::IsBehaviour BEHAVIOUR_TYPE>
void BehaviourBank<BEHAVIOUR_TYPE>::DeleteBehaviour(const Moonlit::GameObject& _obj)
{
    auto it = std::find_if(m_behaviours.begin(), m_behaviours.end(), [&_obj](const BEHAVIOUR_TYPE& behaviour) {
        return behaviour.GetOwner() == &_obj;
    });
    if (it != m_behaviours.end()) {
        DeleteBehaviour(it);
    }
}

template <Moonlit::IsBehaviour BEHAVIOUR_TYPE>
void BehaviourBank<BEHAVIOUR_TYPE>::DeleteBehaviour(const BEHAVIOUR_TYPE& _behaviour){
    auto it = std::find(m_behaviours.begin(), m_behaviours.end(), _behaviour);
    if (it != m_behaviours.end()) {
        DeleteBehaviour(it);
    }
}

template <Moonlit::IsBehaviour BEHAVIOUR_TYPE>
void BehaviourBank<BEHAVIOUR_TYPE>::DeleteBehaviour(std::vector<BEHAVIOUR_TYPE>::iterator _it) {
    m_behaviours.erase(_it);
}

template <Moonlit::IsBehaviour BEHAVIOUR_TYPE>
BEHAVIOUR_TYPE* BehaviourBank<BEHAVIOUR_TYPE>::GetBehaviour(const Moonlit::GameObject& _obj) {
    auto it = std::find_if(m_behaviours.begin(), m_behaviours.end(), [&_obj](const BEHAVIOUR_TYPE& behaviour) {
        return behaviour.GetOwner() == &_obj;
    });
    if (it != m_behaviours.end()) {
        return &(*it);
    }
    return nullptr;
}

#endif //MOONLIT_BEHAVIOURBANK_H