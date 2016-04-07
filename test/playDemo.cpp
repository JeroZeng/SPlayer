#include "../SPlayer.h"

int main(int argc, char *argv[]){
    printf("%d\t%s\n", argc, argv[0]);
    SPlayer *player = new SPlayer("test.mp4");
    player->Init();
    player->Play();
    return 0;
}
