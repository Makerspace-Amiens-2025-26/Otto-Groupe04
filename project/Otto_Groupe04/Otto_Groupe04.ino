/*    -- HK --   */

// ================== REMOTEXY ==================
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <ESP32Servo.h>

#define REMOTEXY_BLUETOOTH_NAME "HK"
#define REMOTEXY_ACCESS_PASSWORD "silksong"

#include <RemoteXY.h>

#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] =   // 57 bytes V19
  { 255,5,0,0,0,50,0,19,0,0,0,72,75,0,200,1,106,200,1,1,
  4,0,5,15,43,80,80,32,21,172,31,3,6,6,53,8,135,21,26,1,
  6,133,32,32,0,21,31,0,1,70,134,33,33,0,21,31,0 };

struct {
  int8_t  joystick_01_x;
  int8_t  joystick_01_y;
  uint8_t selectorSwitch_01; // 0=A 1=B 2=C 3=D 4=E 5=F 6=G(dance+musique)
  uint8_t button_01;         // virage gauche
  uint8_t button_02;         // virage droit
  uint8_t connect_flag;
} RemoteXY;
#pragma pack(pop)

// ================== VITESSE ==================
struct SpeedProfile { int stepDelay; int pauseDelay; };

SpeedProfile getSpeed() {
  switch (RemoteXY.selectorSwitch_01) {
    case 0: return {20, 120};
    case 1: return {16,  95};
    case 2: return {12,  70};
    case 3: return { 9,  50};
    case 4: return { 5,  20};
    case 5: return { 5,  10};
    case 6: return { 9,  50};
    default: return {10,  60};
  }
}

// ================== PINS ==================
#define BUZZER_PIN    D0   // ton buzzer est sur D0
#define PIN_BTN_DANCE D3
#define PIN_BTN_MUSIC D4

// ================== PITCHES ==================
#define NOTE_D5  587
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_A6  880
#define NOTE_B6  988
#define NOTE_C6  1047
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_A7  1760
#define NOTE_B7  1976
#define NOTE_C7  2093
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794

// ================== MELODIE ==================
int melody[] = {
  NOTE_D5,  NOTE_D5,  NOTE_FS5, NOTE_G5,
  NOTE_FS5, NOTE_FS5, NOTE_G5,  NOTE_A6,
  NOTE_A6,  NOTE_A6,  NOTE_B6,  NOTE_C6,
  NOTE_B6,  NOTE_DS6, NOTE_E6,  NOTE_FS6,
  NOTE_E6,  NOTE_FS6, NOTE_G6,  NOTE_A7,
  NOTE_B7,  NOTE_B7,  NOTE_C7,  NOTE_D7,
  NOTE_C7,  NOTE_C7,  NOTE_E7,  NOTE_F7,
  NOTE_D7,  NOTE_DS7, NOTE_E7
};
int durations[] = {
  400, 400, 400, 400,
  400, 400, 400, 400,
  400, 400, 400, 400,
  400, 400, 400, 400,
  400, 400, 400, 400,
  400, 400, 400, 400,
  400, 400, 400, 400,
  400, 400, 600
};
const int MELODY_LEN = sizeof(melody) / sizeof(melody[0]);

bool          musicOn        = false;
bool          btn_music_prev = false;
int           musicNote      = 0;
unsigned long musicNext      = 0;
unsigned long musicSilence   = 0;
bool          musicPlaying   = false;

void musicStart() {
  musicOn      = true;
  musicNote    = 0;
  musicNext    = millis();
  musicPlaying = false;
}

void musicStop() {
  musicOn      = false;
  musicPlaying = false;
  noTone(BUZZER_PIN);
}

// Non-bloquant : appelé dans chaque itération servo
void updateMusic() {
  if (!musicOn) return;
  unsigned long now = millis();
  // Couper la note à 85% de sa durée
  if (musicPlaying && now >= musicSilence) {
    noTone(BUZZER_PIN);
    musicPlaying = false;
  }
  // Lancer la note suivante
  if (!musicPlaying && now >= musicNext) {
    tone(BUZZER_PIN, melody[musicNote], durations[musicNote]);
    musicPlaying = true;
    musicSilence = now + (unsigned long)(durations[musicNote] * 0.85);
    musicNext    = now + (unsigned long)durations[musicNote];
    musicNote    = (musicNote + 1) % MELODY_LEN;
  }
}

// ================== SERVOS ==================
Servo L_LEG, R_LEG, L_FOOT, R_FOOT;

