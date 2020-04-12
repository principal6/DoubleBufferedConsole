#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>

enum class EArrowKeys
{
	None,
	Up,
	Left,
	Right,
	Down
};

enum class EBackgroundColor
{
	Black = 0,
	DarkGray = BACKGROUND_INTENSITY,
	LightGray = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	White = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,

	Red = BACKGROUND_RED,
	Green = BACKGROUND_GREEN,
	Blue = BACKGROUND_BLUE,
	Cyan = BACKGROUND_GREEN | BACKGROUND_BLUE,
	Magenta = BACKGROUND_RED | BACKGROUND_BLUE,
	Yellow = BACKGROUND_RED | BACKGROUND_GREEN,

	LightRed = BACKGROUND_INTENSITY | BACKGROUND_RED,
	LightGreen = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	LightBlue = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	LightCyan = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
	LightMagenta = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	LightYellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
};

enum class EForegroundColor
{
	Black = 0,
	DarkGray = FOREGROUND_INTENSITY,
	LightGray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	White = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

	Red = FOREGROUND_RED,
	Green = FOREGROUND_GREEN,
	Blue = FOREGROUND_BLUE,
	Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
	Yellow = FOREGROUND_RED | FOREGROUND_GREEN,

	LightRed = FOREGROUND_INTENSITY | FOREGROUND_RED,
	LightGreen = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	LightBlue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	LightCyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	LightMagenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	LightYellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
};

class CDoubleBufferedConsole
{
public:
	CDoubleBufferedConsole(short Width, short Height, const char* Title) : m_Width{ Width }, m_Height{ Height }
	{ 
		StartUp(Title);
	}
	~CDoubleBufferedConsole() 
	{
		CleanUp();
	}

public:
	void Clear()
	{
		COORD Coord{ 0, 0 };
		DWORD Written{};
		WORD Attribute{ (WORD)((WORD)m_eClearBackground | (WORD)m_eDefaultForeground) };
		for (short Y = 0; Y < m_Height; ++Y)
		{
			Coord.Y = Y;
			FillConsoleOutputCharacterA(m_BackBuffer, ' ', m_Width, Coord, &Written);
			FillConsoleOutputAttribute(m_BackBuffer, Attribute, m_Width, Coord, &Written);
		}
	}

public:
	void PrintChar(short X, short Y, char Char, WORD Attribute)
	{
		DWORD Written{};
		COORD Coord{ X, Y };
		FillConsoleOutputCharacterA(m_BackBuffer, Char, 1, Coord, &Written);
		FillConsoleOutputAttribute(m_BackBuffer, Attribute, 1, Coord, &Written);
	}

	void PrintChar(short X, short Y, char Char, EBackgroundColor eBackground, EForegroundColor eForeground)
	{
		PrintChar(X, Y, Char, GetNewAttribute(eBackground, eForeground));
	}

	void PrintChar(short X, short Y, char Char, EBackgroundColor eBackground)
	{
		PrintChar(X, Y, Char, GetPatchedAttribute(X, Y, eBackground));
	}

	void PrintChar(short X, short Y, char Char, EForegroundColor eForeground)
	{
		PrintChar(X, Y, Char, GetPatchedAttribute(X, Y, eForeground));
	}

	void PrintChar(short X, short Y, char Char)
	{
		PrintChar(X, Y, Char, GetCurrentAttribute(X, Y));
	}

public:
	void PrintHString(short X, short Y, const char* String, WORD Attribute, int StringLength = -1)
	{
		if (StringLength == -1) StringLength = (int)strlen(String);

		DWORD Written{};
		COORD Coord{ X, Y };
		WriteConsoleOutputCharacterA(m_BackBuffer, String, StringLength, Coord, &Written);
		FillConsoleOutputAttribute(m_BackBuffer, Attribute, StringLength, Coord, &Written);
	}

	void PrintHString(short X, short Y, const char* String, EBackgroundColor eBackground, EForegroundColor eForeground, int StringLength = -1)
	{
		PrintHString(X, Y, String, GetNewAttribute(eBackground, eForeground), StringLength);
	}

