#include <Arduino.h>
#include <WiFi.h> 
#include <ESP32Servo.h> 
#include <DHT.h> 
#include <Preferences.h> 

Servo motorgaragem; 

// --- GESTÃO DE CREDENCIAIS DE REDE ---
Preferences preferences;
String ssid = ""; 
String password = ""; 
bool isAPMode = false;

WiFiServer server(80);

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

// --- ESTADOS DA CASA ---
bool st_cozinha = false;
bool st_sala = false;
bool st_garagem = false;
bool st_varanda = false;
bool st_quarto = false;
bool st_sotao = false;
bool st_porta = false;    
bool st_alarme = false;   

float temperatura = 0.0;
float umidade = 0.0;

// --- TEMPORIZADORES ---
unsigned long lastDhtTime = 0;
unsigned long lastWiFiCheck = 0;
unsigned long previousAlarmTime = 0; 
bool buzzerState = false;           
const long alarmInterval = 500;

// --- NOTAS MUSICAIS ---
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

const int melodyNatal[] = {   NOTE_C5,4, NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,  NOTE_F5,2, NOTE_C5,4,  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,  NOTE_E5,2, NOTE_E5,4,  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,  NOTE_C5,2, NOTE_A5,4,  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,  NOTE_F5,2, NOTE_C5,4,   NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,   NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,  NOTE_F5,2, NOTE_C5,4,  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,  NOTE_E5,2, NOTE_E5,4,  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,  NOTE_C5,2, NOTE_A5,4,  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,  NOTE_F5,2, NOTE_C5,4,   NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,   NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,  NOTE_F5,2, NOTE_C5,4,  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,   NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,  NOTE_F5,2, REST,4};
const int melodyStarWars[] = { NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8, NOTE_B4,8,  NOTE_AS4,8, NOTE_B4,8, NOTE_A4,8, REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_G4,8, NOTE_G4,4,  NOTE_E4,-2, NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8, NOTE_A4,-4, NOTE_A4,-4, NOTE_GS4,8, NOTE_A4,-4, NOTE_D5,8,  NOTE_C5,-4, NOTE_B4,-4, NOTE_A4,-4, NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,8,  NOTE_E5,-4, NOTE_B4,8, REST,8,  NOTE_AS4,8, NOTE_B4,8, NOTE_D5,4, NOTE_D5,-4, NOTE_B4,8, NOTE_A4,-4, NOTE_G4,-4, NOTE_E4,-2, NOTE_E4, 2, NOTE_G4,2, NOTE_B4, 2, NOTE_D5,2, NOTE_F5, -4, NOTE_E5,-4, NOTE_AS4,8, NOTE_AS4,8, NOTE_B4,4, NOTE_G4,4};
const int melodyCat[] = { REST,1, REST,1, NOTE_C4,4, NOTE_E4,4, NOTE_G4,4, NOTE_E4,4, NOTE_C4,4, NOTE_E4,8, NOTE_G4,-4, NOTE_E4,4, NOTE_A3,4, NOTE_C4,4, NOTE_E4,4, NOTE_C4,4, NOTE_A3,4, NOTE_C4,8, NOTE_E4,-4, NOTE_C4,4, NOTE_G3,4, NOTE_B3,4, NOTE_D4,4, NOTE_B3,4, NOTE_G3,4, NOTE_B3,8, NOTE_D4,-4, NOTE_B3,4, NOTE_G3,4, NOTE_G3,8, NOTE_G3,-4, NOTE_G3,8, NOTE_G3,4, NOTE_G3,4, NOTE_G3,4, NOTE_G3,8, NOTE_G3,4, NOTE_C4,4, NOTE_E4,4, NOTE_G4,4, NOTE_E4,4, NOTE_C4,4, NOTE_E4,8, NOTE_G4,-4, NOTE_E4,4, NOTE_A3,4, NOTE_C4,4, NOTE_E4,4, NOTE_C4,4, NOTE_A3,4, NOTE_C4,8, NOTE_E4,-4, NOTE_C4,4, NOTE_G3,4, NOTE_B3,4, NOTE_D4,4, NOTE_B3,4, NOTE_G3,4, NOTE_B3,8, NOTE_D4,-4, NOTE_B3,4, NOTE_G3,-1};

// --- CONTROLE DA MÚSICA ---
const int* currentMelody = nullptr;
int currentMelodySize = 0;         
int currentNoteIndex = 0;           
int currentTempo = 0;               
unsigned long nextNoteTime = 0;     
bool isPlaying = false;             

