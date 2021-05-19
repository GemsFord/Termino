#ifndef __TERMINO_H__
#define __TERMINO_H__

#include <ncurses.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>

/*
    SPECIAL CHARACTERS FOR MAKING SPRITES -
    █ ▀ ▄ ■ ▓ ▒ ▒ 
    
    ▁ ▂ ▃ ▄ ▅ ▆ ▇ ▉
    
    ▊ ▋ █ ▌ ▍ ▎ ▏ ▐ ▔ ▕ 
    
    ▖ ▗ ▝ ▝ ▘

    ▞  ▚ ▙ ▟ ▛ ▜ 
    
    ■ □ ▢ ▣ ▤ ▥ ▦ ▧ ▨ ▩
    
    ▪ ▫ ▬ ▭ ▮ ▯
    
    ☰ ☲ ☱ ☴ ☵ ☷ ☶ ☳
    
    ❏ ❑ ❐ ❒ ◈ ⊞ 
*/


namespace TERMINO{

    /*FAST RANDOM FUNCTION FROM STACK OVERFLOW*/
    static unsigned int g_seed;
    // Used to seed the generator.           
    inline void fast_srand(int seed) {
        g_seed = seed;
    }
    // Compute a pseudorandom integer.
    // Output value in range [0, 32767]
    inline int fast_rand(void) {
        g_seed = (214013*g_seed+2531011);
        return (g_seed>>16)&0x7FFF;
    }
    /*FAST RANDOM FUNCTION FROM STACK OVERFLOW*/


    //VARAIBLES START

    int COLOR_INTENSITY = 3;
    
    enum POS{
        POS_CENTER = 0,
        POS_TOP,
        POS_BOTTOM,
        POS_LEFT,
        POS_RIGHT
    };

    //VARAIBLES END
    
    //WINDOWS START

    class WINDOW{
        public:
            int rows=0, cols=0;

            WINDOW(){
                init();
            }

            ~WINDOW(){
                end();
            }

            void init(){
                setlocale(LC_ALL, "");
                initscr();
                noecho();
                curs_set(false);
                start_color();
                keypad(stdscr,true);
                
            }

            void updateRowsCols(){
                rows = LINES;
                cols = COLS;
            }

            void update(){
                ::refresh();
                updateRowsCols();
            }

            void clear(){
                ::clear();
            }

            void end(){
                endwin();
            }

            void nodelay(bool b=true){
                ::nodelay(stdscr, b);
            }

            int get(){
                return getch();
            }

            void beep(){
                ::beep();
            }
            
            void print(const char* c, int y=0, int x=0){
                mvprintw(y,x,c);
            }

            void print(const char* c, POS posy, POS posx, int yoff=0, int xoff=0){
                print(c,anchorY(posy,1,yoff),anchorX(posx,strlen(c),xoff));
            }

            void print(const char c, int y=0, int x=0){
                print(&c,y,x);
            }
            
            void print(const char c, POS posy, POS posx, int yoff=0, int xoff=0){
                print(&c,anchorY(posy,1,yoff),anchorX(posx,1,xoff));
            }

            int anchorX(POS position, int length=0, int offset=0){
                updateRowsCols();
                switch(position){
                    case POS_CENTER:
                        return ((cols/2)-(length/2) + offset);
                    case POS_LEFT:
                        return 0 + offset;
                    case POS_RIGHT:
                        return (cols - length + offset);
                    default:
                        return 0;
                }
            }
            int anchorY(POS position, int length=0, int offset=0){
                updateRowsCols();
                switch(position){
                    case POS_CENTER:
                        return ((rows/2)-(length/2) + offset);
                    case POS_TOP:
                        return 0 + offset;
                    case POS_BOTTOM:
                        return (rows - length + offset);
                    default:
                        return 0;
                }
            }

            bool checkColors(){
                return has_colors();
            }
            bool checkChangeColors(){
                return can_change_color();
            }

            bool checkFPS(int fps){
                static clock_t oldTime = clock();
                clock_t newTime = clock();
                clock_t deltaTme = newTime - oldTime;
                clock_t tickPerFrame = CLOCKS_PER_SEC/fps;
                if(deltaTme > tickPerFrame){
                    oldTime = newTime;
                    return true;
                }else{
                    return false;
                }
            }

            bool isWindowResized(){
                static int rows_;
                static int cols_;
                if (rows_ != LINES){rows_ = LINES; return true;}
                if (cols_ != COLS){cols_ = COLS; return true;}
                return false;
            }

            #define KEY_SPACE ' '

