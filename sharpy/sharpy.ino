#define DEBUG false

// Digital Inputs
#define BTN_START 2
#define BTN_STOP 3

// Analog Inputs
#define POT_F1 0
#define POT_P1 1
#define POT_F2 2
#define POT_P2 3

// Digital Outputs
#define RELAY1 4
#define RELAY2 5
#define LED_MAIN 6
#define LED_STOP 7
#define LED_F1 8
#define LED_P1 9
#define LED_F2_1 10
#define LED_F2_2 11
#define LED_F2_3 12
#define LED_P2 13

// Delays
#define POT_MAX_MS 10000
#define BASE_DELAY_MS 50

// Phases
#define PHASE_F1 0
#define PHASE_P1 1
#define PHASE_F2_1 2
#define PHASE_F2_2 3
#define PHASE_F2_3 4
#define PHASE_P2_1 5
#define PHASE_P2_2 6
#define PHASE_P2_3 7

#if DEBUG
bool isOn = true;
#else
bool isOn = false;
#endif
int currentDelay = 0;
int currentPhase = PHASE_F1;

int getDelayInMs(int pinNumber) {
  int value = analogRead(pinNumber);
  int delay = POT_MAX_MS * (double)value / 1023;
  if (delay < 1000) {
    return 1000;
  }
  return delay;
}

int getDelayInMsByPhase() {
  switch (currentPhase) {
    case PHASE_F1:
      return getDelayInMs(POT_F1);
    case PHASE_P1:
      return  getDelayInMs(POT_P1);
    case PHASE_F2_1: 
    case PHASE_F2_2:
    case PHASE_F2_3:
      return getDelayInMs(POT_F2);
    case PHASE_P2_1:
    case PHASE_P2_2:
    case PHASE_P2_3:
      return getDelayInMs(POT_P2);
    default:
      return 0;
  }
}

void setup() {
  #if DEBUG
  Serial.begin(9600);
  #endif

  pinMode(BTN_START, INPUT);
  pinMode(BTN_STOP, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(LED_MAIN, OUTPUT);
  pinMode(LED_STOP, OUTPUT);
  pinMode(LED_F1, OUTPUT);
  pinMode(LED_P1, OUTPUT);
  pinMode(LED_F2_1, OUTPUT);
  pinMode(LED_F2_2, OUTPUT);
  pinMode(LED_F2_3, OUTPUT);
  pinMode(LED_P2, OUTPUT);

  digitalWrite(LED_MAIN, HIGH);
  if (!isOn) {
    digitalWrite(LED_STOP, HIGH);
  }
}

void loop() {
  int btnStartValue = digitalRead(BTN_START);
  int btnStopValue = digitalRead(BTN_STOP);
  if (btnStartValue == HIGH && btnStopValue != HIGH) {
    isOn = true;
    currentDelay = 0;
    currentPhase = PHASE_F1;
    digitalWrite(LED_STOP, LOW);
    digitalWrite(LED_F1, HIGH);
    digitalWrite(RELAY1, HIGH);
  }
  if (btnStopValue == HIGH) {
    isOn = false;
    digitalWrite(LED_STOP, HIGH);
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
    int delay = getDelayInMsByPhase();
    if (currentDelay >= delay) {
      currentDelay = 0;
      switch (currentPhase) {
        case PHASE_F1:
          currentPhase = PHASE_P1;
          digitalWrite(LED_F1, LOW);
          digitalWrite(RELAY1, LOW);
          digitalWrite(LED_P1, HIGH);
          break;
        case PHASE_P1:
          currentPhase = PHASE_F2_1;
          digitalWrite(LED_P1, LOW);
          digitalWrite(LED_F2_1, HIGH);
          digitalWrite(RELAY2, HIGH);
          break;
        case PHASE_F2_1:
          currentPhase = PHASE_P2_1;
          digitalWrite(LED_F2_1, LOW);
          digitalWrite(LED_P2, HIGH);
          digitalWrite(RELAY2, LOW);
          break;
        case PHASE_P2_1:
          currentPhase = PHASE_F2_2;
          digitalWrite(LED_P2, LOW);
          digitalWrite(LED_F2_2, HIGH);
          digitalWrite(RELAY2, HIGH);
          break;
        case PHASE_F2_2:
          currentPhase = PHASE_P2_2;
          digitalWrite(LED_F2_2, LOW);
          digitalWrite(LED_P2, HIGH);
          digitalWrite(RELAY2, LOW);
          break;
        case PHASE_P2_2:
          currentPhase = PHASE_F2_3;
          digitalWrite(LED_P2, LOW);
          digitalWrite(LED_F2_3, HIGH);
          digitalWrite(RELAY2, HIGH);
          break;
        case PHASE_F2_3:
          currentPhase = PHASE_P2_3;
          digitalWrite(LED_F2_3, LOW);
          digitalWrite(LED_P2, HIGH);
          digitalWrite(RELAY2, LOW);
          break;
        case PHASE_P2_3:
          currentPhase = PHASE_F1;
          digitalWrite(LED_P2, LOW);
          digitalWrite(LED_F1, HIGH);
          digitalWrite(RELAY1, HIGH);
          break;
      }
    }
    #if DEBUG
    Serial.print("phase: ");
    Serial.println(currentPhase);
    Serial.print("delay: ");
    Serial.println(currentDelay);
    #endif
    currentDelay += BASE_DELAY_MS;
  }
  delay(BASE_DELAY_MS);
}
