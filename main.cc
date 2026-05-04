// ======================================================
// PROJETO: M.A.B. (Monitor de Adulteração em Bebidas)
// Programador: Vinicius Pereira de Araujo
// ======================================================
//
// DESCRIÇÃO:
// Sistema embarcado que detecta possíveis anomalias
// (adulteração) em bebidas alcoólicas através da leitura
// de vapores utilizando um sensor de gás.
//
// FUNCIONAMENTO:
// 1. Usuário liga o sistema (botão)
// 2. Sensor entra em aquecimento (15 minutos)
// 3. Após aquecer:
//    → sistema realiza leitura do ambiente (baseline)
//    → define automaticamente um limite de detecção
// 4. A partir disso:
//    → LED desligado = normal
//    → LED ligado = suspeita de adulteração
//
// ESTADOS DO SISTEMA:
//  - Desligado → LED apagado
//  - Aquecendo → aguardando estabilização
//  - Calibrando → definição do baseline
//  - Monitorando → detecção ativa
//
// ======================================================


// ---------------- PINOS ----------------

int sensorPin = A0;   // Sensor de gás
int botaoPower = 3;   // Botão liga/desliga
int led = 5;          // LED indicador


// ---------------- CONTROLE DO SISTEMA ----------------

bool sistemaLigado = false;
bool ultimoEstadoBotao = false;


// ---------------- CONTROLE DE TEMPO ----------------

unsigned long tempoInicio = 0;
bool sensorPronto = false;


// ---------------- LEITURA DO SENSOR ----------------

float soma = 0;
int contagem = 0;
float media = 0;


// ---------------- CALIBRAÇÃO AUTOMÁTICA ----------------

bool calibrado = false;
float baseline = 0;
float limite = 420; // valor mínimo de segurança


// ======================================================
void setup() {

  pinMode(sensorPin, INPUT);
  pinMode(botaoPower, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
}


// ======================================================
void loop() {

  // ================= BOTÃO =================
  bool estadoBotao = digitalRead(botaoPower);

  if (estadoBotao == HIGH && ultimoEstadoBotao == LOW) {

    sistemaLigado = !sistemaLigado;

    if (sistemaLigado) {
      tempoInicio = millis();
      sensorPronto = false;
      calibrado = false;

      soma = 0;
      contagem = 0;

      Serial.println("Sistema LIGADO");

    } else {
      Serial.println("Sistema DESLIGADO");
      digitalWrite(led, LOW);
    }
  }

  ultimoEstadoBotao = estadoBotao;

  if (!sistemaLigado) return;


  // ================= AQUECIMENTO =================
  if (!sensorPronto) {

    if (millis() - tempoInicio >= 900000) {
      sensorPronto = true;
      Serial.println("Sensor pronto!");
    }

    return;
  }


  // ================= LEITURA =================
  soma += analogRead(sensorPin);
  contagem++;

  if (contagem >= 50) {

    media = soma / 50.0;

    Serial.print("Media: ");
    Serial.println(media);

    soma = 0;
    contagem = 0;


    // ================= CALIBRAÇÃO =================
    if (!calibrado) {

      baseline = media;
      limite = baseline * 1.5;

      if (limite < 420) {
        limite = 420;
      }

      calibrado = true;

      Serial.print("Baseline: ");
      Serial.println(baseline);

      Serial.print("Limite: ");
      Serial.println(limite);

      return;
    }


    // ================= DETECÇÃO =================
    if (media >= limite) {

      digitalWrite(led, HIGH);
      Serial.println("ALERTA: POSSIVEL ADULTERACAO");

    } else {

      digitalWrite(led, LOW);
    }
  }
}