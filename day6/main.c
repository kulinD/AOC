#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define NUM_RACES 4

typedef struct {
    double time;
    double dist;
} Race;

Race races[NUM_RACES] = {
    {47, 282},
    {70, 1079},
    {75, 1147},
    {66, 1062}
};

// Calculate min/max hold times using quadratic formula and determine number of valid integer strategies
int64_t count_winning_ways(double race_time, double previous_record) {
    double a = -1.0;
    double b = race_time;
    double c = -previous_record;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0) {
        return 0; // No real roots
    }

    double sqrt_discriminant = sqrt(discriminant);

    // floating point needed for accurate calculation
    double root1 = (-b + sqrt_discriminant) / (2.0 * a);
    double root2 = (-b - sqrt_discriminant) / (2.0 * a);

    // Ensure root1 is the smaller and root2 the larger
    if (root1 > root2) {
        double temp = root1;
        root1 = root2;
        root2 = temp;
    }

    // Get valid integer bounds between the roots
    int64_t min_hold = (int64_t)(floor(root1)) + 1;
    int64_t max_hold = (int64_t)(ceil(root2)) - 1;

    if (max_hold < min_hold) {
        return 0;
    }

    return max_hold - min_hold + 1;
}

int main(void) {
    int64_t total_ways = 1;

    for (int i = 0; i < NUM_RACES; ++i) {
        int64_t ways = count_winning_ways(races[i].time, races[i].dist);
        total_ways = total_ways * ways;
    }

    printf("Part1: %lld\n", total_ways);

    // Part 2: Using concatenated values
    int64_t big_time = 47707566;
    int64_t big_record = 282107911471062;

    int64_t big_ways = count_winning_ways(big_time, big_record);
    printf("Part2: %lld\n", big_ways);

    return 0;
}