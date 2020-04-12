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
	std::thread ThrInput{
		[&]()
		{
			while (true)
			{
				if (Console.IsTerminated()) break;

				if (Console.HitKey())
				{
					if (Console.IsHitKey(EArrowKeys::Right)) ++X;
					else if (Console.IsHitKey(EArrowKeys::Left)) --X;
					else if (Console.IsHitKey(EArrowKeys::Down)) ++Y;
					else if (Console.IsHitKey(EArrowKeys::Up)) --Y;
					else if (Console.IsHitKey(VK_RETURN))
					{
						if (Console.ReadCommand())
						{
							if (Console.IsLastCommand("/quit"))
							{
								Console.Terminate();
							}
						}
					}
				}
			}
		}
	};

	while (true)
	{
		if (Console.IsTerminated()) break;

		Console.Clear();

		Console.FillBox(5, 5, 7, 10, '~', EBackgroundColor::Cyan, EForegroundColor::White);

		Console.PrintBox(0, 0, 70, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);

		Console.PrintBox(70, 0, 40, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);
		Console.PrintCommandLog(70, 0, 40, 29);
		Console.PrintCommand(0, 29);

		Console.PrintChar(X, Y, '@', EForegroundColor::LightYellow);

		Console.PrintChar(112, 1, 'X');
		Console.PrintChar(112, 2, 'Y');
		Console.PrintHString(114, 1, X);
		Console.PrintHString(114, 2, Y);

		Console.Render();
	}

	ThrInput.join();
	return 0;
}