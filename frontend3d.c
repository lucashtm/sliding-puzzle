#include "raylib.h"
#include "raymath.h"
#include "backend.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define SQUARE_SIZE 2.0f
#define CUBE_THICKNESS 0.3f
#define FONT_SIZE 72
int screenWidth = 800;
int screenHeight = 600;
Camera3D camera = { 0 };
Font fontBig;
Font fontSmall;
Color square_color;

void UpdateDrawFrame(void);     // Update and Draw one frame
void init_camera(void);
void draw_piece(float x_pos, float y_pos, int game_pos);
Texture gen_texture(const char* str, Image* image);

int main() {
  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  fontBig = LoadFontEx("FS Clerkenwell Bold.otf", FONT_SIZE, NULL, 0);
  fontSmall = LoadFontEx("FS Clerkenwell Bold.otf", FONT_SIZE - 12, NULL, 0);
  square_color = GetColor(0x574a8fff);
  init_game();
  init_camera();

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

void init_camera(void) {
  camera.position = (Vector3){ -5.0f, 5.0f, 15.0f };  // Camera position
  camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
  camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
  camera.fovy = 30.0f;                                // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE;
}

void draw_board() {
  for (int i = 0; i < game.size * game.size; i++) {
    int x = i % game.size;
    int y = i / game.size;
    int pos = game.positions[i];
    if (pos != EMPTY) {
      float x_pos = x * SQUARE_SIZE - SQUARE_SIZE * (game.size - 1)/2;
      float y_pos = y * SQUARE_SIZE - SQUARE_SIZE * (game.size - 1)/2;

      draw_piece(x_pos, y_pos, pos);
    }
  }
}

Mesh GenPieceMesh(float width, float height, float length) {
  Mesh cube = GenMeshCube(width, height, length);
  return cube;
}

void draw_piece(float x_pos, float y_pos, int game_pos) {
  Material material = LoadMaterialDefault();
  Matrix transform = { 0 };
  Image image;
  image = GenImageColor(SQUARE_SIZE * 50, SQUARE_SIZE * 50, square_color);
  Vector3 cubePosition = { x_pos, -y_pos, 0.0f };
  float squareDimension = SQUARE_SIZE * 0.9f;
  Mesh piece = GenPieceMesh(squareDimension, squareDimension, CUBE_THICKNESS);
  char* str = (char*)malloc(2);
  sprintf(str, "%c", game.text[game_pos]);
  material.maps[MATERIAL_MAP_DIFFUSE].texture = gen_texture(str, &image);
  DrawMesh(piece, material, MatrixTranslate(cubePosition.x, cubePosition.y, cubePosition.z));
  free(str);
  UnloadImage(image);
  UnloadMaterial(material);
  UnloadMesh(piece);
}

Texture gen_texture(const char* str, Image* image) {
  Vector2 text_dimensions = MeasureTextEx(fontBig, str, FONT_SIZE, 0);
  Vector2 position = {image->height/2.0f - text_dimensions.x/2.0f, image->height/2.0f - text_dimensions.y/2.0f};
  ImageDrawTextEx(image, fontBig, str, position, (float)fontBig.baseSize, 0, RAYWHITE);
  ImageFlipVertical(image);
  return LoadTextureFromImage(*image);
}


void UpdateDrawFrame(void) {
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
        BeginMode3D(camera);
        draw_board();
        EndMode3D();

        if (check_finished()) {
          /* draw_complete_message(); */
        }

    EndDrawing();
    //----------------------------------------------------------------------------------
}
