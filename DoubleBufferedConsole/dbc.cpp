#include "DoubleBufferedConsole.h"
#include <string>
#include <thread>
#include <atomic>

int main()
{
	CDoubleBufferedConsole Console{ 130, 30, "Console" };
	Console.SetClearBackground(EBackgroundColor::Black);
	Console.SetDefaultForeground(EForegroundColor::LightYellow);

	std::atomic<short> X{};
	std::atomic<short> Y{};

	std::thread thr_input{
		[&]()
		{
			while (true)
			{
				if (Console.IsCleanedUp()) break;

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
						Console.CleanUp();
					}
					else if (Key == VK_RETURN)
					{
						if (Console.GetCommand())
						{
							Console.GetLastCommand();
						}
					}
				}
			}
		}
	};

	while (true)
	{
		if (Console.IsCleanedUp()) break;

		Console.Clear();

		Console.FillBox(5, 5, 7, 10, '~', EBackgroundColor::Cyan, EForegroundColor::White);

		Console.PrintBox(0, 0, 70, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);

		Console.PrintBox(70, 0, 40, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);
		Console.PrintCommandLog(70, 0, 40, 29);
		Console.PrintCommand(0, 29);

		Console.PrintChar(X, Y, '@');

		Console.PrintChar(112, 1, 'X');
		Console.PrintChar(112, 2, 'Y');
		Console.PrintHString(114, 1, std::to_string(X).c_str());
		Console.PrintHString(114, 2, std::to_string(Y).c_str());

		Console.Render();
	}

	thr_input.join();

	return 0;
}