	void PrintHString(short X, short Y, const char* String, EBackgroundColor eBackground, int StringLength = -1)
	{
		PrintHString(X, Y, String, GetPatchedAttribute(X, Y, eBackground), StringLength);
	}

	void PrintHString(short X, short Y, const char* String, EForegroundColor eForeground, int StringLength = -1)
	{
		PrintHString(X, Y, String, GetPatchedAttribute(X, Y, eForeground), StringLength);
	}

	void PrintHString(short X, short Y, const char* String, int StringLength = -1)
	{
		PrintHString(X, Y, String, GetCurrentAttribute(X, Y), StringLength);
	}

	void PrintHString(short X, short Y, const std::string& String)
	{
		PrintHString(X, Y, String.c_str(), (int)String.size());
	}

	void PrintHString(short X, short Y, short Short)
	{
		std::string String{ std::to_string((int)Short) };
		PrintHString(X, Y, String.c_str(), (int)String.size());
	}

public:
	void PrintVString(short X, short Y, const char* String, WORD Attribute, int StringLength = -1)
	{
		if (StringLength == -1) StringLength = (int)strlen(String);

		DWORD Written{};
		COORD Coord{ X, Y };
		for (int i = 0; i < StringLength; ++i)
		{
			Coord.Y = Y + i;
			WriteConsoleOutputCharacterA(m_BackBuffer, &String[i], 1, Coord, &Written);
			FillConsoleOutputAttribute(m_BackBuffer, Attribute, 1, Coord, &Written);
		}
	}

	void PrintVString(short X, short Y, const char* String, EBackgroundColor eBackground, EForegroundColor eForeground, int StringLength = -1)
	{
		PrintVString(X, Y, String, GetNewAttribute(eBackground, eForeground), StringLength);
	}

	void PrintVString(short X, short Y, const char* String, EBackgroundColor eBackground, int StringLength = -1)
	{
		PrintVString(X, Y, String, GetPatchedAttribute(X, Y, eBackground), StringLength);
	}

	void PrintVString(short X, short Y, const char* String, EForegroundColor eForeground, int StringLength = -1)
	{
		PrintVString(X, Y, String, GetPatchedAttribute(X, Y, eForeground), StringLength);
	}

	void PrintVString(short X, short Y, const char* String, int StringLength = -1)
	{
		PrintVString(X, Y, String, GetCurrentAttribute(X, Y), StringLength);
	}

public:
	void FillHChar(short X, short Y, char Char, int Count, WORD Attribute)
	{
		DWORD Written{};
		COORD Coord{ X, Y };
		FillConsoleOutputCharacterA(m_BackBuffer, Char, Count, Coord, &Written);
		FillConsoleOutputAttribute(m_BackBuffer, Attribute, Count, Coord, &Written);
	}

	void FillHChar(short X, short Y, char Char, int Count, EBackgroundColor eBackground, EForegroundColor eForeground)
	{
		FillHChar(X, Y, Char, Count, GetNewAttribute(eBackground, eForeground));
	}

	void FillHChar(short X, short Y, char Char, int Count, EBackgroundColor eBackground)
	{
		FillHChar(X, Y, Char, Count, GetPatchedAttribute(X, Y, eBackground));
	}

	void FillHChar(short X, short Y, char Char, int Count, EForegroundColor eForeground)
	{
		FillHChar(X, Y, Char, Count, GetPatchedAttribute(X, Y, eForeground));
	}

	void FillHChar(short X, short Y, char Char, int Count)
	{
		FillHChar(X, Y, Char, Count, GetCurrentAttribute(X, Y));
	}

public:
	void FillVChar(short X, short Y, char Char, int Count, WORD Attribute)
	{
		DWORD Written{};
		COORD Coord{ X, Y };
		for (int i = 0; i < Count; ++i)
		{
			Coord.Y = Y + i;
			FillConsoleOutputCharacterA(m_BackBuffer, Char, 1, Coord, &Written);
			FillConsoleOutputAttribute(m_BackBuffer, Attribute, 1, Coord, &Written);
		}
	}

