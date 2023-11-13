#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

DS3231 rtc(SDA, SCL);

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo servo;

// deklarasi pin tombol
#define tKanan 4
#define tKiri 5
#define tOk 6
#define tKeluar 7

int kanan, ok, kiri, keluar;

// deklarasi pin Buzzer dan LED`
#define Buzzer 12
#define LED 11

// deklarasi menu
volatile boolean sisa_waktu, cek_status, milih_makan;
volatile boolean atur_waktu_makan1, atur_waktu_makan2;
volatile boolean aktivasi1 = false;
volatile boolean aktivasi2 = false;
int makan_jam1, makan_menit1, makan_jam2, makan_menit2;
int penanda1, penanda2;

Time t;


void setup() {
  lcd.begin();

  rtc.begin();

  servo.attach(10);

  servo.write(0);

  pinMode(tKanan, INPUT);
  pinMode(tOk, INPUT);
  pinMode(tKiri, INPUT);
  pinMode(tKeluar, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  // Serial.println(rtc.getDOWStr(FORMAT_SHORT));    //tampilkan hari dalam format singkat
  // Serial.println(rtc.getDOWStr(FORMAT_LONG));     //tampilkan hari dalam format panjang
  // Serial.println(rtc.getMonthStr(FORMAT_SHORT));  //tampilkan bulan dalam format singkat
  // Serial.println(rtc.getMonthStr(FORMAT_LONG));   //tampilkan bulan dalam format panjang
  // Serial.println(rtc.getDateStr());               //tampilkan tanggal
  // Serial.println(rtc.getTimeStr());               //tampilkan waktu
  // Serial.println(rtc.getUnixTime(rtc.getTime())); //konvert ke Unix time
}

void loop() {
  bacaTombol();
  getTimeRTC();
  homeScreen();
  aturWaktuMakan();
  sisaWaktu();
  cekStatus();
  waktuMakan();

  // kanan = digitalRead(tKiri);
  // if(kanan == HIGH){
  //   Serial.println("Tombol Kanan");
  // } else {
  //   Serial.println("Kanan Low");
  // }
}

void bacaTombol() {
  kanan = digitalRead(tKanan);
  ok = digitalRead(tOk);
  kiri = digitalRead(tKiri);
  keluar = digitalRead(tKeluar);
}

void getTimeRTC() {
  // rtc.setDOW(MONDAY);
  // rtc.setTime(16, 32, 00);
  // rtc.setDate(15, 05, 2023);
  t = rtc.getTime();
}

void homeScreen() {
  lcd.setCursor(0, 0);
  lcd.print("Pet Feeder");

  // date
  lcd.setCursor(12, 0);
  lcd.print(rtc.getDateStr(FORMAT_SHORT));

  // time
  lcd.setCursor(12, 1);
  lcd.print(rtc.getTimeStr());

  // write dow
  lcd.setCursor(8, 3);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
}

void aturWaktuMakan() {
  if (ok == HIGH) {
    milih_makan = true;
    lcd.clear();

    while (milih_makan == true) {
      bacaTombol();
      lcd.setCursor(0, 0);
      lcd.print("Waktu makan 1:Kanan");
      lcd.setCursor(0, 1);
      lcd.print("Waktu makan 2:Kiri");

      if (kanan == HIGH) {
        // makan 1
        atur_waktu_makan1 = true;
        aktivasi1 = true;
        penanda1 = 1;
        lcd.clear();

        while (atur_waktu_makan1 == true) {
          bacaTombol();
          lcd.setCursor(0, 0);
          lcd.print("Set Waktu Makan 1");
          lcd.setCursor(0, 1);
          lcd.print("Waktu: ");
          lcd.print(makan_jam1);
          lcd.print(" : ");
          lcd.print(makan_menit1);
        

          // atur waktu makan dengan tombol
          // menit
          if (kiri == HIGH) {
            makan_menit1++;
            delay(300);
            if (makan_menit1 > 59) {
              makan_menit1 = 0;
              lcd.clear();
            }
          }
          // jam
          if (kanan == HIGH) {
            makan_jam1++;
            delay(300);
            if (makan_jam1 > 23) {
              makan_jam1 = 0;
              lcd.clear();
            }
          }
          // keluar
          if (keluar == HIGH) {
            atur_waktu_makan1 = false;
            lcd.clear();
          }
        }
      } else if (keluar == HIGH) {
        milih_makan = false;
        lcd.clear();
      } else if (kiri == HIGH) {
        // makan 1
        atur_waktu_makan2 = true;
        aktivasi2 = true;
        penanda2 = 1;
        lcd.clear();

        while (atur_waktu_makan2 == true) {
          bacaTombol();
          lcd.setCursor(0, 0);
          lcd.print("Set Waktu Makan 2");
          lcd.setCursor(0, 1);
          lcd.print("Waktu: ");
          lcd.print(makan_jam2);
          lcd.print(" : ");
          lcd.print(makan_menit2);
        

          // atur waktu makan dengan tombol
          // menit
          if (kiri == HIGH) {
            makan_menit2++;
            delay(300);
            if (makan_menit2 > 59) {
              makan_menit2 = 0;
              lcd.clear();
            }
          }
          // jam
          if (kanan == HIGH) {
            makan_jam2++;
            delay(300);
            if (makan_jam2 > 23) {
              makan_jam2 = 0;
              lcd.clear();
            }
          }
          // keluar
          if (keluar == HIGH) {
            atur_waktu_makan2 = false;
            lcd.clear();
          }
        }
      } else if (keluar == HIGH) {
        milih_makan = false;
        lcd.clear();
      }
    }
  }
}

void sisaWaktu() {
  if (kanan == HIGH) {
    sisa_waktu = true;
    lcd.clear();

    while (sisa_waktu == true) {
      bacaTombol();
      lcd.setCursor(0, 0);
      lcd.print("Sisa Waktu");
      lcd.setCursor(0, 1);
      lcd.print("Makan 1:");
      lcd.setCursor(9, 1);

      // makan 1
      if (aktivasi1 == false) {
        lcd.print("Belum Aktif");
      } else if (aktivasi1 == true) {
        if (t.hour > makan_jam1) {
          lcd.print(24 - (t.hour - makan_jam1));
        } else if (t.hour < makan_jam1) {
          lcd.print(abs(t.hour - makan_jam1));
        } else if (t.hour == makan_jam1) {
          if (t.min <= makan_menit1) lcd.print("0");
          if (t.min > makan_menit1) lcd.print("23");
        }
        lcd.print(" J ");
        if (t.min > makan_menit1) {
          lcd.print(60 - (t.min - makan_menit1));
        } else if (t.min <= makan_menit1) {
          lcd.print(abs(t.min - makan_menit1));
        }
        lcd.print(" M ");
      }

      //makan 2
      lcd.setCursor(0, 2);
      lcd.print("Makan 2:");
      lcd.setCursor(9, 2);

      if (aktivasi2 == false) {
        lcd.print("Belum Aktif");
      } else if (aktivasi2 == true) {
        if (t.hour > makan_jam2) {
          lcd.print(24 - (t.hour - makan_jam2));
        } else if (t.hour < makan_jam2) {
          lcd.print(abs(t.hour - makan_jam2));
        } else if (t.hour == makan_jam2) {
          if (t.min <= makan_menit2) lcd.print("0");
          if (t.min > makan_menit2) lcd.print("23");
        }
        lcd.print(" J ");
        if (t.min > makan_menit2) {
          lcd.print(60 - (t.min - makan_menit2));
        } else if (t.min <= makan_menit2) {
          lcd.print(abs(t.min - makan_menit2));
        }
        lcd.print(" M ");
      }

      // kalau tombol keluar ditekan
      if (keluar == HIGH) {
        sisa_waktu = false;
        lcd.clear();
      }
    }
  }
}

void cekStatus() {
  if (kiri == HIGH) {
    cek_status = true;
    lcd.clear();

    while (cek_status == true) {
      bacaTombol();
      lcd.setCursor(0, 0);
      lcd.print("Cek Status : ");
      lcd.setCursor(0, 1);
      lcd.print("Makan 1:");
      lcd.setCursor(9, 1);

      // makan 1
      if (aktivasi1 == false) {
        lcd.print("Belum Aktif");
      } else if (aktivasi1 == true) {
        lcd.print("Berjalan");
        digitalWrite(LED, HIGH);
      }

      lcd.setCursor(0, 2);
      lcd.print("Makan 2: ");
      lcd.setCursor(9, 2);

      // makan 2
      if (aktivasi2 == false) {
        lcd.print("Belum Aktif");
      } else if (aktivasi2 == true) {
        lcd.print("Berjalan");
        digitalWrite(LED, HIGH);
      }

      // tombol keluar ditekan
      if (keluar == HIGH) {
        cek_status = false;
        digitalWrite(LED, LOW);
        lcd.clear();
      }
    }
  }
}

void waktuMakan() {
  if (aktivasi1 == true) {
    // makan 1
    if (penanda1 == 1) {
      if (t.hour == makan_jam1 && t.min == makan_menit1) {

        // servo terbuka
        servo.write(180);
        delay(1000);
        servo.write(0);

        // Tampilkan di LCD
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Waktunya Makan");
        delay(100);

        // LED dan Buzzer menyala
        for (int i = 0; i <= 4; i++) {
          digitalWrite(LED, HIGH);
          digitalWrite(Buzzer, HIGH);
          delay(100);
          digitalWrite(LED, LOW);
          digitalWrite(Buzzer, LOW);
          delay(100);
          digitalWrite(LED, HIGH);
          digitalWrite(Buzzer, HIGH);
          delay(100);
          digitalWrite(LED, LOW);
          digitalWrite(Buzzer, LOW);
          delay(100);
          digitalWrite(LED, HIGH);
          digitalWrite(Buzzer, HIGH);
          delay(100);
          digitalWrite(LED, LOW);
          digitalWrite(Buzzer, LOW);
          delay(100);

          delay(1000);
        }

        lcd.clear();

        penanda1 = 0;
      } else if (t.hour != makan_jam1 && t.min != makan_menit1) {
        penanda1 = 1;
      }
    }
  }

  if (aktivasi2 == true) {
    // makan 1
    if (penanda2 == 1) {
      if (t.hour == makan_jam2 && t.min == makan_menit2) {

        // servo terbuka
        servo.write(180);
        delay(1000);
        servo.write(0);

        // Tampilkan di LCD
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Waktunya Makan");
        delay(100);

        // LED dan Buzzer menyala
        for (int i = 0; i <= 4; i++) {
          digitalWrite(LED, HIGH);
          digitalWrite(Buzzer, HIGH);
          delay(100);
          digitalWrite(LED, LOW);
          digitalWrite(Buzzer, LOW);
          delay(100);
          digitalWrite(LED, HIGH);
          digitalWrite(Buzzer, HIGH);
          delay(100);
          digitalWrite(LED, LOW);
          digitalWrite(Buzzer, LOW);
          delay(100);
          digitalWrite(LED, HIGH);
          digitalWrite(Buzzer, HIGH);
          delay(100);
          digitalWrite(LED, LOW);
          digitalWrite(Buzzer, LOW);
          delay(100);

          delay(1000);
        }

        lcd.clear();

        penanda2 = 0;
      } else if (t.hour != makan_jam2 && t.min != makan_menit2) {
        penanda2 = 1;
      }
    }
  }
}