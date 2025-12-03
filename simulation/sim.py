import numpy as np
import matplotlib.pyplot as plt

# ----------------------------------------------------
# 1. Simulation Parameters
# ----------------------------------------------------
steps = 240
# Phase 1: Draining (tank empties)
drain = np.linspace(10, 50, 80)  

# Phase 2: Filling (pump starts at 40)
fill = np.linspace(50, 10, 80)

# Phase 3: Overflow (sensor reading drops abnormally)
overflow = np.linspace(10, 0, 80)

# Combined
true_level = np.concatenate([drain, fill, overflow])

# true_level = np.linspace(50, 10, steps)  # Tank filling over time (cm)
noise = np.random.normal(0, 5, steps)    # Ultrasonic noise
raw_measurement = true_level + noise

# ----------------------------------------------------
# 2. Kalman Filter
# ----------------------------------------------------
Q = 0.5   # process noise
R = 5     # measurement noise

X = 10    # initial estimate
P = 1     # initial error
filtered_measurement = []

def kalman(z):
    global X, P
    P = P + Q
    K = P / (P + R)
    X = X + K * (z - X)
    P = (1 - K) * P
    return X

for z in raw_measurement:
    filtered_measurement.append(kalman(z))

# ----------------------------------------------------
# 3. FSM Simulation
# ----------------------------------------------------
FILL = 0
HOLD = 1
STOP = 2

fill_threshold = 40
hold_threshold = 20
max_safe = 400

state = HOLD
fsm_states = []

for d in filtered_measurement:

    # safety check
    if d <= 0 or d > max_safe:
        state = STOP

    if state == FILL:
        if d <= hold_threshold:
            state = HOLD

    elif state == HOLD:
        if d > fill_threshold:
            state = FILL

    elif state == STOP:
        if 0 < d < max_safe:
            state = HOLD

    fsm_states.append(state)

# ----------------------------------------------------
# 4. Plot Results
# ----------------------------------------------------
plt.figure(figsize=(12,8))

plt.subplot(3,1,1)
plt.plot(true_level, label="True Level")
plt.plot(raw_measurement, label="Noisy Measurement")
plt.plot(filtered_measurement, label="Kalman Filtered")
plt.legend()
plt.title("Ultrasonic Simulation")

plt.subplot(3,1,2)
plt.plot(fsm_states)
plt.yticks([0,1,2], ["FILL","HOLD","STOP"])
plt.title("FSM State Over Time")

plt.subplot(3,1,3)
pump_output = [1 if s == FILL else 0 for s in fsm_states]
plt.plot(pump_output)
plt.title("Pump ON (1) / OFF (0)")

plt.tight_layout()
plt.savefig('output.jpg')