	void FillVChar(short X, short Y, char Char, int Count, EBackgroundColor eBackground, EForegroundColor eForeground)
	{
		FillVChar(X, Y, Char, Count, GetNewAttribute(eBackground, eForeground));
	}

	void FillVChar(short X, short Y, char Char, int Count, EBackgroundColor eBackground)
	{
		FillVChar(X, Y, Char, Count, GetPatchedAttribute(X, Y, eBackground));
	}

	void FillVChar(short X, short Y, char Char, int Count, EForegroundColor eForeground)
	{
		FillVChar(X, Y, Char, Count, GetPatchedAttribute(X, Y, eForeground));
	}

	void FillVChar(short X, short Y, char Char, int Count)
	{
		FillVChar(X, Y, Char, Count, GetCurrentAttribute(X, Y));
	}

public:
	void PrintBox(short X, short Y, short Width, short Height, char Char, EBackgroundColor eBackground, EForegroundColor eForeground)
	{
		FillHChar(X, Y, Char, Width, eBackground, eForeground);
		FillHChar(X, Y + Height - 1, Char, Width, eBackground, eForeground);
		FillVChar(X, Y, Char, Height, eBackground, eForeground);
		FillVChar(X + Width - 1, Y, Char, Height, eBackground, eForeground);
	}

	void FillBox(short X, short Y, short Width, short Height, char Char, EBackgroundColor eBackground, EForegroundColor eForeground)
	{
		for (short i = 0; i < Height; ++i)
		{
			FillHChar(X, Y + i, Char, Width, eBackground, eForeground);
		}
	}

private:
	WORD GetCurrentAttribute(short X, short Y) const
	{
		WORD Attribute{};
		DWORD Count{};
		ReadConsoleOutputAttribute(m_BackBuffer, &Attribute, 1, COORD{ X, Y }, &Count);
		return Attribute;
	}

	WORD GetNewAttribute(EBackgroundColor eBackground, EForegroundColor eForeground) const
	{
		return (WORD)((WORD)eBackground | (WORD)eForeground);
	}

	WORD GetPatchedAttribute(short X, short Y, EBackgroundColor eBackground) const
	{
		return (WORD)((GetCurrentAttribute(X, Y) & 0x000F) + (WORD)eBackground);
	}

	WORD GetPatchedAttribute(short X, short Y, EForegroundColor eForeground) const
	{
		return (WORD)((GetCurrentAttribute(X, Y) & 0x00F0) + (WORD)eForeground);
	}

public:
	void Render()
	{
		SetConsoleActiveScreenBuffer(m_BackBuffer);
		std::swap(m_FrontBuffer, m_BackBuffer);
	}

public:
	void SetClearBackground(EBackgroundColor eBackground)
	{
		m_eClearBackground = eBackground;
	}

	void SetDefaultForeground(EForegroundColor eForeground)
	{
		m_eDefaultForeground = eForeground;
	}

public:
	bool HitKey()
	{
		if (GetAsyncKeyState(VK_LMENU) < 0 && GetAsyncKeyState(VK_RETURN) < 0)
		{
			Reset();
			return false;
		}

		if (_kbhit())
		{
			m_HitKey = _getch();

			if (m_HitKey == 224)
			{
				m_HitKey = 0;
				int Key{ _getch() }; // arrow keys (up 72, left 75, right 77, down 80)
				if (Key == 72) m_eHitArrowKey = EArrowKeys::Up;
				else if (Key == 75) m_eHitArrowKey = EArrowKeys::Left;
				else if (Key == 77) m_eHitArrowKey = EArrowKeys::Right;
				else if (Key == 80) m_eHitArrowKey = EArrowKeys::Down;
				else m_eHitArrowKey = EArrowKeys::None;
			}
			else
			{
				m_eHitArrowKey = EArrowKeys::None;
			}

			return true;
		}
		m_HitKey = 0;
		return false;
	}

