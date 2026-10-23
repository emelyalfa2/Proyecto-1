// FASE 4 - FSM orientada a eventos (matriz Estado x Evento, despacho O(1))
#pragma once
#include <stdint.h>
#include <stddef.h>

enum class State : uint8_t {
    IDLE = 0,
    RUNNING,
    FAULT,
    COUNT
};

enum class Event : uint8_t {
    NONE = 0,
    START,
    STOP,     // paro de emergencia (ISR en IO33)
    RESET,
    COUNT
};

struct Transition {
    State nextState;
    void (*action)();
};

extern const Transition TransitionFSM[static_cast<size_t>(State::COUNT)][static_cast<size_t>(Event::COUNT)];

// Handlers de accion
void onStart();
void onFault();
void onReset();
void onNoOp();

// API
void        fsmInit();
void        fsmDispatch(Event ev);
State       fsmGetState();
const char* fsmStateName(State s);
const char* fsmEventName(Event e);