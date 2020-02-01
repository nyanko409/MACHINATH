#pragma once

// qte types
enum QTE
{
	QTE_DEFAULT,
	QTE_MULTIPRESS
};

// call to start qte event, returns true if success
bool StartQTE(QTE type);

// returns true if qte is currently active
bool IsQTEActive();

// returns the slowmotion factor
float getSlowmoFactor();

void InitQTE();
void UninitQTE();
void UpdateQTE();
void DrawQTE();
