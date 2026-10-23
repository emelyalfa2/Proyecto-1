// FASE 4 - FSM: estados, eventos, transiciones y handlers
#include "fsm_control.h"
#include "hal_motor.h"

namespace {
constexpr uint32_t RUN_DUTY = 700;       // ~68 % de 1023
State currentState = State::IDLE;

constexpr size_t NS = static_cast<size_t>(State::COUNT);
constexpr size_t NE = static_cast<size_t>(Event::COUNT);

const char* const STATE_NAMES[NS] = {"IDLE", "RUNNING", "FAULT"};
const char* const EVENT_NAMES[NE] = {"NONE", "START", "STOP", "RESET"};
}  // namespace

// ---------- acciones (handlers) ----------
void onStart() { motorPWM(RUN_DUTY); }
void onFault() { motorBrake(); }        // frenado dinamico ante paro de emergencia
void onReset() { motorStop(); }         // deja el motor libre (coast)
void onNoOp()  {}

// ---------- matriz de transiciones: filas = Estado, columnas = Evento ----------
//                      NONE                      START                      STOP                       RESET
constexpr Transition TransitionFSM[NS][NE] = {
/* IDLE    */ { {State::IDLE,    onNoOp}, {State::RUNNING, onStart}, {State::IDLE,    onNoOp},  {State::IDLE,    onNoOp}  },
/* RUNNING */ { {State::RUNNING, onNoOp}, {State::RUNNING, onNoOp},  {State::FAULT,   onFault}, {State::RUNNING, onNoOp}  },
/* FAULT   */ { {State::FAULT,   onNoOp}, {State::FAULT,   onNoOp},  {State::FAULT,   onNoOp},  {State::IDLE,    onReset} },
};

void fsmInit() {
  currentState = State::IDLE;
  motorStop();
}

// Acceso directo por indice: sin if/else ni switch -> tiempo constante O(1)
void fsmDispatch(Event ev) {
  const Transition& t = TransitionFSM[static_cast<size_t>(currentState)][static_cast<size_t>(ev)];
  t.action();
  currentState = t.nextState;
}

State fsmGetState() { return currentState; }

const char* fsmStateName(State s) { return STATE_NAMES[static_cast<size_t>(s)]; }
const char* fsmEventName(Event e) { return EVENT_NAMES[static_cast<size_t>(e)]; }