            bool checkKey(char c){
                char b;
                b = getch();
                if(b == c) return true; else return false;
            }
            bool checkKey(int c){
                int b;
                b = getch();
                if(b == c) return true; else return false;
            }
            bool checkKey(int c, int &i){
                i = getch();
                if(i == c){
                    return true;
                }else{
                    return false;
                }
            }
    };

    //FPS START

    struct FPS{
        clock_t oldTime;
        FPS(){
            oldTime = clock();
        }
        bool checkFPS(int fps){
            clock_t newTime = clock();
            clock_t deltaTme = newTime - oldTime;
            clock_t tickPerFrame = CLOCKS_PER_SEC/fps;
            if(deltaTme > tickPerFrame){
                oldTime = newTime;
                return true;
            }else{
                return false;
            }
        }
    };

    //FPS END

    //WINDOWS END


    // COLORS START

    struct RGB{
        int r,g,b;
        RGB(int r_=255, int g_=255, int b_=255):r(r_),g(g_),b(b_){}

        bool operator == (const RGB& c){
            bool result=true;
            if(result) {if(r == c.r) result=true; else result=false;}
            if(result) {if(g == c.g) result=true; else result=false;}
            if(result) {if(b == c.b) result=true; else result=false;}
            return result;
        }
    };

    class COLOR{
        public:
            RGB value;
            int *i;
            int n;

        public:

            COLOR(int r_=255, int g_=255, int b_=255, int n_=-1, int i_=0):value(RGB(r_,g_,b_)),n(n_){
                if(i_ == 0) i = &COLOR_INTENSITY;
                else i = new int(i_);
            }
            COLOR(int r_, int g_, int b_, int n_, int *i_):value(RGB(r_,g_,b_)),n(n_),i(i_){ }
            
            void intensity(int i_){
                if(i_>3) i_ == 3;
                i = new int(i_);
            }

            void change(int r_, int g_, int b_){
                value.r=r_; value.g=g_; value.b=b_;
            }

            bool operator == (const COLOR& c){
                bool result=true;
                if(result) {if(value == c.value) result=true; else result = false;}
                if(result) {if(*i == *c.i) result=true; else result = false;}
                return result;
            }

        friend void style(COLOR, COLOR);
    };

    COLOR BG_COLOR(0,0,0,0,&COLOR_INTENSITY);

    class THEME{
        public:
            COLOR BLACK;
            COLOR RED;
            COLOR GREEN;
            COLOR YELLOW;
            COLOR BLUE;
            COLOR MAGENTA;
            COLOR CYAN;
            COLOR WHITE;
            THEME(int intensity=0){
                int *intensity_;
                if(intensity==0) intensity_ = &COLOR_INTENSITY; 
                else intensity_ = new int(intensity);

                BLACK = COLOR(0,0,0,0,intensity_);
                RED =  COLOR(255,0,0,1,intensity_);
                GREEN = COLOR(0,255,0,2,intensity_);
                YELLOW = COLOR(255,255,0,3,intensity_);
                BLUE = COLOR(0,0,255,4,intensity_);
                MAGENTA = COLOR(255,0,255,5,intensity_);
                CYAN = COLOR(0,255,255,6,intensity_);
                WHITE = COLOR(255,255,255,7,intensity_);
            }

            COLOR random(bool fast=true){
                COLOR c;
                    if(!fast){fast_srand(time(0));}
                    else{fast_srand(clock());}
                random_restart:
                    int r = fast_rand()%8;
                    switch(r){
                        case 0: c = BLACK; break;
                        case 1: c = RED; break;
                        case 2: c = GREEN; break;
                        case 3: c = YELLOW; break;
                        case 4: c = BLUE; break;
                        case 5: c = MAGENTA; break;
                        case 6: c = CYAN; break;
                        case 7: c = WHITE; break;
                        default: c = WHITE; break;
                    }
                if(c == BG_COLOR) goto random_restart;
                return c;
            }
            
            COLOR random(COLOR notthis, COLOR notthis2=COLOR()){
                COLOR c;
                    fast_srand(clock());
                random_restart_2:
                    int r = fast_rand()%8;
                    switch(r){
                        case 0: c = BLACK; break;
                        case 1: c = RED; break;
                        case 2: c = GREEN; break;
                        case 3: c = YELLOW; break;
                        case 4: c = BLUE; break;
                        case 5: c = MAGENTA; break;
                        case 6: c = CYAN; break;
                        case 7: c = WHITE; break;
                        default: c = WHITE; break;
                    }
                if((c == notthis || c == notthis2) || c == BG_COLOR){ fast_srand(clock()); goto random_restart_2;}
                return c;
            }

            void invert(){
                COLOR temp = BLACK;
                BLACK = WHITE;
                WHITE = temp;
            }
    };