	bool IsHitKey(int KeyCode) const
	{
		return (m_HitKey == KeyCode);
	}

	bool IsHitKey(EArrowKeys ArrowKey) const
	{
		return (m_eHitArrowKey == ArrowKey);
	}

public:
	bool IsReadingCommand() const { return m_bReadingCommand; }
	bool ReadCommand()
	{
		// Initialize variables
		m_bReadingCommand = true;
		m_CommandReadBytes = 0; 
		int CurrentLogIndex{ m_CommandLogIndex };
		while (true)
		{
			if (_kbhit())
			{
				bool ShouldRead{ true };
				int Key{ _getch() };
				if (Key == 224)
				{
					int arrow{ _getch() };
					if (arrow == 72)
					{
						memset(m_CommandBuffer, 0, KCommandBufferSize);

						--CurrentLogIndex;
						if (CurrentLogIndex < 0) CurrentLogIndex = KCommandLogSize - 1;

						memcpy(m_CommandBuffer, m_CommandLog[CurrentLogIndex], strlen(m_CommandLog[CurrentLogIndex]));
						m_CommandReadBytes = (short)strlen(m_CommandBuffer);
						ShouldRead = false;
					}
					else if (arrow == 80)
					{
						memset(m_CommandBuffer, 0, KCommandBufferSize);

						++CurrentLogIndex;
						if (CurrentLogIndex >= KCommandLogSize) CurrentLogIndex = 0;

						memcpy(m_CommandBuffer, m_CommandLog[CurrentLogIndex], strlen(m_CommandLog[CurrentLogIndex]));
						m_CommandReadBytes = (short)strlen(m_CommandBuffer);
						ShouldRead = false;
					}
					else
					{
						continue;
					}
				}
				if (Key == VK_ESCAPE)
				{
					m_CommandReadBytes = 0;
					break;
				}
				if (Key == VK_RETURN)
				{
					break;
				}
				if (Key == VK_BACK)
				{
					if (m_CommandReadBytes)
					{
						if (m_CommandBuffer[m_CommandReadBytes - 1] < 0)
						{
							// Non-ASCII

							m_CommandBuffer[m_CommandReadBytes - 2] = 0;
							m_CommandBuffer[m_CommandReadBytes - 1] = 0;

							m_CommandReadBytes -= 2;
						}
						else
						{
							// ASCII

							--m_CommandReadBytes;
							m_CommandBuffer[m_CommandReadBytes] = 0;
						}
					}
					ShouldRead = false;
				}

				if (ShouldRead)
				{
					m_CommandBuffer[m_CommandReadBytes] = Key;
					++m_CommandReadBytes;
				}
			}
		}

		for (auto& ch : m_CommandBuffer)
		{
			if (ch == '\r') ch = 0;
			if (ch == '\n') ch = 0;
		}
		if (m_CommandReadBytes)
		{
			strcpy_s(m_CommandLog[m_CommandLogIndex], m_CommandBuffer);
			++m_CommandLogIndex;
			if (m_CommandLogIndex >= KCommandLogSize) m_CommandLogIndex = 0;
		}

		memset(m_CommandBuffer, 0, KCommandBufferSize); // Initialize
		m_bReadingCommand = false;
		return m_CommandReadBytes;
	}

	const char* GetLastCommand() const { return m_CommandLog[ (m_CommandLogIndex == 0) ? KCommandLogSize : m_CommandLogIndex - 1]; }

	bool IsLastCommand(const char* Cmp) const
	{
		if (!Cmp) return false;
		int Length{ (int)strlen(Cmp) };
		if (strncmp(GetLastCommand(), Cmp, Length) == 0)
		{
			return true;
		}
		return false;
	}

