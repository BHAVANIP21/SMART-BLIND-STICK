#define trigF 2
#define echoF 3

#define trigL 4
#define echoL 5

#define trigR 6
#define echoR 7

#define buzzer 8

long duration;
int distF, distL, distR;

int getDistance(int trigPin, int echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;

    return distance;
}

void setup()
{
    pinMode(trigF, OUTPUT);
    pinMode(echoF, INPUT);

    pinMode(trigL, OUTPUT);
    pinMode(echoL, INPUT);

    pinMode(trigR, OUTPUT);
    pinMode(echoR, INPUT);

    pinMode(buzzer, OUTPUT);

    Serial.begin(9600);
}

void loop()
{
    distF = getDistance(trigF, echoF);
    distL = getDistance(trigL, echoL);
    distR = getDistance(trigR, echoR);

    Serial.print("Front: ");
    Serial.print(distF);
    Serial.print(" | Left: ");
    Serial.print(distL);
    Serial.print(" | Right: ");
    Serial.println(distR);

    // Threshold distance (cm)
    int threshold = 30;

    // FRONT ALERT (continuous beep)
    if (distF > 0 && distF < threshold)
    {
        digitalWrite(buzzer, HIGH);
        delay(300);
        digitalWrite(buzzer, LOW);
        delay(300);
    }
    // LEFT ALERT (slow beep)
    else if (distL > 0 && distL < threshold)
    {
        digitalWrite(buzzer, HIGH);
        delay(700);
        digitalWrite(buzzer, LOW);
        delay(700);
    }
    // RIGHT ALERT (fast beep)
    else if (distR > 0 && distR < threshold)
    {
        digitalWrite(buzzer, HIGH);
        delay(150);
        digitalWrite(buzzer, LOW);
        delay(150);
    }
    else
    {
        digitalWrite(buzzer, LOW);
    }
}