    void init_color(int n, int r, int g, int b, int i){
        if(i<0)i=0; if(i>4)i=4;
        r *= i; g *= i;  b *= i;
        if(r>1000)r=1000; if(g>1000)g=1000; if(b>1000)b=1000;
        ::init_color(n,r,g,b);
    }

    class colorPair{
        public:
            static int colorPairCount;
            COLOR f,b;
            int i;

            colorPair(COLOR f_, COLOR b_, bool temp = false):f(f_),b(b_){
                if(!temp) i = ++colorPairCount;
                else i = 0;
            }

            bool operator == (const colorPair& c){
                bool result=true;
                if(result) {if(f == c.f) result=true; else result = false;}
                if(result) {if(b == c.b) result=true; else result = false;}
                return result;
            }
    };

    int colorPair::colorPairCount = 0;
    std::vector<colorPair> colorPairs;

    int init_pair(COLOR f_, COLOR b_){
        if(colorPairs.size()>256){ 
            colorPairs.clear(); colorPair::colorPairCount = 0;
        }
        
        colorPair temp = colorPair(f_,b_,true);
        std::vector<colorPair>::iterator it;
        it = std::find(colorPairs.begin(), colorPairs.end(), temp);
        
        if ( it != colorPairs.end() ){
            int pos = it - colorPairs.begin();
            return colorPairs[pos].i;
        }else{
            init_color(f_.n,f_.value.r, f_.value.g, f_.value.b, *f_.i);
            init_color(b_.n,b_.value.r, b_.value.g, b_.value.b, *b_.i);
            colorPairs.push_back(colorPair(f_,b_));
            int pos = colorPairs.back().i;
            ::init_pair(pos, f_.n, b_.n);
            return pos;
        }
    }


    void styleON(COLOR cF,COLOR cB = BG_COLOR){
        attron(COLOR_PAIR(init_pair(cF, cB)));
    }

    void styleOFF(COLOR cF,COLOR cB = BG_COLOR){
        attroff(COLOR_PAIR(init_pair(cF, cB)));
    }

    void styleON(int i){
        attron(i);
    }
    void styleOFF(int i){
        attroff(i);
    }

    void styleClear(){
        attrset(0);
    }

    void bgcolor(COLOR cF=BG_COLOR){
        BG_COLOR = cF;
        wbkgd(stdscr, COLOR_PAIR(init_pair(COLOR(0,0,0,0),BG_COLOR)));
    }

    // COLORS END

    // ECS START

    struct POSITION{
        int x=0;
        int y=0;
        POSITION(int x_=0, int y_=0):x(x_),y(y_){ }
    };

    struct SIZE{
        int w=0;
        int h=0;
        SIZE(int w_=0, int h_=0):w(w_),h(h_){ }
    };

    // struct SPRITE{
        
    // };

    enum COL{
        COL_LEFT = 0,
        COL_RIGHT,
        COL_TOP,
        COL_BOTTOM,
        COL_ALL
    };

    class ENTITY{
        public:
            SIZE size;
            POSITION pos;
            const char* sprite[100];
            COLOR mColor;
            bool bounded;
            bool dead;

            ENTITY(int h=0, int w=0, int y=0, int x=0, COLOR color_ = COLOR(), bool bounded_ = false){
                size.w=w;
                size.h=h;
                pos.x=x;
                pos.y=y;
                mColor = color_;
                bounded = bounded_;
                dead=false;
            }

            void bound(bool b=true){
                bounded=b;
            }

            void color(COLOR color_){
                mColor = color_;
            }

            void die(bool b=true){
                dead=b;
            }

            void live(bool b=false){
                dead=b;
            }

            void print(WINDOW &win, COLOR color_ = COLOR(), int y_ = -999, int x_ = -999){
                if(!dead){
                    if(color_.n < 0){
                        color_ = mColor;
                        if(color_.n < 0){
                            color_ = COLOR(256,256,256,7);
                        }
                    }
                    
                    win.updateRowsCols();

                    if(x_ == -999) x_ = pos.x; else{
                        if(bounded){
                            if(0 > x_) x_ = 0;
                            if(win.cols < (x_ + size.w)) x_ = (x_ - ((x_ + size.w) - win.cols));
                        }
                        pos.x = x_;
                    }
                    
                    if(y_ == -999) y_ = pos.y; else{
                        if(bounded){
                            if(0 > y_) y_ = 0;
                            if(win.rows < (y_ + size.h)) y_ = (y_ - ((y_ + size.h) - win.rows));
                        }
                        pos.y = y_;
                    }

                    styleON(color_);
                    for(int h=0;h<size.h;h++){
                        win.print(sprite[h],y_+h,x_);
                    }
                    styleOFF(color_);
                }
            }

