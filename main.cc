// ======================================================
// PROJETO: M.A.B.
// Monitor Analítico de Bebidas
// ======================================================
//
// DESCRIÇÃO:
// Sistema embarcado utilizando sensor MQ-3 para detectar
// possíveis alterações em amostras através de análise
// comparativa.
//
// ======================================================
// FUNCIONALIDADES
// ======================================================
//
// - Botão liga/desliga (toggle)
// - Debounce profissional sem delay()
// - Aquecimento controlado com millis()
// - Baseline automático
// - Média de múltiplas amostras
// - Histerese anti-oscilação
// - LEDs de status
// - Código modularizado
// - Sem bloqueios críticos
//
// ======================================================
// ESTADOS
// ======================================================
//
// DESLIGADO  -> LEDs apagados
// AQUECENDO  -> LED amarelo
// NORMAL     -> LED verde
// ALERTA     -> LED vermelho
//
// ======================================================



// ======================================================
// PINOS
// ======================================================

const byte sensorPin = A0;

const byte botaoPower = 3;

const byte ledVerde = 4;
const byte ledVermelho = 5;
const byte ledAmarelo = 7;



// ======================================================
// CONFIGURAÇÕES
// ======================================================

// ======================================================
// TEMPO DE AQUECIMENTO
// ======================================================

// TESTE RÁPIDO:
// const unsigned long tempoAquecimento = 3000;

// USO REAL:
const unsigned long tempoAquecimento = 15UL * 60UL * 1000UL;


// ======================================================
// SENSOR
// ======================================================

const int totalAmostras = 50;

// margem para detectar alteração
float toleranciaAlerta = 40.0;

// margem para voltar ao normal
float toleranciaNormal = 30.0;


// ======================================================
// DEBOUNCE
// ======================================================

const unsigned long debounceDelay = 50;


// ======================================================
// INTERVALO ENTRE LEITURAS
// ======================================================

const unsigned long intervaloLeitura = 10;



// ======================================================
// VARIÁVEIS DE CONTROLE
// ======================================================

bool sistemaLigado = false;
bool sensorPronto = false;

bool estadoAlertaAtivo = false;



// ======================================================
// CONTROLE DO BOTÃO
// ======================================================

bool estadoBotaoAtual = HIGH;
bool ultimoEstadoBotao = HIGH;

unsigned long ultimoDebounce = 0;



// ======================================================
// CONTROLE DE TEMPO
// ======================================================

unsigned long tempoLigado = 0;
unsigned long ultimaLeitura = 0;



// ======================================================
// LEITURA SENSOR
// ======================================================

long somaLeituras = 0;

int contadorLeituras = 0;

float baseline = 0;



// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(9600);

  // ======================================================
  // PINOS
  // ======================================================

  pinMode(botaoPower, INPUT_PULLUP);

  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);

  desligarTodosLEDs();

  // ======================================================
  // SERIAL
  // ======================================================

  Serial.println("====================================");
  Serial.println(" SISTEMA M.A.B INICIADO ");
  Serial.println(" ESTADO: DESLIGADO ");
  Serial.println("====================================");
}



// ======================================================
// LOOP PRINCIPAL
// ======================================================

void loop() {

  verificarBotao();

  if (!sistemaLigado) {
    return;
  }

  verificarAquecimento();

  if (!sensorPronto) {
    return;
  }

  processarSensor();
}



// ======================================================
// BOTÃO POWER
// ======================================================

void verificarBotao() {

  bool leitura = digitalRead(botaoPower);

  // detecta mudança física
  if (leitura != ultimoEstadoBotao) {

    ultimoDebounce = millis();
  }

  // debounce
  if ((millis() - ultimoDebounce) > debounceDelay) {

    // mudança confirmada
    if (leitura != estadoBotaoAtual) {

      estadoBotaoAtual = leitura;

      // botão pressionado
      if (estadoBotaoAtual == LOW) {

        sistemaLigado = !sistemaLigado;

        if (sistemaLigado) {
          ligarSistema();
        }
        else {
          desligarSistema();
        }
      }
    }
  }

  ultimoEstadoBotao = leitura;
}



// ======================================================
// LIGAR SISTEMA
// ======================================================

