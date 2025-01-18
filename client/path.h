#pragma once

#include "vector2.h"
#include <vector>

class Path
{
public:
    Path(const std::vector<Vector2>& point_list) {
        this->point_list = point_list;
        for (size_t i = 0; i < point_list.size() - 1; i++){
            float segment_len = (point_list[i + 1] - point_list[i]).length();
            segment_len_list.push_back(segment_len);
            total_length += segment_len;
        }
    }

    Vector2 get_position_at_progress(float progress) {
        if (progress <= 0) return point_list.front();
        if (progress >= 1) return point_list.back();

        float target_len = total_length * progress;

        float accumulated_len = 0;

        for (size_t i = 1; i < point_list.size(); i++) {
            accumulated_len += segment_len_list[i-1];
            if (accumulated_len >= target_len) {
                float segment_progress = (target_len - (accumulated_len - segment_len_list[i - 1])) / segment_len_list[i - 1];
                return point_list[i - 1] + (point_list[i] - point_list[i - 1]) * segment_progress;
            }
        }
        return point_list.back();
    }

    ~Path() = default;
private:
    float total_length = 0;
    std::vector<Vector2> point_list;
    std::vector<float> segment_len_list;
};