/*Arduino UNO için hazırlanan PCB ye uygun kod....
MF(Üyelik fonk.): Üçgen (trim)
Antecedent      : AND yani min  -> Kuralların if kısımlarına bulanık operator uygulama işlemi...
impilacation    : AND yani min  -> Kuralların if ve then kısımlarına bulanık operator uygulama işlemi
Aggregation     : OR yani max   -> imlication (Gerektirme) işlemi sonuçlarının birleştirilmesi
Defuzzify       : Centroid      -> Sonuc degerin keskin deger olarak üretilmesi işlemi....
*/
#include <math.h>   //c matematik kütüphanesi dahil ediliyor

#define FIS_TYPE float     // float kesirli sayı tipo değişken FIS_TYPE olarak tanımlandı
#define FIS_RESOLUSION 101 // Çıkışta oluşan sonuç grafiğin x ekseninin taranması için çözünürlük değeri..
//***
#include <IRremote.h>

#define vestelTahta 0x679830CF     //nec 32 bit
#define vestelEsra1 0x84C   //rc5 12 bit
#define vestelEsra2 0x4C   //rc5 12 bit
#define samsungNghH 0xE0E040BF // SAMSUNG 32 bits
#define lgAtillaH 0x20DF10EF //NEC (32 bits)

int bas=1;
IRsend irsend;
//*****
#define tvLed 12        //TV durumunu gösteren LED bağlantı pini tanımlaması.
#define uyariLed 13     //UYARI LED bağlantı pini tanımlaması
#define trig1 10        //Ultrasonic sensöreler bağlantı pinleri tanımlamaları.
#define echo1 11
#define trig2 8
#define echo2 9
#define irRpin 7       //Infrared alıcı bağlantı pini tanımlaması
#define UzSinir 150.0
#define UzYakin 100.0
#define UzCokYakin 50.0 
#define kapali 1.0    // TV ait durumların tanılmaları
#define acik 0.0
//-->  mega2560 oc2b pin 9 pwm pin  IR send için. 
//-->  9 nolu pwm pinini "IRsend" kullanıyor...
//Uno iççin 3 nolu pin
//*****
// Sensör girişi için üçgen üyelik fonksiyonlarının değerleri a,b,c olarak dizilere aktarılıyor.
FIS_TYPE mfi_S1_2_ua[5] = { 0.0, 49.0, 99.0, 149.0, 199.0 };    //  üçgenin a ucu
FIS_TYPE mfi_S1_2_ub[5] = { 0.0, 75.0, 125.0, 175.0, 250.0 };   //  üçgenin b ucu
FIS_TYPE mfi_S1_2_uc[5] = { 50.0, 100.0, 150.0, 200.0, 300.0 }; //  üçgenin c ucu
//Zaman girişi için üçgen üyelik fonksiyonlarının değerleri a,b,c olarak dizilere aktarılıyor.
FIS_TYPE mfi_Z1_ua[5] = { 0.0, 3.0, 7.0, 11.0, 15.0 };
FIS_TYPE mfi_Z1_ub[5] = { 0.0, 5.5, 10.0, 14.5, 22.5 };
FIS_TYPE mfi_Z1_uc[5] = { 5.0, 8.0, 12.0, 17.0, 30.0 };

//Çıkış için üyelik fonk. degerleri (a,b,c) 
//                       TVac ,TVkapat
FIS_TYPE mfO_Ac_ua[2] = { 0.0, 1.0 };
FIS_TYPE mfO_Ac_ub[2] = { 0.0, 1.0 };
FIS_TYPE mfO_Ac_uc[2] = { 0.0, 1.0 };

//Kuralların çıkış(then) kısımlarına ait indekslenmiş tablosu:
//Çıkış üyelik fonk tutan dizilerdeki ilgili üyelik fonk. hangi 
//indekste olduğunu gösteriyor... Excell de hazırlanan tabloya göre 
//hazırlanmıştır...
int kurallar[25] = {
          			0, 0, 0, 1, 1,
          			0, 0, 0, 1, 1,
          			0, 0, 0, 1, 1,
          			0, 0, 0, 0, 0,
          			0, 0, 0, 0, 0
								};
