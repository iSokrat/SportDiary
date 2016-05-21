#ifndef STATES_H
#define STATES_H

/* Структура состояний
*/
struct States
{
    enum class ChangeStates:char { isChanged, notChanged };

public:
    ChangeStates changeState = ChangeStates::isChanged;

};

#endif // STATES_H
