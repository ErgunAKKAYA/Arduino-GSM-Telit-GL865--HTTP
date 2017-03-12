#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial cell(11, 10); 
float temp;
void setup()
{
  cell.begin(9600);   // GSM Seri haberleşme başlangıç Baud hızı (uno için 9600)
  Serial.begin(9600); // PC Seri haberleşme başlangıç Baud hızı
  delay(1000);

  cell.println("AT");
  Serial.print("AT:");

  delay(1000);
  cevap_oku();

  cell.println("AT&K0");
  Serial.print("ATK0:");
  delay(1000);
  cevap_oku();

  cell.println("AT+CREG?");
  Serial.print("CREG?");
  delay(1000);
  cevap_oku();



  cell.println("AT#GPRS?");
  delay(3000);
  Serial.print("GPRS:");
  cevap_oku();

  cell.println("AT#GPRS=1");
  delay(3000);
  Serial.print("GPRS:");
  cevap_oku();

  cell.println("AT+CGDCONT=1,\"IP\",\"internet\",\"0.0.0.0\",0,0");
  delay(3000);
  cevap_oku();

}

void loop()
{

  temp = analogRead(A0);
  Serial.print("Temp:");
  Serial.println(temp);
  sending();    // temp değişkeninin içeriğini internete yollama prosedürü
  delay(20000); // Thingspeak gibi siteler için süre aralığı 20 sn yeterli 
}

void sending()
{

  cell.println("AT#SD=1,0,80,\"api.thingspeak.com\",0,0");//İnternet bağlantısı yapılıyor
  delay(3500);
  Serial.println("Baglanti:");
  cevap_oku();

  String str = "GET http://api.thingspeak.com/update?api_key=xxxxxxxxxxxxxxxxxxxxxxx=";
  str += (int (temp)); // yollanacak datanın gideceği server adresi ve += ile String devamı

  cell.println(str);// Server data transveri için modüle yükleme yapıldı
  delay(2000);

  cell.println((char)26);// Yollama komutu ( char(26)= Enter)
  delay(5000);// İnternetten gelecek cevap için bu bekleme önemli, süres ise 
              //bağlantı hızınızla alakalı değişir

  cell.println();
  cevap_oku();

}
void cevap_oku() // GSM modulüne yolladığımız komutlara karşı modülün veridiği 
                 // cevapları seri port ekranında görmemizi sağlayan kod parçası
{
  while (cell.available() != 0)
    Serial.write(cell.read());
}
