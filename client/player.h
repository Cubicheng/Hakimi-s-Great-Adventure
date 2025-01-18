#pragma once

#include "animation.h"

class Player {
public:
	enum class Facing {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	Player(Atlas* atlas_idle_up, Atlas* atlas_idle_down, Atlas* atlas_idle_left, Atlas* atlas_idle_right, Atlas* atlas_walk_up, Atlas* atlas_walk_down, Atlas* atlas_walk_left, Atlas* atlas_walk_right) {
		animation_idle_up.set_loop(true);
		animation_idle_up.set_interval(0.1f);
		animation_idle_up.add_frame(atlas_idle_up);

        animation_idle_down.set_loop(true);
        animation_idle_down.set_interval(0.1f);
        animation_idle_down.add_frame(atlas_idle_down);

        animation_idle_left.set_loop(true);
        animation_idle_left.set_interval(0.1f);
        animation_idle_left.add_frame(atlas_idle_left);
			
        animation_idle_right.set_loop(true);
        animation_idle_right.set_interval(0.1f);
        animation_idle_right.add_frame(atlas_idle_right);

        animation_walk_up.set_loop(true);
        animation_walk_up.set_interval(0.1f);
        animation_walk_up.add_frame(atlas_walk_up);

        animation_walk_down.set_loop(true);
        animation_walk_down.set_interval(0.1f);
        animation_walk_down.add_frame(atlas_walk_down);

        animation_walk_left.set_loop(true);
        animation_walk_left.set_interval(0.1f);
        animation_walk_left.add_frame(atlas_walk_left);

        animation_walk_right.set_loop(true);
        animation_walk_right.set_interval(0.1f);
        animation_walk_right.add_frame(atlas_walk_right);
	}

	~Player() = default;

    void on_update(float delta) {
        if (!position.approx(pos_target)) {
            velocity = (pos_target - position).normalize() * SPEED_WALK;
        }
        else {
            velocity = Vector2(0,0);
        }

        if ((pos_target - position).length() <= (velocity * delta).length()) {
            position = pos_target;
        }
        else {
            position += velocity * delta;
        }

        if (velocity.approx(Vector2(0, 0))) {
            switch (facing) {
            case Facing::UP: {
                animation_current = &animation_idle_up;
                break;
            }
            case Facing::DOWN: {
                animation_current = &animation_idle_down;
                break;
            }
            case Facing::LEFT: {
                animation_current = &animation_idle_left;
                break;
            }
            case Facing::RIGHT: {
                animation_current = &animation_idle_right;
                break;
            }
            }
        }
        else {
            if (abs(velocity.y) >= 0.0001f) {
                facing = (velocity.y < 0) ? Facing::UP : Facing::DOWN;
            }
            if (abs(velocity.x) >= 0.0001f) {
                facing = (velocity.x < 0) ? Facing::LEFT : Facing::RIGHT;
            }

            switch (facing) {
            case Facing::UP: {
                animation_current = &animation_walk_up;
                break;
            }
            case Facing::DOWN: {
                animation_current = &animation_walk_down;
                break;
            }
            case Facing::LEFT: {
                animation_current = &animation_walk_left;
                break;
            }
            case Facing::RIGHT: {
                animation_current = &animation_walk_right;
                break;
            }
            }
        }
        if (!animation_current) return;
        animation_current->set_position(position);
        animation_current->on_update(delta);
    }

    void on_render(const Camera& camera) {
        if (!animation_current) return;
        animation_current->on_render(camera);
    }

    void set_position(const Vector2& position) {
        this->position = position;
    }

    const Vector2& get_position() const {
        return position;
    }

    void set_target(const Vector2& target) {
        pos_target = target;
    }

private:
	const float SPEED_WALK = 100.0f;

	Vector2 position;
	Vector2 velocity;
	Vector2 pos_target;

	Animation animation_idle_up;
    Animation animation_idle_down;
    Animation animation_idle_left;
    Animation animation_idle_right;
    Animation animation_walk_up;
    Animation animation_walk_down;
    Animation animation_walk_left;
    Animation animation_walk_right;
	Animation* animation_current = nullptr;

	Facing facing = Facing::DOWN;
};