# 🚨 M.A.B. — Monitor Analítico de Bebidas

Sistema embarcado utilizando Arduino e sensor MQ-3 para análise comparativa de amostras líquidas através da leitura e interpretação de vapores alcoólicos.

---

## 📸 Demonstração do Projeto

![Protótipo Arduino](./arduino.png)

---

# 📖 Sobre o Projeto

O **M.A.B. (Monitor Analítico de Bebidas)** é um sistema embarcado desenvolvido com Arduino capaz de detectar possíveis alterações em bebidas através da análise comparativa de leituras obtidas pelo sensor MQ-3.

O sistema realiza:

- Aquecimento controlado do sensor
- Calibração automática de baseline
- Média de múltiplas amostras
- Comparação contínua com referência inicial
- Controle de estados utilizando histerese

Toda a lógica foi desenvolvida sem bloqueios críticos, utilizando `millis()` para gerenciamento de tempo.

---

# ⚙️ Funcionalidades

- 🔘 Botão liga/desliga (toggle)
- 🧠 Debounce profissional sem `delay()`
- ⏳ Aquecimento controlado do sensor MQ-3
- 📊 Média de múltiplas leituras
- 📏 Baseline automática
- 🎯 Detecção por tolerância configurável
- 🔄 Histerese anti-oscilação
- 💡 LEDs de status
- 🖥️ Saída serial para debug
- 🧩 Código modularizado
- ⚡ Sistema não bloqueante

---

# 🚦 Estados do Sistema

| Estado | LED | Descrição |
|---|---|---|
| ⚫ Desligado | Todos apagados | Sistema inativo |
| 🟡 Aquecendo | Amarelo | Sensor estabilizando |
| 🟢 Normal | Verde | Leituras dentro da tolerância |
| 🔴 Alerta | Vermelho | Possível alteração detectada |

---

# 🔌 Componentes Utilizados

- Arduino Uno/Nano
- Sensor MQ-3
- Botão push-button
- LED verde
- LED vermelho
- LED amarelo
- Resistores
- Jumpers
- Protoboard

---

# 🔌 Pinagem

| Componente | Pino |
|---|---|
| MQ-3 (AOUT) | A0 |
| Botão Power | D3 |
| LED Verde | D4 |
| LED Vermelho | D5 |
| LED Amarelo | D7 |

---

# 🧠 Lógica do Sistema

## 1️⃣ Inicialização

O sistema inicia desligado:

- LEDs apagados
- Monitor serial inicializado
- Sensor aguardando acionamento

---

## 2️⃣ Acionamento

Ao pressionar o botão:

- O sistema alterna entre ligado/desligado
- O aquecimento do MQ-3 é iniciado

---

## 3️⃣ Aquecimento do Sensor

O MQ-3 necessita estabilização térmica antes da análise.

```cpp
const unsigned long tempoAquecimento = 15UL * 60UL * 1000UL;
