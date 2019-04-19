#include <stdint.h>
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LCDPORT                  GPIO_PORTB_BASE
#define LCDPORTENABLE    SYSCTL_PERIPH_GPIOB
#define RS                               GPIO_PIN_0
#define E                                GPIO_PIN_1
#define PortD4                               GPIO_PIN_4
#define PortD5                               GPIO_PIN_5
#define PortD6                               GPIO_PIN_6
#define PortD7                               GPIO_PIN_7

volatile unsigned long tmp;   // bu degisken gecikme yapmak icin gerekli
int kasa[3] = {5,5,5}; // 0 25 kurus, 1 50 kurus , 2 100 kurus
int para[4] = {0,0,0}; // 0 100 kurus, 1 50 kurus, 25 kurus

int urun[5][2]={30,50,20,100,5,150,50,175,100,200}, alinanUrun[]={0,0,0,0,0}; //0 sut, 1 cay, 2 kahve, 3 cikolata, 4 biskuvi
int main(void) {

  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);
  
  portlariAktiflestir();  
  Lcd_init();
  
  paraYukle();

}

void paraYukle(){
  lcdTemizle();
  while(1){
  if((GPIO_PORTE_DATA_R & 0b00100000) == 0){
        Lcd_Goto(1,1);
        ekranaYazdir("Para yukleme    ");
        Lcd_Goto(2,1);
        ekranaYazdir("isi tamamlandi");
        for (tmp = 0 ; tmp < 2500000 ; tmp++);
        break;
  }else if((GPIO_PORTE_DATA_R & 0b00001000) == 0){ // 25kurus ekle
        Lcd_Goto(1,1);
        para[2]++;
        kasa[0]++;
        atilanPara();     
        bekle();
  }else if((GPIO_PORTE_DATA_R & 0b00000100) == 0){ // 50 kurus ekle
        Lcd_Goto(1,1);
        para[1]++;
        kasa[1]++;
        atilanPara();     
        bekle();
  }else if((GPIO_PORTE_DATA_R & 0b00000010) == 0){ // 100 kurus ekle 
        Lcd_Goto(1,1);
        para[0]++;
        kasa[2]++;
        atilanPara();     
        bekle();
  }
  }
  
  secim();
}

void hata(){
       int x = rand()%4+1;
       x = (para[0]+x)%4+1;
        
      
        if(x==2){
         // kirmizi led yak
         GPIO_PORTC_DATA_R |=  0b100000;
         GPIO_PORTC_DATA_R &= ~0b010000;
        
        char y[] = "";  
        sprintf(y, "%d", x);
        char *yy = &y[0];
      
         
        for(int i=0; i<5; i++){
        alinanUrun[i]=0;
        }        
        lcdTemizle();
        Lcd_Goto(1,1);
        ekranaYazdir("resetleyin");

        for(int i=0; i<3 ; i++){
         while(para[0] + para[1] + para[2] > 0 ){
          if(para[0] > 0){
           para[0]--;
           kasa[2]--;
          }else if(para[1] > 0){
           para[1]--; 
            kasa[1]--;
          }else if(para[2] > 0){
           para[2]--;
          kasa[0]--; 
          } 
         }
        }
        
        while(1){
        if((GPIO_PORTE_DATA_R & 0b00000010) == 0){
         x++; 
        }
          
        if ((GPIO_PORTE_DATA_R & 0b00010000) == 0) {
          GPIO_PORTC_DATA_R &= ~0b11111111;
           break;
         }        
        }
        
          Lcd_Goto(1,1);
          ekranaYazdir("Para yukleme.   ");
          Lcd_Goto(2,1);
          ekranaYazdir("islemi basliyor.");
          for (tmp = 0 ; tmp < 2500000 ; tmp++);
          
          paraYukle();
        }else {
         //yesil ledi yak
         
         GPIO_PORTC_DATA_R |=  0b010000;
         GPIO_PORTC_DATA_R &= ~0b100000;
          
      
        for(int i=0; i<5 ; i++){
         urun[i][0] = urun[i][0] - alinanUrun[i];
        
        
          if(urun[0][i]<0){
            lcdTemizle();
            Lcd_Goto(1,1);
            ekranaYazdir("yeterli urun yok");
            Lcd_Goto(2,1);
            ekranaYazdir("Yeniden secin");
            
            for (tmp = 0 ; tmp < 5000000 ; tmp++);            
            urun[i][0] = urun[i][0] + alinanUrun[i];            
            secim();
          } 
        }
        
        while(1){
        if ((GPIO_PORTE_DATA_R & 0b00010000) == 0) {
          GPIO_PORTC_DATA_R &= ~0b11111111l;
          Lcd_Goto(1,1);
          ekranaYazdir("resetleyin");
           break;
         }        
        }   
        
          Lcd_Goto(1,1);
          ekranaYazdir("s  t  k  c  b");
          Lcd_Goto(2,1);
          for(int i=0; i<5; i++){
          char y[] = "";  
          sprintf(y, "%d", urun[i][0]);
          char *yy = &y[0];
          ekranaYazdir(yy);
          ekranaYazdir(" ");    
          }
           for (tmp = 0 ; tmp < 10000000 ; tmp++);
        }       
     paraUstu();      
}


