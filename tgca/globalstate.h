#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#define FREE 0
#define BUSY 1

class GlobalState
{
private:
    static int globalState;
public:
    void setGlobalState(int);
    int getGlobalState() const;
};

#endif // GLOBALSTATE_H
