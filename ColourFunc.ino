void StaticColour(String colour, int panels[] = { 999 })
{
	for (int i = 0; i < sizeof(panels)/sizeof(panels[0]); i++)
	{
		Serial.println(sprintf("%03d COL %d", panels[i], colour));
	}
}
