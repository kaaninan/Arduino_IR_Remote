#include <EEPROM.h>
#include <IRremote.h>

long unsigned int code;
char tur;
int bitt;

// EEPROM Bolme Uzunluğu
int eeprom_bolme = 20;


// CIKIS
int ir_verici = 3;

int buzzer = 4;

int led1 = 10;
int led2 = 11;
int led3 = 12;
int led_bilgi = 13;


// GIRIS
int ir_alici = 2;

int b0 = 0;

int b1 = 5;
int b2 = 6;
int b3 = 7;
int b4 = 8;

int switch1 = 9;

int potansiyometre = A0;


// IR
IRsend irsend;
IRrecv irrecv(ir_alici);
decode_results results;


// DURUM
int buton_durum = 0;
int led_durum = 0;

int b1S;
int b2S;
int b3S;
int b4S;
int secenek;
int sayfa;

void setup() {
  
  //for(int i = 0; i < 34; i++)
  //  eeprom_temizle(i);
  
  pinMode(ir_verici,OUTPUT);
  
  Serial.begin(9600);
  Serial.println("KAAN (C) - KUMANDA SISTEMI");
  Serial.println("v1.0 - 2014");
  Serial.println();
  
  // LED
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led_bilgi, OUTPUT);
  
  // CIKIS
  pinMode(buzzer, OUTPUT);
  
  // GIRIS
  pinMode(b0, INPUT);
  
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
  
  pinMode(switch1, INPUT);
  
  
  // Acılıs Sesi
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  
  
  // Acilis Animasyonu
  for(int i = 0; i < 2; i++){
    int zaman = 50;
    led_1(1);
    delay(zaman);
    led_1(0);
    delay(zaman);
    led_2(1);
    delay(zaman);
    led_2(0);
    delay(zaman);
    led_3(1);
    delay(zaman);
    led_3(0);
    delay(zaman);
  }
  
  // IR
  irrecv.enableIRIn();
  
  //eeprom_yaz('0', 12, 0x38, 2);
  
}

void loop() {
    
  // Switch Durumları
  int switch_durum1 = digitalRead(switch1);
  
  int zaman = 10;
  
  if(switch_durum1 == HIGH){
    
    // KAYDET
    
    int sil = digitalRead(b0);
    
    if(sil == HIGH){
      for(int i = 0; i < 34; i++){
        eeprom_temizle(i);  
      }
    }
    
    int pot = analogRead(potansiyometre);
    
    if(600 > pot){
      sayfa = 0;
      led_2(0);
      led_3(0);
      
      led_1(1);
      
      for(int i = 0; i < 10; i++){
        delay(zaman);
        kaydet_buton();
      }
      
      led_1(0);
      
      for(int i = 0; i < 10; i++){
        delay(zaman);
        kaydet_buton();
      }
      
    }else if(pot > 900){
      sayfa = 1;
      led_1(0);
      led_2(0);
      
      led_3(1);
      
      for(int i = 0; i < 10; i++){
        delay(zaman);
        kaydet_buton();
      }
        
      led_3(0);
      
      for(int i = 0; i < 10; i++){
        delay(zaman);
        kaydet_buton();
      }
            
    }else{
      sayfa = 2;
      led_1(0);
      led_3(0);
      
      led_2(1);
      
      for(int i = 0; i < 10; i++){
        delay(zaman);
        kaydet_buton();
      }
      
      led_2(0);
      
      for(int i = 0; i < 10; i++){
        delay(zaman);
        kaydet_buton();
      }
    }
    
    
  }else{
    
    // GONDER
    b1S = digitalRead(b1);
    b2S = digitalRead(b2);
    b3S = digitalRead(b3);
    b4S = digitalRead(b4);
    
    int zaman = 100;
    
    if(b1S == HIGH){
      secenek = 1;
      buton_durum = 1;
    }
    else if(b2S == HIGH){
      secenek = 2;
      buton_durum = 1;
    }
    else if(b3S == HIGH){
      secenek = 3;
      buton_durum = 1;
    }
    else if(b4S == HIGH){
      secenek = 4;
      buton_durum = 1;
    }else{
      buton_durum = 0;
    }
      
    int sayfa = 0;
    int pot = analogRead(potansiyometre);
    
    if(600 > pot){
      sayfa = 0;
      led_1(1);
      led_2(0);
      led_3(0);
      
    }else if(pot > 900){
      sayfa = 1;
      led_1(0);
      led_2(0);
      led_3(1);
      
    }else{
      sayfa = 2;
      led_1(0);
      led_2(1);
      led_3(0);
    }
    
    int tam_secenek = 0;
    
    if(sayfa == 1)
      tam_secenek = secenek + 5;
    
    else if(sayfa == 2)
      tam_secenek = secenek + 10;
     
    else 
      tam_secenek = secenek;
     
     
    if(buton_durum == 1){
      eeprom_oku(tam_secenek);
      
      if(tur == 'B'){
        led_b(1);
        delay(600);
        led_b(0);
      }else{
        
        ir_sinyal_gonder();
        
      }
    }
     
  }
  
  
}


