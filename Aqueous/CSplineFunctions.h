#pragma once

#include "CProgramContext.h"

class CSplineFunctions {

public:

    static bool fEquals(glm::vec3 a, glm::vec3 b, f32 epsilon) {

        return (a.x <= b.x + epsilon && a.x >= b.x - epsilon) &&

            (a.y <= b.y + epsilon && a.y >= b.y - epsilon) &&

            (a.z <= b.z + epsilon && a.z >= b.z - epsilon);

    };

    static f32 AngleCos(glm::vec3 cur, glm::vec3 pre, glm::vec3 post) //returns in radians
    {
        return acos((f32)glm::dot(glm::vec3(pre - cur), glm::vec3(post - cur)) / (f32)(glm::length(pre - cur)*glm::length(post - cur)));

    };
};