const int OFFSET_L_LEG  = -35;
const int OFFSET_R_LEG  = +55;
const int OFFSET_L_FOOT = +20;
const int OFFSET_R_FOOT =   0;

int pos_L_LEG  = 90 + OFFSET_L_LEG;
int pos_R_LEG  = 90 + OFFSET_R_LEG;
int pos_L_FOOT = 90 + OFFSET_L_FOOT;
int pos_R_FOOT = 90 + OFFSET_R_FOOT;

// ================== PRIMITIVES ==================
void moveSlow(Servo &servo, int &currentPos, int target, int stepDelay) {
  if (currentPos < target)
    for (int i = currentPos; i <= target; i++) { servo.write(i); updateMusic(); RemoteXY_delay(stepDelay); yield(); }
  else
    for (int i = currentPos; i >= target; i--) { servo.write(i); updateMusic(); RemoteXY_delay(stepDelay); yield(); }
  currentPos = target;
}

void moveSlowDuo(Servo &s1, int &p1, int t1,
                 Servo &s2, int &p2, int t2, int stepDelay) {
  int steps = max(abs(t1-p1), abs(t2-p2));
  for (int i = 0; i <= steps; i++) {
    if (p1 != t1) { p1 += (t1>p1)?1:-1; s1.write(p1); }
    if (p2 != t2) { p2 += (t2>p2)?1:-1; s2.write(p2); }
    updateMusic(); RemoteXY_delay(stepDelay); yield();
  }
}

void moveSlowTrio(Servo &s1, int &p1, int t1,
                  Servo &s2, int &p2, int t2,
                  Servo &s3, int &p3, int t3, int stepDelay) {
  int steps = max({abs(t1-p1), abs(t2-p2), abs(t3-p3)});
  for (int i = 0; i <= steps; i++) {
    if (p1 != t1) { p1 += (t1>p1)?1:-1; s1.write(p1); }
    if (p2 != t2) { p2 += (t2>p2)?1:-1; s2.write(p2); }
    if (p3 != t3) { p3 += (t3>p3)?1:-1; s3.write(p3); }
    updateMusic(); RemoteXY_delay(stepDelay); yield();
  }
}

void moveSlowQuad(Servo &s1, int &p1, int t1,
                  Servo &s2, int &p2, int t2,
                  Servo &s3, int &p3, int t3,
                  Servo &s4, int &p4, int t4, int stepDelay) {
  int steps = max({abs(t1-p1), abs(t2-p2), abs(t3-p3), abs(t4-p4)});
  for (int i = 0; i <= steps; i++) {
    if (p1 != t1) { p1 += (t1>p1)?1:-1; s1.write(p1); }
    if (p2 != t2) { p2 += (t2>p2)?1:-1; s2.write(p2); }
    if (p3 != t3) { p3 += (t3>p3)?1:-1; s3.write(p3); }
    if (p4 != t4) { p4 += (t4>p4)?1:-1; s4.write(p4); }
    updateMusic(); RemoteXY_delay(stepDelay); yield();
  }
}

// ================== INITIALISATION ==================
void initialisation() {
  pos_L_LEG  = 90 + OFFSET_L_LEG;
  pos_R_LEG  = 90 + OFFSET_R_LEG;
  pos_L_FOOT = 90 + OFFSET_L_FOOT;
  pos_R_FOOT = 90 + OFFSET_R_FOOT;
  L_LEG.write(pos_L_LEG);   RemoteXY_delay(400); yield();
  R_LEG.write(pos_R_LEG);   RemoteXY_delay(400); yield();
  L_FOOT.write(pos_L_FOOT); RemoteXY_delay(400); yield();
  R_FOOT.write(pos_R_FOOT); RemoteXY_delay(400); yield();
}

