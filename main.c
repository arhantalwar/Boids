#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <raymath.h>

#define GET_VAR(x, y) x##y

#define WIDTH   1760
#define HEIGHT  990

#define MAX_BOIDS 200
#define NUM_BOIDS 200
#define MAX_SPEED 5

#define BOID_RADIUS 50

static int boid_count = 0;

typedef struct Boid {
    Vector2 position;
    float radius;
    float speed_x;
    float speed_y;
    Color color;
} Boid;

Boid* get_boid() {
    Boid* b = (Boid*) malloc(sizeof(Boid));
    return b;
}

void init_boid(Boid* b, Boid** boids_arr) {

    Color colors[] = { MAROON, BLUE, PINK, ORANGE };
    boids_arr[boid_count++] = b;

    b->radius = BOID_RADIUS;
    b->position.x = rand() % WIDTH;
    b->position.y = rand() % HEIGHT;
    b->speed_x = (rand() % (2 * MAX_SPEED)) - MAX_SPEED;
    b->speed_y = (rand() % (2 * MAX_SPEED)) - MAX_SPEED;
    b->color = colors[rand() % 4];

}

int main() {

    srand(time(NULL));

    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);
    InitWindow(WIDTH, HEIGHT, "BOIDS");

    Boid* boids_arr[MAX_BOIDS];

    float separation_factor = 0.01f;
    float alignment_factor = 0.5f;
    float cohesion_factor = 0.009f;

    bool draw_dist = false;
    bool draw_radius = false;

    // Generate Boids
    for(int i = 0; i < NUM_BOIDS; i++) {
        Boid* GET_VAR(b, i) = get_boid();
        init_boid(GET_VAR(b, i), boids_arr);
    }

    while(!WindowShouldClose()) {

        if (IsKeyPressed(KEY_L)) {
            draw_dist = !draw_dist;
        }

        if (IsKeyPressed(KEY_R)) {
            draw_radius = !draw_radius;
        }

        if (IsKeyPressed(KEY_ONE)) {
            separation_factor -= 0.001;
            printf("[-] SEPARATION: %f\n", separation_factor);
        }

        if (IsKeyPressed(KEY_TWO)) {
            separation_factor += 0.001;
            printf("[+] SEPARATION: %f\n", separation_factor);
        }

        if (IsKeyPressed(KEY_THREE)) {
            alignment_factor -= 0.001;
            printf("[-] ALIGNMENT: %f\n", alignment_factor);
        }

        if (IsKeyPressed(KEY_FOUR)) {
            alignment_factor += 0.001;
            printf("[+] ALIGNMENT: %f\n", alignment_factor);
        }

        if (IsKeyPressed(KEY_FIVE)) {
            cohesion_factor -= 0.001;
            printf("[-] COHESION: %f\n", cohesion_factor);
        }

        if (IsKeyPressed(KEY_SIX)) {
            cohesion_factor += 0.001;
            printf("[+] COHESION: %f\n", cohesion_factor);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for(int i = 0; i < boid_count; i++) {

            float avg_speed_x = 0.0f;
            float avg_speed_y = 0.0f;
            int avg_speed_count = 0;

            float avg_position_x = 0.0f;
            float avg_position_y = 0.0f;
            int avg_position_count = 0;

            // Drawing Boids

            DrawCircle(boids_arr[i]->position.x, boids_arr[i]->position.y, 7, boids_arr[i]->color);

            Vector2 direction = Vector2Normalize((Vector2){boids_arr[i]->speed_x, boids_arr[i]->speed_y});
            Vector2 end = Vector2Add(boids_arr[i]->position, Vector2Scale(direction, 20));
            DrawLineEx(boids_arr[i]->position, end, 10, boids_arr[i]->color);

            if (draw_radius) {
                DrawCircleLines(boids_arr[i]->position.x, boids_arr[i]->position.y, boids_arr[i]->radius, boids_arr[i]->color);
            }

            // Implementing Rules

            for(int j = 0; j < boid_count; j++) {

                if (i != j) {
                    bool in_range = CheckCollisionPointCircle(boids_arr[i]->position, boids_arr[j]->position, boids_arr[j]->radius);
                    if (in_range) {

                        if (draw_dist) DrawLineEx(boids_arr[i]->position, boids_arr[j]->position, 1.0f, RAYWHITE);

                        // RULE 1 : SEPARATION
                        Vector2 move = Vector2Subtract(boids_arr[i]->position, boids_arr[j]->position);

                        boids_arr[i]->speed_x += move.x * separation_factor;
                        boids_arr[i]->speed_y += move.y * separation_factor;

                        avg_speed_x += boids_arr[j]->speed_x;
                        avg_speed_y += boids_arr[j]->speed_y;
                        avg_speed_count++;

                        avg_position_x += boids_arr[j]->position.x;
                        avg_position_y += boids_arr[j]->position.y;
                        avg_position_count++;

                    }
                }
            }

            Vector2 velocity = { boids_arr[i]->speed_x, boids_arr[i]->speed_y };

            if (Vector2Length(velocity) > MAX_SPEED) {
                velocity = Vector2Scale(Vector2Normalize(velocity), MAX_SPEED);
                boids_arr[i]->speed_x = velocity.x;
                boids_arr[i]->speed_y = velocity.y;
            }

            // RULE 2 : ALIGNMENT
            if (avg_speed_count > 0) {
            
                avg_speed_x /= avg_speed_count;
                avg_speed_y /= avg_speed_count;
            
                boids_arr[i]->speed_x += (avg_speed_x - boids_arr[i]->speed_x) * alignment_factor;
                boids_arr[i]->speed_y += (avg_speed_y - boids_arr[i]->speed_y) * alignment_factor;
            
            }


            // RULE 3 : COHESION
            if (avg_position_count > 0) {
            
                avg_position_x /= avg_position_count;
                avg_position_y /= avg_position_count;
            
                Vector2 direction_to_center = {
                    .x = avg_position_x - boids_arr[i]->position.x,
                    .y = avg_position_y - boids_arr[i]->position.y
                };
            
                boids_arr[i]->speed_x += direction_to_center.x * cohesion_factor;
                boids_arr[i]->speed_y += direction_to_center.y * cohesion_factor;
            
            }

            // Bound check

            if (boids_arr[i]->position.x < 0) boids_arr[i]->position.x = WIDTH;
            if (boids_arr[i]->position.x > WIDTH) boids_arr[i]->position.x = 0;
            if (boids_arr[i]->position.y < 0) boids_arr[i]->position.y = HEIGHT;
            if (boids_arr[i]->position.y > HEIGHT) boids_arr[i]->position.y = 0;

            boids_arr[i]->position.x += boids_arr[i]->speed_x * alignment_factor;
            boids_arr[i]->position.y += boids_arr[i]->speed_y * alignment_factor;

        }

        EndDrawing();

    }

    CloseWindow();

    // Free Boids
    for(int i = 0; i < boid_count; i++) {
        free(boids_arr[i]);
    }

    return 0;

}
