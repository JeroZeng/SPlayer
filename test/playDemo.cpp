#include "../SPlayer.h"
#include "GLFW/glfw3.h"

void keycallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods) {
    SPlayer *player = (SPlayer*)glfwGetWindowUserPointer(window);
    if (action != GLFW_PRESS)
        return;
    switch (key) {
        case GLFW_KEY_ESCAPE: player->Stop();  break;
        case GLFW_KEY_SPACE:  player->Pause(); break;
        case GLFW_KEY_ENTER:  player->Play();  break;
    }
}

int main(int argc, char *argv[]){
    printf("%d\t%s\n", argc, argv[0]);
    SPlayer *player = new SPlayer("test.mp4");
    player->Init();
    player->Play();
    printf("------------------------------------->delete player\n");
    delete player;
    return 0;
}
