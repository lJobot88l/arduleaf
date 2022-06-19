void Identnext()
{
	while (Serial.read() != id && !isMaster)
	{
		Serial.read();
	} // Waits until master has ack'd this panel's ID

	// Panel A:
	digitalWrite(AReqIdent, HIGH); // Request ident from panel at A
	int s = millis();
	bool success = true;
	while (digitalRead(AFB) == LOW) // Wait for ident feedback at A (timeout of 1 second)
	{
		if (millis() - s > 1000)
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
			Serial.write(id);
		}
	}

	// Panel B:
	digitalWrite(BReqIdent, HIGH); // Request ident from panel at B
	s = millis();
	success = true;
	while (digitalRead(BFB) == LOW) // Wait for ident feedback at B (timeout of 1 second)
	{
		if (millis() - s > 1000)
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
			Serial.write(id);
		}
	}
}

void Ident()
{
	while (digitalRead(VReqIdent) == LOW)
	{
		Serial.read();
	}
	if (digitalRead(VReqIdent) == HIGH && !startident)
	{
		startident = true;
		digitalWrite(VFB, HIGH);
		id = Serial.read();
		digitalWrite(VFB, LOW);
		Identnext();
	}
}
