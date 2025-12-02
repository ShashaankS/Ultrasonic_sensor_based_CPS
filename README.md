# Ultrasonic Water Level Controller

## 1. Problem Statement

A water tank requires automatic control of its filling operation using an ultrasonic distance sensor. The sensor measures the distance to the water surface, and based on this distance, a pump must be turned ON or OFF. The system must:

* Stabilize noisy ultrasonic readings using a Kalman Filter.
* Implement a Finite State Machine (FSM) with states: **FILL**, **HOLD**, **STOP**.
* Trigger the correct state transitions based on filtered sensor data.
* Enter a **STOP** state if the sensor gives abnormal readings (failsafe).
* Provide clean modular software architecture using separate libraries.

The main goal is a robust and noise‑resistant water‑level controller suitable for embedded systems.

---

## 2. Melee Machine Diagram (Overall System Architecture)

```
 ┌──────────────────────────────┐
 │     Ultrasonic Sensor        │
 │  (HC-SR04 / JSN-SR04T)       │
 └─────────────┬────────────────┘
               │ Raw Distance
               ▼
 ┌──────────────────────────────┐
 │       Kalman Filter          │
 │  (Smooths noisy readings)    │
 └─────────────┬────────────────┘
               │ Filtered Distance
               ▼
 ┌──────────────────────────────┐
 │      Tank FSM Module         │
 │  FILL / HOLD / STOP states   │
 └─────────────┬────────────────┘
               │ Pump Control Signal
               ▼
 ┌──────────────────────────────┐
 │           Relay              │
 │      (Pump ON / OFF)         │
 └──────────────────────────────┘
```

---

## 3. FSM Diagram (FILL–HOLD–STOP)

```
                     ┌────────────┐
                     │   STOP     │
                     └─────┬──────┘
                           │ valid reading
                           ▼
                     ┌────────────┐
             ┌──────▶│   HOLD     │─────────────┐
             │       └─────┬──────┘             │
             │             │             distance > FILL 
 distance ≤ HOLD           ▼                    │
             │       ┌────────────┐             │
             └───────│   FILL     │◀────────────┘
                     └────────────┘
```

### Transition Conditions

* **HOLD → FILL**: Distance > `fillThreshold` (tank emptying)
* **FILL → HOLD**: Distance ≤ `holdThreshold` (tank full)
* **Any → STOP**: Sensor fault (0, negative, or > maxSafe)
* **STOP → HOLD**: Sensor reading returns to normal

---

## 4. BFS to Get Reachable States

We perform a **Breadth-First Search (BFS)** on the FSM to confirm reachable transitions.

### Graph Representation

```
HOLD → FILL
HOLD → STOP
FILL → HOLD
FILL → STOP
STOP → HOLD
```

### BFS Starting From Initial State: HOLD

```
Start: HOLD
Queue: [HOLD]
Visited: {}

1) Pop HOLD
   Neighbors: FILL, STOP
   Queue: [FILL, STOP]
   Visited: {HOLD}

2) Pop FILL
   Neighbors: HOLD, STOP
   Queue: [STOP, HOLD, STOP]
   Visited: {HOLD, FILL}

3) Pop STOP
   Neighbors: HOLD
   Queue: [HOLD, STOP, HOLD]
   Visited: {HOLD, FILL, STOP}

All states visited.
```

### Result

✔ The FSM is **fully reachable**.
✔ No dead states.
✔ STOP properly integrates into the recovery path.

---

## 5. Kalman Filter Explanation

The ultrasonic sensor produces noisy measurements due to:

* Echo reflection inconsistencies
* Temperature & humidity variation
* Irregular tank geometry
* Electrical noise

To stabilize readings, a **1D Kalman filter** is used.

The filter assumes:

* Distance changes slowly
* Noise is random

### Kalman Filter Formula

Prediction:

```
P = P + Q
```

Update:

```
K = P / (P + R)
X = X + K * (measurement − X)
P = (1 − K) * P
```

Where:

* **X** = estimated distance
* **P** = estimation error
* **Q** = process noise (how fast distance is expected to change)
* **R** = measurement noise (how noisy sensor readings are)
* **K** = Kalman gain (confidence in measurement)

---

## 6. Parameters Used in This Project

### Q — Process Noise (0.5)

* Represents real distance variability
* Higher Q → faster reaction but more noise
* Lower Q → smoother but slower

### R — Measurement Noise (5)

* Represents sensor noise
* Larger R → system trusts sensor less
* Smaller R → system trusts sensor more

### Initial Estimate (50 cm)

* Starting guess of distance
* Automatically corrected in first few cycles

### Why Kalman Filter Works Well Here

* Ultrasonic noise spikes are rejected
* Provides smooth transitions for FSM
* Prevents rapid state-toggling or false STOP states

---

## 7. Summary

This project integrates:

* A **Kalman-filtered ultrasonic sensor**
* A robust **FILL–HOLD–STOP FSM**
* Full **failsafe behavior**
* Clean, modular library design