FIS_TYPE bulanikS1_2[5]={0.0,0.0,0.0,0.0,0.0};// Sensör girişlerine ait bulanık değerlerini tutan dizi.
FIS_TYPE bulanikZ1[5]={0.0,0.0,0.0,0.0,0.0};//Süre girişine ait bulanık değerlerini tutan dizi.

FIS_TYPE dizi_blOperator[25];	// Girişlerin bulanık değerlerine bulanık operatör(and-min) uygulama işelminin
                              // sonuçlarını tutan dizi. Antecedent işlemi.
FIS_TYPE dizi_imp[25];		    // Implication(gerektirme)işleminin sonucunun tutulduğu dizi min operatoru ile..

FIS_TYPE s1 =116.0; // Sensör girişi 1
FIS_TYPE s2 =135.0; // Sensör giriş 2
FIS_TYPE z1 = 8.0;  // Zaman değişkeni
FIS_TYPE tv = 0.0;  // Bulanık çıkarım işlemi sonucunu tutan değişken.
                    //TV nin açık, kapalı olmasına bu değişkenin
                    // sonucuna göre karar veriliyor...
FIS_TYPE tv1 = 0.0; // Sensör1'e göre Bulanık çıkarım işlemi sonucunu tutan değişken.
FIS_TYPE tv2 = 0.0; // Sensör2'ye göre Bulanık çıkarım işlemi sonucunu tutan değişken.

long olc_ms = 0, olc_ms2 = 0, olc = 0;// Zaman ölçme işleminde kullanılan değişkenler.
long sure = 0.0;

bool tvAcik = 1;  // TV nin açık olup olmadığını kontrol eder 1 ise tv Açık 
                  //değilse kapalıdır

void setup() {
   pinMode(tvLed,OUTPUT);
   pinMode(uyariLed,OUTPUT);
   pinMode(irRpin,INPUT);
   digitalWrite(uyariLed,HIGH);
   digitalWrite(tvLed,LOW);
   Serial.begin(9600);
   delay(1000);
   digitalWrite(uyariLed,LOW);
}

