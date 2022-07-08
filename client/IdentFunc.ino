void Identnext()
{
	// Panel A:
	digitalWrite(AReqIdent, HIGH); // Request ident from panel at A
	int s = millis();
	bool success = true;
	while (digitalRead(AFB) == LOW) // Wait for ident feedback at A (timeout of .1 s)
	{
		if (millis() - s > 100)
		{
			success = false;
			break;
		}
	}
	digitalWrite(AReqIdent, LOW); // Stop requesting ident from panel at A
	if (success)
	{
		if (isMaster)
		{
			Serial.write(panelcount);
			panelcount++;
		}
		else
		{
			digitalWrite(VFB, HIGH);

			while (Serial.available() == 0)
			{
			}
			digitalWrite(VFB, LOW);
		}
	}

	// Panel B:
	digitalWrite(BReqIdent, HIGH); // Request ident from panel at B
	s = millis();
	success = true;
	while (digitalRead(BFB) == LOW) // Wait for ident feedback at B (timeout of .1 second)
	{
		if (millis() - s > 100)
		{
			success = false;
			break;
		}
	}
	digitalWrite(BReqIdent, LOW); // Stop requesting ident from panel at B
	if (success)
	{
		if (isMaster)
		{
			Serial.write(panelcount);
			panelcount++;
		}
		else
		{
			digitalWrite(VFB, HIGH);

			while(Serial.available() == 0) {} // Waits for ack from master
			digitalWrite(VFB, LOW);
		}
	}
}

void Ident()
{
	while (digitalRead(VReqIdent) == LOW)
	{
		Serial.read(); // constantly clears buffer while waiting for ident
	}
	if (digitalRead(VReqIdent) == HIGH && !startident)
	{
		startident = true;
		digitalWrite(VFB, HIGH);

		while(Serial.available() == 0) {}
		id = Serial.read();
		digitalWrite(VFB, LOW);
		Identnext();
	}
}
