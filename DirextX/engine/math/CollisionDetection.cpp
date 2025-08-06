#include "CollisionDetection.h"
#include <algorithm>
using namespace MathUtility;
namespace CollisionDetection
{

    bool IsCollision(Sphere s1, Sphere s2)
    {
        float len = Length(s1.center - s2.center);
        if (len >= s1.radius + s2.radius) {
            return true;
        }
        return false;
    }

    bool IsCollision(AABB aabb1, AABB aabb2)
    {
        if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x &&
            aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y &&
            aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z) {
            return true;
        }
        return false;
    }

    bool IsCollision(Sphere s, AABB aabb)
    {
        Vector3 a = {
        std::clamp(s.center.x, aabb.min.x, aabb.max.x),
        std::clamp(s.center.y, aabb.min.y, aabb.max.y),
        std::clamp(s.center.z, aabb.min.z, aabb.max.z),
        };
        float len = Length(a - s.center);

        if (len <= s.radius) {
            return true;
        }

        return false;
    }

    bool IsCollision(AABB aabb, Sphere s)
    {
        return IsCollision(s, aabb);
    }

    bool IsCollision(Sphere s, Vector3 v)
    {
        float len = Length(s.center -v);
        if (len >= s.radius) {
            return true;
        }
        return false;
    }

    bool IsCollision(Vector3 v, Sphere s)
    {
        return IsCollision(s, v);
    }

    bool IsCollision(AABB aabb, Vector3 v)
    {
        Vector3 a = {
        std::clamp(v.x, aabb.min.x, aabb.max.x),
        std::clamp(v.y, aabb.min.y, aabb.max.y),
        std::clamp(v.z, aabb.min.z, aabb.max.z),
        };
        float len = Length(a - v);

        if (len <= 0.0f) {
            return true;
        }

        return false;
    }

    bool IsCollision(Vector3 v, AABB aabb)
    {
        return IsCollision(aabb, v);
    }


}