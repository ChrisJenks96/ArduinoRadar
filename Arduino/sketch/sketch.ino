const int triggerPin = 11;
const int echoPin = 12;

unsigned long distance;

void setup()
{
    //set the trigger pin as output
    pinMode(triggerPin, OUTPUT);
    //set the echo pin as input
    pinMode(echoPin, INPUT);
    //USB communication to PC
    Serial.begin(9600);
}

void loop()
{
    //send a LOW value to ensure clean trigger on high
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    
    //Set the trigger pin to HIGH value for 10micros
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    //reset back to LOW once 10micros burst is done
    digitalWrite(triggerPin, LOW);

    //read the receiving echo pin to see if anything is returned
    distance = pulseIn(echoPin, HIGH);
    //Write Serial out to PC
    Serial.print("Distance ");
    Serial.println(distance);
}
