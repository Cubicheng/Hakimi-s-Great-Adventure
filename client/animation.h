#pragma once

#include "atlas.h"
#include "camera.h"
#include "util.h"
#include "timer.h"
#include "vector2.h"

#include <vector>
#include <functional>

class Animation {
public :
	Animation() {
		timer.set_one_shot(false);
		timer.set_on_timeout([&]() {
			idx_frame++;
			if (idx_frame >= frames.size()) {
				idx_frame = is_loop?0:frames.size()-1;
				if (!is_loop && on_finished) {
					on_finished();
				}
		}
		});
	}

	~Animation() = default;

	void reset() {
		timer.restart();
        idx_frame = 0;
	}

	void set_position(const Vector2& position) {
		this->position = position;
	}

	void set_loop(bool is_loop) {
        this->is_loop = is_loop;
	}

	void set_interval(float interval){
		timer.set_wait_time(interval);
	}

	void set_on_finished(std::function<void()> on_finished) {
		this->on_finished = on_finished;
	}

	void add_frame(IMAGE* image,int num_h) {
		int width = image->getwidth();
		int height = image->getheight();
		int width_frame = width / num_h;

		for (int i = 0; i < num_h; i++) {
			ut::Rect rect_src;
			rect_src.x = i * width_frame;
			rect_src.y = 0;
            rect_src.w = width_frame;
            rect_src.h = height;

			frames.emplace_back(image, rect_src);
		}
	}

	void add_frame(Atlas* atlas) {
		for (int i = 0; i < atlas->get_size(); i++) {
			IMAGE* image = atlas->get_image(i);
			int width = image->getwidth();
			int height = image->getheight();

			ut::Rect rect_src;
			rect_src.x = 0;
            rect_src.y = 0;
            rect_src.w = width;
            rect_src.h = height;

			frames.emplace_back(image, rect_src);
		}
	}

    void on_update(float delta) {
		timer.on_update(delta);
	}

	void on_render(const Camera& camera) {
		const Frame& frame = frames[idx_frame];
		ut::Rect rect_dst;
        rect_dst.x = position.x - frame.rect_src.w / 2;
        rect_dst.y = position.y - frame.rect_src.h / 2;
        rect_dst.w = frame.rect_src.w;
        rect_dst.h = frame.rect_src.h;

        putimage_ex(camera, frame.image, &rect_dst, &frame.rect_src);
	}

private:
	struct Frame {
		IMAGE* image = nullptr;
		ut::Rect rect_src;
		Frame() = default;
		Frame(IMAGE* image,const ut::Rect& rect_src):image(image),rect_src(rect_src){}
		~Frame() = default;
	};

	Timer timer;
	Vector2 position;
	bool is_loop = false;
	size_t idx_frame = 0;
	std::vector<Frame> frames;
	std::function<void()> on_finished;
};