void paraUstu(){
  
  int toplamBorc = 0,toplam;
  for(int i=0; i<5; i++){
   toplamBorc = toplamBorc + (alinanUrun[i] * urun[i][1]);    
  }
  
  toplam = para[0]*100 + para[1]*50 + para[2]*25;
  
  int paraIade = toplam - toplamBorc ;
  
  
  char y[] = "";  
  sprintf(y, "%d", paraIade);
  char *yy = &y[0];
  lcdTemizle();
  Lcd_Goto(1,1);
  ekranaYazdir("iade edilecek");
  Lcd_Goto(2,1);
  ekranaYazdir("tutar  ");
  ekranaYazdir(yy);
  for (tmp = 0 ; tmp < 10000000 ; tmp++);
  
  int flag=1;
  while(paraIade<0){
   if(flag != 0){
    lcdTemizle();
    ekranaYazdir("yetersiz bakiye."); 
    flag=0;
   }
  }
  
  flag = 0;
  int iade[] = {0,0,0};
  while(paraIade>0){
    if(paraIade >= 100 && kasa[2]>=1){
     paraIade=paraIade-100;
     kasa[2]--;
     iade[2]++;
    }else if(paraIade >= 50 && kasa[1]>=1){
     paraIade= paraIade-50; 
     kasa[1]--;
     iade[1]++;
    }else if(paraIade >= 25 && kasa[0]>=1){
     paraIade = paraIade - 25;
     kasa[0]--;
     iade[0]++; 
    }else{
      if(flag == 0){
      lcdTemizle();
      ekranaYazdir("kasada para yok"); 
      flag++;
    }
    } 
  }

  lcdTemizle();
  ekranaYazdir("*1tl 50krs 25krs");
  Lcd_Goto(2,2);
  for(int i=2; i>=0 ; i--){
      char xx[] = "";
      sprintf(xx, "%d", iade[i]);
      char *x = &xx[0];
      ekranaYazdir(x);
      ekranaYazdir("    ");
  }
  for (tmp = 0 ; tmp < 10000000 ; tmp++);

  lcdTemizle();
  ekranaYazdir("1tl 50krs 25krs");
  Lcd_Goto(2,2);
  for(int i=2; i>=0 ; i--){
      char xx[] = "";
      sprintf(xx, "%d", kasa[i]);
      char *x = &xx[0];
      ekranaYazdir(x);
      ekranaYazdir("    ");
 
  }
}



