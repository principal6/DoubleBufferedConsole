#include "DoubleBufferedConsole.h"
#include <string>

int main()
{
	CDoubleBufferedConsole Console{ 130, 30, "Console" };
	Console.SetClearBackground(EBackgroundColor::Black);
	Console.SetDefaultForeground(EForegroundColor::LightYellow);

	short X{}, Y{};
	while (true)
	{
		if (Console.HitKey())
		{
			EArrowKeys ArrowKey{ Console.GetHitArrowKey() };
			if (ArrowKey == EArrowKeys::Right) ++X;
			if (ArrowKey == EArrowKeys::Left) --X;
			if (ArrowKey == EArrowKeys::Down) ++Y;
			if (ArrowKey == EArrowKeys::Up) --Y;

			int Key{ Console.GetHitKey() };
			if (Key == VK_ESCAPE)
			{
				break;
			}
			else if (Key == VK_RETURN)
			{
				if (Console.GetCommand(0, 29))
				{
					Console.GetLastCommand();
				}
			}
		}

		Console.Clear();

		Console.FillBox(5, 5, 7, 10, '~', EBackgroundColor::Cyan, EForegroundColor::White);

		Console.PrintBox(0, 0, 70, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);

		Console.PrintBox(70, 0, 40, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);
		Console.PrintCommandLog(70, 0, 40, 29);

		Console.PrintChar(X, Y, '@');

		Console.PrintChar(112, 1, 'X');
		Console.PrintChar(112, 2, 'Y');
		Console.PrintHString(114, 1, std::to_string(X).c_str());
		Console.PrintHString(114, 2, std::to_string(Y).c_str());

		Console.Render();
	}
	return 0;
}