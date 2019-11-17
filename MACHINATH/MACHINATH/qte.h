#pragma once

// qte types
enum QTE
{
	QTE_DEFAULT,
	QTE_LONGPRESS,
	QTE_MULTIPRESS
};

// call to start qte event
void StartQTE(QTE type);

void InitQTE();
void UninitQTE();
void UpdateQTE();
void DrawQTE();
