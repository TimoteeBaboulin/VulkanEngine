#define VK_USE_PLATFORM_WIN32_KHR

#include "Editor/Utility/CommandLineParser.h"
#include "Editor/MoonlitEditor.h"

int main(int argc, char** argv)
{
	Moonlit::Editor::CommandLineParser::Parse(argc, argv);
	MoonlitEditor* editor = new MoonlitEditor();
}