	// Print the command currently being typed onto back-buffer
	void PrintCommand(short X, short Y)
	{
		if (!IsReadingCommand()) return;

		COORD Coord{ X, Y };
		DWORD Written{};
		SetConsoleCursorPosition(m_BackBuffer, Coord);
		WriteConsoleOutputCharacterA(m_BackBuffer, "> ", 2, Coord, &Written);
		Coord.X += 2;

		WriteConsoleOutputCharacterA(m_BackBuffer, m_CommandBuffer, m_Width, Coord, &Written);
		SetConsoleCursorPosition(m_BackBuffer, COORD{ (short)(Coord.X + m_CommandReadBytes), Y });

		short CommandBufferSize{ (short)strlen(m_CommandBuffer) };
		wchar_t wsEnd[2]{ 0x25c4 };
		Coord.X += CommandBufferSize;
		WriteConsoleOutputCharacterW(m_BackBuffer, wsEnd, 1, Coord, &Written);
	}

	// Prints log bottom up
	// X, Y, Width, Height are outer-box size
	void PrintCommandLog(short X, short Y, short Width, short Height)
	{
		X += 1;
		Y += 1;
		Width -= 2;
		Height -= 2;

		short _Height{ min(min(KCommandLogSize, m_Height), Height) };
		for (short i = 0; i < _Height; ++i)
		{
			short CurrentLogIndex{ m_CommandLogIndex - 1 - i };
			if (CurrentLogIndex < 0) CurrentLogIndex += KCommandLogSize;
			short _Y{ max(Y + _Height - i - 1, Y) };

			PrintHString(X, _Y, m_CommandLog[CurrentLogIndex], m_eClearBackground, EForegroundColor::LightYellow,
				min(Width, (int)(strlen(m_CommandLog[CurrentLogIndex]))));
		}
	}

private:
	void StartUp(const char* Title)
	{
		CleanUp();

		m_FrontBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
		m_BackBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);

		SetConsoleTitleA(Title);

		Reset();

		m_bRunning = true;
	}

	void Reset()
	{
		CONSOLE_SCREEN_BUFFER_INFOEX BufferInfo{};
		BufferInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(m_FrontBuffer, &BufferInfo);
		BufferInfo.dwMaximumWindowSize.X = BufferInfo.srWindow.Right = BufferInfo.dwSize.X = m_Width;
		BufferInfo.dwMaximumWindowSize.Y = BufferInfo.srWindow.Bottom = BufferInfo.dwSize.Y = m_Height;
		SetConsoleScreenBufferInfoEx(m_FrontBuffer, &BufferInfo);
		SetConsoleScreenBufferInfoEx(m_BackBuffer, &BufferInfo);

		CONSOLE_CURSOR_INFO CursorInfo{ sizeof(CONSOLE_CURSOR_INFO), false };
		SetConsoleCursorInfo(m_FrontBuffer, &CursorInfo);
		SetConsoleCursorInfo(m_BackBuffer, &CursorInfo);
	}

	void CleanUp()
	{
		if (m_BackBuffer)
		{
			CloseHandle(m_BackBuffer);
			m_BackBuffer = nullptr;
		}
		if (m_FrontBuffer)
		{
			CloseHandle(m_FrontBuffer);
			m_FrontBuffer = nullptr;
		}
	}

public:
	void Terminate() { m_bRunning = false; }
	bool IsTerminated() const { return !m_bRunning; }

private:
	static constexpr short KCommandBufferSize{ 200 };
	static constexpr short KCommandLogSize{ 30 };

private:
	bool m_bRunning{};
	short m_Width{};
	short m_Height{};
	HANDLE m_FrontBuffer{};
	HANDLE m_BackBuffer{};

private:
	EBackgroundColor m_eClearBackground{};
	EForegroundColor m_eDefaultForeground{ EForegroundColor::White };

private:
	int m_HitKey{};
	EArrowKeys m_eHitArrowKey{ EArrowKeys::None };

private:
	bool m_bReadingCommand{};
	char m_CommandBuffer[KCommandBufferSize]{};
	short m_CommandReadBytes{};
	char m_CommandLog[KCommandLogSize][KCommandBufferSize]{};
	short m_CommandLogIndex{};
};
