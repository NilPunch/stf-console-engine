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


# License
[MIT](https://choosealicense.com/licenses/mit/)
