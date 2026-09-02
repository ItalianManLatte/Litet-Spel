//Prepocessor command to ensure that this file is included once per compilation.
#pragma once

//IMPORTING HEADER FILES --------------------------------------------------------
#include <Windows.h> //Header to access the main Windows API.


//DECLARING METHODS -------------------------------------------------------------

bool SetupWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow,	HWND & window);