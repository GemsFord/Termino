#include<iostream>
#include<string>
#include"termino.hpp"

int main(){
    termino::window win;
    win.nodelay();
    termino::theme light(5);
    termino::bgcolor(light.WHITE);
    int input = 0, xpos = 0, ypos = 0;
    bool reverse = false, vertical = false, quit = false;
    std::string key_pressed;

    while(!quit){
        if(win.checkFPS(10)){
            if(!win.checkKey('q', input)){
                switch(input){
                    case KEY_RIGHT:
                        vertical = false; key_pressed = " KEY PRESSED : RIGHT";
                        if(reverse) reverse=false; beep();
                        break;
                    case KEY_LEFT:
                        vertical = false; key_pressed = " KEY PRESSED : LEFT ";
                        if(!reverse) reverse=true; beep();
                        break;
                    case KEY_UP:
                        vertical = true; key_pressed = " KEY PRESSED : UP   ";
                        if(!reverse) reverse=true; beep();
                        break;
                    case KEY_DOWN:
                        vertical = true; key_pressed = " KEY PRESSED : DOWN ";
                        if(reverse) reverse=false; beep();
                        break;
                    case KEY_SPACE:
                        light.invert();
                        win.clear();
                        termino::bgcolor(light.WHITE);
                }
                if(!vertical){
                    if(!reverse){
                        if(xpos+1<=win.cols) xpos++;
                        else reverse = true;
                    }else{
                        if(xpos-1>=0) xpos--;
                        else reverse = false;
                    }
                }else{
                    if(!reverse){
                        if(ypos+1<=win.rows) ypos++;
                        else reverse = true;
                    }else{
                        if(ypos-1>=0) ypos--;
                        else reverse = false;
                    }
                }

                termino::styleClear();
                termino::styleON(light.random(),light.random(false));
                win.print(' ',ypos,xpos);
                termino::styleON(light.WHITE,light.BLACK);
                win.print("WELCOME TO TERMINO GAME ENGINE",termino::POS_CENTER, termino::POS_CENTER, -5);
                termino::styleON(light.YELLOW);
                termino::styleON(S_UNDERLINE | S_BOLD);
                win.print("THIS IS AN EXAMPLE PROGRAM",termino::POS_CENTER, termino::POS_CENTER);
                win.print("BUILT IN TERMINO",termino::POS_CENTER, termino::POS_CENTER,1);
                win.print("CODE : UNDER 80 LINES",termino::POS_CENTER, termino::POS_CENTER,2);
                win.print("ONLY C++",termino::POS_CENTER, termino::POS_CENTER,3);
                termino::styleOFF(S_UNDERLINE | S_BOLD);
                win.print(key_pressed.c_str(),termino::POS_TOP,termino::POS_RIGHT, 2, -4);
                termino::styleON(light.random(false));
                termino::styleON(S_STANDOUT);
                win.print("I HAVE A LOT OF ABILITIES",termino::POS_CENTER, termino::POS_LEFT, 10);
                termino::styleOFF(S_STANDOUT);
                termino::styleON(light.random(false),light.BLACK);
                win.print("THIS IS DONE IN PURE TERMINAL/CONSOLE/CMD",termino::POS_CENTER, termino::POS_RIGHT, 10);
                termino::styleON(S_UNDERLINE | S_BOLD);
                termino::styleON(light.random());
                win.print("SEE YOU SOON",termino::POS_BOTTOM, termino::POS_CENTER, -1);
                termino::styleOFF(S_UNDERLINE | S_BOLD);
                win.update();
            }else quit=true;
        }
    }
    return 0;
}