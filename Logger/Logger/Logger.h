/*

Logger.h
Custom built logging library for Dino

*/

#pragma once

#ifndef GAURD_LOGGER_H
#define GAURD_LOGGER_H

#include <iostream>
#include <string>

#include <Windows.h>

enum Format {
	PRIMARY, SECONDARY, FAILURE, WARNING, SUCCESS
};

class Logger
{

public:
	static Logger& getInstance() { if (instance == 0) instance = new Logger(); return *instance; }

	// Log with certain format
	void log(const std::string& text, Format format=PRIMARY);
	void logLine(const std::string& text, Format format=PRIMARY);

	// Log and maintain ip with format (Dino specific)
	void log(const std::string& text, const std::string& ip, Format format=PRIMARY);

	// Log with certain color
	void logColor(const std::string& text, int color);
	void logColorLine(const std::string& text, int color);

private:
	Logger();
	static Logger* instance;

	HANDLE hConsole;

	bool changeColor(int color) const;
	int getCurrentColor() const;

};

#define LOGGER Logger::getInstance()

#endif