void atilanPara(){//atilan parayi hesaplar
  int toplam =   para[0]*100 + para[1]*50 + para[2]*25;
  
  lcdTemizle();
  
  int kurus = toplam%100;
  int ytl = toplam/100;
  
  char tl[] = "";
  sprintf(tl, "%d", ytl);
  char *x = &tl[0];
  Lcd_Goto(1,1);
  ekranaYazdir(x);
  
  ekranaYazdir(".");  
  
  char krs[] = "";  
  sprintf(krs, "%d", kurus);
  char *y = &krs[0];
  ekranaYazdir(y);

  ekranaYazdir(" tl");

  Lcd_Goto(2,1);
  ekranaYazdir("para attiniz.");
}


void secim() { // urun secme fonksiyonu
  lcdTemizle();
  while(1){
  if((GPIO_PORTA_DATA_R & 0b00000100) == 0){
        Lcd_Goto(1,1);
        alinanUrun[0]++;
        ekranaYazdir("sut alindi.   ");
        bekle();
  }else if((GPIO_PORTA_DATA_R & 0b00001000) == 0){
        Lcd_Goto(1,1);
        alinanUrun[1]++;
        ekranaYazdir("cay alindi.   ");
        bekle();
  }else if((GPIO_PORTA_DATA_R & 0b00010000) == 0){
        Lcd_Goto(1,1);
        alinanUrun[2]++;
        ekranaYazdir("kahve alindi.   ");
        bekle();
  }else if((GPIO_PORTA_DATA_R & 0b00100000) == 0){
        Lcd_Goto(1,1);
        alinanUrun[3]++;
        ekranaYazdir("cikolata alindi.   ");
        bekle();
  }else if((GPIO_PORTA_DATA_R & 0b01000000) == 0){
        Lcd_Goto(1,1);
        alinanUrun[4]++;
        ekranaYazdir("biskuvi alindi.   ");
        bekle();
  }else if((GPIO_PORTA_DATA_R & 0b10000000) == 0){
        Lcd_Goto(1,1);
        ekranaYazdir("Urun alma      ");
        Lcd_Goto(2,1);
        ekranaYazdir("isi tamamlandi");
        break;
  }else if ((GPIO_PORTE_DATA_R & 0b00010000) == 0) {
        Lcd_Goto(1,1);   
        lcdTemizle();
        ekranaYazdir("iptal       ");
        for(int i=0; i<5; i++){
        alinanUrun[i]=0;
        }
        for (tmp = 0 ; tmp < 4000000 ; tmp++);
        secim();
  }
  }
  hata();
}

void bekle(){ // bekleme yapar
          for (tmp = 0 ; tmp < 300000 ; tmp++);
}

void portlariAktiflestir() {

  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;          // E portunu aktive et
  tmp = SYSCTL_RCGCGPIO_R;                       // E portunun aktive edilmesini bekle
  GPIO_PORTE_AMSEL_R = 0x00;    	         // E portunun analog modunu devre dışı bırak
  GPIO_PORTE_PCTL_R = 0x00000000;                // E portundaki pinlerin voltajını düzenle 
  GPIO_PORTE_DIR_R = 0x00;      	         // E portunun giriş çıkışlarını belirle
  GPIO_PORTE_AFSEL_R = 0x00;    	         // E portundaki alternatif fonksiyonları seç
  GPIO_PORTE_PUR_R = 0x3F;                       // Buton ıcın
  GPIO_PORTE_DEN_R = 0x3F;              	 // E portunun pinlerini aktifleştir

  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;		// A portunu aktive et
  tmp = SYSCTL_RCGC2_R;				// A portunun aktive edilmesini bekle
  GPIO_PORTA_AMSEL_R &= ~0b00000000;		// A portunun analog modunu devre dışı bırak
  GPIO_PORTA_PCTL_R &= ~0x00000000;		// A portundaki pinlerin voltajını düzenle 
  GPIO_PORTA_DIR_R |= 0b00000000;		// A portunun giriş çıkışlarını belirle
  GPIO_PORTA_AFSEL_R &= 0b00000000;		// A portundaki alternatif fonksiyonları seç
  GPIO_PORTA_PUR_R = 0xFF;                      // Buton ıcın
  GPIO_PORTA_DEN_R = 0xFF; 		        // A portunun pinlerini aktifleştir
  
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;        // C portunu aktif et
  tmp = SYSCTL_RCGC2_R;                        // C portunun aktif edilmesini bekle
  GPIO_PORTC_DIR_R |= 0b11110000;              // C portundaki giris cikislari belirle
  GPIO_PORTC_AFSEL_R &= ~0xF0;
  GPIO_PORTC_DEN_R |= 0b11110000;              // C portunun pinlerini aktiflestir.
  
}

