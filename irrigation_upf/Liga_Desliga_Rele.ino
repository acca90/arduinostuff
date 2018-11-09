//the relays connect to
int IN3 = 7; //entra no pino 7 do arduino, int com nome in3 pq entra no IN3 do rele
int IN4 = 5; //entra no pino 7 do arduino, int com nome in3 pq entra no IN3 do rele
int teste;  //variavel usada para receber inf (1 ou 2)
int a = 0; //variavel usada pra q o serial.println("Digite ...") n repita + de 1 vez na porta serial
int b = 0;
int t;
void setup() {
  Serial.begin(115200); //inicia a porta serial 
  pinMode(IN3, OUTPUT); //indica o pino
  digitalWrite(IN3,HIGH); //inicia desligado
  pinMode(IN4, OUTPUT); //indica o pino
  digitalWrite(IN4,HIGH); //inicia desligado
}

void loop() {

  if (b==0){    //faz o controle se necessita repetir ou nao
      Serial.println("Qual porta do rele?"); //printa no serial pro user digita
      b++;
  }
t = Serial.parseInt(); //registra no int teste o valor inserido

  if(t == 1){
      if (a==0){    //faz o controle se necessita repetir ou nao
        Serial.println("Digite 1 para ligar o rele ou digite 2 para desligar o rele"); //printa no serial pro user digita
        a++;
    }
    while (a != 0){                                //espera ate o user digita 1 ou 0 
      teste = Serial.parseInt(); //registra no int teste o valor inserido
      if (teste == 1){
          digitalWrite(IN3, LOW);
          Serial.println("Ligado");
          a = 0;
          b = 0;
      }
      if (teste == 2){
          digitalWrite(IN3, HIGH);
          Serial.println("Desligado");
          a = 0;
          b = 0;
      }
        //else
   //Serial.println("Erro");
    }
  }

  if(t == 2){
      if (a==0){    //faz o controle se necessita repetir ou nao
          Serial.println("Digite 1 para ligar o rele ou digite 2 para desligar o rele"); //printa no serial pro user digita
          a++;
      }
    while (a != 0){
      teste = Serial.parseInt(); //registra no int teste o valor inserido
      if (teste == 1){
          digitalWrite(IN4, LOW);
          Serial.println("Ligado");
          a = 0;
          b = 0;
      }
      if (teste == 2){
          digitalWrite(IN4, HIGH);
          Serial.println("Desligado");
          a = 0;
          b = 0;
      }
    }
  }
}
