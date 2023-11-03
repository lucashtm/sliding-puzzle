/*******************************************************************************************
*
*   raylib [core] example - Basic window (adapted for HTML5 platform)
*
*   This example is prepared to compile for PLATFORM_WEB and PLATFORM_DESKTOP
*   As you will notice, code structure is slightly different to the other examples...
*   To compile it for PLATFORM_WEB just uncomment #define PLATFORM_WEB at beginning
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "backend.h"
#include <stdio.h>
#include <unistd.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 800;
int screenHeight = 450;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void);     // Update and Draw one frame

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------

#define SQUARE_SIZE 100
#define FONT_SIZE 72
Font fontBig;
Font fontSmall;
Color square_color;

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    fontBig = LoadFontEx("FS Clerkenwell Bold.otf", FONT_SIZE, NULL, 0);
    fontSmall = LoadFontEx("FS Clerkenwell Bold.otf", FONT_SIZE - 12, NULL, 0);
    square_color = GetColor(0x574a8fff);

    init_game();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

void draw_board() {
  short margin = SQUARE_SIZE / 10;
  for (int i = 0; i < game.size * game.size; i++) {
    int x = i % game.size;
    int y = i / game.size;
    int pos = game.positions[i];
    if (pos != EMPTY) {
      int x_pos = x * SQUARE_SIZE + margin * x;
      int y_pos = y * SQUARE_SIZE + margin * y;
      DrawRectangle(x_pos, y_pos, SQUARE_SIZE, SQUARE_SIZE, square_color);
      char* str = (char*)malloc(2);
      sprintf(str, "%c", game.text[pos]);
      Vector2 text_dimensions = MeasureTextEx(fontBig, str, FONT_SIZE, 0);
      Vector2 position = {x_pos + SQUARE_SIZE/2.0f - text_dimensions.x/2.0f, y_pos + SQUARE_SIZE/2.0f - text_dimensions.y/2.0f};
      DrawTextEx(fontBig, str, position, (float)fontBig.baseSize, 0, RAYWHITE);
      free(str);
    }
  }
}

void draw_complete_message() {
  int x = game.size * SQUARE_SIZE + game.size * SQUARE_SIZE / 10;
  Vector2 position = {x, 200};
  DrawTextEx(fontSmall, "Resolvido!", position, FONT_SIZE - 12, 0, LIGHTGRAY);
}

void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    if (IsKeyPressed(KEY_DOWN)) {
      move_down();
    } else if (IsKeyPressed(KEY_UP)) {
      move_up();
    } else if (IsKeyPressed(KEY_LEFT)) {
      move_left();
    } else if (IsKeyPressed(KEY_RIGHT)) {
      move_right();
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        if (check_finished()) {
          draw_complete_message();
        }
        draw_board();

    EndDrawing();
    //----------------------------------------------------------------------------------
}
