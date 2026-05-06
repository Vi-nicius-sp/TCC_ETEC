// ======================================================
// PROJETO: M.A.B.
// Programador: Vinicius Pereira de Araujo
// ======================================================
// DESCRIÇÃO:
// Sistema embarcado que detecta presença de gás utilizando
// sensor analógico.
//
// FUNCIONALIDADES:
// - Botão liga/desliga (modo toggle)
// - Tempo de aquecimento do sensor (15 minutos)
// - Leitura com média (50 amostras)
// - Indicação visual por LEDs
//
// ESTADOS DO SISTEMA:
//  Aquecendo -> amarelo
//	Seguro -> verde
//	Gás detectado -> vermelho
// ======================================================


// ---------------- PINOS ----------------

// Pino analógico do sensor de gás
int sensorPin = A0;

// Botão liga/desliga
int botaoPower = 3;

// LEDs
int ledVermelho = 5;   //indica gás detectado
int ledVerde = 4;      //indica ambiente seguro


// ---------------- CONTROLE DO SISTEMA ----------------

// Indica se o sistema está ligado ou desligado
bool sistemaLigado = false;

// Guarda o estado anterior do botão (para detectar clique)
bool ultimoEstadoBotao = false;


// ---------------- CONTROLE DE TEMPO ----------------

// Guarda o momento em que o sistema foi ligado
unsigned long tempoInicio = 0;

// Indica se o sensor já está pronto (aquecido)
bool sensorPronto = false;


// ---------------- LEITURA DO SENSOR ----------------

// Soma das leituras para cálculo de média
float soma = 0;

// Contador de leituras realizadas
int contagem = 0;

// Resultado da média
float media = 0;


// ======================================================
void setup() {

  // Define o sensor como entrada
  pinMode(sensorPin, INPUT);

  // Define o botão como entrada
  pinMode(botaoPower, INPUT);

  // Define LEDs como saída
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  // Inicializa monitor serial (para testes)
  Serial.begin(9600);
}


// ======================================================
void loop() {

  // ==================================================
  //  1. BOTÃO LIGA/DESLIGA (TOGGLE)
  // ==================================================

  // Lê o estado atual do botão
  bool estadoBotao = digitalRead(botaoPower);

  // Detecta apenas o momento do clique (LOW → HIGH)
  if (estadoBotao == HIGH && ultimoEstadoBotao == LOW) {

    // Inverte estado do sistema
    sistemaLigado = !sistemaLigado;

    // Se ligou o sistema
    if (sistemaLigado) {

      // Marca o tempo inicial
      tempoInicio = millis();

      // Sensor começa como não pronto
      sensorPronto = false;

      // Zera leituras anteriores
      soma = 0;
      contagem = 0;

      Serial.println("Sistema LIGADO");

    } else {
      Serial.println("Sistema DESLIGADO");
    }
  }

  // Atualiza estado anterior do botão
  ultimoEstadoBotao = estadoBotao;


  // ==================================================
  //  2. SISTEMA DESLIGADO
  // ==================================================
  if (!sistemaLigado) {

    // Garante que tudo está desligado
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, LOW);

    return; // Sai do loop
  }


  // ==================================================
  //  3. TEMPO DE AQUECIMENTO
  // ==================================================
  // Sensores de gás precisam de tempo para estabilizar

  if (!sensorPronto) {

    // 15 minutos = 900000 milissegundos
    if (millis() - tempoInicio >= 900) { // para teste alterar para 900
      sensorPronto = true;

      Serial.println("Sensor pronto!");
    }
  }


  // ==================================================
  //  4. AQUECENDO
  // ==================================================
  if (!sensorPronto) {

    // Amarelo = vermelho + verde ligados juntos
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, HIGH);

    return; // Não continua para leitura
  }


  // ==================================================
  //  5. LEITURA DO SENSOR
  // ==================================================

  // Lê o valor do sensor e soma
  soma += analogRead(sensorPin);

  // Conta quantas leituras já foram feitas
  contagem++;

  // Quando atingir 50 leituras
  if (contagem >= 50) {

    // Calcula média
    media = soma / 50.0;

    // Mostra no serial (para debug)
    Serial.print("Media: ");
    Serial.println(media);

    // Reseta valores
    soma = 0;
    contagem = 0;


    // ==================================================
    //  6. DETECÇÃO DE GÁS
    // ==================================================

    if (media >= 500) {

      //  GÁS DETECTADO
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(ledVerde, LOW);

    } else {

      //  AMBIENTE SEGURO
      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, HIGH);
    }
  }
}