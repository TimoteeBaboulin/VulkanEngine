#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <nlohmann/json.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

    template<>
    struct adl_serializer<glm::fquat> {
        static void to_json(json& j, const glm::fquat& q) {
            j = {{"x", q.x}, {"y", q.y}, {"z", q.z}, {"w", q.w}};
        }
        static void from_json(const json& j, glm::fquat& q) {
            j.at("x").get_to(q.x);
            j.at("y").get_to(q.y);
            j.at("z").get_to(q.z);
            j.at("w").get_to(q.w);
        }
    };
}