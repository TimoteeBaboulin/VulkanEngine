#pragma once

namespace Moonlit::Inputs
{

	struct ScreenSize
	{
		int width;
		int height;
	};

	enum KEYBOARD_KEY : int
	{
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		NUM0,
		NUM1,
		NUM2,
		NUM3,
		NUM4,
		NUM5,
		NUM6,
		NUM7,
		NUM8,
		NUM9,
		ESCAPE,
		CTRL,
		SHIFT,
		ALT,
		LSYSTEM,
		RSYSTEM,
		SPACE,
		BACKSPACE,
		TAB,
		ENTER,
		SLASH,
		ASTERISK,
		ADD,
		SUBTRACT,
		ALPHANUM0,
		ALPHANUM1,
		ALPHANUM2,
		ALPHANUM3,
		ALPHANUM4,
		ALPHANUM5,
		ALPHANUM6,
		ALPHANUM7,
		ALPHANUM8,
		ALPHANUM9,
		ARROW_LEFT,
		ARROW_RIGHT,
		ARROW_UP,
		ARROW_DOWN
	};

	enum MOUSE_KEY : char
	{
		LEFT_CLICK,
		RIGHT_CLICK,
		MIDDLE_CLICK
	};

	enum GAMEPAD_KEY : char
	{
		DPAD_LEFT,
		DPAD_RIGHT,
		DPAD_UP,
		DPAD_DOWN,
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_LB,
		BUTTON_RB,
		BUTTON_LT,
		BUTTON_RT,
		BUTTON_START,
		BUTTON_SELECT,
		AXIS_LEFT,
		AXIS_RIGHT,
	};

	enum KEY_STATE : char
	{
		PRESSED,
		RELEASED,
		HELD
	};

	// Structure representing a single input event instance
	// Used to pass input data from platform specific code to the input manager
	struct InputInstance
	{
		// Enum used to store the type of device that generated the input
		// System is used for native window events like resize and close
		enum struct INPUT_DEVICE_TYPE : char
		{
			MOUSE, KEYBOARD, GAMEPAD, SYSTEM
		} DeviceType;

		// Enum used to store the state of the input
		// If it is an axis input, the state is AXIS
		enum struct INPUT_STATE : char
		{
			STARTED, CANCELLED, AXIS
		} InputType;

		// Handle to the window that generated the input
		void* WindowHandle;

		// Raw input data
		void* RawInput;
	};
}