void ligarSistema() {

  Serial.println("");
  Serial.println(">>> SISTEMA LIGADO");
  Serial.println("Sensor aquecendo...");

  sensorPronto = false;

  estadoAlertaAtivo = false;

  tempoLigado = millis();

  resetLeituras();

  desligarTodosLEDs();

  digitalWrite(ledAmarelo, HIGH);
}



// ======================================================
// DESLIGAR SISTEMA
// ======================================================

void desligarSistema() {

  Serial.println("");
  Serial.println(">>> SISTEMA DESLIGADO");

  sensorPronto = false;

  estadoAlertaAtivo = false;

  resetLeituras();

  desligarTodosLEDs();
}



// ======================================================
// AQUECIMENTO
// ======================================================

void verificarAquecimento() {

  if (sensorPronto) {
    return;
  }

  unsigned long tempoAtual = millis();

  if ((tempoAtual - tempoLigado) >= tempoAquecimento) {

    sensorPronto = true;

    digitalWrite(ledAmarelo, LOW);

    Serial.println("");
    Serial.println("Sensor estabilizado!");
    Serial.println("Realizando calibracao...");
    
    calibrarBaseline();

    Serial.println("Analise iniciada.");
  }
}



// ======================================================
// CALIBRAÇÃO AUTOMÁTICA
// ======================================================

void calibrarBaseline() {

  long soma = 0;

  for (int i = 0; i < 100; i++) {

    soma += analogRead(sensorPin);

    delay(20);
  }

  baseline = soma / 100.0;

  Serial.print("Baseline definido: ");
  Serial.println(baseline);
}



// ======================================================
// PROCESSAMENTO DO SENSOR
// ======================================================

void processarSensor() {

  // controle de tempo sem delay
  if (millis() - ultimaLeitura < intervaloLeitura) {
    return;
  }

  ultimaLeitura = millis();

  int leitura = analogRead(sensorPin);

  somaLeituras += leitura;

  contadorLeituras++;

  // debug opcional
  // Serial.println(leitura);

  // ======================================================
  // PROCESSA MÉDIA
  // ======================================================

  if (contadorLeituras >= totalAmostras) {

    float media = somaLeituras / (float)totalAmostras;

    float diferenca = abs(media - baseline);

    Serial.println("");
    Serial.print("Media: ");
    Serial.println(media);

    Serial.print("Diferenca: ");
    Serial.println(diferenca);

    analisarResultado(diferenca);

    resetLeituras();
  }
}



// ======================================================
// ANÁLISE
// ======================================================

void analisarResultado(float diferenca) {

  // ======================================================
  // ENTRA EM ALERTA
  // ======================================================

  if (!estadoAlertaAtivo && diferenca > toleranciaAlerta) {

    estadoAlertaAtivo = true;

    estadoAlerta();

    return;
  }

  // ======================================================
  // VOLTA AO NORMAL
  // ======================================================

  if (estadoAlertaAtivo && diferenca < toleranciaNormal) {

    estadoAlertaAtivo = false;

    estadoNormal();

    return;
  }

  // ======================================================
  // MANTÉM ESTADO
  // ======================================================

  if (estadoAlertaAtivo) {

    estadoAlerta();
  }
  else {

    estadoNormal();
  }
}



// ======================================================
// ESTADO NORMAL
// ======================================================

void estadoNormal() {

  digitalWrite(ledVerde, HIGH);

  digitalWrite(ledVermelho, LOW);

  digitalWrite(ledAmarelo, LOW);

  Serial.println("STATUS: NORMAL");
}



// ======================================================
// ESTADO ALERTA
// ======================================================

void estadoAlerta() {

  digitalWrite(ledVerde, LOW);

  digitalWrite(ledVermelho, HIGH);

  digitalWrite(ledAmarelo, LOW);

  Serial.println("STATUS: POSSIVEL ALTERACAO");
}



// ======================================================
// RESET LEITURAS
// ======================================================

void resetLeituras() {

  somaLeituras = 0;

  contadorLeituras = 0;
}



// ======================================================
// DESLIGA LEDs
// ======================================================

void desligarTodosLEDs() {

  digitalWrite(ledVerde, LOW);

  digitalWrite(ledVermelho, LOW);

  digitalWrite(ledAmarelo, LOW);
}