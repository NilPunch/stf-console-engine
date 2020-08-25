# stf-console-engine
A simple console engine I made a year ago inspired by https://github.com/OneLoneCoder.

There are a lot of dubious solutions here that I'm not really happy with, but there are also pretty good solutions that allowed me to achieve excellent usability and good performance.

The code may seem a little confusing, but using this engine looks very clean. Just take a look at this:
```cplusplus
#include <stf/ConsoleEngine.h>

class Demo : public ConsoleEngine
{
protected:
    void Init() override
    {
		SetAppName(L"Work Demo");
    }

    void Update() override
    {
		if (GetKey(KEY::ESC).Pressed)
			Quit();

		for (int x = 0; x < ScreenWidth(); ++x)
			for (int y = 0; y < ScreenHeight(); ++y)
				DrawPixel(x, y, L'#', Random::rand() % 16);
    }
};

int main()
{
    Demo t;
    if (t.ConstructConsole(36, 14, 8, 16))
        t.Start();
    return 0;
}
```
And here is the result:

```cplusplus
void Update() override
    {
		if (GetKey(KEY::ESC).Pressed)
			Quit();

		// Fill screen with white
		DrawRect(iVec2{ 0, 0 }, Screen(), QUAD::SOLID, COLOR::FG_WHITE | COLOR::BG_WHITE);

		constexpr const short colorPalette[3] = 
		{
			COLOR::FG_MAGENTA | COLOR::BG_MAGENTA,
			COLOR::FG_CYAN | COLOR::BG_CYAN,
			COLOR::FG_DARK_YELLOW | COLOR::BG_DARK_YELLOW
		};
		constexpr const int boundaryOffset = 3;
		constexpr const int treshold = 8;

		// Draw back lines
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j) // Im not implement the way to draw fat lines right now, so draw them manually
			{
				int offset = j - 1;
				DrawLine(0, boundaryOffset + treshold * i + offset, ScreenWidth() - 1, boundaryOffset + treshold * (i + 1) + offset, QUAD::SOLID, colorPalette[i]);
			}
		}

		// Draw letter N
		for (int i = 0; i < 3; ++i)
		{
			int offset = i - 1;
			DrawLine(0 + offset, ScreenHeight() - 1, ScreenWidth() / 2 + offset, 0, QUAD::SOLID, COLOR::FG_BLACK | COLOR::BG_BLACK);
			DrawLine(ScreenWidth() / 2 + offset, ScreenHeight() - 1, ScreenWidth() / 2 + offset, 0, QUAD::SOLID, COLOR::FG_BLACK | COLOR::BG_BLACK);
			DrawLine(ScreenWidth() / 2 + offset, ScreenHeight() - 1, ScreenWidth() - 1, 0 + offset, QUAD::SOLID, COLOR::FG_BLACK | COLOR::BG_BLACK);
		}
    }
```

# License
[MIT](https://choosealicense.com/licenses/mit/)
