```mermaid
graph TD
    IDLE((IDLE))
    RUNNING((RUNNING))
    FAULT((FAULT))

    IDLE -- "START\n[onStart]" --> RUNNING
    RUNNING -- "STOP (paro de emergencia)\n[onFault]" --> FAULT
    FAULT -- "RESET\n[onReset]" --> IDLE

    IDLE -. "NONE / STOP / RESET\n[onNoOp]" .-> IDLE
    RUNNING -. "NONE / START / RESET\n[onNoOp]" .-> RUNNING
    FAULT -. "NONE / START / STOP\n[onNoOp]" .-> FAULT
```

## Matriz de transiciones (Estado x Evento)

| Estado \ Evento | NONE | START | STOP | RESET |
|---|---|---|---|---|
| IDLE | IDLE | RUNNING (onStart) | IDLE | IDLE |
| RUNNING | RUNNING | RUNNING | FAULT (onFault) | RUNNING |
| FAULT | FAULT | FAULT | FAULT | IDLE (onReset) |

## Flujo de datos

```mermaid
graph TD
    BTN[Microswitch IO33] --> ISR[ISR IRAM_ATTR: CCOUNT + debounce 25 ms]
    ISR --> FLAG[Bandera volatile + spinlock]
    FLAG --> LOOP[loop: readEvent]
    SER[Serial: s / r] --> LOOP
    LOOP --> FSM[fsmDispatch: TransitionFSM O1]
    FSM --> HAL[hal_motor: PWM / freno / coast]
```