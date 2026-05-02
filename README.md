
# 🏠 Smart Home

> *"Automação não é sobre apertar botões. É sobre sentir que o ambiente responde a você."*

Bem-vindo ao meu projeto. Este projeto não é apenas um controlador de relés; é o **cérebro digital** de uma residência moderna. Utilizando o poder do **ESP32**, transformamos uma simples maquete em um ecossistema vivo, capaz de sentir o clima, proteger o perímetro e interagir através de música e luz.

Diferente de sistemas básicos que travam enquanto executam tarefas, este projeto utiliza um **Núcleo Assíncrono (Non-blocking logic)**, permitindo que a casa toque música, monitore sensores e sirva a interface web simultaneamente, sem atrasos.

---

## 🔥 Funcionalidades Principais

O sistema hospeda uma interface **Web App (Dark Mode)** diretamente na memória do ESP32, oferecendo:

* **🛡️ Sentinela Ativa:** Sistema de alarme integrado com sensor PIR. Detecta intrusos e emite alertas visuais ("PERIGO") e sonoros instantâneos.
* **🌡️ Climatologia em Tempo Real:** Monitoramento constante de Temperatura (°C) e Umidade (%) via sensor DHT11.
* **🎵 Jukebox Polifônica:** Reprodução de temas (Natal, Star Wars, Memes) via buzzer passivo, com lógica não-bloqueante (a música toca sem travar o site).
* **💡 Controle de Iluminação:** Gestão de 6 ambientes distintos (Sala, Cozinha, Quarto, Sótão, Varanda, Garagem).
* **🚗 Acesso Remoto:** Controle de servo motor para abertura e fechamento do portão da garagem.

---

## 🔌 Diagrama de Conexões (Hardware)

Para replicar este projeto, siga o mapa de conexões abaixo. O sistema foi desenhado para maximizar o uso dos pinos do ESP32.

```mermaid
graph TD
    ESP32[Microcontrolador ESP32]
    
    %% Sensores (Entrada)
    DHT[DHT11 Sensor Clima] -->|GPIO 5| ESP32
    PIR[PIR Sensor Movimento] -->|GPIO 18| ESP32
    
    %% Atuadores (Saída)
    ESP32 -->|GPIO 13| Servo[Servo Motor Portão]
    ESP32 -->|GPIO 19| Buzzer[Buzzer Piezo]
    ESP32 -->|GPIO 2| LedAlarme[LED Indicador Alarme]
    
    %% Iluminação (Relés/LEDs)
    ESP32 -->|GPIO 4| Sala[Luz Sala]
    ESP32 -->|GPIO 12| Varanda[Luz Varanda]
    ESP32 -->|GPIO 14| Garagem[Luz Garagem]
    ESP32 -->|GPIO 15| Cozinha[Luz Cozinha]
    ESP32 -->|GPIO 16| Quarto[Luz Quarto]
    ESP32 -->|GPIO 17| Sotao[Luz Sótão]