void iniciarMusica(const int* melodia, int tamanho, int tempo) {
  currentMelody = melodia;
  currentMelodySize = tamanho;
  currentTempo = tempo;
  currentNoteIndex = 0;
  nextNoteTime = 0;
  isPlaying = true;
  buzzerState = false; 
}

void handleMusic() {
  if (!isPlaying || currentMelody == nullptr) return;

  if (millis() >= nextNoteTime) {
    if (currentNoteIndex >= currentMelodySize) {
      isPlaying = false;
      noTone(som);
      return;
    }
    int note = currentMelody[currentNoteIndex];
    int durationVal = currentMelody[currentNoteIndex + 1];
    int wholenote = (60000 * 4) / currentTempo;
    int noteDuration = (durationVal > 0) ? (wholenote / durationVal) : ((wholenote / abs(durationVal)) * 1.5);

    if(note != REST) tone(som, note, noteDuration * 0.9);
    nextNoteTime = millis() + noteDuration;
    currentNoteIndex += 2;
  }
}

// --- FUNÇÕES DE HARDWARE ---
void handleWiFi() {
  if (!isAPMode && WiFi.status() != WL_CONNECTED) {
    if (millis() - lastWiFiCheck > 10000) {
      WiFi.disconnect();
      WiFi.begin(ssid.c_str(), password.c_str());
      lastWiFiCheck = millis();
    }
  }
}

void handleSensor() {
  if (millis() - lastDhtTime > 2000) {
    float novaT = dht.readTemperature();
    float novaH = dht.readHumidity();
    if (!isnan(novaT)) temperatura = novaT;
    if (!isnan(novaH)) umidade = novaH;
    lastDhtTime = millis();
  }
}

void handleAlarm() {
  bool sensorDisparado = (digitalRead(pir) == HIGH);
  digitalWrite(sinalalarme, st_alarme ? HIGH : LOW);

  if (!st_alarme || !sensorDisparado) {
      if (!isPlaying) { noTone(som); buzzerState = false; }
  } else if (st_alarme && sensorDisparado && !isPlaying) {
      if (millis() - previousAlarmTime >= alarmInterval) {
          previousAlarmTime = millis(); 
          buzzerState = !buzzerState;
          if (buzzerState) tone(som, 2000); 
          else noTone(som); 
      }
  }
}

void salvarNovaRedeWiFi(String req) {
  int sIndex = req.indexOf("?s=");
  int pIndex = req.indexOf("&p=");
  int spaceIndex = req.indexOf(" HTTP/");
  if (sIndex > 0 && pIndex > sIndex && spaceIndex > pIndex) {
    String newSSID = req.substring(sIndex + 3, pIndex);
    String newPass = req.substring(pIndex + 3, spaceIndex);
    newSSID.replace("+", " "); newPass.replace("+", " ");
    newSSID.replace("%20", " "); newPass.replace("%20", " ");
    preferences.putString("ssid", newSSID);
    preferences.putString("password", newPass);
  }
}

void processarComandos(String req) {
  if (req.indexOf("GET /cozinha/ligado") >= 0) { st_cozinha = true; digitalWrite(luzcozinha, HIGH); }
  else if (req.indexOf("GET /cozinha/desligado") >= 0) { st_cozinha = false; digitalWrite(luzcozinha, LOW); }
  else if (req.indexOf("GET /sala/ligado") >= 0) { st_sala = true; digitalWrite(luzsala, HIGH); }
  else if (req.indexOf("GET /sala/desligado") >= 0) { st_sala = false; digitalWrite(luzsala, LOW); }
  else if (req.indexOf("GET /garagem/ligado") >= 0) { st_garagem = true; digitalWrite(luzgaragem, HIGH); }
  else if (req.indexOf("GET /garagem/desligado") >= 0) { st_garagem = false; digitalWrite(luzgaragem, LOW); }
  else if (req.indexOf("GET /varanda/ligado") >= 0) { st_varanda = true; digitalWrite(luzvaranda, HIGH); }
  else if (req.indexOf("GET /varanda/desligado") >= 0) { st_varanda = false; digitalWrite(luzvaranda, LOW); }
  else if (req.indexOf("GET /quarto/ligado") >= 0) { st_quarto = true; digitalWrite(luzquarto, HIGH); }
  else if (req.indexOf("GET /quarto/desligado") >= 0) { st_quarto = false; digitalWrite(luzquarto, LOW); }
  else if (req.indexOf("GET /sotao/ligado") >= 0) { st_sotao = true; digitalWrite(luzsotao, HIGH); }
  else if (req.indexOf("GET /sotao/desligado") >= 0) { st_sotao = false; digitalWrite(luzsotao, LOW); }
  else if (req.indexOf("GET /porta/aberta") >= 0) { st_porta = true; motorgaragem.write(180); }
  else if (req.indexOf("GET /porta/fechada") >= 0) { st_porta = false; motorgaragem.write(0); }
  else if (req.indexOf("GET /alarme/ativado") >= 0) { st_alarme = true; }
  else if (req.indexOf("GET /alarme/desativado") >= 0) { st_alarme = false; isPlaying = false; noTone(som); }
  else if (req.indexOf("GET /musica/natal") >= 0) { iniciarMusica(melodyNatal, sizeof(melodyNatal)/sizeof(int), 140); }
  else if (req.indexOf("GET /musica/starwars") >= 0) { iniciarMusica(melodyStarWars, sizeof(melodyStarWars)/sizeof(int), 140); }
  else if (req.indexOf("GET /musica/cat") >= 0) { iniciarMusica(melodyCat, sizeof(melodyCat)/sizeof(int), 160); }
  else if (req.indexOf("GET /musica/parar") >= 0) { isPlaying = false; noTone(som); }
}

