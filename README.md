## **KoKuBot**  

Proyek ini adalah sistem **kontrol robot omniwheel 4 roda** menggunakan **ESP32**, **L298N Motor Driver**, dan **MQTT**.  
Robot dapat dikendalikan melalui **perintah MQTT** dari aplikasi seperti **MQTT Explorer atau IoT MQTT Dashboard**.  

---

## **🎯 Fitur**  
✅ Menggunakan **MQTT** untuk menerima perintah arah.  
✅ **Dapat bergerak**: maju, mundur, geser kiri/kanan, dan berputar.  
✅ **Auto reconnect** jika koneksi WiFi atau MQTT terputus.  
✅ **Mengirim status ke MQTT** setiap 5 detik (`robot/status`).  

---

## **📌 Koneksi Hardware**  

Berikut adalah koneksi **ESP32 ke Motor Driver L298N**:  

| ESP32 Pin | Motor Driver 1 (Kiri) | Motor Driver 2 (Kanan) |  
|-----------|-----------------------|------------------------|  
| GPIO 13   | IN1                   | -                      |  
| GPIO 12   | IN2                   | -                      |  
| GPIO 14   | ENA                   | -                      |  
| GPIO 27   | IN3                   | -                      |  
| GPIO 26   | IN4                   | -                      |  
| GPIO 25   | ENB                   | -                      |  
| GPIO 33   | -                     | IN5                    |  
| GPIO 32   | -                     | IN6                    |  
| GPIO 15   | -                     | ENA2                   |  
| GPIO 4    | -                     | IN7                    |  
| GPIO 2    | -                     | IN8                    |  
| GPIO 0    | -                     | ENB2                   |  

📌 **Catatan**  
- **Motor Driver 1** mengontrol **dua motor kiri**.  
- **Motor Driver 2** mengontrol **dua motor kanan**.  
- **Sumber daya motor** harus menggunakan **baterai Li-Ion 7.4V atau 12V**.  

---

## **📜 Instalasi & Konfigurasi**  

### **1️⃣ Persiapkan Perangkat**  
- **ESP32** sebagai mikrokontroler utama.  
- **Dua modul L298N** sebagai driver motor.  
- **Empat motor DC dengan Omniwheel**.  
- **Baterai Li-Ion 7.4V / 12V** untuk motor.  

### **2️⃣ Library yang Dibutuhkan**  
Sebelum mengunggah kode, pastikan sudah menginstal library berikut di **Arduino IDE**:  
- **WiFi.h** → Sudah ada di ESP32 SDK.  
- **MQTT.h** → Tambahkan melalui **Library Manager**.  

---

## **📌 Cara Menggunakan**  
1. **Hubungkan ESP32 ke WiFi**.  
2. **Gunakan MQTT Client** (seperti MQTT Explorer atau IoT MQTT Dashboard).  
3. **Kirim perintah ke MQTT**:  
   ```
   Topic: robot/control  
   Message: forward | backward | left | right | turn_left | turn_right | stop  
   ```  
4. Robot akan bergerak sesuai perintah! 🚀  

---
