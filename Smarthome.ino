#include <WiFi.h> 
#include <ESP32Servo.h> 
#include <DHT.h> 

Servo motorgaragem; 

// --- CONFIGURAÇÃO DA REDE ---
const char* ssid = "NOME DA REDE"; 
const char* password = "SENHA DO WiFI"; 

WiFiServer server(80);
String header;

// --- CONFIGURAÇÃO DO SENSOR DHT ---
#define DHTPIN 5     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

// --- PINOS ---
const int luzsala = 4;
const int luzvaranda = 12;
const int pinoServo = 13; 
const int luzgaragem = 14;
const int luzcozinha = 15;
const int luzquarto = 16;
const int luzsotao = 17;

#define pir 18
#define som 19
#define sinalalarme 2

// --- DEFINIÇÕES DE NOTAS MUSICAIS ---
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

// --- PARTITURAS GLOBAIS ---
// Movidas para cá para serem acessíveis a qualquer momento

const int melodyNatal[] = {
  NOTE_C5,4, NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
  NOTE_E5,2, NOTE_E5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
  NOTE_C5,2, NOTE_A5,4,
  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,
  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4, 
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8, 
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
  NOTE_E5,2, NOTE_E5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
  NOTE_C5,2, NOTE_A5,4,
  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,
  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4, 
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8, 
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, 
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, REST,4
};

const int melodyStarWars[] = {
  NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, 
  NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8, 
  NOTE_B4,8,  NOTE_AS4,8, NOTE_B4,8, NOTE_A4,8, REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_G4,8,
  NOTE_G4,4,  NOTE_E4,-2, 
  NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, 
  NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8,
  NOTE_A4,-4, NOTE_A4,-4, NOTE_GS4,8, NOTE_A4,-4,
  NOTE_D5,8,  NOTE_C5,-4, NOTE_B4,-4, NOTE_A4,-4,
  NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, 
  NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8,
  NOTE_D5,4, NOTE_D5,-4, NOTE_B4,8, NOTE_A4,-4,
  NOTE_G4,-4, NOTE_E4,-2,
  NOTE_E4, 2, NOTE_G4,2,
  NOTE_B4, 2, NOTE_D5,2,
  NOTE_F5, -4, NOTE_E5,-4, NOTE_AS4,8, NOTE_AS4,8, NOTE_B4,4, NOTE_G4,4
};

const int melodyCat[] = {
  REST,1, REST,1,
  NOTE_C4,4, NOTE_E4,4, NOTE_G4,4, NOTE_E4,4, 
  NOTE_C4,4, NOTE_E4,8, NOTE_G4,-4, NOTE_E4,4,
  NOTE_A3,4, NOTE_C4,4, NOTE_E4,4, NOTE_C4,4,
  NOTE_A3,4, NOTE_C4,8, NOTE_E4,-4, NOTE_C4,4,
  NOTE_G3,4, NOTE_B3,4, NOTE_D4,4, NOTE_B3,4,
  NOTE_G3,4, NOTE_B3,8, NOTE_D4,-4, NOTE_B3,4,
  NOTE_G3,4, NOTE_G3,8, NOTE_G3,-4, NOTE_G3,8, NOTE_G3,4, 
  NOTE_G3,4, NOTE_G3,4, NOTE_G3,8, NOTE_G3,4,
  NOTE_C4,4, NOTE_E4,4, NOTE_G4,4, NOTE_E4,4, 
  NOTE_C4,4, NOTE_E4,8, NOTE_G4,-4, NOTE_E4,4,
  NOTE_A3,4, NOTE_C4,4, NOTE_E4,4, NOTE_C4,4,
  NOTE_A3,4, NOTE_C4,8, NOTE_E4,-4, NOTE_C4,4,
  NOTE_G3,4, NOTE_B3,4, NOTE_D4,4, NOTE_B3,4,
  NOTE_G3,4, NOTE_B3,8, NOTE_D4,-4, NOTE_B3,4,
  NOTE_G3,-1
};

