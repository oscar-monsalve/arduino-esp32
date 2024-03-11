// Program to control the sequence of delta-star induction motor start

// Inputs:
// - Stop button
// - Right turn button
// - Left turn button

// Outputs:
// - KM1 (left turn)
// - KM2 (right turn)
// - KM3 (delta conection)
// - KM4 (star conection)

// Inputs (buttons)
const int PulsadorParo = 33; //Pulsador que detiene el sistema3
const int PulsadorIzquierda = 32; // Pulsador Giro Izquierda
const int PulsadorDerecha = 25; // Pulsador Giro Derecha

// Digital outputs (relays)
const int KM1 = 26; //Giro Izquierda
const int KM2 = 27; //Giro derecha
const int KM3 = 14; //Estrella
const int KM4 = 12; // Delta

//Variables que guardan el estado de los pulsadores.
int EstadoPulsadorParo = 0;
int EstadoPulsadorIzquierda = 0;
int EstadoPulsadorDerecha = 0;

//Variables que servirán para activar o desactivar la parte del código funcional a los temporizadores.
bool TemporizadorGiroIzquierda = false;
bool TemporizadorGiroDerecha = false;

//Marca que indica si el sistema está detenido o no. sirve para anular un giro contrario antes de presionar el pulsador paro.
bool SistemaDetenido = true;

//Variables que sirven para llamar la variable millis() de otra forma en el código.
unsigned long previousMillisIzquierda = 0;
unsigned long previousMillisDerecha = 0;

//Tiempo designado para que se realice el cambio de estrella a delta, según el sentido de giro.
const unsigned long TiempoEstrellaGiroIzquierda = 2000;
const unsigned long TiempoEstrellaGiroDerecha = 2000;


void setup() {
    pinMode(PulsadorParo,INPUT); // pull-up
    pinMode(PulsadorDerecha,INPUT); // pull-up
    pinMode(PulsadorIzquierda,INPUT); // pull-up

    pinMode(KM1,OUTPUT);
    pinMode(KM2,OUTPUT);
    pinMode(KM3,OUTPUT);
    pinMode(KM4,OUTPUT);

    //Inician los relés inactivos. Recordar que la programación en el módulo indica que HIGH envía 0V y LOW envía 3,3V
    digitalWrite(KM1,HIGH);
    digitalWrite(KM2,HIGH);
    digitalWrite(KM3,HIGH);
    digitalWrite(KM4,HIGH);
}

void loop() {
    Arranque();
}

void Arranque() {
    // Lectura del estado de los pulsadores
    EstadoPulsadorParo = digitalRead(PulsadorParo);
    EstadoPulsadorIzquierda = digitalRead(PulsadorIzquierda);
    EstadoPulsadorDerecha = digitalRead(PulsadorDerecha);

    //Inicio de la variable millis(). Guarda el tiempo hasta 50 días y después se reinicia.
    unsigned long currentMillis = millis();

    // Giro izquierda
    //Pregunta si el pulsador está presionado (enviando un cero lógico) y evalua condiciones. Pulsador ubicado en la izquierda.
    if(EstadoPulsadorIzquierda==LOW && TemporizadorGiroDerecha == false && SistemaDetenido == true) {
        previousMillisIzquierda = currentMillis; //Se igualan ambas variables, para tener la acción de millis().
        digitalWrite(KM1, HIGH); //Desactiva el giro a la derecha
        digitalWrite(KM2, LOW); //Activa el giro a la izquierda
        digitalWrite(KM3, LOW); //Activa la conexión en estrella
        digitalWrite(KM4, HIGH); //Desactiva la conexión en delta
        TemporizadorGiroIzquierda = true; //Coloca como verdadera la variable TemporizadorGiroIzquierda, que es la que da apertura al temporizador.
        SistemaDetenido = false; //Marca que evita que se active el giro contrario
    }

    //Ejecuta el tiempo designado cuando lee TiempoEstrellaGiroIzquierda = True. Luego realiza las acciones.
    if(TemporizadorGiroIzquierda) {
        if (currentMillis - previousMillisIzquierda >= TiempoEstrellaGiroIzquierda) { //Representa el temporizador para el cambio de estrella-delta
            digitalWrite(KM1, HIGH); //Desactiva el giro a la derecha
            digitalWrite(KM2, LOW); //Activa el giro a la izquierda
            digitalWrite(KM3, HIGH); //Desactiva la conexión en estrella
            digitalWrite(KM4, LOW); //Activa la conexión en delta
            TemporizadorGiroIzquierda = false; //Termina el tiempo y se sale de la función
        }
    } // Fin código giro a la izquierda.

    // Giro a la derecha
    //Pulsador ubicado en la derecha.
    //Pregunta si el pulsador está presionado (enviando un cero lógico) y evalua condiciones.
    if(EstadoPulsadorDerecha==LOW && TemporizadorGiroIzquierda == false && SistemaDetenido == true) {
        previousMillisDerecha = currentMillis; //Se igualan ambas variables, para tener la acción de millis().
        digitalWrite(KM1, LOW); //Activa el giro a la derecha.
        digitalWrite(KM2, HIGH); //Desactiva el giro a la izquierda.
        digitalWrite(KM3, LOW); //Activa la conexión en estrella.
        digitalWrite(KM4, HIGH); //Desactiva la conexión en delta.
        TemporizadorGiroDerecha = true; //Coloca como verdadera la variable TemporizadorGiroDerecha, que es la que da apertura al temporizador.
        SistemaDetenido = false; //Marca que evita que se active el giro contrario.
    }
    //Ejecuta el tiempo designado cuando lee TiempoEstrellaGiroDerecha = True. Luego realiza las acciones.
    if(TemporizadorGiroDerecha) {
        //Representa el temporizador para el cambio de estrella-delta.
        if (currentMillis - previousMillisDerecha >= TiempoEstrellaGiroDerecha) {
            digitalWrite(KM1, LOW); //Activa el giro a la derecha
            digitalWrite(KM2, HIGH); //Desactiva el giro a la izquierda
            digitalWrite(KM3, HIGH); //Desactiva la conexión en estrella
            digitalWrite(KM4, LOW); //Activa la conexión en delta
            TemporizadorGiroDerecha = false; //Termina el tiempo y se sale de la función
        }
    } //Fin código giro a la derecha.

    // Paro
    //Pulsador ubicado en la izquierda. Pregunta si el pulsador está presionado (enviando un cero lógico)
    if(EstadoPulsadorParo==LOW) {
        digitalWrite(KM1, HIGH); //Desactiva el giro a la derecha
        digitalWrite(KM2, HIGH); //Desactiva el giro a la izquierda
        digitalWrite(KM3, HIGH); //Desactiva la conexión en estrella
        digitalWrite(KM4, HIGH); //Desactiva la conexión en delta
        TemporizadorGiroIzquierda = false; //Detiene el temporizador de giro a la izquierda
        TemporizadorGiroDerecha = false; //Detiene el temporizador de giro a la derecha
        SistemaDetenido = true; //Marca que indica que el sistema está detenido
        delay(3000);
    } //Fin código Paro

}
