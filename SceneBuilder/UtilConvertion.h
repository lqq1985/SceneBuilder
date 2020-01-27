#ifndef UTIL_CONVERSION
#define UTIL_CONVERSION

#include <glm/glm.hpp>
#include <bullet/btBulletCollisionCommon.h>

namespace UtilConversion {
    glm::mat4 btScalar2mat4(btScalar* matrix);
};

#endif
