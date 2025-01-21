#CNCController3Axis

CNCController3Axis adalah sebuah pustaka untuk mengontrol mesin CNC tiga sumbu menggunakan ESP32 dan driver stepper berbasis AccelStepper. Proyek ini mendukung berbagai perintah G-code dan M-code, termasuk interpolasi linear dan melingkar, serta fitur tambahan seperti kompensasi panjang alat dan radius alat.

## Fitur Utama

- **Kontrol Sumbu XYZ:** Menggunakan driver stepper melalui pustaka AccelStepper.
- **Dukungan G-code:**
  - Gerakan linear (G0, G1)
  - Interpolasi melingkar searah (G2) dan berlawanan arah jarum jam (G3)
  - Pemilihan sistem koordinat kerja (G54 - G59)
  - Pemilihan bidang kerja (G17, G18, G19)
  - Kompensasi radius alat (G40, G41, G42)
  - Kompensasi panjang alat (G43, G49)
  - Pengaturan mode unit (G20, G21)
  - Home posisi (G28)
- **Dukungan M-code:**
  - Kontrol spindle (M3, M4, M5)
- **Spindle Control:** Mendukung pengaturan kecepatan spindle menggunakan PWM.
- **Mode Posisi:** Mode absolut (G90) dan inkremental (G91).
- **Kecepatan Umpan:** Feedrate per menit (G94).

## Komponen yang Dibutuhkan

1. **Hardware:**
   - ESP32
   - Stepper motor hanpose 23hs7628 dan driver tb6600
   - Power supply untuk motor stepper

2. **Software:**
   - Arduino IDE

## Instalasi

1. Unduh file pustaka (CncController.h dan CncController.cpp).
2. Tempatkan file tersebut di direktori berikut:
   Documents/Arduino/libraries/CncController3axis/
3. Restart IDE Arduino untuk mengenali pustaka tersebut.


Aristo Muhamad Nanda