void loop() {
	  //s1=random(0,300);
    //s2=random(0,300);
    //z1=random(0,30);
    olc=millis();

    s1=HC_SR04(trig1,echo1);    //Sensörlerden mesafe değeri alınıyor...
    s2=HC_SR04(trig2,echo2);
    
    if(s1>250) s1=250.0;      // Mesafe değerleri 250 cm ile sınırlanıyor..
    if(s2>250) s2=250.0;
    if(z1>25) z1=25.0;        // Süre değeri 25 saniye ile sınırlendırılıyor...
   
    
    if((s1<UzSinir)||(s2<UzSinir)){  //150cm... Bulanık çıkarım bu noktadan sonra çalışacak..
      
        if((s1<UzYakin)||(s2<UzYakin)){ //100
          uyariLedBlink(25);
        }else digitalWrite(uyariLed,HIGH);
              
        olc_ms2=millis();
        if((olc_ms2-olc_ms)>750){
          olc_ms=millis();   //sureyi yeniden başlat
          z1++;
        }     
        
        girisleriBulaniklastir(s1,z1);  //1. sensör girişi için..
        bulanikOperatorUygula(bulanikS1_2, bulanikZ1);
        tv1= durulama();
  
        girisleriBulaniklastir(s2, z1); //2. sensör girişi için..
        bulanikOperatorUygula(bulanikS1_2, bulanikZ1);
        tv2= durulama();

        tv = max(tv1, tv2); // tv=(s1 veya s2) ve (z1)=(s1+s2).z1 (Mantıksal işlemi sonucu)
   
        
    }//if sınır uzaklık
    else{
      digitalWrite(uyariLed,LOW);
      z1=0.0;
      tv = 0.0; tv1 = 0.0; tv2 =0.0; //Çıkışın durumunu belirleyen keskin değerler resetleniyor
    }
    //....Hesaplanan tv sonucu değerlendirilerek işlem yapılıyor...
    if(tv==kapali){  
        //1.0    * kapalı:TV açık ise kapat değilse kapalı kalsın.
            digitalWrite(tvLed,HIGH);
            if(tvAcik){ // TV açık ise kapat
              tvOnOff(1);  
             // tvOnOff(2);//esra tv
              tvAcik=0;  //TV kapalı durumu
            }     
     }
     else if(tv==acik){   
            //0.0  * açık: TV kapalı ise aç değilse açık kalsın
            digitalWrite(tvLed,LOW);
            if(!tvAcik){ // tv kapalı ise aç
              tvOnOff(1);
              //tvOnOff(2);//esra tv
              tvAcik=1;  //TV açık durumu
              delay(5000);//tv nin açılmaSINI BEKLE
            }
    }
    
    Serial.print("S1 : "); Serial.println(s1);
    Serial.print("S2 : "); Serial.println(s2);
    Serial.print("Z1 : "); Serial.println(z1);

    Serial.print("TV : "); Serial.println(tv);
   // Serial.print("sure : "); Serial.println(millis()-olc);
    Serial.println("----------------------------------------");
  
}
//******************** Fonksiyonlar ***************************** //
//Girişe ait keskin değerlerin Üçgen fonksiyonlarına göre bulanıklaştırma işleminin
//yapıldığı fonksiyon... Bu işlem sonucunda keskin giriş değeri bulanıklaştırılarak
//her bir giriş fonksiyonundaki üyelik değeri bir dizide tutulur...
void girisleriBulaniklastir(FIS_TYPE keskin_g1, FIS_TYPE keskin_g2){
	int x = 0;
  
	for (x = 0; x < 5; x++) 
	  bulanikS1_2[x] = ucgenUyelikFonk(keskin_g1, mfi_S1_2_ua[x], mfi_S1_2_ub[x], mfi_S1_2_uc[x]);
	for (x = 0; x < 5; x++)
	  bulanikZ1[x] = ucgenUyelikFonk(keskin_g2, mfi_Z1_ua[x], mfi_Z1_ub[x], mfi_Z1_uc[x]);
}
//antecedent işlemi: kuralların if kısmına bulanık operator işlemi uygula
//                                    s           z
void bulanikOperatorUygula(FIS_TYPE* blg1, FIS_TYPE* blg2){
	int i = 0;
	for (int x = 0; x < 5; x++){
		for (int y = 0; y < 5; y++){
			dizi_blOperator[i++] = min(blg1[x], blg2[y]); 
			//Kuralların if kısımlarına bulanık operator and-->min
			//uygulandı ve diziye alındı...
		}
	}
}
// defuzz:centroid (Ağırlık Merkezi) yöntemi ile bulanık olan çıkış gerçek çıkış değerine dönüştürülecek...
// Her bir çıkış x değerinin girişe olan etkisi bulunarak çıkış keskin değeri bulunur...

