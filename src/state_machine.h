#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "types.h"
#include "ecs.h"

#include <vector>
#include <unordered_map>

typedef struct State 
{
    bool active;
    int stateID;
} State;

typedef bool (*stateEntryFunction)(EntityID id, State state);
typedef bool (*stateExitFunction)(EntityID id, State state);
typedef bool (*stateRunFunction)(EntityID id, State state);
typedef bool (*stateCheckFunction)(EntityID id, State state);

static bool stateDefaultTrue(EntityID id, State state)
{
    (void) id;
    (void) state;
    return true;
}

typedef struct StateInfo
{
    int id;
    stateExitFunction exit;
    stateEntryFunction entry;
    stateRunFunction run;
    stateCheckFunction check;
} StateInfo;

class StateMachineManager : public System
{
public:
    StateMachineManager() : System(EntityManager::getInstance().newSystem("state_machine_manager"))
    {
        components.set(EntityManager::getInstance().getComponentID<State>());
    }

    virtual void update(float deltaTimeS) override
    {
        (void) deltaTimeS;
        for (EntityID eid : EntityManager::getInstance().getSystemEntities(id))
        {
            State st = EntityManager::getInstance().getComponent<State>(eid);
            stateInfos[st.stateID].run(eid, st);
        }
    }

    int registerState(StateInfo stateInfo)
    {
        int index = stateInfos.size();
        stateInfo.id = index;
        if ( stateInfo.check == nullptr )
        {
            stateInfo.check = stateDefaultTrue;
        }
        if ( stateInfo.run == nullptr )
        {
            stateInfo.run = stateDefaultTrue;
        }
        if ( stateInfo.entry == nullptr )
        {
            stateInfo.entry = stateDefaultTrue;
        }
        if ( stateInfo.exit == nullptr )
        {
            stateInfo.exit = stateDefaultTrue;
        }
        stateInfos.push_back(stateInfo);
        return index;
    }

    int connectStates(std::vector<int> stateList)
    {
        for ( int stateId : stateList )
        {
            connectedStates[stateId] = stateList;
        }
        return stateList[0];
    }

private:
    std::unordered_map<int, std::vector<int>> connectedStates;
    std::vector<StateInfo> stateInfos {};
};

/*
class StateMachineManager
{
public:
    StateMachineManager &getInstance()
    {
        static StateMachineManager smm;
        return smm;
    }
    void update(EntityID eid)
    {
        Component
    }
private:
    StateMachineManager() {}
};

class State
{
public:
    State(EntityID id) : id(id) {}
    virtual bool test();
    virtual void exit();
    virtual void enter();
    virtual void run();
private:
    const EntityID id;
};
*/
#if 0
#define MAX_NUM_STATES 64
class StateMachine
{
public:
    StateMachine(std::vector<const std::string> _stateNames) : maxNumStates(_stateNames.size()), stateNames(_stateNames) {
        int i = 0;
        for ( const std::string &st : _stateNames )
        {
            stateNamesMap.at(st) = i++;
            transitions.push_back(std::bitset<MAX_NUM_STATES>(false));
        }
    }
    void setState(State *st, std::string name, std::vector<std::string> transitions)
    {
        std::map<std::string, int>::const_iterator stateIndex = stateNamesMap.find(name);
        if ( stateIndex != stateNamesMap.end() )
        {
            states[stateIndex->second] = st;
            
            std::bitset<MAX_NUM_STATES> currentTransitions;
            std::map<std::string, int>::const_iterator where;
            for ( const std::string &other : transitions )
            {
                where = stateNamesMap.find(other);
                if ( where != stateNames.end() )
                {
                    currentTransitions.set(where->second);
                }
            }
            transitions[stateIndex->second] = currentTransitions;
        }
    }
    void setActive(const std::string name)
    {
        activeState = stateNamesMap[name].second;
    }
    void update()
    {
        states[activeState]->run();
        bool perform_transition = false;
        int targetState = 0;
        for ( targetState = 0; (not perform_transition) && targetState < stateNames.size(); ++targetState )
        {
            if ( transitions[activeState].test(targetState) )
            {
                if ( states[targetState]->test() )
                {
                    perform_transition = true;
                }
            }
        }
        if ( perform_transition )
        {
            states[activeState]->exit();
            activeState = targetState;
            states[activeState]->enter();
        }
    }
    void log()
    {
        /// TODO
    }
private:
    int activeState;
    const size_t maxNumStates;
    const std::vector<const std::string> stateNames;
    const std::map<const std::string, int> stateNamesMap;
    std::vector<State*> states;
    std::vector<std::bitset<MAX_NUM_STATES>> transitions;
};

#endif

#endif // _STATE_MACHINE_H_