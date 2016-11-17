#include "AfterCharStdioFlusher.h"

#include <termios.h>

class AfterCharStdioFlusher::Impl {
public:
	struct termios original_settings;
};

AfterCharStdioFlusher::AfterCharStdioFlusher()
	: impl(new Impl())
{
	struct termios new_settings;
    tcgetattr(0, &impl->original_settings);
    new_settings = impl->original_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new_settings);
}

AfterCharStdioFlusher::~AfterCharStdioFlusher()
{
	tcsetattr(0, TCSANOW, &impl->original_settings);
}
