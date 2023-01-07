#define BTN_START 2
#define BTN_STOP 3

#define RELAY1 4
#define RELAY2 5
#define LED_MAIN 6
#define LED_RUN 7
#define LED_F1 8
#define LED_P1 9
#define LED_F2_1 10
#define LED_F2_2 11
#define LED_F2_3 12
#define LED_P2 13

#define POT_F1 0
#define POT_P1 1
#define POT_F2 2
#define POT_P2 3

#define POT_MAX_MS 10000
#define BASE_DELAY 50

#define PHASE_F1 0
#define PHASE_P1 1
#define PHASE_F2_1 2
#define PHASE_F2_2 3
#define PHASE_F2_3 4
#define PHASE_P2_1 5
#define PHASE_P2_2 6
#define PHASE_P2_3 7

bool isOn = true;
int currentDelay = 0;
int phase = PHASE_F1;

int getDurationInMs(int pinNumber) {
  int value = analogRead(pinNumber);
  return POT_MAX_MS * (double)value / 1023;
}

int getDelayByPhase() {
  switch (phase) {
    case PHASE_F1: return getDurationInMs(POT_F1);
    case PHASE_P1: return  getDurationInMs(POT_P1);
    case PHASE_F2_1: 
    case PHASE_F2_2:
    case PHASE_F2_3:
      return getDurationInMs(POT_F2);
    case PHASE_P2_1:
    case PHASE_P2_2:
    case PHASE_P2_3:
      return getDurationInMs(POT_P2);
    default: return 0;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(BTN_START, INPUT);
  pinMode(BTN_STOP, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(LED_MAIN, OUTPUT);
  pinMode(LED_RUN, OUTPUT);
  pinMode(LED_F1, OUTPUT);
  pinMode(LED_P1, OUTPUT);
  pinMode(LED_F2_1, OUTPUT);
  pinMode(LED_F2_2, OUTPUT);
  pinMode(LED_F2_3, OUTPUT);
  pinMode(LED_P2, OUTPUT);

  digitalWrite(LED_MAIN, HIGH);
}

void loop() {
  if (digitalRead(BTN_START) == HIGH) {
    isOn = true;
    digitalWrite(LED_RUN, HIGH);
    currentDelay = 0;
    phase = PHASE_F1;
    digitalWrite(LED_F1, HIGH);
    digitalWrite(RELAY1, HIGH);
  }
  if (digitalRead(BTN_STOP) == HIGH) {
    isOn = false;
    digitalWrite(LED_RUN, LOW);
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
    digitalWrite(LED_F1, LOW);
    digitalWrite(LED_P1, LOW);
    digitalWrite(LED_F2_1, LOW);
    digitalWrite(LED_F2_2, LOW);
    digitalWrite(LED_F2_3, LOW);
    digitalWrite(LED_P2, LOW);
  }

  if (isOn) {
    int delay = getDelayByPhase();
    if (currentDelay >= delay) {
      currentDelay = 0;
      switch (phase) {
        case PHASE_F1:
          phase = PHASE_P1;
          digitalWrite(LED_F1, LOW);
          digitalWrite(RELAY1, LOW);
          digitalWrite(LED_P1, HIGH);
          break;
        case PHASE_P1:
          phase = PHASE_F2_1;
          digitalWrite(LED_P1, LOW);
          digitalWrite(LED_F2_1, HIGH);
          digitalWrite(RELAY2, HIGH);
          break;
        case PHASE_F2_1:
          phase = PHASE_P2_1;
          digitalWrite(LED_F2_1, LOW);
          digitalWrite(LED_P2, HIGH);
          digitalWrite(RELAY2, LOW);
          break;
        case PHASE_P2_1:
          phase = PHASE_F2_2;
          digitalWrite(LED_P2, LOW);
          digitalWrite(LED_F2_2, HIGH);
          digitalWrite(RELAY2, HIGH);
          break;
        case PHASE_F2_2:
          phase = PHASE_P2_2;
          digitalWrite(LED_F2_2, LOW);
          digitalWrite(LED_P2, HIGH);
          digitalWrite(RELAY2, LOW);
          break;
        case PHASE_P2_2:
          phase = PHASE_F2_3;
          digitalWrite(LED_P2, LOW);
          digitalWrite(LED_F2_3, HIGH);
          digitalWrite(RELAY2, HIGH);
          break;
        case PHASE_F2_3:
          phase = PHASE_P2_3;
          digitalWrite(LED_F2_3, LOW);
          digitalWrite(LED_P2, HIGH);
          digitalWrite(RELAY2, LOW);
          break;
        case PHASE_P2_3:
          phase = PHASE_F1;
          digitalWrite(LED_P2, LOW);
          digitalWrite(LED_F1, HIGH);
          digitalWrite(RELAY1, HIGH);
          break;
      }
    }
    Serial.print('f');
    Serial.println(delay);
    Serial.print('c');
    Serial.println(currentDelay);
    currentDelay += BASE_DELAY;
  }
  delay(BASE_DELAY);
}
