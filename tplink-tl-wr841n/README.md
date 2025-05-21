# 🛠️ UART Hacking TP-Link TL-WR841N

We attempted to gain UART shell access on a **TP-Link TL-WR841N router**, and this README documents the process, lessons learned, and some very satisfying success.

---

## 🧩 Step 1: Identify UART Pins

Fortunately, TP-Link was kind enough to **label the UART pins** on the board:

- `VCC`
- `GND`
- `RX`
- `TX`

📸 _Image: UART Pin Labels_

![UART Pin Labels](images/uart-pins.jpg)

---

## 🔌 Step 2: First Attempt — USB Cable Hack

We had the **primitive idea** of using a regular USB cable.

- We cut off the micro USB end.
- Soldered **female jumper wires** to the inner wires.
- Soldered **male headers** to the router's UART interface.
- Connected directly to the PC.

📸 _Image: USB cable hack_

![USB Cable Hack](images/usb-hack.jpg)

**Result:** Nothing. No output using `picocom`.

---

## 🔬 Step 3: Logic Analyzer Debugging

We connected a **logic analyzer** and viewed the signals in **PulseView**.

- Detected clean UART signals at `115200 baud`.
- Conclusion: **Connection worked electrically**, but USB wasn’t speaking UART.

📸 _Image: PulseView signals_

![PulseView Signals](images/pulseview.jpg)

---

## ❌ Why USB ≠ UART?

**USB and UART are entirely different protocols**:

- USB is packet-based and requires drivers.
- UART is a simple asynchronous serial protocol (raw TX/RX lines).
- You **cannot connect USB directly to UART** without a **USB-to-UART bridge**.

---

## 🧠 Step 4: Arduino as USB-UART Passthrough

We didn’t want to wait for a proper USB-UART adapter, so we:

- Used an **Arduino** as a serial passthrough.
- Wired the router’s TX to Arduino RX and vice versa.
- Flashed a simple passthrough sketch.

**Result:** Partial success. We received **gibberish with some readable output**.

📸 _Image: Arduino passthrough setup_

![Arduino Passthrough](images/arduino.jpg)

---

## ⚠️ Why Arduino Failed at 115200 Baud

Arduino Uno (and similar 8-bit microcontrollers) are **too slow** to handle high-speed UART reliably at 115200 baud. The software serial (or even hardware serial with forwarding) couldn’t keep up with the data stream.

---

## 💡 Step 5: ESP32 to the Rescue

We used an **ESP32** instead:

- Much faster CPU, proper hardware serial support.
- Wired it correctly, but **still got garbled output**.

After trial and error, we realized...

---

## 🔀 TX/RX Must Be Crossed!

This was a **classic UART mistake**:

- The router's `TX` (transmit) must go to the ESP32’s `RX` (receive).
- And vice versa: `RX` ↔ `TX`.

📸 _Image: Final working wiring with ESP32_

![Working UART](images/esp32-working.jpg)

**After swapping the lines**, everything worked flawlessly. 🎉

---

## 🐚 Step 6: Root Shell!

Once connected, we were **immediately dropped into a root shell** — no login prompt, no password. Jackpot.

---

## 🔐 Found: Hashed Root Password

We discovered the root password stored as an **MD5 hash** in the `/etc/passwd` or `passwd.bak` file:

admin:$1$$iC.dUsGpxNNJGeOm1dFio/:0:0:root:/:/bin/sh

We're planning to **crack this hash** next.

---

## 📦 Tools Used

- TP-Link TL-WR841N
- Logic Analyzer + PulseView
- Arduino Uno
- ESP32
- `picocom`
- Soldering gear

---

## 🚧 Lessons Learned

- Don't connect USB to UART directly — always use a bridge.
- Always cross TX ↔ RX.
- Arduino is too slow for high-speed UART passthrough.
- ESP32 makes a great quick UART interface.
- Embedded devices often lack security by default 😏

---

## 📁 Images

Make sure to place all images in a subfolder like `images/`, e.g.:



## 🧪 Coming Next

- Crack the MD5 hash.
- Explore router firmware.
- Try privilege escalation and network access.

---

## 🧠 Stay Curious, Hack Legally

This was done on a **personally owned router**, purely for educational purposes. Please always respect device ownership, networks, and the law.

---

🛠️ Happy hacking!

