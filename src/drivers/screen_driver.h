#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H

#include "../include/common/pokmonos.h"

using namespace PokmonOS;

    class ScreenDriver{
        private:
            static const u32 VGA_MEMORY = 0xB8000;

            static const u32 SCREEN_WIDTH = 80;
            static const u32 SCREEN_HEIGHT = 25;


            u32 cursor_x;
            u32 cursor_y;

            u8 text_color;

            void updateCursor();

        public:
            ScreenDriver();

            void clear();

            void write(const char* text);
            void whiteChar(char c);


            void setColor(u8 color);
    };


#endif //SCREEN_DRIVER_H