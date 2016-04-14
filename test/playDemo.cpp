#include "../SPlayer.h"

void keyCallback(GLFWwindow *window, int key, int scancode,
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

SWindow* createWindow(SPlayer *player) {
    if (!glfwInit())
        return NULL;
    SWindow* win = glfwCreateWindow(800, 600, "SPlayer", NULL, NULL);
    if (!win) {
        printf("Create Window Failed\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(win);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(win);
    glfwMakeContextCurrent(NULL);
    glfwSetWindowPos(win, 200, 200);
    glfwShowWindow(win);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetWindowUserPointer(win, player);
    return win;

}

void waitForWindowClose(SWindow *w) {
    while (!glfwWindowShouldClose(w)) {
        glfwWaitEvents();
    }
    glfwHideWindow(w);
}

int main(int argc, char *argv[]){
    printf("%d\t%s\n", argc, argv[0]);
    SPlayer *player = new SPlayer("test.mp4");
    SWindow *win = createWindow(player);
    player->Init(win);
    player->Play();
    waitForWindowClose(win);
    delete player;
    return 0;
}
