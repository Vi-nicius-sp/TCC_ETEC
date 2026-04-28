# 🚨 M.A.B. — Monitoramento de Ambiente com Gás

Sistema embarcado utilizando Arduino para detecção de gás com indicação visual por LEDs.

---

## 📸 Demonstração do Projeto

![Protótipo Arduino](./arduino.jpg)

---

## 📖 Sobre o Projeto

O **M.A.B. (Monitoramento de Ambiente Básico)** é um sistema embarcado desenvolvido para detectar a presença de gás em ambientes, utilizando um sensor analógico conectado ao Arduino.

O sistema realiza leituras contínuas, aplica média para maior precisão e informa o estado do ambiente através de LEDs.

---

## ⚙️ Funcionalidades

- 🔘 Botão liga/desliga (modo toggle)
- ⏳ Tempo de aquecimento do sensor (15 minutos)
- 📊 Leitura com média de 50 amostras
- 💡 Indicação visual por LEDs
- 🖥️ Saída no monitor serial (debug)

---

## 🚦 Estados do Sistema

| Estado            | LED              | Descrição                     |
|------------------|------------------|------------------------------|
| 🟡 Aquecendo     | Vermelho + Verde | Sensor estabilizando         |
| 🟢 Seguro        | Verde            | Ambiente sem gás             |
| 🔴 Gás Detectado | Vermelho         | Presença de gás identificada |

---

## 🔌 Componentes Utilizados

- Arduino Uno (ou similar)
- Sensor de gás (MQ-2 / MQ-135)
- Botão push-button
- LED vermelho
- LED verde
- Resistores
- Jumpers

---

## 🔌 Pinagem

| Componente     | Pino |
|---------------|------|
| Sensor de gás | A0   |
| Botão Power   | 3    |
| LED Vermelho  | 5    |
| LED Verde     | 4    |

---

## 🧠 Lógica do Sistema

1. O sistema inicia desligado.
2. O botão alterna o estado (liga/desliga).
3. Ao ligar, inicia o tempo de aquecimento.
4. Após estabilização, realiza leituras do sensor.
5. Calcula média de 50 amostras.
6. Detecta presença de gás com base no valor médio.

---

## 💻 Código

O sistema implementa:

- Controle de estado com botão (toggle)
- Temporização com `millis()`
- Média de 50 leituras analógicas
- Lógica de detecção por limiar
- Controle de LEDs por estado

---

## 🧪 Ajuste para Testes

Para testes rápidos, reduza o tempo de aquecimento:

```cpp
// Original (15 minutos)
900000

// Teste rápido
900