void kaydet_buton(){
  
  int sure = 100;

  b1S = digitalRead(b1);
  b2S = digitalRead(b2);
  b3S = digitalRead(b3);
  b4S = digitalRead(b4);
  
  if(b1S == HIGH){
    secenek = 0;
    buton_durum = 1;
    int kacincii = sayfa_hesap(sayfa, secenek);
    delay(sure);
    ir_sinyal_al(kacincii, secenek);
  }
  else if(b2S == HIGH){
    secenek = 1;
    buton_durum = 1;
    int kacincii = sayfa_hesap(sayfa, secenek);
    delay(sure);
    ir_sinyal_al(kacincii, secenek);
  }
  else if(b3S == HIGH){
    secenek = 2;
    buton_durum = 1;
    int kacincii = sayfa_hesap(sayfa, secenek);
    delay(sure);
    ir_sinyal_al(kacincii, secenek);
  }
  else if(b4S == HIGH){
    secenek = 3;
    buton_durum = 1;
    int kacincii = sayfa_hesap(sayfa, secenek);
    delay(sure);
    ir_sinyal_al(kacincii, secenek);
  }
}


// Sayfa Hesapla
int sayfa_hesap(int sayfa, int secenek){
  int tam_secenek = 0;
    
  if(sayfa == 1)
    tam_secenek = secenek + 5;
  
  else if(sayfa == 2)
    tam_secenek = secenek + 10;
   
  else 
    tam_secenek = secenek;
    
  return tam_secenek;
}


// ### IR ###
void ir_sinyal_gonder(){
  
  if(tur == '1'){
    // RC5
    irsend.sendRC5(code, bitt);
  }
  
  if(tur == '2'){
    // RC6
    irsend.sendRC6(code, bitt);
  }
  
  if(tur == '3'){
    // NEC
    irsend.sendNEC(code, bitt);
  }
  
  if(tur == '4'){
    // SONY
    irsend.sendSony(code, bitt);  
  }
  
  led_b(1);
  delay(200);
  led_b(0);
  delay(200);
  
}

void ir_sinyal_al(int kacinci, int sec){

  led_1(0);
  led_2(0);
  led_3(0);
  
  int devam = 1;
  
  while(devam == 1){
    
    if (irrecv.decode(&results)) {
      dump(&results, kacinci);
      irrecv.resume();
      devam = 0;
    }
    
    // Buton Tanımlama
    int butonn = 0;
    if(sec == 0)
      butonn = b1;
    if(sec == 1)
      butonn = b2;
    if(sec == 2)
      butonn = b3;
    if(sec == 3)
      butonn = b4;
      
    delay(100);
    int bdurum = digitalRead(butonn);
    
    if(bdurum == HIGH){
      devam = 0;
      delay(100);
    }
    
    int sswitch = digitalRead(switch1);
      
    if(sswitch == LOW){
      devam = 0;
    }
    
    
  }
  
}

void dump(decode_results *results, int kac) {
  int count = results->rawlen;
  char tuur = '0';
  
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
    tuur = '5';
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
    tuur = '3';
  } 
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
    tuur = '4';
  } 
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
    tuur = '1';
  } 
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
    tuur = '2';
  }
  
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Kayıt Sayısı -> ");
  Serial.println(kac);
  
  eeprom_yaz(tuur, results->bits, results->value, kac);
  
  led_b(1);
  buzzerr(1);
  delay(300);
  led_b(0);
  buzzerr(0);
  
  Serial.println("");
}


// ### EEPROM ###