// ================== PAS DROIT ==================
void pasDroit(int ampActif, int ampPivot, int balance) {
  SpeedProfile sp = getSpeed();
  moveSlow(R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT - 30, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  moveSlowTrio(L_LEG,  pos_L_LEG,  80  + OFFSET_L_LEG  + ampActif,
               R_LEG,  pos_R_LEG,  90  + OFFSET_R_LEG  + ampPivot,
               L_FOOT, pos_L_FOOT, 100 + OFFSET_L_FOOT + balance,
               sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  moveSlowDuo(R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT,
              L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
}

// ================== PAS GAUCHE ==================
void pasGauche(int ampActif, int ampPivot, int balance) {
  SpeedProfile sp = getSpeed();
  moveSlow(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT + 30, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  moveSlowTrio(L_LEG,  pos_L_LEG,  100 + OFFSET_L_LEG  - ampPivot,
               R_LEG,  pos_R_LEG,   90 + OFFSET_R_LEG  - ampActif,
               R_FOOT, pos_R_FOOT,  80 + OFFSET_R_FOOT + balance,
               sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  moveSlowDuo(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT,
              R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
}

// ================== PAS ARRIÈRE — INCHANGÉS ==================
void pasArriereDroit(int amplitude, int balance) {
  SpeedProfile sp = getSpeed();
  moveSlow(R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT - 30, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  moveSlowTrio(L_LEG,  pos_L_LEG,  90 + OFFSET_L_LEG  - amplitude,
               R_LEG,  pos_R_LEG,  90 + OFFSET_R_LEG  - amplitude,
               L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT - balance,
               sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  moveSlowDuo(R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT,
              L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
}

void pasArriereGauche(int amplitude, int balance) {
  SpeedProfile sp = getSpeed();
  moveSlow(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT + 30, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  moveSlowTrio(L_LEG,  pos_L_LEG,  90 + OFFSET_L_LEG  + amplitude,
               R_LEG,  pos_R_LEG,  90 + OFFSET_R_LEG  + amplitude,
               R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT + balance,
               sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  moveSlowDuo(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT,
              R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
}

// ================== DANCE ==================
bool danceRunning() {
  return (RemoteXY.selectorSwitch_01 == 6) || (digitalRead(PIN_BTN_DANCE) == LOW);
}

void danceWiggle() {
  SpeedProfile sp = getSpeed();
  for (int r = 0; r < 4; r++) {
    if (!danceRunning()) return;
    moveSlowDuo(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT + 20,
                R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT - 20, sp.stepDelay);
    moveSlowDuo(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT - 20,
                R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT + 20, sp.stepDelay);
  }
  initialisation();
}

void danceSway() {
  SpeedProfile sp = getSpeed();
  for (int r = 0; r < 3; r++) {
    if (!danceRunning()) return;
    moveSlow(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT + 35, sp.stepDelay);
    RemoteXY_delay(sp.pauseDelay * 2);
    moveSlow(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT - 35, sp.stepDelay);
    RemoteXY_delay(sp.pauseDelay * 2);
  }
  initialisation();
}

void danceBow() {
  SpeedProfile sp = getSpeed();
  if (!danceRunning()) return;
  moveSlowDuo(L_LEG, pos_L_LEG, 90 + OFFSET_L_LEG + 30,
              R_LEG, pos_R_LEG, 90 + OFFSET_R_LEG + 30, sp.stepDelay);
  RemoteXY_delay(600);
  moveSlowDuo(L_LEG, pos_L_LEG, 90 + OFFSET_L_LEG,
              R_LEG, pos_R_LEG, 90 + OFFSET_R_LEG, sp.stepDelay);
  initialisation();
}

void danceStep() {
  for (int r = 0; r < 2; r++) {
    if (!danceRunning()) return;
    pasDroit (20, 20, 15);
    pasGauche(20, 20, 15);
  }
  initialisation();
}

void danceSpin() {
  for (int r = 0; r < 3; r++) {
    if (!danceRunning()) return;
    pasDroit (55, 10, 25);
    pasGauche(55, 10, 25);
  }
  initialisation();
}

void danceJump() {
  SpeedProfile sp = getSpeed();
  for (int r = 0; r < 3; r++) {
    if (!danceRunning()) return;
    moveSlowQuad(L_LEG,  pos_L_LEG,  90 + OFFSET_L_LEG  + 30,
                 R_LEG,  pos_R_LEG,  90 + OFFSET_R_LEG,
                 L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT,
                 R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT - 25, sp.stepDelay);
    RemoteXY_delay(200);
    moveSlowQuad(L_LEG,  pos_L_LEG,  90 + OFFSET_L_LEG,
                 R_LEG,  pos_R_LEG,  90 + OFFSET_R_LEG  + 30,
                 L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT + 25,
                 R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT, sp.stepDelay);
    RemoteXY_delay(200);
  }
  initialisation();
}

void playDance() {
  typedef void (*DanceMove)();
  DanceMove moves[] = { danceWiggle, danceSway, danceBow,
                        danceStep,   danceSpin, danceJump };
  for (int i = 0; i < 4; i++) {
    if (!danceRunning()) return;
    moves[random(6)]();
    RemoteXY_delay(300);
  }
}

// ================== CRABE ==================
// crabeGauche : L_FOOT incliné fixe (+20°) pour stabiliser,
//               R_LEG monte + R_FOOT pousse → robot glisse à gauche
void crabeGauche() {
  SpeedProfile sp = getSpeed();
  // 1. Stabiliser le pied gauche en angle
  moveSlow(L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT + 20, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  // 2. Lever la jambe droite + abaisser le pied droit (prise d'appui)
  moveSlowDuo(R_LEG,  pos_R_LEG,  90 + OFFSET_R_LEG  - 20,
              R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT + 25, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  // 3. Redescendre la jambe droite + remonter le pied droit (poussée → glisse à gauche)
  moveSlowDuo(R_LEG,  pos_R_LEG,  90 + OFFSET_R_LEG,
              R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
}

// crabeDroit : R_FOOT incliné fixe (-20°) pour stabiliser,
//              L_LEG monte + L_FOOT pousse → robot glisse à droite
void crabeDroit() {
  SpeedProfile sp = getSpeed();
  // 1. Stabiliser le pied droit en angle
  moveSlow(R_FOOT, pos_R_FOOT, 90 + OFFSET_R_FOOT - 20, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  // 2. Lever la jambe gauche + abaisser le pied gauche (prise d'appui)
  moveSlowDuo(L_LEG,  pos_L_LEG,  90 + OFFSET_L_LEG  + 20,
              L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT - 25, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
  // 3. Redescendre la jambe gauche + remonter le pied gauche (poussée → glisse à droite)
  moveSlowDuo(L_LEG,  pos_L_LEG,  90 + OFFSET_L_LEG,
              L_FOOT, pos_L_FOOT, 90 + OFFSET_L_FOOT, sp.stepDelay);
  RemoteXY_delay(sp.pauseDelay);
}

// ================== VIRAGES ==================
void virageGauche() {
  pasDroit (10, 55, 25);   // L_LEG petit pas, R_LEG grand pas
  pasGauche(55, 10, 25);   // R_LEG petit pas, L_LEG grand pas
}

void virageDroit() {
  pasDroit (55, 10, 25);   // L_LEG grand pas, R_LEG petit pas
  pasGauche(10, 55, 25);   // R_LEG grand pas, L_LEG petit pas
}

// ================== SETUP ==================
void setup() {
  RemoteXY_Init();
  Serial.begin(115200);
  pinMode(PIN_BTN_DANCE, INPUT_PULLUP);
  pinMode(PIN_BTN_MUSIC, INPUT_PULLUP);
  L_LEG.attach(D7);
  L_FOOT.attach(D8);
  R_FOOT.attach(D9);
  R_LEG.attach(D10);
  randomSeed(analogRead(0));
  RemoteXY_delay(500);
  initialisation();
  RemoteXY_delay(2000);
}

// ================== LOOP ==================
void loop() {
  RemoteXY_Handler();
  updateMusic();

  // --- D4 physique : toggle musique seule ---
  bool btn_music_now = (digitalRead(PIN_BTN_MUSIC) == LOW);
  if (btn_music_now && !btn_music_prev) {
    if (musicOn) musicStop();
    else         musicStart();
  }
  btn_music_prev = btn_music_now;

  // --- Position G (6) OU D3 physique : dance + musique en boucle ---
  if (danceRunning()) {
    if (!musicOn) musicStart();
    playDance();
    if (!danceRunning()) musicStop();
    return;
  } else {
    if (musicOn) musicStop();
  }

  // --- Déplacement joystick ---
  if (RemoteXY.joystick_01_y > 30 && abs(RemoteXY.joystick_01_x) <= 30) {
    if (RemoteXY.button_02) {
      virageGauche();
    }
    else if (RemoteXY.button_01) {
      virageDroit();
    }
    else {
      pasDroit (40, 40, 25);
      pasGauche(40, 40, 25);
    }
  }
  else if (RemoteXY.joystick_01_y < -30 && abs(RemoteXY.joystick_01_x) <= 30) {
    pasArriereDroit (40, 25);
    pasArriereGauche(40, 25);
  }
  else if (RemoteXY.joystick_01_x < -30 && abs(RemoteXY.joystick_01_y) <= 30) {
    crabeGauche();
  }
  else if (RemoteXY.joystick_01_x > 30 && abs(RemoteXY.joystick_01_y) <= 30) {
    crabeDroit();
  }
  else {
    initialisation();
  }
}
