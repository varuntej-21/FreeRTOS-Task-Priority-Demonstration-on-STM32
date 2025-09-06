# FreeRTOS Task Priority Demonstration on STM32

This project demonstrates the concept of task scheduling and priorities in a Real-Time Operating System (FreeRTOS) on an STM32 microcontroller. Two tasks with configurable priorities control two separate LEDs, visually showing how the RTOS scheduler manages task execution. 🚦

---

## Hardware Requirements ⚙️

* **STM32 Development Board** (e.g., stm32f411ceu6)
* **1x Green LED** & **1x Red LED**
* **2x 220Ω Resistors**
* **Breadboard** and **Jumper Wires**

---

## Software Requirements 💻

* **STM32CubeIDE** (or another compatible IDE)
* **FreeRTOS** (via CMSIS-OS v2 API)
* **STM32 HAL Drivers**

---

## Setup and Wiring

The two LEDs are connected to GPIO pins **PA2** and **PA3**.

* **Green LED (Task 1):** Connect the anode (longer lead) to pin `PA2` via a 220Ω resistor. Connect the cathode (shorter lead) to `GND`.
* **Red LED (Task 2):** Connect the anode (longer lead) to pin `PA3` via a 220Ω resistor. Connect the cathode (shorter lead) to `GND`.

---

## How It Works

The program initializes FreeRTOS and creates two tasks that run based on their assigned priority.

### Scenario 1: Preemptive Scheduling (Task 1 > Task 2)

When `Task1` has a higher priority, it always preempts `Task2`. The scheduler will only run `Task1`.

**Code:**
```c
const osThreadAttr_t Task1_attributes = {
  .name = "Task1",
  .stack_size = 128 * 4,
  .priority = osPriorityHigh
};
const osThreadAttr_t Task2_attributes = {
  .name = "Task2",
  .stack_size = 128 * 4,
  .priority = osPriorityNormal
};
```
Result: Only the Green LED blinks. The Red LED remains off.
![IMG_20250906_194838](https://github.com/user-attachments/assets/28ca4bd9-20c0-44c6-a017-5f85f4d7f2c9)


### Scenario 2: Preemptive Scheduling (Task 2 > Task 1)
When Task2 has the higher priority, it will always run, and Task1 will be starved of CPU time.

**Code:**
```c

const osThreadAttr_t Task1_attributes = {
  .name = "Task1",
  .stack_size = 128 * 4,
  .priority = osPriorityNormal
};
const osThreadAttr_t Task2_attributes = {
  .name = "Task2",
  .stack_size = 128 * 4,
  .priority = osPriorityHigh
};
```
Result: Only the Red LED blinks. The Green LED remains off.
![IMG_20250906_195041](https://github.com/user-attachments/assets/a0ed67a8-1c30-4484-b196-1b1e39205bf4)


### Scenario 3: Round-Robin Scheduling (Task 1 == Task 2)
When both tasks have the same priority, the scheduler uses time-slicing to share CPU time between them, making them appear to run concurrently.

Code:

```c

const osThreadAttr_t Task1_attributes = {
  .name = "Task1",
  .stack_size = 128 * 4,
  .priority = osPriorityHigh
};
const osThreadAttr_t Task2_attributes = {
  .name = "Task2",
  .stack_size = 128 * 4,
  .priority = osPriorityHigh
};
Result: Both the Green LED and the Red LED blink together.
![IMG_20250906_194923](https://github.com/user-attachments/assets/35dc66de-4e8a-45c2-9efe-460a59615636)
```