// --- VARIÁVEIS DE CONTROLE DA MÚSICA (NÃO BLOQUEANTE) ---
const int* currentMelody = nullptr; // Ponteiro para a música atual
int currentMelodySize = 0;          // Tamanho da música atual
int currentNoteIndex = 0;           // Nota atual sendo tocada
int currentTempo = 0;               // Tempo da música atual
unsigned long nextNoteTime = 0;     // Quando tocar a próxima nota
bool isPlaying = false;             // Se está tocando ou não

// Variáveis de Estado da Casa
String cozinha = "Desligada";
String sala = "Desligada";
String garagem = "Desligada";
String varanda = "Desligada";
String quarto = "Desligada";
String sotao = "Desligada";
String porta = "Fechada"; 
String alarme = "Desativado";
float temperatura = 0.0;
float umidade = 0.0;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// Variáveis para o alarme
unsigned long previousAlarmTime = 0; 
bool buzzerState = false;           
const long alarmInterval = 500;

// --- FUNÇÃO PARA INICIAR UMA MÚSICA ---
void iniciarMusica(const int* melodia, int tamanho, int tempo) {
  currentMelody = melodia;
  currentMelodySize = tamanho;
  currentTempo = tempo;
  currentNoteIndex = 0;
  nextNoteTime = 0; // Tocar imediatamente
  isPlaying = true;
  buzzerState = false; // Garante que o alarme não interfira
}