void enviarPaginaConfig(WiFiClient &client) {
  client.print("HTTP/1.1 200 OK\r\nContent-type:text/html\r\nConnection: close\r\n\r\n");
  client.print(R"rawliteral(
  <!DOCTYPE html><html>
  <head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Configuração Wi-Fi</title>
  <style>
    body{background:#121212;color:#e0e0e0;font-family:sans-serif;text-align:center;padding:30px;}
    .box{background:#1e1e1e;padding:30px;border-radius:15px;max-width:400px;margin:auto;box-shadow: 0 4px 6px rgba(0,0,0,0.3);}
    input{width:90%;padding:12px;margin:10px 0;border-radius:5px;border:none;background:#333;color:white;font-size:16px;}
    .btn{background:#2196F3;color:#fff;padding:12px 20px;border:none;border-radius:5px;cursor:pointer;font-size:16px;width:100%;margin-top:10px;font-weight:bold;}
    a {color: #888; text-decoration: none; margin-top: 20px; display: inline-block;}
  </style></head>
  <body>
    <h2>⚙️ Configurar Nova Rede Wi-Fi</h2>
    <div class="box">
      <form action="/save">
        <input type="text" name="s" placeholder="Nome da Rede (SSID)" required><br>
        <input type="password" name="p" placeholder="Senha da Rede" required><br>
        <button class="btn" type="submit">Salvar e Reiniciar ESP32</button>
      </form>
    </div><a href="/">⬅ Voltar ao Início</a>
  </body></html>
  )rawliteral");
}

void enviarPaginaWeb(WiFiClient &client) {
  client.print("HTTP/1.1 200 OK\r\nContent-type:text/html\r\nConnection: close\r\n\r\n");
  
  // Enviando o HTML gigante desacoplado
  client.print(R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Home - Dashboard IoT</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <style>
        body { font-family: 'Inter', sans-serif; background-color: #0f172a; color: #f8fafc; }
        .pulse-mic { animation: pulse 1.5s infinite; }
        @keyframes pulse { 0% { box-shadow: 0 0 0 0 rgba(59, 130, 246, 0.7); } 70% { box-shadow: 0 0 0 15px rgba(59, 130, 246, 0); } 100% { box-shadow: 0 0 0 0 rgba(59, 130, 246, 0); } }
    </style>
</head>
<body class="min-h-screen flex flex-col md:flex-row">
    <aside class="w-full md:w-64 bg-slate-900 border-r border-slate-800 flex flex-col shadow-2xl z-20">
        <div class="p-6 border-b border-slate-800 flex items-center gap-3">
            <div class="bg-blue-600 p-2 rounded-lg"><i class="fa-solid fa-microchip text-white"></i></div>
            <h1 class="font-bold text-xl tracking-wide">Casa<span class="text-blue-500">MDF</span></h1>
        </div>
        <div class="p-4 flex-1">
            <p class="text-xs text-slate-500 font-bold uppercase mb-4 px-2">Monitoramento</p>
            <div class="space-y-2">
                <div class="bg-slate-800 rounded-xl p-4 flex items-center justify-between border border-slate-700">
                    <div class="flex items-center gap-3"><i class="fa-solid fa-temperature-half text-orange-500"></i><span class="text-sm">Temp.</span></div>
                    <span id="txt-temp" class="font-bold text-orange-400">-- °C</span>
                </div>
                <div class="bg-slate-800 rounded-xl p-4 flex items-center justify-between border border-slate-700">
                    <div class="flex items-center gap-3"><i class="fa-solid fa-droplet text-blue-400"></i><span class="text-sm">Umidade</span></div>
                    <span id="txt-umid" class="font-bold text-blue-400">-- %</span>
                </div>
                <div class="bg-slate-800 rounded-xl p-4 flex items-center justify-between border border-slate-700">
                    <div class="flex items-center gap-3"><i class="fa-solid fa-chart-line text-purple-400"></i><span class="text-sm">Média Temp.</span></div>
                    <div class="text-right">
                        <span id="txt-media-temp" class="font-bold text-purple-400 block">-- °C</span>
                        <span class="text-[10px] text-slate-500">(Última Hora)</span>
                    </div>
                </div>
            </div>
        </div>
        <div class="p-4 border-t border-slate-800 text-xs text-center text-slate-500">
            <a href="/config" class="text-blue-400 hover:text-blue-300">⚙️ Configurações Wi-Fi</a>
        </div>
    </aside>

    <main class="flex-1 overflow-y-auto p-4 md:p-8 bg-slate-950 relative">
        <header class="flex flex-col md:flex-row justify-between items-center mb-8 gap-4">
            <div>
                <h2 class="text-2xl font-bold">Painel de Controle</h2>
                <p class="text-slate-400 text-sm">Controle sua casa por voz ou pelos botões.</p>
            </div>
            <button id="btn-mic" onclick="toggleVoice()" class="flex items-center gap-3 bg-blue-600 hover:bg-blue-500 text-white px-6 py-3 rounded-full font-bold shadow-[0_0_15px_rgba(37,99,235,0.4)] transition-all">
                <i class="fa-solid fa-microphone text-xl"></i>
                <span id="txt-mic">Ouvir Comandos</span>
            </button>
        </header>

        <h3 class="text-slate-400 font-semibold mb-4 text-sm uppercase tracking-wider">Dispositivos</h3>
        <div class="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-4 mb-8" id="device-container">
            <!-- Dispositivos injetados via JS -->
        </div>

        <h3 class="text-slate-400 font-semibold mb-4 text-sm uppercase tracking-wider">Mídia e Acessos</h3>
        <div class="grid grid-cols-1 md:grid-cols-2 gap-4 mb-8">
            <div class="bg-slate-900 border border-slate-800 rounded-2xl p-4 flex justify-between items-center">
                <div class="flex items-center gap-3">
                    <i id="icon-porta" class="fa-solid fa-door-closed text-2xl text-slate-600"></i>
                    <span class="font-medium text-sm text-slate-300">Porta Principal</span>
                </div>
                <button id="btn-porta" onclick="togglePorta()" class="bg-slate-700 px-4 py-1.5 rounded-full text-xs font-bold hover:bg-slate-600">ABRIR</button>
            </div>
            <div class="bg-slate-900 border border-red-900/30 rounded-2xl p-4 flex justify-between items-center">
                <div class="flex items-center gap-3">
                    <i id="icon-alarme" class="fa-solid fa-shield-halved text-2xl text-slate-600"></i>
                    <span class="font-medium text-sm text-slate-300">Alarme Geral</span>
                </div>
                <button id="btn-alarme" onclick="toggleAlarme()" class="bg-slate-700 px-4 py-1.5 rounded-full text-xs font-bold hover:bg-red-600">ARMAR</button>
            </div>
        </div>

        <div class="bg-slate-900 border border-slate-800 rounded-2xl p-4 flex flex-wrap gap-3 mb-8 items-center">
            <i class="fa-solid fa-music text-purple-500 text-xl mr-2"></i>
            <button onclick="playMusic('natal')" class="bg-slate-800 hover:bg-purple-600/50 text-slate-200 border border-slate-700 px-4 py-2 rounded-xl text-sm transition-colors">🎄 Natal</button>
            <button onclick="playMusic('starwars')" class="bg-slate-800 hover:bg-blue-600/50 text-slate-200 border border-slate-700 px-4 py-2 rounded-xl text-sm transition-colors">⚔️ Star Wars</button>
            <button onclick="playMusic('cat')" class="bg-slate-800 hover:bg-orange-600/50 text-slate-200 border border-slate-700 px-4 py-2 rounded-xl text-sm transition-colors">🐱 Keyboard Cat</button>
            <button onclick="playMusic('parar')" class="bg-red-900/50 hover:bg-red-600 text-white border border-red-800 px-4 py-2 rounded-xl text-sm transition-colors ml-auto font-bold"><i class="fa-solid fa-stop"></i> Parar</button>
        </div>

        <h3 class="text-slate-400 font-semibold mb-4 text-sm uppercase tracking-wider flex items-center gap-2"><i class="fa-solid fa-bolt text-yellow-400"></i> Consumo de Energia Virtual</h3>
        <div class="grid grid-cols-1 md:grid-cols-3 gap-4 mb-8">
            <div class="bg-slate-800 p-5 rounded-2xl border border-slate-700 shadow-inner">
                <p class="text-xs text-slate-400 mb-1">Custo das Luzes Físicas</p>
                <h4 class="text-2xl font-bold text-white mb-1" id="cost-lights">R$ 0,00</h4>
                <p class="text-xs text-slate-500" id="time-lights">0h 0m 0s (15W por luz)</p>
            </div>
            <div class="bg-slate-800 p-5 rounded-2xl border border-slate-700 shadow-inner border-l-4 border-l-blue-500">
                <p class="text-xs text-slate-400 mb-1">Custo Processamento IA</p>
                <h4 class="text-2xl font-bold text-blue-400 mb-1" id="cost-ai">R$ 0,00</h4>
                <p class="text-xs text-slate-500"><span id="count-ai">0</span> requisições (3Wh cada)</p>
            </div>
        </div>

        <div class="bg-slate-900 border border-slate-800 p-4 rounded-xl min-h-[100px] text-sm text-slate-400" id="ai-log">
            Clique no microfone e diga: <em>"Ligue a sala, abra a porta e toque Star Wars"</em>.
        </div>
    </main>

    <script>
        // 1. ESTADO GLOBAL E INTEGRAÇÃO C++ API REST
        let houseState = {
            sala: false, cozinha: false, quarto: false, garagem: false, varanda: false, sotao: false,
            porta: false, alarme: false
        };

        const devices = [
            { id: 'sala', name: 'Luz Sala', icon: 'fa-lightbulb' },
            { id: 'cozinha', name: 'Luz Cozinha', icon: 'fa-lightbulb' },
            { id: 'quarto', name: 'Luz Quarto', icon: 'fa-bed' },
            { id: 'garagem', name: 'Garagem', icon: 'fa-car' },
            { id: 'varanda', name: 'Varanda', icon: 'fa-sun' },
            { id: 'sotao', name: 'Sótão', icon: 'fa-box' }
        ];

        const container = document.getElementById('device-container');
        devices.forEach(dev => {
            container.innerHTML += `
                <div class="bg-slate-900 border border-slate-800 rounded-2xl p-4 flex flex-col items-center justify-center gap-3 hover:border-slate-600 transition-colors">
                    <i id="icon-${dev.id}" class="fa-solid ${dev.icon} text-3xl text-slate-600 transition-colors"></i>
                    <span class="font-medium text-sm text-slate-300">${dev.name}</span>
                    <label class="relative inline-flex items-center cursor-pointer">
                        <input type="checkbox" id="toggle-${dev.id}" onchange="updateDevice('${dev.id}', this.checked)" class="sr-only peer">
                        <div class="w-11 h-6 bg-slate-700 peer-focus:outline-none rounded-full peer peer-checked:after:translate-x-full peer-checked:after:border-white after:content-[''] after:absolute after:top-[2px] after:left-[2px] after:bg-white after:border-gray-300 after:border after:rounded-full after:h-5 after:w-5 after:transition-all peer-checked:bg-blue-500"></div>
                    </label>
                </div>
            `;
        });

        // 2. FUNÇÕES DE COMUNICAÇÃO COM O ESP32 (FETCH API)
        function updateDevice(device, state) {
            houseState[device] = state;
            const toggle = document.getElementById('toggle-' + device);
            const icon = document.getElementById('icon-' + device);
            if(toggle) toggle.checked = state;
            if(icon) {
                if(state) icon.classList.replace('text-slate-600', 'text-yellow-400');
                else icon.classList.replace('text-yellow-400', 'text-slate-600');
            }
            // Envia requisição real pro C++ sem recarregar a tela!
            fetch(`/${device}/${state ? 'ligado' : 'desligado'}`).catch(e => console.error("ESP32 Offline"));
        }

        function togglePorta() {
            houseState.porta = !houseState.porta;
            const btn = document.getElementById('btn-porta');
            const icon = document.getElementById('icon-porta');
            if (houseState.porta) {
                btn.innerText = "FECHAR"; btn.classList.replace('bg-slate-700', 'bg-yellow-600');
                icon.classList.replace('fa-door-closed', 'fa-door-open'); icon.classList.replace('text-slate-600', 'text-yellow-400');
            } else {
                btn.innerText = "ABRIR"; btn.classList.replace('bg-yellow-600', 'bg-slate-700');
                icon.classList.replace('fa-door-open', 'fa-door-closed'); icon.classList.replace('text-yellow-400', 'text-slate-600');
            }
            fetch(`/porta/${houseState.porta ? 'aberta' : 'fechada'}`).catch(e => console.error("ESP32 Offline"));
        }

        function toggleAlarme() {
            houseState.alarme = !houseState.alarme;
            const btn = document.getElementById('btn-alarme');
            const icon = document.getElementById('icon-alarme');
            if (houseState.alarme) {
                btn.innerText = "DESARMAR"; btn.classList.replace('bg-slate-700', 'bg-red-600');
                icon.classList.replace('text-slate-600', 'text-red-500'); icon.classList.add('fa-beat-fade');
            } else {
                btn.innerText = "ARMAR"; btn.classList.replace('bg-red-600', 'bg-slate-700');
                icon.classList.replace('text-red-500', 'text-slate-600'); icon.classList.remove('fa-beat-fade');
            }
            fetch(`/alarme/${houseState.alarme ? 'ativado' : 'desativado'}`).catch(e => console.error("ESP32 Offline"));
        }

        function playMusic(song) {
            fetch(`/musica/${song}`).catch(e => console.error("ESP32 Offline"));
        }

        // 3. BUSCA SENSORES EM TEMPO REAL NO ESP32
        let tempHistory = [];
        setInterval(() => {
            fetch('/sensores')
            .then(res => res.json())
            .then(data => {
                document.getElementById('txt-temp').innerText = `${data.temp.toFixed(1)} °C`;
                document.getElementById('txt-umid').innerText = `${data.umid.toFixed(1)} %`;
                document.getElementById('txt-temp').classList.replace('text-slate-500', 'text-orange-400');
                
                tempHistory.push({ temp: data.temp, time: Date.now() });
                tempHistory = tempHistory.filter(reading => Date.now() - reading.time <= 60000); // Média de 1 minuto
                const avg = tempHistory.reduce((acc, curr) => acc + curr.temp, 0) / tempHistory.length;
                document.getElementById('txt-media-temp').innerText = `${avg.toFixed(1)} °C`;
            })
            .catch(() => {
                document.getElementById('txt-temp').innerText = `Offline`;
                document.getElementById('txt-umid').innerText = `Offline`;
                document.getElementById('txt-temp').classList.replace('text-orange-400', 'text-slate-500');
            });
        }, 2000);

        // 4. MEDIDOR VIRTUAL DE ENERGIA
        const KWH_PRICE = 0.95; 
        let secsLights = 0; let aiRequests = 0;
        setInterval(() => {
            let activeLights = ['sala','cozinha','quarto','garagem','varanda','sotao'].filter(d => houseState[d]).length;
            if(activeLights > 0) {
                secsLights += activeLights; 
                let costLights = ((15 * (secsLights / 3600)) / 1000) * KWH_PRICE;
                document.getElementById('time-lights').innerText = `${Math.floor(secsLights/3600)}h ${Math.floor((secsLights%3600)/60)}m ${secsLights%60}s (${activeLights} ligadas)`;
                document.getElementById('cost-lights').innerText = `R$ ${costLights.toFixed(5)}`;
            }
        }, 1000);
// 5. INTELIGÊNCIA ARTIFICIAL (WAKE WORD BLINDADO & GEMINI)
        const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
        let recognition = null;
        let isModoJarvisAtivo = false;
        
        // DICA SÊNIOR: Lista de palavras que o Chrome costuma confundir com "Jarvis"
        const WAKE_WORDS = ["jarvis", "jarves", "jardis", "chaves", "já vi", "charlie"]; 

        function falarLocalmente(texto) {
            if ('speechSynthesis' in window) {
                window.speechSynthesis.cancel();
                const utterance = new SpeechSynthesisUtterance(texto);
                utterance.lang = 'pt-BR';
                utterance.rate = 1.0;
                utterance.pitch = 1.1;
                window.speechSynthesis.speak(utterance);
            }
        }

        if(SpeechRecognition) {
            recognition = new SpeechRecognition();
            recognition.lang = 'pt-BR'; 
            recognition.continuous = true; 
            recognition.interimResults = false; 

            recognition.onresult = (event) => {
                const lastIndex = event.results.length - 1;
                let transcriptRaw = event.results[lastIndex][0].transcript.toLowerCase();
                
                // Limpeza Sênior: Remove pontos e vírgulas que o Chrome insiste em colocar
                let transcript = transcriptRaw.replace(/[.,!?]/g, "").trim();
                
                console.log("🎙️ Captura bruta:", transcript);

                // Procura se alguma das "Wake Words" está na frase
                let palavraDisparadora = WAKE_WORDS.find(w => transcript.includes(w));

                if (palavraDisparadora) {
                    const partes = transcript.split(palavraDisparadora);
                    const comando = partes[1] ? partes[1].trim() : "";

                    if (comando.length > 2) {
                        processCommandWithAI(comando);
                    } else {
                        falarLocalmente("Sim, senhor?");
                        document.getElementById('ai-log').innerHTML = `<p class="text-blue-400">Jarvis: "Sim, senhor?"</p>`;
                    }
                }
            };

            recognition.onend = () => {
                // BACKOFF PATTERN: Se cair, espera 1 segundo antes de reabrir para evitar o loop de "network" error
                if (isModoJarvisAtivo) {
                    setTimeout(() => {
                        try { recognition.start(); } catch(e) {}
                    }, 1000);
                } else {
                    const btn = document.getElementById('btn-mic');
                    btn.classList.remove('pulse-mic', 'bg-red-600');
                    btn.classList.add('bg-blue-600');
                    document.getElementById('txt-mic').innerText = "Ativar Modo Jarvis";
                }
            }
            
            recognition.onerror = (event) => {
                // Filtramos os erros visuais para não sujar o seu console com 'network' ou 'no-speech'
                if (event.error !== 'no-speech' && event.error !== 'network') {
                    console.error("Erro no microfone:", event.error);
                }
            };
        }

        function toggleVoice() {
            if(!recognition) { alert("Seu navegador não suporta voz."); return; }
            
            isModoJarvisAtivo = !isModoJarvisAtivo;
            const btn = document.getElementById('btn-mic');
            
            if (isModoJarvisAtivo) {
                btn.classList.add('pulse-mic');
                btn.classList.replace('bg-blue-600', 'bg-red-600');
                document.getElementById('txt-mic').innerText = "Jarvis Escutando...";
                document.getElementById('ai-log').innerHTML = `<span class="text-blue-400">Modo ambiente ativado. Fale normalmente, e comece com "Jarvis" quando quiser algo.</span>`;
                try { recognition.start(); } catch(e) {}
            } else {
                recognition.stop();
                document.getElementById('ai-log').innerHTML = `Modo Jarvis desativado.`;
            }
        }

        async function processCommandWithAI(transcript) {
            aiRequests++;
            document.getElementById('count-ai').innerText = aiRequests;
            document.getElementById('cost-ai').innerText = `R$ ${(((3 * aiRequests) / 1000) * KWH_PRICE).toFixed(4)}`;
            document.getElementById('ai-log').innerHTML = `<p><b>Você:</b> "Jarvis, ${transcript}"</p><p class="text-blue-400 mt-2"><i class="fa-solid fa-microchip fa-spin"></i> Processando lógica e voz...</p>`;

            // ATENÇÃO: COLOQUE SUA CHAVE AQUI
            const apiKey = "Chave-de-API-Google-Aqui"; 
            const apiUrl = `https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=${apiKey}`;

            const systemPrompt = `Você é a inteligência artificial de uma casa inteligente (estilo Alexa/JARVIS). 
            Retorne EXATAMENTE um objeto JSON com duas propriedades:
            1. "speech": Uma frase curta, natural e amigável confirmando o que você está fazendo.
            2. "commands": Um array de objetos com as ações físicas.
            
            Dispositivos válidos (boolean): "sala", "cozinha", "quarto", "garagem", "varanda", "sotao", "porta", "alarme".
            Música (string): "natal", "starwars", "cat", "parar".
            
            Exemplo: 
            {
              "speech": "Entendido! Ligando a luz da sala e tocando a marcha imperial.",
              "commands": [{"device": "sala", "state": true}, {"device": "musica", "state": "starwars"}]
            }
            
            Comando do usuário: "${transcript}"`;

            try {
                const response = await fetch(apiUrl, {
                    method: 'POST', 
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ contents: [{ parts: [{ text: systemPrompt }] }] })
                });
                
                if (!response.ok) throw new Error(`Falha na API: ${response.status}`);

                const result = await response.json();
                let rawText = result.candidates[0].content.parts[0].text;
                let cleanText = rawText.replace(/```json/g, '').replace(/```/g, '').trim();
                
                const aiResponse = JSON.parse(cleanText);
                
                falarLocalmente(aiResponse.speech);
                document.getElementById('ai-log').innerHTML += `<p class="text-green-400 mt-2"><i class="fa-solid fa-comment-dots"></i> IA: "${aiResponse.speech}"</p>`;
                
                if (aiResponse.commands && Array.isArray(aiResponse.commands)) {
                    aiResponse.commands.forEach(cmd => {
                        if(typeof cmd.state === "boolean") {
                            if(cmd.device === "porta") { if(cmd.state !== houseState.porta) togglePorta(); }
                            else if(cmd.device === "alarme") { if(cmd.state !== houseState.alarme) toggleAlarme(); }
                            else updateDevice(cmd.device, cmd.state);
                        } else if (cmd.device === "musica") {
                            playMusic(cmd.state);
                        }
                    });
                }
                
            } catch (e) { 
                console.error(e);
                document.getElementById('ai-log').innerHTML += `<p class="text-red-400 mt-2">Erro de comunicação com a IA.</p>`; 
            }
        }
    </script>
</body></html>
  )rawliteral");
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  preferences.begin("wifi_creds", false);
  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");

  ESP32PWM::allocateTimer(0);
  motorgaragem.setPeriodHertz(50); 
  motorgaragem.attach(pinoServo, 500, 2400); 
  motorgaragem.write(0); 

  pinMode(luzcozinha, OUTPUT); pinMode(luzsala, OUTPUT); pinMode(luzgaragem, OUTPUT);
  pinMode(luzvaranda, OUTPUT); pinMode(luzquarto, OUTPUT); pinMode(luzsotao, OUTPUT);
  pinMode(som, OUTPUT); pinMode(sinalalarme, OUTPUT); pinMode(pir, INPUT);

  if(ssid == "") {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("SmartHome_Setup"); 
    isAPMode = true;
  } else {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 15) { delay(1000); tentativas++; }
    if(WiFi.status() == WL_CONNECTED) isAPMode = false;
    else { WiFi.mode(WIFI_AP); WiFi.softAP("SmartHome_Setup"); isAPMode = true; }
  }
  server.begin();
}

void loop() {
  handleWiFi(); handleSensor(); handleMusic(); handleAlarm();

  WiFiClient client = server.available();
  if (client) {
    unsigned long currentTime = millis();
    unsigned long previousTime = currentTime;
    String reqLine = "";
    
    while (client.connected() && (currentTime - previousTime <= 2000)) {
      currentTime = millis(); handleMusic();
      if (client.available()) {
        char c = client.read();
        reqLine += c;
        if (reqLine.length() > 200 || c == '\n') break; 
      }
    }

    if (reqLine.length() > 0) {
      if (reqLine.indexOf("GET /config") >= 0) {
        enviarPaginaConfig(client);
      } else if (reqLine.indexOf("GET /save?") >= 0) {
        salvarNovaRedeWiFi(reqLine);
        client.print("HTTP/1.1 200 OK\r\nContent-type:text/html\r\nConnection: close\r\n\r\n<html><body><h2>Salvo! Reiniciando...</h2></body></html>");
        delay(1500); ESP.restart();
      } else if (reqLine.indexOf("GET /sensores") >= 0) {
        // RESPOSTA RÁPIDA (API REST) DE SENSORES
        client.print("HTTP/1.1 200 OK\r\nContent-type:application/json\r\nConnection: close\r\n\r\n");
        client.print("{\"temp\": " + String(temperatura) + ", \"umid\": " + String(umidade) + "}");
      } else if (reqLine.indexOf("GET / ") >= 0) {
        // SÓ ENVIA A PÁGINA GIGANTE NA PRIMEIRA VEZ
        enviarPaginaWeb(client);
      } else if (reqLine.indexOf("GET /favicon.ico") >= 0) {
        client.print("HTTP/1.1 404 Not Found\r\n\r\n");
      } else {
        // É UM COMANDO (Botão ou IA) - RÁPIDO
        processarComandos(reqLine);
        client.print("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n");
      }
    }
    while(client.available()) client.read(); 
    client.stop();
  }
}