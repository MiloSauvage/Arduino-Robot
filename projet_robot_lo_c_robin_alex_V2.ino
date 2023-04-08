#include "Ultrasonic.h" // import de la bibliotèque

Ultrasonic ultrasonic(A0); // connected to digital pin  anal 0

#include <Servo.h> // import de la bibliotèque
Servo moteur_roueG; // sur le port 2
Servo moteur_roueD; // sur le port 3

const int moustache = 4; // the number of the pushbutton pin 4
const int moustache2 = 5;
int moustache_state = 0; // variable for reading the pushbutton status
int moustache_state2 = 0; // variable for reading the pushbutton status

const int button_start = 8; // connected to digital pin 8
int button_read_start = 0;  // the number of the pushbutton pin
int button_state_start = 0 ;    // variable for reading the pushbutton status

int timer1 = 0 ; // cooldown pour le bouton
int timer2 = 0 ;

int timer_capteur_g = 0 ; // cooldown maneuvre capteur
int timer_capteur_d = 0 ; // cooldown maneuvre capteur

long RangeInCentimeters; // variable de centimètre

const int line_finder =  6;    // connected to digital pin 6
int line_state_finder = 0;     // variable for reading the line_finder status

int led = 7;  // connected to digital pin 7


void setup()   {   // setting

  pinMode(button_start, INPUT); // initialize the digital pin   
  pinMode(line_finder, INPUT); // initialize the digital pin   
  pinMode(led, OUTPUT);  // initialize the digital pin     
  pinMode(moustache, INPUT);
  pinMode(moustache2, INPUT);
  
  Serial.begin(9600);  // initialize serial communications at 9600 bps:
  
  moteur_roueG.attach(2); // connection du moteur gauche
  moteur_roueD.attach(3); // connection du moteur droite


}

void loop() {
  timer2 = timer2 +1 ; // cooldown pour le bouton entre
  //Serial.println(timer2); // permet de voir le cooldown
  
  button_read_start = digitalRead(button_start); // verifie l'etat du bouton de depart

  if (button_read_start == HIGH && timer2 >= 100) { // si le bouton est appuyer
        delay(5000); // delay de 5s avat de commencer
        timer2 = 0 ; // réinitiallisation du timer
        button_state_start = 1 ; // passer la variable à true
        Serial.println("bouton apuyé");
  }
  while (button_state_start == 1 ){ // tant que le bouton est appuyer

    timer1 = timer1 +1 ; // cooldown pour le bouton sortie
    //Serial.println(timer2);
    //Serial.println(timer1);

         
    moteur_roueG.attach(2); // connection du moteur gauche
    moteur_roueD.attach(3); // connection du moteur gauche

    moustache_state = digitalRead(moustache ); // lit les informations du capteur moustache

    if (moustache_state == HIGH) {
        digitalWrite(led, HIGH); // allumer la lampe quand le capteur est en contact
        timer_capteur_g = timer_capteur_g + 1 ; // demare le compteurs
        //Serial.println("contacte");
    }
    else {
        digitalWrite(led, LOW); // eteindre la led
        timer_capteur_g = 0 ; // réinitialiser le cooldown
        //Serial.println("pas de contacte");
    }

    moustache_state2 = digitalRead(moustache2 ); // lit les informations du capteur moustache

    if (moustache_state2 == HIGH) {
        digitalWrite(led, HIGH); // allumer la lampe quand le capteur est en contact
        timer_capteur_d = timer_capteur_d + 1 ; // demare le compteurs
        //Serial.println("contacte");
    }
    else {
        digitalWrite(led, LOW); // eteindre la led
        timer_capteur_d = 0 ; // réinitialiser le cooldown
        //Serial.println("pas de contacte");
    }


    //Serial.print(timer_capteur_g);
    //Serial.println(" tick_g");
    //Serial.println(timer_capteur_d);
    //Serial.println(" tick_d");

    if(timer_capteur_g >= 200 ){
        defense_g();
    }
    if(timer_capteur_d >= 200 ){
        defense_d();
    }


    RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // recupère les cm grave au capteur à ultrason
    //Serial.print(RangeInCentimeters);//0~400cm
    //Serial.println(" cm");

    if(RangeInCentimeters <= 20 ){
        digitalWrite(led, HIGH); // allumer la led si il y a un obstacle a moins de 30 cm
        ultrason();
        delay(1100);

    }
    else{
      digitalWrite(led, LOW); //eteindre la led

    }

    moustache_state = digitalRead(moustache2); // recupère la valeur du capteur moustache

    if (moustache_state == HIGH) { // si le capteur moustache est enfoncer
        digitalWrite(led, HIGH);  //allumer la led
        //Serial.println("contacte");
    }
    else {
        digitalWrite(led, LOW);  //eteindre la led
        //Serial.println("pas de contacte");
    }

    button_read_start = digitalRead(button_start); // verifie l'etat du bouton de depart
    
    if (button_read_start == HIGH && timer1 >= 100) { // si le bouton est appuiyer

        button_state_start = 0; // remet le robot eteint
        timer1 = 0 ; // réinitialise le cooldown
        Serial.println("bouton apuyer");

    }

    line_state_finder = digitalRead(line_finder); // verifie l'etat du line finder
            
    if(line_state_finder == HIGH){ // si une couleur est detecter
          //Serial.println("couleur : n°1");
          moteur_roueG.write(0); // demarer la roue gauche
          moteur_roueD.write(360);  // demarer la roue droite
                 
                
      }    
     else  {
       evitement();
         }   
  }
  
   Serial.println("robot eteint");
   moteur_roueG.detach(); // eteint le moteur gauche
   moteur_roueD.detach(); // eteint le moteur droite
      
}

void evitement(){
  
    //Serial.println("couleur : n°2");
    moteur_roueG.write(360); //rotation
    moteur_roueD.write(0);  //rotation
    delay(1000);
    moteur_roueG.write(0);  //rotation
    moteur_roueD.write(0);  //rotation
    delay(2100);
     
}

void defense_g(){
     moteur_roueG.write(360); //demi-tour
     moteur_roueD.write(85);  //demi-tour
     delay(6000);
     moteur_roueG.write(265);
     moteur_roueD.write(360);
     delay(1300);
}

void defense_d(){
     moteur_roueG.write(85); //demi-tour
     moteur_roueD.write(0);  //demi-tour
     delay(4000);
     moteur_roueG.write(0);// rotation
     moteur_roueD.write(90); // rotation
     delay(1100);

}

void ultrason(){
    moteur_roueG.write(0); // rotation
    moteur_roueD.write(0); // rotation
    Serial.print("rotaion");
    delay(800);
    
}


   
  
