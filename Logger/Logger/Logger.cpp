#include "Logger.h"

using namespace std;

Logger* Logger::instance = NULL;

void Logger::log(const string& text, Format format)
{
	int pColor = getCurrentColor();
	switch (format)
	{
		case PRIMARY:
			changeColor(15);
			break;
		case SECONDARY:
			changeColor(8);
			break;
		case FAILURE:
			changeColor(12);
			break;
		case WARNING:
			changeColor(14);
			break;
		case SUCCESS:
			changeColor(2);
			break;
		default:
			break;
	}
	cout << text;
	changeColor(pColor);
}

void Logger::logLine(const string& text, Format format)
{
	log(text, format);
	cout << endl;
}

void Logger::log(const string& text, const string& ip, Format format)
{
	cout << endl;
	log(text, format);
	cout << "\n" << ip << "> ";	// Ip will be "" if empty
}

void Logger::logColor(const string& text, int color)
{
	int pColor = getCurrentColor();
	changeColor(color);
	cout << text;
	changeColor(pColor);
}

void Logger::logColorLine(const string& text, int color)
{
	logColor(text, color);
	cout << endl;
}

int Logger::getCurrentColor() const
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole, &info);
	return info.wAttributes;
}

bool Logger::changeColor(int color) const
{
	if (SetConsoleTextAttribute(hConsole, color)) return true;
	return false;
}

Logger::Logger()
{
	cout << "Logger Started" << endl;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

