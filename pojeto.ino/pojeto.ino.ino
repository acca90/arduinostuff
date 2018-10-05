// Programa : Sensor de temperatura DS18B20
// Autor : FILIPEFLOP

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define TIMER_UMIDADE 10000
#define TIMER_TEMPERATURA 5000
#define TIMER_TEMPERATURA_UMIDADE 15000
#define TIMER_DISTANCIA 1000
#define TIMER_PASSA_AGUA 500 

// Porta do pino de sinal do DS18B20
#define TERMOMETRO 4 // MATHEUS
#define N_SENSORES 5

#define DHTPIN 2
#define DHTTYPE DHT22  // ALEX

#define PORTA_GOTAS = 3 // ONEIDE

const int pinSensor = 1; // FERNANDO

const uint8_t trig_pin = 8; // MANU LUCAS
const uint8_t echo_pin = 9;

int ACUMULADOR_TIMER_UMIDADE = 0;
int ACUMULADOR_TIMER_TEMPERATURA = 0;
int ACUMULADOR_TIMER_TEMPERATURA_UMIDADE = 0;
int ACUMULADOR_TIMER_DISTANCIA = 0;
int ACUMULADOR_PASSA_AGUA = 0;
long int ACUMULADOR_TEMPO_SEM_INTERRUPCAO = 0;

DHT dht(DHTPIN, DHTTYPE); 

/**
 * Declarações responsáveis pelo termometro
 */
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(TERMOMETRO);
// Armazena temperaturas minima e maxima
DallasTemperature sensors(&oneWire);
DeviceAddress sensor[N_SENSORES];


void setup(void) {
    Serial.begin(9600);
    pinMode(A0, INPUT);
    dht.begin();
    sensors.begin();
    int count = 0;
    for (int i=0;i<N_SENSORES;i++)
        sensors.getAddress(sensor[i], i);

    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    digitalWrite(trig_pin, LOW);
    pinMode(pinSensor, INPUT);

    attachInterrupt(
        digitalPinToInterrupt(3),
        passou_gota,
        RISING
    ); //Interrupção externa do sensor 1 (pino 19 do MEGA,3 do Uno), quando mudar para nível alto
}

/**
 * Mede temperatura
 */
void temperatura() {
    ACUMULADOR_TIMER_TEMPERATURA = 0;
    // Le a informacao do sensor
    sensors.requestTemperatures();
    float sum = 0;
    for (int i=0;i<N_SENSORES;i++) {
        sum += sensors.getTempC(sensor[i]);
    }
    float tempMedia = sum/N_SENSORES;
    // Mostra dados no serial monitor
    Serial.print("Temp Média: ");
    Serial.println(tempMedia);
}

/**
 * Mede distancia
 */
void temperaturaUmidade() {
    ACUMULADOR_TIMER_TEMPERATURA_UMIDADE = 0;
    // Leitura da umidade
    float umidade = dht.readHumidity();
    // Leitura da temperatura (Celsius)
    float temperatura = dht.readTemperature();
    // Verifica se o sensor esta respondendo
    if (!(isnan(umidade) || isnan(temperatura))) {
        Serial.print("DHTPIN: "); 
        Serial.print(temperatura);
        Serial.print(" *C  ");
        // Mostra a umidade no serial monitor e no display
        Serial.print(umidade);
        Serial.println(" %");
    }
}

/**
 * Mede umidade pelo analógico
 */
void umidade() { // GIOVANI
    ACUMULADOR_TIMER_UMIDADE = 0;
    Serial.print("Sensor de humildade: ");
    Serial.println(analogRead(A0));
}

/**
 * Mede distancia
 */
void distancia() {
    // Espera 0,5s (500ms) entre medições.
    ACUMULADOR_TIMER_DISTANCIA = 0;
    // Pulso de 5V por pelo menos 10us para iniciar medição.
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(trig_pin, LOW);
    /* Mede quanto tempo o pino de echo ficou no estado alto, ou seja,
    o tempo de propagação da onda. */
    uint32_t pulse_time = pulseIn(echo_pin, HIGH);
    /* A distância entre o sensor ultrassom e o objeto será proporcional a velocidade
    do som no meio e a metade do tempo de propagação. Para o ar na
    temperatura ambiente Vsom = 0,0343 cm/us. */
    double distance = 0.01715 * pulse_time;
    // Imprimimos o valor na porta serial;
    Serial.print(distance);
    Serial.println(" cm");
}

/**
 * Acumula tempos
 */
void acumular(int tempoAcumular) {
    delay(tempoAcumular);
    ACUMULADOR_TIMER_UMIDADE += tempoAcumular;
    ACUMULADOR_TIMER_TEMPERATURA += tempoAcumular;
    ACUMULADOR_TIMER_TEMPERATURA_UMIDADE += tempoAcumular;
    ACUMULADOR_TIMER_DISTANCIA += tempoAcumular;
    ACUMULADOR_TEMPO_SEM_INTERRUPCAO += tempoAcumular;
}

void detectaPassaAgua() {
    ACUMULADOR_PASSA_AGUA = 0;
    //analogRead(pinSensor);
}

void passou_gota() {
    Serial.print("Tempo em milisegundos: ");
    Serial.println(ACUMULADOR_TEMPO_SEM_INTERRUPCAO);
    ACUMULADOR_TEMPO_SEM_INTERRUPCAO = 0;
}

void detectaAtrasoGota20Segundos() {
    if (ACUMULADOR_TEMPO_SEM_INTERRUPCAO == 20000) {
        Serial.println("Atraso na passagem de água");
    }
}

/**
 * Loop principal
 */
void loop() {
    
    acumular(1);

    detectaAtrasoGota20Segundos();

    if (ACUMULADOR_PASSA_AGUA >= TIMER_PASSA_AGUA) {
        //detectaPassaAgua();
    }
    if (ACUMULADOR_TIMER_DISTANCIA >= TIMER_DISTANCIA) {
        //distancia();
    }
    if (ACUMULADOR_TIMER_UMIDADE >= TIMER_UMIDADE) {
        umidade();
    }
    if (ACUMULADOR_TIMER_TEMPERATURA >= TIMER_TEMPERATURA) {
        temperatura();
    }
    if (ACUMULADOR_TIMER_TEMPERATURA_UMIDADE >= TIMER_TEMPERATURA_UMIDADE) {
        temperaturaUmidade();
    }
}
