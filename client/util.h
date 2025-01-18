#pragma once

#include <graphics.h>
#include "camera.h"
#pragma comment(lib,"WINMM.lib")
#pragma comment(lib,"MSIMG32.lib")

namespace ut {

	struct Rect {
		int x, y, w, h;
	};

	inline void putimage_ex(const Camera& camera, IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr) {
		static BLENDFUNCTION blend_fun = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
		const Vector2 pos_camera = camera.get_position();
		if (rect_src != nullptr)
			AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x - pos_camera.x, rect_dst->y - pos_camera.y, rect_dst->w, rect_dst->h,
				GetImageHDC(img), rect_src->x, rect_src->y, rect_src->w, rect_src->h, blend_fun);
		else
			AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x - pos_camera.x, rect_dst->y - pos_camera.y, rect_dst->w, rect_dst->h,
				GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(), blend_fun);
	}

	inline void load_audio(LPCTSTR path, LPCTSTR id) {
		static TCHAR str_cmd[512];
		_stprintf_s(str_cmd, _T("open %s alias %s"), path, id);
		mciSendString(str_cmd, NULL, 0, NULL);
	}

	inline void play_audio(LPCTSTR id, bool is_loop = false) {
		static TCHAR str_cmd[512];
		_stprintf_s(str_cmd, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
		mciSendString(str_cmd, NULL, 0, NULL);
	}

	inline void stop_audio(LPCTSTR id) {
		static TCHAR str_cmd[512];
		_stprintf_s(str_cmd, _T("stop %s"), id);
		mciSendString(str_cmd, NULL, 0, NULL);
	}
}