            void print(WINDOW &win, int y_, int x_, COLOR color_ = COLOR()){
                print(win, color_, y_, x_);
            }
            void print(WINDOW &win, POS posy, POS posx, COLOR color_ = COLOR()){
                print(win, color_, win.anchorY(posy,size.h), win.anchorX(posx,size.w));
            }
            void print(WINDOW &win, POS posy, int posx, COLOR color_ = COLOR()){
                print(win, color_, win.anchorY(posy,size.h), posx);
            }
            void print(WINDOW &win, int posy, POS posx, COLOR color_ = COLOR()){
                print(win, color_, posy, win.anchorX(posx,size.w));
            }
            void print(WINDOW &win, POS posy, int posx, int offy, COLOR color_ = COLOR()){
                print(win, color_, win.anchorY(posy,size.h,offy), posx);
            }
            void print(WINDOW &win, int posy, POS posx, int offy, COLOR color_ = COLOR()){
                print(win, color_, posy, win.anchorX(posx,size.w,offy));
            }
            void print(WINDOW &win, POS posy, POS posx, int offy, int offx, COLOR color_ = COLOR()){
                print(win, color_, win.anchorY(posy,size.h,offy), win.anchorX(posx,size.w,offx));
            }
            void set(WINDOW &win, POS posy, POS posx, int offy=0, int offx=0){
                pos.y = win.anchorY(posy,size.h,offy);
                pos.x = win.anchorX(posx,size.w,offx);
            }
            void set(WINDOW &win, POS posy, int posx, int offy=0){
                pos.y = win.anchorY(posy,size.h,offy);
                pos.x = posx;
            }
            void set(WINDOW &win, int posy, POS posx, int offx=0){
                pos.y = posy;
                pos.x = win.anchorX(posx,size.w,offx);
            }
            void set(WINDOW &win, int posy, int posx){
                pos.y = posy;
                pos.x = posx;
            }

            static bool checkCollision(ENTITY e1, ENTITY e2, COL col = COL_ALL){
                if(!e1.dead && !e2.dead){
                    switch (col)
                    {
                    case COL_ALL:
                    case COL_LEFT:
                        if(e1.pos.x + e1.size.w == e2.pos.x && (e1.pos.y + e1.size.h >= e2.pos.y && e1.pos.y <= e2.pos.y + e2.size.h)) return true; else if(col != COL_ALL) break;
                    case COL_RIGHT:
                        if(e1.pos.x == e2.pos.x + e2.size.w && (e1.pos.y + e1.size.h >= e2.pos.y && e1.pos.y <= e2.pos.y + e2.size.h)) return true; else if(col != COL_ALL) break;
                    case COL_TOP:
                        if(e1.pos.y + e1.size.h == e2.pos.y && (e1.pos.x + e1.size.w >= e2.pos.x && e1.pos.x <= e2.pos.x + e2.size.w)) return true; else if(col != COL_ALL) break;
                    case COL_BOTTOM:
                        if(e1.pos.y == e2.pos.y + e2.size.h && (e1.pos.x + e1.size.w >= e2.pos.x && e1.pos.x <= e2.pos.x + e2.size.w)) return true; else if(col != COL_ALL) break;
                    default:
                        return false;
                    }
                    return false;
                }
            }
            static bool checkCollision(ENTITY e1,COL col = COL_ALL){
                switch (col)
                {
                case COL_ALL:
                case COL_LEFT:
                    if(e1.pos.x <= 0) return true; else if(col != COL_ALL) break; else return false;
                case COL_RIGHT:
                    if(e1.pos.x + e1.size.w >= COLS) return true; else if(col != COL_ALL) break; else return false;
                case COL_TOP:
                    if(e1.pos.y <= 0) return true; else if(col != COL_ALL) break; else return false;
                case COL_BOTTOM:
                    if(e1.pos.y + e1.size.h >= LINES) return true; else if(col != COL_ALL) break; else return false;
                default:
                    return false;
                }
                return false;
            }
    };


    // ECS END


    #define S_BOLD A_BOLD
    #define S_NORMAL A_NORMAL
    #define S_STANDOUT A_STANDOUT
    #define S_UNDERLINE A_UNDERLINE
    #define S_REVERSE A_REVERSE
    #define S_BLINK A_BLINK
    #define S_DIM A_DIM	
    #define S_BOLD A_BOLD
    #define S_INVIS A_INVIS
    #define S_PROTECT A_PROTECT
    #define S_HORIZONTAL A_HORIZONTAL
    #define S_LEFT A_LEFT
    #define S_LOW A_LOW
    #define S_RIGHT A_RIGHT
    #define S_TOP A_TOP
    #define S_VERTICAL A_VERTICAL

}

#endif // __TERMINO_H__