# Boids Algorithm

An implementation of Boids Algorithm which was developed by Craig Reynolds.
It simulates the flocking behaviour of birds

## QUICK START

Dependencies

- [raylib](https://www.raylib.com/)

## TWEAKS

```c
    float avoid_factor = 0.01f;
    float alignment_factor = 0.35f;
    float cohesion_factor = 0.02f;
```
https://github.com/user-attachments/assets/baebad49-9c12-4e7b-82d6-1c6f76ae7be8

### QUICK START

```console
$ gcc main.c -o main -lraylib -lm
$ ./main
```

### QUICK START

    SHORT_CUT_KEYS              FUNCTION
        L                       Toggle the distance between every boid within the radius
        R                       Toggle the radius for every boid
        1                       Decrement separation_factor
        2                       Increment separation_factor
        3                       Decrement alignment_factor
        4                       Increment alignment_factor
        5                       Decrement cohesion_factor
        6                       Increment cohesion_factor