// --- FUNÇÃO PARA GERENCIAR A MÚSICA (NO LOOP) ---
void handleMusic() {
  if (!isPlaying || currentMelody == nullptr) return;

  if (millis() >= nextNoteTime) {
    // Verifica se a música acabou
    if (currentNoteIndex >= currentMelodySize) {
      isPlaying = false;
      noTone(som);
      return;
    }

    // Lógica de cálculo da nota (igual ao original, mas sem delay)
    int note = currentMelody[currentNoteIndex];
    int durationVal = currentMelody[currentNoteIndex + 1];
    
    int wholenote = (60000 * 4) / currentTempo;
    int noteDuration = 0;

    if (durationVal > 0) {
      noteDuration = (wholenote) / durationVal;
    } else if (durationVal < 0) {
      noteDuration = (wholenote) / abs(durationVal);
      noteDuration *= 1.5; 
    }

    // Toca a nota
    tone(som, note, noteDuration * 0.9);

    // Define quando a função deve rodar novamente
    nextNoteTime = millis() + noteDuration;
    
    // Avança para a próxima nota (pares: nota + duração)
    currentNoteIndex += 2;
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Configuração do Servo
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  motorgaragem.setPeriodHertz(50); 
  motorgaragem.attach(pinoServo, 500, 2400); 
  motorgaragem.write(0); 

  pinMode(luzcozinha, OUTPUT);
  pinMode(luzsala, OUTPUT);
  pinMode(luzgaragem, OUTPUT);
  pinMode(luzvaranda, OUTPUT);
  pinMode(luzquarto, OUTPUT);
  pinMode(luzsotao, OUTPUT);
  pinMode(som, OUTPUT);
  pinMode(sinalalarme, OUTPUT);
  pinMode(pir, INPUT);

  digitalWrite(luzcozinha, LOW);
  digitalWrite(luzsala, LOW);
  digitalWrite(luzgaragem, LOW);
  digitalWrite(luzvaranda, LOW);
  digitalWrite(luzquarto, LOW);
  digitalWrite(luzsotao, LOW);
  noTone(som); 
  digitalWrite(sinalalarme, LOW);

  Serial.println("Conectando a ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_2dBm); 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // 1. GERENCIA A MÚSICA (Toca notas se for a hora certa)
  handleMusic();

  // 2. SERVIDOR WEB
  WiFiClient client = server.available();

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Novo Cliente Conectado.");
    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      // IMPORTANTE: Continuar tocando música enquanto carrega a página!
      handleMusic(); 

      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // --- LEITURA DO SENSOR DHT ---
            float novaT = dht.readTemperature();
            float novaH = dht.readHumidity();
            if (!isnan(novaT)) { temperatura = novaT; }
            if (!isnan(novaH)) { umidade = novaH; }

            // --- PROCESSAMENTO DOS COMANDOS ---
            if (header.indexOf("GET /cozinha/ligado") >= 0) { cozinha = "Ligado"; digitalWrite(luzcozinha, HIGH); }
            else if (header.indexOf("GET /cozinha/desligado") >= 0) { cozinha = "Desligada"; digitalWrite(luzcozinha, LOW); }
            
            else if (header.indexOf("GET /sala/ligado") >= 0) { sala = "Ligado"; digitalWrite(luzsala, HIGH); }
            else if (header.indexOf("GET /sala/desligado") >= 0) { sala = "Desligada"; digitalWrite(luzsala, LOW); }
            
            else if (header.indexOf("GET /garagem/ligado") >= 0) { garagem = "Ligado"; digitalWrite(luzgaragem, HIGH); }
            else if (header.indexOf("GET /garagem/desligado") >= 0) { garagem = "Desligada"; digitalWrite(luzgaragem, LOW); }
            
            else if (header.indexOf("GET /varanda/ligado") >= 0) { varanda = "Ligado"; digitalWrite(luzvaranda, HIGH); }
            else if (header.indexOf("GET /varanda/desligado") >= 0) { varanda = "Desligada"; digitalWrite(luzvaranda, LOW); }
            
            else if (header.indexOf("GET /quarto/ligado") >= 0) { quarto = "Ligado"; digitalWrite(luzquarto, HIGH); }
            else if (header.indexOf("GET /quarto/desligado") >= 0) { quarto = "Desligada"; digitalWrite(luzquarto, LOW); }
            
            else if (header.indexOf("GET /sotao/ligado") >= 0) { sotao = "Ligado"; digitalWrite(luzsotao, HIGH); }
            else if (header.indexOf("GET /sotao/desligado") >= 0) { sotao = "Desligada"; digitalWrite(luzsotao, LOW); }

            else if (header.indexOf("GET /porta/aberta") >= 0) { 
              porta = "Aberta"; 
              motorgaragem.write(180); 
            }
            else if (header.indexOf("GET /porta/fechada") >= 0) { 
              porta = "Fechada"; 
              motorgaragem.write(0); 
            }

            else if (header.indexOf("GET /alarme/ativado") >= 0) { 
              alarme = "Ativado"; 
              digitalWrite(sinalalarme, HIGH);
            }
            else if (header.indexOf("GET /alarme/desativado") >= 0) { 
              alarme = "Desativado"; 
              digitalWrite(sinalalarme, LOW); 
              noTone(som); 
              isPlaying = false; // Garante que a música para também
            }

            // --- COMANDOS DE MÚSICA (Agora chamam a função iniciar) ---
            else if (header.indexOf("GET /musica/natal") >= 0) {
              iniciarMusica(melodyNatal, sizeof(melodyNatal)/sizeof(int), 140);
            }
            else if (header.indexOf("GET /musica/starwars") >= 0) {
              iniciarMusica(melodyStarWars, sizeof(melodyStarWars)/sizeof(int), 140);
            }
            else if (header.indexOf("GET /musica/cat") >= 0) { 
              iniciarMusica(melodyCat, sizeof(melodyCat)/sizeof(int), 160);
            }
            else if (header.indexOf("GET /musica/parar") >= 0) { // NOVO COMANDO
               isPlaying = false;
               noTone(som);
            }

            // --- HTML (Igual ao anterior, adicionei botão de parar música) ---
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"); 
            client.println("<title>Smart Home Gui</title>");
            
            client.println("<style>");
            client.println("body { font-family: 'Segoe UI', Roboto, Helvetica, sans-serif; background-color: #121212; color: #e0e0e0; text-align: center; margin: 0; padding: 0px; }");
            client.println("h1 { color: #ffffff; margin: 30px 0; font-weight: 300; letter-spacing: 2px; }");
            client.println(".container { display: flex; flex-wrap: wrap; justify-content: center; max-width: 1200px; margin: 0 auto; gap: 15px; padding: 20px; }");
            client.println(".alert-box { width: 100%; padding: 20px 0; background-color: #ff0000; color: white; font-size: 24px; font-weight: bold; text-align: center; animation: piscar 0.8s infinite; border-bottom: 4px solid darkred; }");
            client.println("@keyframes piscar { 0% { background-color: #ff0000; } 50% { background-color: #800000; } 100% { background-color: #ff0000; } }");
            client.println(".card { background-color: #1e1e1e; border-radius: 15px; padding: 20px; width: 150px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); transition: transform 0.2s; }");
            client.println(".card:hover { transform: translateY(-5px); }");
            client.println(".icon { font-size: 40px; margin-bottom: 10px; display: block; }");
            client.println("h3 { margin: 10px 0; font-size: 18px; color: #bbb; }");
            client.println(".status { font-size: 12px; font-weight: bold; margin-bottom: 15px; display: block; text-transform: uppercase; letter-spacing: 1px;}");
            client.println(".temp-value { font-size: 24px; font-weight: bold; color: #FF9800; }");
            client.println(".humid-value { font-size: 24px; font-weight: bold; color: #2196F3; }");
            client.println(".st-on { color: #4CAF50; }"); 
            client.println(".st-off { color: #F44336; }"); 
            client.println(".st-warn { color: #FF9800; }"); 
            client.println("a { text-decoration: none; }");
            client.println(".btn { border: none; padding: 10px 20px; border-radius: 30px; cursor: pointer; font-weight: bold; width: 100%; transition: 0.3s; color: white; }");
            client.println(".btn-on { background-color: #4CAF50; box-shadow: 0 0 10px rgba(76, 175, 80, 0.4); }");
            client.println(".btn-off { background-color: #333; color: #888; border: 1px solid #555; }");
            client.println(".btn-danger { background-color: #d32f2f; }");
            client.println(".btn-blue { background-color: #2196F3; }");
            client.println(".btn-gold { background-color: #FFC107; color: black; margin-bottom: 5px; }"); 
            client.println(".btn-cyan { background-color: #00BCD4; color: white; margin-bottom: 5px;}"); 
            client.println(".btn-purple { background-color: #9C27B0; color: white; margin-bottom: 5px;}");
            client.println(".btn-stop { background-color: #607D8B; color: white; }");
            client.println(".section-title { width: 100%; margin-top: 30px; border-bottom: 1px solid #333; padding-bottom: 10px; margin-bottom: 20px; font-size: 20px; color: #888; }");
            client.println("</style></head>");

            client.println("<body>");

            if (digitalRead(pir) == HIGH) {
               client.println("<div class='alert-box'>⚠️ PERIGO: MOVIMENTO DETECTADO! ⚠️</div>");
            }

            client.println("<h1>🏠 CONTROLE INTELIGENTE</h1>");

            client.println("<div class='container'>");
            client.println("<div class='card'><span class='icon'>🌡️</span><h3>Temperatura</h3>");
            client.println("<span class='temp-value'>" + String((int)temperatura) + " &deg;C</span>");
            client.println("</div>");
            client.println("<div class='card'><span class='icon'>💧</span><h3>Umidade</h3>");
            client.println("<span class='humid-value'>" + String((int)umidade) + " %</span>");
            client.println("</div>");
            client.println("</div>");

            client.println("<div class='section-title'>ILUMINAÇÃO</div>");
            client.println("<div class='container'>");
            
            // Cards de Luz (Resumido para economizar espaço, lógica igual)
            String cards[][3] = {
              {"Cozinha", cozinha, "/cozinha"}, {"Sala", sala, "/sala"}, 
              {"Luz Garagem", garagem, "/garagem"}, {"Varanda", varanda, "/varanda"}, 
              {"Quarto", quarto, "/quarto"}, {"Sótão", sotao, "/sotao"}
            };
            
            for(int i=0; i<6; i++){
               client.println("<div class='card'><span class='icon'>💡</span><h3>"+cards[i][0]+"</h3>");
               if(cards[i][1]=="Ligado") {
                 client.println("<span class='status st-on'>LIGADA</span>");
                 client.println("<a href='"+cards[i][2]+"/desligado'><button class='btn btn-off'>DESLIGAR</button></a>");
               } else {
                 client.println("<span class='status st-off'>DESLIGADA</span>");
                 client.println("<a href='"+cards[i][2]+"/ligado'><button class='btn btn-on'>LIGAR</button></a>");
               }
               client.println("</div>");
            }
            client.println("</div>"); 

            client.println("<div class='section-title'>SEGURANÇA</div>");
            client.println("<div class='container'>");

            // Portão
            client.println("<div class='card' style='width: 200px;'><span class='icon'>🚪</span><h3>Portão</h3>");
            if(porta=="Aberta") {
               client.println("<span class='status st-warn'>ABERTO</span>");
               client.println("<a href='/porta/fechada'><button class='btn btn-on'>FECHAR</button></a>");
            } else {
               client.println("<span class='status st-on'>FECHADO</span>");
               client.println("<a href='/porta/aberta'><button class='btn btn-off'>ABRIR</button></a>");
            }
            client.println("</div>");

            // Alarme
            client.println("<div class='card' style='width: 200px;'><span class='icon'>🚨</span><h3>Alarme Geral</h3>");
            if(alarme=="Ativado") {
               client.println("<span class='status st-on' style='color:red'>ATIVADO</span>");
               client.println("<a href='/alarme/desativado'><button class='btn btn-off'>DESARMAR</button></a>");
            } else {
               client.println("<span class='status st-off'>DESATIVADO</span>");
               client.println("<a href='/alarme/ativado'><button class='btn btn-danger'>ARMAR</button></a>");
            }
            client.println("</div>");
            
            // Músicas (Com botão de parar)
            client.println("<div class='card' style='width: 200px;'><span class='icon'>🎵</span><h3>Melodia</h3>");
            client.println("<span class='status' style='color:#FFC107'>ESCOLHA UMA</span>");
            client.println("<a href='/musica/natal'><button class='btn btn-gold'>NATAL</button></a>");
            client.println("<a href='/musica/starwars'><button class='btn btn-cyan'>STAR WARS</button></a>");
            client.println("<a href='/musica/cat'><button class='btn btn-purple'>KEYBOARD CAT</button></a>");
            client.println("<a href='/musica/parar'><button class='btn btn-stop'>SILÊNCIO</button></a>");
            client.println("</div>");

            client.println("</div>");
            client.println("<br><footer style='color:#555; font-size:10px;'>Sistema ESP32 v2.0 - Async Core</footer>");
            client.println("</body></html>");
            client.println();
            break;
          } else { currentLine = ""; }
        } else if (c != '\r') { currentLine += c; }
      }
    }
    header = "";
    client.stop();
  }

  // --- LÓGICA DO ALARME (Também sem delay) ---
  bool alarmeAtivado = digitalRead(sinalalarme);
  bool sensorDisparado = (digitalRead(pir) == HIGH);

  if (!alarmeAtivado || !sensorDisparado) {
      // Se não houver música tocando, desliga o som
      if (!isPlaying) {
        noTone(som); 
        buzzerState = false;      
      }
  }
  else if (alarmeAtivado && sensorDisparado && !isPlaying) { // Prioridade para música
      unsigned long currentAlarmTime = millis();
      if (currentAlarmTime - previousAlarmTime >= alarmInterval) {
          previousAlarmTime = currentAlarmTime; 
          if (buzzerState == false) {
            tone(som, 2000); 
            buzzerState = true;
          } else {
            noTone(som); 
            buzzerState = false;
          }
      }
  }
}
