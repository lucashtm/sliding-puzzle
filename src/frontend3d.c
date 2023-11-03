#include <string.h>
#include "raylib.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "raymath.h"

#include "backend.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define SQUARE_SIZE 2.0f
#define CUBE_THICKNESS 0.3f
#define FONT_SIZE 256
int screenWidth = 800;
int screenHeight = 600;
Camera3D camera = { 0 };
Font fontBig;
Font fontSmall;
Color square_color;
Shader shader;
Image image;
Model pieceModel;

struct Piece {
  int position;
  char character;
  Texture2D* texture;
};
typedef struct Piece Piece;

Piece* pieces;
Texture2D* textures;
void UpdateDrawFrame(void);     // Update and Draw one frame
void init_camera(void);
void gen_pieces(void);
void draw_piece(float x_pos, float y_pos, int game_pos, size_t index);
Texture2D gen_char_texture(const char character);


int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);


  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  pieceModel = LoadModel("piece.obj");
  fontBig = LoadFontEx("FS Clerkenwell Bold.otf", FONT_SIZE, NULL, 0);
  fontSmall = LoadFontEx("FS Clerkenwell Bold.otf", FONT_SIZE - 12, NULL, 0);
  square_color = GetColor(0x574a8fff);

  image = GenImageColor(SQUARE_SIZE * 512, SQUARE_SIZE * 512, square_color);
  /* shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION), */
                               /* TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION)); */
  init_game();
  gen_pieces();
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
  UnloadModel(pieceModel);
  UnloadImage(image);
  UnloadFont(fontBig);
  UnloadFont(fontSmall);
  free(textures); // TODO: Not sure if need to free each texture individually
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

void gen_pieces() {
  pieces = (Piece*)malloc(sizeof(Piece) * game.size * game.size);

  // Figure out unique characters in the game text
  char* unique = (char*)malloc(sizeof(char) * game.size * game.size - 1);
  memset(unique, 0, sizeof(char) * game.size * game.size - 1);

  int unique_count = 0;
  for(size_t i = 0; i < game.size*game.size-1; i++) {
    pieces[i].character = game.text[i];
    pieces[i].position = i;
    for(size_t j = 0; j < game.size*game.size-1; j++) {
      if (unique[j] == 0) {
        unique[j] = game.text[i];
        Texture2D texture = gen_char_texture(game.text[i]);
        pieces[i].texture = (Texture2D*)malloc(sizeof(Texture2D));
        *pieces[i].texture = texture;
        /* UnloadTexture(texture); */
        unique_count++;
        break;
      }
      if (game.text[i] == unique[j]) {
        pieces[i].texture = pieces[j].texture;
        break;
      }
    }
  }

  printf("UNIQUE CHARS: %s\n", unique);


  free(unique);
}

void draw_board() {
  for (size_t i = 0; i < game.size * game.size; i++) {
    int x = i % game.size;
    int y = i / game.size;
    int pos = game.positions[i];
    if (pos != EMPTY) {
      float x_pos = x * SQUARE_SIZE - SQUARE_SIZE * (game.size - 1)/2;
      float y_pos = y * SQUARE_SIZE - SQUARE_SIZE * (game.size - 1)/2;

      draw_piece(x_pos, y_pos, pos, i);
    }
  }
}

/* Mesh GenPieceMesh(float width, float height, float length) { */
  /* Mesh cube = GenMeshCube(width, height, length); */
  /* return cube; */
/* } */

void draw_piece(float x_pos, float y_pos, int game_pos, size_t index) {
  Matrix transform = { 0 };
  Vector3 cubePosition = { x_pos, -y_pos, 0.0f };
  SetMaterialTexture(&pieceModel.materials[0], MATERIAL_MAP_DIFFUSE, *pieces[game_pos].texture);
  DrawModel(pieceModel, cubePosition, 1.0f, WHITE);
  DrawModelWires(pieceModel, cubePosition, 1, BLACK);
}

Texture2D gen_char_texture(const char character) {
  char* str = (char*)malloc(2);
  sprintf(str, "%c", character);
  Vector2 text_dimensions = MeasureTextEx(fontBig, str, FONT_SIZE, 0);
  Vector2 position = {image.width/4.0f - text_dimensions.x/2.0f, image.height/4.0f - text_dimensions.y/2.0f};
  ImageDrawRectangle(&image, 0, 0, image.width/2.0f, image.height/2.0f, square_color);
  ImageDrawTextEx(&image, fontBig, str, position, (float)fontBig.baseSize, 0, RAYWHITE);

  // DEBUG
  /* Rectangle rect = {position.x, position.y, text_dimensions.x, text_dimensions.y}; */
  /* ImageDrawRectangleLines(&image, rect, 5, BLACK); */
  /* ImageDrawCircle(&image, image.width/4.0f, image.height/4.0f, 5, GREEN); */
  /* ImageDrawLine(&image, image.width/4.0f, 0, image.width/4.0f, image.height-1, GREEN); */
  /* ImageDrawLine(&image, image.width/2.0f, 0, image.width/2.0f, image.height-1, GREEN); */
  /* ImageDrawLine(&image, 3*image.width/4.0f, 0, 3*image.width/4.0f, image.height-1, GREEN); */
  /* ImageDrawLine(&image, 0, image.height/4.0f, image.width-1, image.height/4.0f, GREEN); */
  /* ImageDrawLine(&image, 0, image.height/2.0f, image.width-1, image.height/2.0f, GREEN); */
  /* ImageDrawLine(&image, 0, 3*image.height/4.0f, image.width-1, 3*image.height/4.0f, GREEN); */
  /* ExportImage(image, TextFormat("test_%s.png", str)); */
  // END DEBUG
  free(str);
  return LoadTextureFromImage(image);
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