void Lcd_init() {
 SysCtlPeripheralEnable(LCDPORTENABLE);
  GPIOPinTypeGPIOOutput(LCDPORT, 0xFF);

  SysCtlDelay(50000);

  GPIOPinWrite(LCDPORT, RS,  0x00 );

  GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7,  0x30 );
  GPIOPinWrite(LCDPORT, E, 0x02);
  SysCtlDelay(10);
  GPIOPinWrite(LCDPORT, E, 0x00);

  SysCtlDelay(50000);

  GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7,  0x30 );
  GPIOPinWrite(LCDPORT, E, 0x02);
  SysCtlDelay(10);
  GPIOPinWrite(LCDPORT, E, 0x00);

  SysCtlDelay(50000);

  GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7,  0x30 );
  GPIOPinWrite(LCDPORT, E, 0x02);
  SysCtlDelay(10);
  GPIOPinWrite(LCDPORT, E, 0x00);

  SysCtlDelay(50000);

  GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7,  0x20 );
  GPIOPinWrite(LCDPORT, E, 0x02);
  SysCtlDelay(10);
  GPIOPinWrite(LCDPORT, E, 0x00);

  SysCtlDelay(50000);

  Lcd_Komut(0x28);
  Lcd_Komut(0xC0);
  Lcd_Komut(0x06);
  Lcd_Komut(0x80);
  Lcd_Komut(0x28);
  Lcd_Komut(0x0f);
  lcdTemizle();

}

void Lcd_Komut(unsigned char c) {

  GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7, (c & 0xf0) );
  GPIOPinWrite(LCDPORT, RS, 0x00);
  GPIOPinWrite(LCDPORT, E, 0x02);
  SysCtlDelay(50000);
  GPIOPinWrite(LCDPORT, E, 0x00);

  SysCtlDelay(50000);

  GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7, (c & 0x0f) << 4 );
  GPIOPinWrite(LCDPORT, RS, 0x00);
  GPIOPinWrite(LCDPORT, E, 0x02);
  SysCtlDelay(10);
  GPIOPinWrite(LCDPORT, E, 0x00);

  SysCtlDelay(50000);

}

void Lcd_Putch(unsigned char d) {

  GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7, (d & 0xf0) );
  GPIOPinWrite(LCDPORT, RS, 0x01);
  GPIOPinWrite(LCDPORT, E, 0x02);
  SysCtlDelay(10);
  GPIOPinWrite(LCDPORT, E, 0x00);

  SysCtlDelay(50000);

  GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7, (d & 0x0f) << 4 );
  GPIOPinWrite(LCDPORT, RS, 0x01);
  GPIOPinWrite(LCDPORT, E, 0x02);
  SysCtlDelay(10);
  GPIOPinWrite(LCDPORT, E, 0x00);

  SysCtlDelay(50000);

}

void Lcd_Goto(char x, char y){
  if(x==1)
    Lcd_Komut(0x80+((y-1)%16));
  else
    Lcd_Komut(0xC0+((y-1)%16));
}

void lcdTemizle(void){
  Lcd_Komut(0x01);
  SysCtlDelay(10);
}

void ekranaYazdir( char* s){
  while(*s)
    Lcd_Putch(*s++);
}

