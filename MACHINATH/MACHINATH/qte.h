#pragma once

// qte types
enum QTE
{
	QTE_DEFAULT,
	QTE_MULTIPRESS
};

// call to start qte event
void StartQTE(QTE type);

// returns true if qte is currently active
bool IsQTEActive();

void InitQTE();
void UninitQTE();
void UpdateQTE();
void DrawQTE();
