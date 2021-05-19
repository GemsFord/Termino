#include<iostream>
#include<string>
#include<sstream>
#include"termino.hpp"
#define BRICKS_COLS 10
#define BRICKS_ROWS 2
#define BRICKS_PACKS 3
#define BRICK_WIDTH 12
#define BRICK_HEIGHT 2
#define MAX_LIEF 3

int main(){
    TERMINO::WINDOW win;
    win.nodelay();
    TERMINO::FPS fGame,fBall;
    TERMINO::THEME theme(5);
    theme.RED.change(231,96,61); theme.BLACK.change(17,29,49);
    theme.YELLOW.change(210,165,92); theme.MAGENTA.change(207,71,124); theme.CYAN.change(218,89,124);
    theme.GREEN.change(63,183,122); theme.BLUE.change(0,187,197); theme.CYAN.change(207,71,124);
    TERMINO::bgcolor(theme.BLACK);


    TERMINO::ENTITY ball(5,11,0,0,theme.WHITE,true);
    TERMINO::ENTITY player(1,30,0,0,theme.GREEN,true);
    ball.sprite[0]="  ▄█████▄  ";
    ball.sprite[1]="▄█████████▄";
    ball.sprite[2]="███████████";
    ball.sprite[3]="▀█████████▀";
    ball.sprite[4]="  ▀█████▀  ";
    player.sprite[0]="▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓";
    player.set(win, TERMINO::POS_BOTTOM, TERMINO::POS_CENTER, -10);
    // ball.set(win, (player.pos.y-ball.size.h), (player.pos.x/2)-(ball.size.w/2));
    
    TERMINO::ENTITY enemy(1,400,0,0,theme.CYAN);
    enemy.sprite[0]="▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲";

    TERMINO::ENTITY brick[BRICKS_PACKS][BRICKS_ROWS][BRICKS_COLS];
    for(int i=0;i<BRICKS_PACKS;i++){
        for(int j=0;j<BRICKS_ROWS;j++){
            for(int k=0;k<BRICKS_COLS;k++){
                switch(i){
                    case 0: brick[i][j][k] = TERMINO::ENTITY(BRICK_HEIGHT,BRICK_WIDTH,0,0,theme.MAGENTA,true); break;
                    case 1: brick[i][j][k] = TERMINO::ENTITY(BRICK_HEIGHT,BRICK_WIDTH,0,0,theme.GREEN,true); break;
                    case 2: brick[i][j][k] = TERMINO::ENTITY(BRICK_HEIGHT,BRICK_WIDTH,0,0,theme.BLUE,true); break;
                }
                brick[i][j][k].sprite[0]="▤▤▤▤▤▤▤▤▤▤▤▤";
                brick[i][j][k].sprite[1]="▤▤▤▤▤▤▤▤▤▤▤▤";
            }
        }
    }

    int input;
    bool quit=false;
    bool left=false, up=false, gameover=true;
    int score=0;int life=MAX_LIEF;
    std::string scoreStr,lifeStr;
    win.updateRowsCols();
    for(int i=0;i<BRICKS_PACKS;i++){
        for(int j=0;j<BRICKS_ROWS;j++){
            for(int k=0;k<BRICKS_COLS;k++){
                brick[i][j][k].set(win, TERMINO::POS_TOP, TERMINO::POS_CENTER, 5+(5*j)+(10*i), -((BRICKS_COLS*BRICK_WIDTH)/2)-(BRICKS_COLS) + k*(BRICK_WIDTH+2));
                brick[i][j][k].print(win);
            }
        }
    }
    while(!quit){
        if(!win.checkKey('q',input)){
            if(fGame.checkFPS(15)){
                win.clear();
                if(win.isWindowResized()){player.set(win, TERMINO::POS_BOTTOM, TERMINO::POS_CENTER, -10);}
                for(int i=0;i<BRICKS_PACKS;i++){
                    for(int j=0;j<BRICKS_ROWS;j++){
                        for(int k=0;k<BRICKS_COLS;k++){
                            brick[i][j][k].set(win, TERMINO::POS_TOP, TERMINO::POS_CENTER, 5+(5*j)+(10*i), -((BRICKS_COLS*BRICK_WIDTH)/2)-(BRICKS_COLS) + k*(BRICK_WIDTH+4));
                            brick[i][j][k].print(win);
                        }
                    }
                }
                player.print(win, TERMINO::POS_BOTTOM, player.pos.x, -10);
                ball.print(win,ball.pos.y,ball.pos.x);
                enemy.print(win, TERMINO::POS_BOTTOM, 0);
                scoreStr = "Score : "+std::to_string(score);
                lifeStr = "";
                for(int i=0;i<MAX_LIEF;i++) if(i+1<=life){lifeStr += " ♥ ";}else{lifeStr += " ♡ ";}
                lifeStr = "Life : "+lifeStr;
                TERMINO::styleON(theme.WHITE);
                win.print(scoreStr.c_str(),2,2);
                win.print(lifeStr.c_str(),TERMINO::POS_TOP, TERMINO::POS_RIGHT,2,-2);
                TERMINO::styleOFF(theme.WHITE);
                if(gameover){
                    TERMINO::styleON(theme.WHITE);
                    TERMINO::styleON(S_BOLD);
                    win.print("PRESS SPACE TO START",TERMINO::POS_CENTER, TERMINO::POS_CENTER,10);
                    TERMINO::styleOFF(S_BOLD);
                    TERMINO::styleOFF(theme.WHITE);
                }
                win.update();
            }

            if(fBall.checkFPS(60)){
                if(!gameover){
                    if(TERMINO::ENTITY::checkCollision(ball,TERMINO::COL_BOTTOM)){ up=true;}
                    if(TERMINO::ENTITY::checkCollision(ball,TERMINO::COL_RIGHT)){ left=true;}
                    if(TERMINO::ENTITY::checkCollision(ball,TERMINO::COL_TOP)){ up=false;}
                    if(TERMINO::ENTITY::checkCollision(ball,TERMINO::COL_LEFT)){ left=false;}
                    if(TERMINO::ENTITY::checkCollision(ball,player,TERMINO::COL_TOP)){ up=true;}
                    if(TERMINO::ENTITY::checkCollision(ball,player,TERMINO::COL_BOTTOM)){ up=false;}

                    for(int i=0;i<BRICKS_PACKS;i++){
                        for(int j=0;j<BRICKS_ROWS;j++){
                            for(int k=0;k<BRICKS_COLS;k++){
                                if(TERMINO::ENTITY::checkCollision(ball,brick[i][j][k],TERMINO::COL_TOP)){ up=true; brick[i][j][k].die(); score++;}
                                if(TERMINO::ENTITY::checkCollision(ball,brick[i][j][k],TERMINO::COL_BOTTOM)){ up=false; brick[i][j][k].die(); score++;}
                                if(TERMINO::ENTITY::checkCollision(ball,brick[i][j][k],TERMINO::COL_LEFT)){ left=true; brick[i][j][k].die(); score++;}
                                if(TERMINO::ENTITY::checkCollision(ball,brick[i][j][k],TERMINO::COL_RIGHT)){ left=false; brick[i][j][k].die(); score++;}
                            }
                        }
                    }
                    if(TERMINO::ENTITY::checkCollision(ball,enemy,TERMINO::COL_TOP)){
                        if(--life < 1){ gameover=true;life=3;score=0;
                            for(int i=0;i<BRICKS_PACKS;i++){
                                for(int j=0;j<BRICKS_ROWS;j++){
                                    for(int k=0;k<BRICKS_COLS;k++){
                                        brick[i][j][k].die(false);
                                    }
                                }
                            }
                        }else gameover=true;
                    }
                    if(!left && !up) {++ball.pos.y;++ball.pos.x;}
                    else if(!left && up) {--ball.pos.y;++ball.pos.x;}
                    else if(left && up) {--ball.pos.y;--ball.pos.x;}
                    else if(left && !up) {++ball.pos.y;--ball.pos.x;}
                }
            }
            switch (input)
            { case KEY_RIGHT: player.pos.x += 10; break;
            case KEY_LEFT: player.pos.x -= 10; break;
            case KEY_SPACE:
                if(gameover){
                    gameover=false;
                    if(player.pos.x>=win.cols/2){up=true; left=false;}
                    if(player.pos.x<=win.cols/2){up=true; left=true;}
                }
                break;
            }
            if(gameover){ ball.set(win, player.pos.y - ball.size.h, player.pos.x  + (player.size.w/2) - (ball.size.w/2)); }else{
                player.pos.x = ((ball.pos.x) + (ball.size.w/2) - (player.size.w/2));
            }
        }else quit=true; 
    }
}