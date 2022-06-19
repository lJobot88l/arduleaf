bool startident = false;
bool isMaster = true;

int AReqIdent = 2; // Output to request the next modules identification (at output A)
int AFB = 3;       // Input for ident feedback (from output A)

int BReqIdent = 4;  // See above
int BFB = 5;

int VReqIdent = 6; // Input for ident request on this panel
int VFB = 7;       // Output for ident feedback from this panel

int R = 6, G = 7, B = 13; // Outputs for RGB

int id;             // This panels ID
int panelcount = 1; // Number of panels in the system

void setup() 
{
    Serial.begin(9600);
    pinMode(AReqIdent, OUTPUT);
    pinMode(AFB, INPUT);
    pinMode(BReqIdent, OUTPUT);
    pinMode(BFB, INPUT);
    if(Serial.print("Check if Master") == 0)
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
                Serial.print(printf("%03d", panelcount));
                panelcount++;
                hasVerifiedA = true;
                s = millis();
            }
            if (digitalRead(BFB) == HIGH && !hasVerifiedB) // Waits for feedback from output B
            {
                Serial.print(printf("%03d", panelcount));
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
                Serial.print("Timeout");
                s = millis();
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
    if(isMaster)
    {
        String out = Serial.readString();
        char address[3];
        sprintf(address, "%03d", id);
        if(out.startsWith(address))
        {
            out = out.substring(4);
            if(out.substring(0, 3) == "RNB")
            {
            }
            else if(out.substring(0, 3) == "COL")
            { // Set static colour, format: ID COL 000000

                //StaticColour(out.substring(4,6));
            }
        }
    }
    else
    {
        String out = Serial.readString();
        char address[3];
        sprintf(address, "%03s", out.substring(3));
        if(out.startsWith(address) || out.startsWith("999"))
        {   // ID  CMD DATA
            // 001 COL 00FF00
            out = out.substring(4); // The id is 3 digits long
            if(out.substring(0, 3 == "COL"))
            {
                char Rc[2];
                char Gc[2];
                char Bc[2];
                sprintf(R, "%03s", out.substring(4, 2));
                sprintf(G, "%03s", out.substring(6, 2));
                sprintf(B, "%03s", out.substring(8, 2));
                analogWrite(R, strtol(Rc, 0, 16));
                analogWrite(G, strtol(Gc, 0, 16));
                analogWrite(B, strtol(Bc, 0, 16));
            }
        }
    }
}
