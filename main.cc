// ======================================================
// PROJETO: M.A.B.
// Programador: Vinicius Pereira de Araujo
// ======================================================
//
// DESCRIÇÃO:
// Sistema embarcado que detecta presença de gás utilizando
// sensor analógico.
//
// FUNCIONALIDADES:
// - Botão liga/desliga (modo toggle)
// - Tempo de aquecimento do sensor (15 minutos)
// - Leitura com média (50 amostras)
// - Sistema de baseline
// - Tolerância estatística
// - Indicação visual por LEDs
//
// LÓGICA DO PROJETO:
// O sistema compara a média atual do sensor com um
// baseline previamente definido.
//
// Se a diferença ultrapassar uma tolerância,
// o sistema considera a amostra suspeita.
//
// FORMULA:
//
// abs(media - baseline) > tolerancia
//
// Onde:
// media    -> valor médio do sensor
// baseline -> referência da bebida original
// tolerancia -> variação aceitável
//
// ESTADOS DO SISTEMA:
// - Aquecendo  -> LED amarelo
// - Normal     -> LED verde
// - Suspeito   -> LED vermelho
// ======================================================
// ---------------- PINOS ----------------

// Pino analógico do sensor de gás (MQ-3)
int sensorPin = A0;

// Botão liga/desliga do sistema
int botaoPower = 3;

// LEDs de indicação de estado
int ledVermelho = 5;   // indica alteração detectada
int ledVerde = 4;      // indica ambiente normal


// ---------------- BASELINE ----------------
//
// baseline:
// Valor médio esperado da bebida original.
//
// tolerancia:
// Variação aceitável em relação ao baseline.
//
// Quanto menor a tolerância:
// - mais sensível
// - mais falso positivo
//
// Quanto maior:
// - menos sensível
// - mais estável
//
float baseline = 520.0;
float tolerancia = 40.0;


// ---------------- ESTADOS DO SISTEMA ----------------

// Controla se o sistema está ligado ou desligado
bool sistemaLigado = false;

// Guarda o estado anterior do botão (para detectar clique)
bool ultimoEstadoBotao = HIGH;

// Controla mudança de status para evitar spam no Serial
bool ultimoStatus = false;

// Indica se o sensor já passou pelo aquecimento
bool sensorPronto = false;


// ---------------- LEITURA DO SENSOR ----------------

// Soma das leituras do sensor
float soma = 0;

// Contador de amostras
int contagem = 0;

// Média das leituras
float media = 0;

// Diferença entre média e baseline
float diferenca = 0;


// ======================================================
// RESET DAS LEITURAS
// ======================================================
//
// Função responsável por limpar os valores
// após cada ciclo de leitura do sensor.
//
void resetLeitura() {
  soma = 0;
  contagem = 0;
  media = 0;
  diferenca = 0;
}


// ======================================================
// SETUP (INICIALIZAÇÃO)
// ======================================================
void setup() {

  // Configuração dos pinos
  pinMode(sensorPin, INPUT);
  pinMode(botaoPower, INPUT_PULLUP);

  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  // Comunicação serial
  Serial.begin(9600);

  // ==================================================
  // ESTADO INICIAL FORÇADO (IMPORTANTE)
  // ==================================================
  // Garante que o sistema sempre inicia desligado
  sistemaLigado = false;
  sensorPronto = false;

  // LEDs desligados no início
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledVerde, LOW);

  // Pequeno delay para estabilizar leitura do botão
  delay(100);

  // Descarta leitura inicial do botão (evita falso clique)
  ultimoEstadoBotao = digitalRead(botaoPower);

  Serial.println("Sistema iniciado (DESLIGADO)");
}


// ======================================================
// LOOP PRINCIPAL
// ======================================================
void loop() {

  // ==================================================
  // 1. BOTÃO (TOGGLE ESTÁVEL)
  // ==================================================
  //
  // Detecta apenas transição real do botão:
  // HIGH → LOW (clique real)
  //

  bool estadoBotao = digitalRead(botaoPower);

  if (ultimoEstadoBotao == HIGH && estadoBotao == LOW) {

    // Alterna estado do sistema
    sistemaLigado = !sistemaLigado;

    if (sistemaLigado) {

      Serial.println(">>> SISTEMA LIGADO");

      sensorPronto = false;
      resetLeitura();
    }
    else {

      Serial.println(">>> SISTEMA DESLIGADO");

      sensorPronto = false;
      resetLeitura();

      // Garante LEDs desligados
      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, LOW);
    }

    delay(200); // debounce simples
  }

  ultimoEstadoBotao = estadoBotao;


  // ==================================================
  // 2. STATUS (SEM SPAM NO SERIAL)
  // ==================================================
  if (sistemaLigado != ultimoStatus) {

    if (sistemaLigado)
      Serial.println("STATUS: LIGADO");
    else
      Serial.println("STATUS: DESLIGADO");

    ultimoStatus = sistemaLigado;
  }


  // ==================================================
  // 3. SISTEMA DESLIGADO
  // ==================================================
  if (!sistemaLigado) {

    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, LOW);

    return;
  }


  // ==================================================
  // 4. AQUECIMENTO DO SENSOR
  // ==================================================
  //
  // Sensores MQ-3 precisam de tempo para estabilizar
  //

  if (!sensorPronto) {

    // LED amarelo (vermelho + verde)
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, HIGH);

    // Tempo de aquecimento (teste: 30s)
    if (millis() > 30000) {
      sensorPronto = true;
      Serial.println("Sensor pronto!");
    }

    return;
  }


  // ==================================================
  // 5. LEITURA DO SENSOR
  // ==================================================
  //
  // Faz média de 50 leituras para reduzir ruído
  //

  soma += analogRead(sensorPin);
  contagem++;


  // ==================================================
  // 6. PROCESSAMENTO
  // ==================================================
  //
  // Quando completa 50 amostras, calcula média
  //

  if (contagem >= 50) {

    media = soma / 50.0;

    // Diferença em relação ao baseline
    diferenca = abs(media - baseline);

    // Reseta ciclo de leitura
    resetLeitura();


    // ==================================================
    // DETECÇÃO DE ALTERAÇÃO
    // ==================================================
    //
    // Se a diferença for maior que tolerância,
    // considera possível adulteração
    //

    if (diferenca > tolerancia) {

      digitalWrite(ledVermelho, HIGH);
      digitalWrite(ledVerde, LOW);

      Serial.println("ALERTA: POSSIVEL ALTERACAO");
    }
    else {

      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, HIGH);

      Serial.println("AMOSTRA NORMAL");
    }
  }
}