double durulama(){
  // Çıkışa ait    max    min değerleri...
	FIS_TYPE adim = (1.0 - 0.0) / (FIS_RESOLUSION - 1.0);// çıkışın girişe etkisi bulunurken çıkışın hangi adımda
                                                       // taranacağı belirlenir... 
	FIS_TYPE x_ekseni = 0.0;                             // x ekseni boyunca oluşan sonuç eğri için x ekseni değeri... 
	FIS_TYPE degisim = 0.0, alan = 0.0, parca = 0.0;
	FIS_TYPE mfCikis = 0.0;

	for (int i = 0; i < FIS_RESOLUSION; i++){

		x_ekseni = 0 + (adim * i);
    
		gerektirmeUygula(x_ekseni);     // Gerektirme işlemi...
		mfCikis = sonuclariBirlestir(); // Aggretation....
    //** 
		parca = adim * mfCikis;         // x ekseni üzerinde alınan parçanın etkisi...
		degisim += parca * x_ekseni;//pay..... 
		alan += parca;              //payda... parçaları toplayarak oluşan sonuç eğrinin alanı bulunur...
    //**
	}
	return ((alan == 0) ? ((1.0 + 0.0) / 2.0) : (degisim / alan));
}
//      
void gerektirmeUygula(FIS_TYPE xEkGiris)   //implication işlemi
{    
	int indeks = 0;
	FIS_TYPE bulanikCikis = 0.0;
	//Butun kuralları her bir çıkış fonk. x ekseni girişi için tara...
	for (int x = 0; x <25; x++){
		indeks = kurallar[x];     //kurallar dizisinden kurala ait indeks değerini getir...
                              //bu değer çıkış üçgen üyelik fonksiyonununun değerilerini tutan
                              //diziden getirilecek değerlerin dizi eleman indeksidir...
		bulanikCikis = ucgenUyelikFonk(xEkGiris, mfO_Ac_ua[indeks], mfO_Ac_ub[indeks], mfO_Ac_uc[indeks]);
		dizi_imp[x] = min(bulanikCikis, dizi_blOperator[x]);// kuralın if(eğer) kısımlarıyla then(ise) kısımlarına
                                                        // belirlenen bulanık operatör(min) uygulanır...
	}
}
// kurallara ait if ve then kısımları belirlenen bulanık operatörle birleştirildikten sonra
// elde edilen diziden keskin sonuç değeri oluşturmak için bulanık operetaor(max) uygulanır.
// sonuç olarak tek bir keskin giriş elde edilmiş olur. Bu sonuç durulama işleminde kullanılır...
double sonuclariBirlestir()  //aggregation işlemi
{
	FIS_TYPE sonuc;
	FIS_TYPE dizi_1[25];

	for (int x = 0; x < 25; x++){
		dizi_1[x] = dizi_imp[x];
	}
	sonuc = dizi_1[0];
	for (int z = 1; z < 25; z++){
		sonuc = max(sonuc, dizi_1[z]);
	}
	return sonuc;
}
//
// Üçgen üyelik fonksiyonları için keskin girişi bulanıklaştıran fonksiyon...
FIS_TYPE ucgenUyelikFonk(FIS_TYPE x, FIS_TYPE a, FIS_TYPE b, FIS_TYPE c)
{
	FIS_TYPE t1 = (x - a) / (b - a);
	FIS_TYPE t2 = (c - x) / (c - b);
	if ((a == b) && (b == c)) return (FIS_TYPE)(x == a);
	if (a == b) return (FIS_TYPE)(t2*(b <= x)*(x <= c));
	if (b == c) return (FIS_TYPE)(t1*(a <= x)*(x <= b));
	t1 = min(t1, t2);
	return (FIS_TYPE)max(t1, 0);
}

//double ucgenUyelikFonk(double x, double a, double b, double c){
//
//	double t1 = (x - a) / (b - a);
//	double t2 = (c - x) / (c - b);
//
//	if ((a == b) && (b == c)) return (double)(a==x);
//	if (a == b){
//		if ((a <= x) && (x <= c)) return (double)(t2);
//	}//else ne olacakk.....
//	if (b == c){
//		if ((a <= x) && (x <= b)) return (double)(t1);
//	}//
//	t1 = min(t1, t2);
//	return (double)max(t1, 0);
//}

// *****  Fonksiyonlar ************   /////
void tvOnOff(int bas_durum){
  
  if(bas_durum==2){
    if(bas==1){
      irsend.sendRC5(vestelEsra1,12);
      bas=2;
    }
    if(bas==2){
      bas=1;
      irsend.sendRC5(vestelEsra2,12);
    }
  }
  else if(bas_durum==1){
    //irsend.sendNEC(vestelTahta,32);
    //irsend.sendSAMSUNG(samsungNghH,32);
    irsend.sendNEC(lgAtillaH,32);
  }
}
////****  sensorler
double HC_SR04(int trigPin,int echoPin)
{
  long duration = 0;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  //delay(50);
  return (double)(duration / 29 / 2);//cm
}
//***
void uyariLedBlink(int sure){
  
  for(int i=0;i<10;i++){
          digitalWrite(uyariLed,HIGH);
          delay(sure);
          digitalWrite(uyariLed,LOW);
          delay(sure);
        }
}
