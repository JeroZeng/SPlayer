#include "../SPlayer.h"

int main(int argc, char *argv[]){

    SPlayer *player = new SPlayer("test.mp4");
    player->Init();
    player->Play();
    return 0;
}
