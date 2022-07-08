bool startident = false;
bool isMaster = true;

int AReqIdent = 2; // Output to request the next modules identification (at output A)
int AFB = 4;       // Input for ident feedback (from output A)

int BReqIdent = 7;  // See above
int BFB = 8;

int VReqIdent = 12; // Input for ident request on this panel
int VFB = 13;       // Output for ident feedback from this panel

int R = 3, G = 5, B = 6; // Outputs for RGB

int id;             // This panels ID
int panelcount = 1; // Number of panels in the system

void setup() 
{
    Serial.begin(9600);
    pinMode(AReqIdent, OUTPUT);
    pinMode(AFB, INPUT);
    pinMode(BReqIdent, OUTPUT);
    pinMode(BFB, INPUT);
    pinMode(VReqIdent, INPUT);
    pinMode(VFB, OUTPUT);
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);

    digitalWrite(AReqIdent, LOW); // Turns off all output pins bc that apparently does not reset (reee)
    digitalWrite(BReqIdent, LOW);
    digitalWrite(VFB, LOW);
    digitalWrite(R, LOW);
    digitalWrite(G, LOW);
    digitalWrite(B, LOW);
    if(Serial.write(1) != 0)
    {
        id = 0;
        Identnext();
        bool hasVerifiedA = false;
        bool hasVerifiedB = false;
        int s = millis();
        while (true) // Sends ping if new panel registered
        {
            if (digitalRead(AFB) == HIGH && !hasVerifiedA) // Waits for feedback from output A
            {
                Serial.write(panelcount);
                panelcount++;
                hasVerifiedA = true;
                s = millis();
            }
            if (digitalRead(BFB) == HIGH && !hasVerifiedB) // Waits for feedback from output B
            {
                Serial.write(panelcount);
                panelcount++;
                hasVerifiedB = true;
                s = millis();
            }

            if (digitalRead(AFB) == LOW && hasVerifiedA) // Once signal turns off new panel can be registered
            {
                hasVerifiedA = false;
            }
            if (digitalRead(BFB) == LOW && hasVerifiedB)
            {
                hasVerifiedB = false;
            }

            if(millis() - s > 1000)
            {
                break; // Timeout
            }
        }
    } 
    else
    {
        isMaster = false;
        

        Ident();
        bool wasPingedfromA = false;
        bool wasPingedfromB = false;
        int s = millis();
        while(true) // Sends ping if new panel registered
        {
            if(digitalRead(AFB) == HIGH && !wasPingedfromA)
            {
                digitalWrite(VFB, HIGH);
                wasPingedfromA = true;
            }
            if(digitalRead(BFB) == HIGH && !wasPingedfromB) // Waits for feedback from both outputs and forwards them to master
            {
                digitalWrite(VFB, HIGH);
                wasPingedfromB = true;
            }

            if(digitalRead(AFB) == LOW && wasPingedfromA) // Forwards FB signal turning off
            {
                wasPingedfromA = false;
                digitalWrite(VFB, LOW);
            }
            if (digitalRead(BFB) == LOW && wasPingedfromB)
            {
                wasPingedfromB = false;
                digitalWrite(VFB, LOW);
            }
            if (millis() - s > 1000)
            {
                break; // Timeout
            }
        }
    }

    // Test
    if(id == 0)
    {
        digitalWrite(R, HIGH);
    }
    if (id == 1)
    {
        digitalWrite(G, HIGH);
    }

}

void loop() 
{
    if(Serial.available() == 0)
    {
        byte bytes[64];
        Serial.readBytes(bytes, 64);
        if((int)bytes[0] == id)
        {
            if(bytes[1] == 0x01)
            {
            }
            else if(bytes[1] == 0x02)
            { // Set static colour, format: ID COL 000000
                analogWrite(R, bytes[2]);
                analogWrite(G, bytes[3]);
                analogWrite(B, bytes[4]);
            }
        }
    }
}
