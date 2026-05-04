# 🚨 M.A.B. — Monitor de Adulteração em Bebidas

Sistema embarcado utilizando Arduino para detecção de possíveis adulterações em bebidas alcoólicas através da análise de vapores.

---

## 📸 Demonstração do Projeto

![Protótipo Arduino](./arduino.png)

---

## 📖 Sobre o Projeto

O **M.A.B. (Monitor de Adulteração em Bebidas)** é um sistema embarcado desenvolvido com Arduino que analisa vapores alcoólicos para identificar possíveis anomalias associadas à adulteração de bebidas.

O sistema realiza leituras contínuas do sensor de gás, aplica média para maior estabilidade e utiliza um método de **calibração automática** para aumentar a confiabilidade da detecção.

---

## ⚙️ Funcionalidades

* 🔘 Botão liga/desliga (modo toggle)
* ⏳ Tempo de aquecimento do sensor (~15 minutos)
* 📊 Leitura com média de 50 amostras
* 🧠 Calibração automática do ambiente (baseline)
* 🚨 Detecção baseada em limite dinâmico
* 💡 Indicação visual com LED único
* 🖥️ Saída no monitor serial (debug)

---

## 🚦 Estados do Sistema

| Estado        | LED     | Descrição                        |
| ------------- | ------- | -------------------------------- |
| 🔴 Desligado  | Apagado | Sistema inativo                  |
| 🟡 Aquecendo  | Apagado | Sensor estabilizando             |
| 🔵 Calibrando | Apagado | Definindo valor base do ambiente |
| 🟢 Normal     | Apagado | Comportamento esperado           |
| 🚨 Suspeito   | Aceso   | Possível adulteração detectada   |

---

## 🔌 Componentes Utilizados

* Arduino Uno (ou similar)
* Sensor de gás (MQ-3 recomendado)
* Botão push-button
* LED
* Resistores
* Jumpers

---

## 🔌 Pinagem

| Componente    | Pino |
| ------------- | ---- |
| Sensor de gás | A0   |
| Botão Power   | 3    |
| LED           | 5    |

---

## 🧠 Lógica do Sistema

1. O sistema inicia desligado.
2. O botão alterna o estado (liga/desliga).
3. Ao ligar, inicia o aquecimento do sensor.
4. Após estabilização:

   * o sistema coleta dados do ambiente
   * define automaticamente um **baseline**
5. O limite de detecção é calculado como:

```
limite = baseline * 1.5
```

6. É aplicado um valor mínimo de segurança:

```
limite mínimo = 420
```

7. Durante o funcionamento:

   * LED apagado → normal
   * LED aceso → possível adulteração

---

## 💻 Código

O sistema implementa:

* Controle de estado com botão (toggle)
* Temporização com `millis()`
* Média de 50 leituras analógicas
* Calibração automática do sensor
* Lógica de detecção com limite dinâmico
* Controle de LED como saída de alerta

---

## 🧪 Ajuste para Testes

Para testes rápidos, reduza o tempo de aquecimento:

```cpp
// Original (~15 minutos)
900000

// Teste rápido
900
```

---

## ⚠️ Limitações

* O sistema **não identifica substâncias específicas**
* Detecta apenas **variações em vapores alcoólicos**
* Pode sofrer influência de:

  * temperatura
  * umidade
  * ventilação do ambiente

---

## 🧠 Considerações Técnicas

Sensores da família MQ apresentam variação entre unidades e condições ambientais.
Por isso, foi adotada uma abordagem de **calibração automática com baseline**, aumentando a confiabilidade da detecção sem necessidade de ajuste manual.

---

## 👨‍💻 Autor

**Vinicius Pereira de Araujo**