void eeprom_yaz(char tip, int bitt2, long unsigned int kod, int buton){
  
  // Butonlar 0'dan başlar.
  
  int basla = buton * eeprom_bolme;

  EEPROM.write(basla, '+');
  basla++;
  EEPROM.write(basla, tip);
  basla++;
  EEPROM.write(basla, '.');
  basla++;
  EEPROM.write(basla, bitt2);
  basla++;
  EEPROM.write(basla, ':');
  basla++;
  
  byte four = (kod & 0xFF);
  byte three = ((kod >> 8) & 0xFF);
  byte two = ((kod >> 16) & 0xFF);
  byte one = ((kod >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(basla, four);
  basla++;
  EEPROM.write(basla, three);
  basla++;
  EEPROM.write(basla, two);
  basla++;
  EEPROM.write(basla, one);
  basla++;
  
  EEPROM.write(basla, '-');
  Serial.println("Kaydedildi");
}


void eeprom_oku(int buton){
  int kacinci = 0;
  
  int basla = 0;
  int bitis = 0;
  
  char kar;
  
  for(int i = 0; i < 1023; i++){
    kar = EEPROM.read(i);
    //Serial.print("Karakter -> ");
    //Serial.println(kar);
    
    if(kar == '+'){
      basla = i;
    }
    else if(kar == '-'){
      bitis = i;
      kacinci++;
    }
    
    if(kacinci == buton)
      break;
  }
  
  /*
  Serial.print("Baslangic -> ");
  Serial.println(basla);
  Serial.print("Bitis -> ");
  Serial.println(bitis);
  delay(1000);
  */
  
  int kac_tane = bitis - basla;
  
  // KOD OZELLIKLERI
  char tip;
  int kod_bit;
  long unsigned int kod;
  
  int char_nerde = 0;
  int tip1 = 0;
  int bit1 = 0;
  int kod1 = 0;
  
  for(int a = basla; a < bitis + 1; a++){
    
    char b = EEPROM.read(a);
    
    if(b == '+'){
      tip1 = 1;
    }
    else if(tip1 == 1){
      tip = b;
      tip1 = 0;
    }
    else if(b == '.'){
      bit1 = 1;
    }
    else if(bit1 == 1){
      
      kod_bit = EEPROM.read(a);
      bit1 = 0;
    }
    else if(b == ':')
      kod1 = 1;
    
    else if(kod1 == 1){
      long four = EEPROM.read(a);
      long three = EEPROM.read(a + 1);
      long two = EEPROM.read(a + 2);
      long one = EEPROM.read(a + 3);

      //Return the recomposed long by using bitshift.
      kod =  ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      
      kod1 = 0;
    }
    
  }
  
  Serial.print("BASLANGIC -> ");
  Serial.println(basla);
  
  Serial.print("KOD -> ");
  Serial.println(kod);
  
  Serial.print("TIP -> ");
  Serial.println(tip);
  
  Serial.print("BIT -> ");
  Serial.println(kod_bit);
  
  Serial.println();
  
  code = kod;
  tur = tip;
  bitt = kod_bit;
}


void eeprom_temizle(int secenek){
  
  Serial.println("temizleniyor");

  if(secenek == 99){
    
    for (int i = 0; i < 1024; i++)
      EEPROM.write(i, 0);
  }
  
  else{
    int basla = secenek * eeprom_bolme;
    int ilk = 1;
    int iki = 0;
    int uc = 0;
    int dort = 0;
    
    for (int i = basla; i < basla + eeprom_bolme; i++){
    
      if(ilk == 1){
        EEPROM.write(i, '+');
        ilk = 0;
        iki = 1;
      }
      else if(iki == 1){
        EEPROM.write(i, 'B');
        i++;
        EEPROM.write(i, '.');
        i++;
        EEPROM.write(i, 0);
        iki = 0;
        uc = 1;
      }
      else if(uc == 1){
        EEPROM.write(i, ':');
        i++;
        EEPROM.write(i, 1);
        i++;
        EEPROM.write(i, 1);
        i++;
        EEPROM.write(i, 1);
        i++;
        EEPROM.write(i, 1);
        uc = 0;
        dort = 1;
      }
      else if(dort == 1){
        EEPROM.write(i, '-');
        dort = 0;
      }else{
        EEPROM.write(i, 0);
      }
    }
  }
  
}






// ### LED ###

void led_1(int i){
  if(i == 0)
    digitalWrite(led1, LOW);
  else    
    digitalWrite(led1, HIGH);
}

void led_2(int i){
  if(i == 0)
    digitalWrite(led2, LOW);
  else
    digitalWrite(led2, HIGH);
}

void led_3(int i){
  if(i == 0)
    digitalWrite(led3, LOW);
  else
    digitalWrite(led3, HIGH);
}

void led_b(int i){
  if(i == 0)
    digitalWrite(led_bilgi, LOW);
  else
    digitalWrite(led_bilgi, HIGH);
}


void buzzerr(int i){
  if(i == 0)
    digitalWrite(buzzer, LOW);
  else
    digitalWrite(buzzer, HIGH);
}
