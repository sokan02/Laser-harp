# 🎼 Laser Harp – Embedded Real-Time Audio System

## 🎥 Demo & Media

Project photos and demonstration videos are available here:  
👉 [View Images and Videos](https://drive.google.com/drive/u/0/folders/16Vb5HLQeS4HylaVSjotVZjERahWZ7sgf)

---

## Overview

Laser Harp is a real-time embedded system integrating an Arduino microcontroller and a Raspberry Pi to create a hardware–software audio pipeline.

The system detects laser beam interruptions using photoresistors connected to Arduino, transmits MIDI messages over USB, and synthesizes sound on a Raspberry Pi running Linux using Python and FluidSynth.

This project focuses on low-latency signal processing, cross-device communication, and reliable system integration.

---

## 🏗 System Architecture

### Hardware Layer (Arduino)
- 24 laser diodes and photoresistor sensors
- Real-time beam interruption detection
- C++ firmware for MIDI Note On/Off generation
- USB serial communication to Raspberry Pi

### Processing Layer (Raspberry Pi – Linux)
- Python-based MIDI processing
- FluidSynth for audio synthesis
- ALSA audio backend
- `systemd` service for automatic startup

---

## 🔄 Signal Flow

1. Laser beam interruption detected by photoresistor  
2. Arduino processes analog input  
3. MIDI Note On/Off message generated  
4. Message sent via USB serial  
5. Raspberry Pi receives MIDI data  
6. Python script forwards to FluidSynth  
7. Audio output through ALSA  

---

## ⚙️ Key Technical Challenges

### 1. Real-Time Performance
Ensuring low-latency response between beam interruption and audio output.  
Timing inconsistencies were debugged across both microcontroller and Linux layers.

### 2. Serial Communication Reliability
Designed stable communication between Arduino firmware and Raspberry Pi process to prevent dropped or duplicated notes.

### 3. Linux Audio Integration
Configured ALSA and FluidSynth for consistent playback, addressing audio device conflicts and initialization timing.

### 4. Autonomous Operation
Implemented a `systemd` service to:
- Automatically start the audio pipeline on boot
- Handle process restarts
- Enable standalone instrument operation

---

## 🛠 Technologies Used

- **C++** (Arduino firmware)
- **Python** (Raspberry Pi processing layer)
- **Linux** (Raspberry Pi OS)
- **systemd**
- **FluidSynth**
- **ALSA**
- USB Serial Communication
- Git for version control
