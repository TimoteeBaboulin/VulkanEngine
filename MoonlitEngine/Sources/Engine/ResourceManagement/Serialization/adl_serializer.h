#pragma once

#include <nlohmann/json.hpp>

namespace nlohmann {
    template<>
    struct adl_serializer<glm::vec2> {
        static void to_json(json& j, const glm::vec2& v) {
            j = {{"x", v.x}, {"y", v.y}};
        }
        static void from_json(const json& j, glm::vec2& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
        }
    };

    template<>
    struct adl_serializer<glm::vec3> {
        static void to_json(json& j, const glm::vec3& v) {
            j = {{"x", v.x}, {"y", v.y}, {"z", v.z}};
        }
        static void from_json(const json& j, glm::vec3& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
        }
    };

    template<>
    struct adl_serializer<glm::vec4> {
        static void to_json(json& j, const glm::vec4& v) {
            j = {{"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w}};
        }
        static void from_json(const json& j, glm::vec4& v) {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
            j.at("w").get_to(v.w);
        }
    };
}