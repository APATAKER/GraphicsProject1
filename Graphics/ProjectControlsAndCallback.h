#ifndef _ProjectControlsAndCallback_HG_
#define _ProjectControlsAndCallback_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void error_callback(int error, const char* description);

#endif