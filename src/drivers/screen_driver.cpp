#include "screen_driver.h"
#include "../utils/io_utils.cpp"


using namespace PokmonOS;


// Konstruktor – inicjalizuje kursor i ustawia domyślny kolor
ScreenDriver::ScreenDriver() {
    cursor_x = 0;
    cursor_y = 0;
    text_color = 0x07; // Domyślny kolor: szary na czarnym
}

// Czyści ekran i resetuje kursor
void ScreenDriver::clear() {
    for (u32 y = 0; y < SCREEN_HEIGHT; ++y) {
        for (u32 x = 0; x < SCREEN_WIDTH; ++x) {
            volatile u16* screen = reinterpret_cast<volatile u16*>(VGA_MEMORY);
            screen[y * SCREEN_WIDTH + x] = ' ' | (text_color << 8);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    updateCursor();
}

// Zapisuje tekst na ekranie
void ScreenDriver::write(const char* text) {
    while (*text) {
        if (*text == '\n') {
            cursor_y++;
            cursor_x = 0;
        } else {
            whiteChar(*text);
        }
        ++text;
    }
    updateCursor();
}

// Wypisuje pojedynczy znak w białym kolorze
void ScreenDriver::whiteChar(char c) {
    volatile u16* screen = reinterpret_cast<volatile u16*>(VGA_MEMORY);
    screen[cursor_y * SCREEN_WIDTH + cursor_x] = c | (0x0F << 8); // Biały na czarnym
    cursor_x++;

    if (cursor_x >= SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= SCREEN_HEIGHT) {
        cursor_y = SCREEN_HEIGHT - 1;
    }
}

// Ustawia kolor tekstu (np. 0x07 = szary na czarnym)
void ScreenDriver::setColor(u8 color) {
    text_color = color;
}

// Aktualizuje pozycję kursora (na potrzeby emulatorów itp.)
void ScreenDriver::updateCursor() {
    u16 position = cursor_y * SCREEN_WIDTH + cursor_x;

    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)(position >> 8));
    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(position & 0xFF));
}