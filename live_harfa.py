#!/usr/bin/env python3
import fluidsynth
import time
import subprocess

# === Putanja do SoundFont-a ===
SOUNDFONT = "/usr/share/sounds/sf2/FluidR3_GM.sf2"

# === Parametri harfe ===
CHANNEL = 0
BANK = 0
PROGRAM = 46  # General MIDI Harp

# === Inicijalizacija FluidSynth ===
fs = fluidsynth.Synth()
fs.start(driver="pulseaudio")  # koristi PulseAudio
sfid = fs.sfload(SOUNDFONT)
fs.program_select(CHANNEL, sfid, BANK, PROGRAM)

print("FluidSynth pokrenut sa zvukom harfe")

# === Funkcija za spajanje Arduina ===
def connect_arduino():
    try:
        # Pronalaženje Arduino porta
        arduino_port = subprocess.check_output(
            "aconnect -l | grep -i Arduino | grep 'client' | awk '{print $2}' | tr -d ':'",
            shell=True,
            text=True
        ).strip()
        # Pronalaženje FluidSynth porta
        fluid_ports = subprocess.check_output(
            "aconnect -l | grep -i 'FLUID' | awk '{print $2}' | tr -d ':'",
            shell=True,
            text=True
        ).strip()
        if arduino_port and fluid_ports:
            print(f"Povezujem Arduino ({arduino_port}) sa FluidSynth ({fluid_ports})")
            subprocess.run(f"aconnect {arduino_port}:0 {fluid_ports}:0", shell=True)
            print("Povezano")
        else:
            print("Nije moguće pronaći Arduino ili FluidSynth port!")
    except subprocess.CalledProcessError:
        print("Greška prilikom povezivanja portova.")

# Prvi pokušaj povezivanja
connect_arduino()

# === Live loop (držimo skript aktivnim) ===
try:
    print("Čekam MIDI note sa Arduino-a. Ctrl+C za izlaz.")
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("\nGasimo FluidSynth...")
    fs.delete()
    print("Gotovo")

