#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#include "play.h"

/*
 * 让控制台全屏
 * 我也不知道是怎么做到的
 */
void SetWindow() {
	system("chcp 65001");
	HWND hwnd = GetForegroundWindow();
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE));
	SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
}

int main(int argc, const char **argv) {
	 SetWindow();
	 play();

	return 0;
}
