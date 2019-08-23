#include "globalstate.h"

int GlobalState::globalState = FREE;

void GlobalState::setGlobalState(int gs)
{
    globalState = gs;
}

int GlobalState::getGlobalState() const
{
    return globalState;
}
