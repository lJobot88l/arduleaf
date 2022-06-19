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
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);
    if(Serial.write(0) != 0)
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

            if (digitalRead(AFB) == LOW && hasVerifiedA) // Waits for feedback from this panel
            {
                hasVerifiedA = false;
            }
            if (digitalRead(BFB) == LOW && hasVerifiedB)
            {
                hasVerifiedB = false;
            }

            if(millis() - s > 1000)
            {
                break;
            }
        }
    } 
    else
    {
        isMaster = false;
        pinMode(VReqIdent, INPUT);
        pinMode(VFB, OUTPUT);
        Ident();
        while(true) // Sends ping if new panel registered
        {
            if(digitalRead(AFB) == HIGH || digitalRead(BFB) == HIGH) // Waits for feedback from output A
            {
                digitalWrite(VFB, HIGH);
                break;
            }
        }
    }
}

void loop() 
{
    if(Serial.available())
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
                digitalWrite(13, HIGH);
            }
        }
    }
}
