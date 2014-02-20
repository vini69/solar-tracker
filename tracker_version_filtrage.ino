//#include <Wire.h>
//#include "RTClib.h"

//RTC_DS1307 RTC;

#include <Stepper.h>

const int NombrePas=200;

const int Pas_1=8; //declaration constante de broche 
const int Pas_2=12; //declaration constante de broche 
const int Pas_3=11; //declaration constante de broche 
const int Pas_4=13; //declaration constante de broche 

Stepper pap(NombrePas, Pas_1, Pas_3, Pas_2, Pas_4);

int pinG=A0;
int pinD=A1;
int valeurG=0;
int valeurD=0;
int ecart=0;
int zoneInactivite=10;

char i=0;
int tableau[10];
int moyenneEcart=0;

int compteur=0;

int pinB=A2;

int ValeurBatterie=5;
int TensionRefBatterie=5;

int Annee = 0;
int Mois = 0;
int Jour=0;
int Heure = 10;
int Minute = 0;
int Seconde = 0;

// définition de la largeur du filtre

const int largeur_moy=10;



int i_moy=0;     // index du tableau des moyennes
int somme = 0;   // somme du tableau



void setup()   { 

  Serial.begin(9600);

//Wire.begin();
//RTC.begin();
//RTC.adjust(DateTime(__DATE__, __TIME__));

pap.setSpeed(15);  

pinMode(9,OUTPUT);
pinMode(10,OUTPUT);
analogWrite(9,50);
analogWrite(10,50);

pinMode(Pas_1, OUTPUT); 
pinMode(Pas_2, OUTPUT); 
pinMode(Pas_3, OUTPUT);  
pinMode(Pas_4, OUTPUT); 

digitalWrite(Pas_1,LOW);
digitalWrite(Pas_2,LOW);
digitalWrite(Pas_3,LOW);
digitalWrite(Pas_4,LOW);

/*
initialisation du tableau de valeurs et de la somme de celui-ci
*/
for (i=0;i<largeur_moy;i++)  {
  valeurG=analogRead(pinG);
  valeurD=analogRead(pinD);
  tableau[i]=valeurG-valeurD;  // enregistrement de la valeur
  somme+=tableau[i];           // calcul de la somme du tableau
  } 



}

void loop(){
 
  
//DateTime now = RTC.now();

//Annee=now.year();
//Mois=now.month();
//Jour=now.day();
//Heure=now.hour();
//Minute=now.minute();
//Seconde=now.second();

//Serial.println (Annee);
//Serial.println (Mois);
//Serial.println (Jour);
//Serial.println (Heure);
//Serial.println (Minute);
//Serial.println (Seconde);

//ValeurBatterie=analogRead(pinB);

//Serial.println (ValeurBatterie);

while(ValeurBatterie >= TensionRefBatterie && Heure >=6 && Heure<=20)

{

  somme-=tableau[i_moy];       // soutraction de la valeur qui sort du tableau
  valeurG=analogRead(pinG);
  valeurD=analogRead(pinD);
  tableau[i_moy]=valeurG-valeurD;  // enregistrement de la nouvelle valeur mesurée
  somme+=tableau[i_moy];       // addition de la nouvelle valeur a la somme totale
  moyenneEcart=somme/largeur_moy;  // calcul de la moyenne
  
  i_moy++;                         // incrementation de l'index du tableau
  
  if (i_moy>=largeur_moy)  {        // ré-initialisation de l'index pour qu'il ne dépasse pas la longeur du tableau  
      i_moy=0;
      }
  
  Serial.println(moyenneEcart);
  
 

if(moyenneEcart < zoneInactivite*-1 && compteur >= -70)
  {
    
    pap.step(-1);
    compteur = compteur - 1;
  }
  else if(moyenneEcart > zoneInactivite && compteur <= 70)
  {
    
    pap.step(1);
    compteur = compteur +1;
  }
  else
  {
     pap.step(0); 
     
    digitalWrite(Pas_1,LOW);
    digitalWrite(Pas_2,LOW);
    digitalWrite(Pas_3,LOW);
    digitalWrite(Pas_4,LOW);
    
  }  
  
//Serial.println(compteur);

}
}
