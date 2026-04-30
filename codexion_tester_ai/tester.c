#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define LOG_PREFIX "helgrind_log_"

bool check_log_for_errors(const char *filename) {
    char command[256];
    // Searches for "ERROR SUMMARY" followed by any number that isn't 0
    snprintf(command, sizeof(command), "grep -q \"ERROR SUMMARY: [1-9]\" %s", filename);
    int ret = system(command);
    return (ret == 0);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <path_to_executable> [number_of_runs]\n", argv[0]);
        return 1;
    }

    

    // define parameters
    char *program_path = argv[1];

    int total_runs = (argc > 2) ? atoi(argv[2]) : 20;

    char *number_of_coders = "10";
    char *time_to_burnout = "310";
    char *time_to_compile = "20";
    char *time_to_debug = "200";
    char *time_to_refactor = "200";
    char *total_compiles = "200";
    char *dongle_cooldown = "200";
    char *scheduler = "fifo";

    printf("🧪 Starting Helgrind Stress Test for: %s\n\n", program_path);
    printf("📋 Parameters in test:\n");
    printf("   - Number_coders = %s\n", number_of_coders);
    printf("   - Time_burnout = %s\n", time_to_burnout);
    printf("   - Time_compile = %s\n", time_to_compile);
    printf("   - Time_debug = %s\n", time_to_debug);
    printf("   - Time_refactor = %s\n", time_to_refactor);
    printf("   - Total_compiles = %s\n", total_compiles);
    printf("   - Dongle_cooldown = %s\n", dongle_cooldown);
    printf("   - Scheduler = %s\n\n", scheduler);
    printf("⚠️  Iterations: %d\n\n", total_runs);

    for (int i = 1; i <= total_runs; i++) {
        char log_name[64];
        snprintf(log_name, sizeof(log_name), "%s%d.txt", LOG_PREFIX, i);
        
        printf("[%02d/%02d] Running... ", i, total_runs);
        fflush(stdout);

        pid_t pid = fork();
        if (pid == 0) { // Child process
            // Note: Adjust the simulation arguments here if needed
            char *args[] = {
                "valgrind", "--tool=helgrind", "--log-file", log_name,
                program_path,
                number_of_coders, 
                time_to_burnout, 
                time_to_compile, 
                time_to_debug, 
                time_to_refactor, 
                total_compiles, 
                dongle_cooldown, 
                scheduler,
                NULL
            };
            execvp("valgrind", args);
            perror("execvp failed");
            exit(1);
        } else { // Parent process
            int status;
            waitpid(pid, &status, 0);

            if (check_log_for_errors(log_name)) {
                printf("\n❌ DATA RACE DETECTED at run %d!\n", i);
                printf("Review detail in: %s\n", log_name);
                return 1;
            } else {
                printf("✅ Clean\n");
                remove(log_name); 
            }
        }
    }

    printf("\n🏆 PASSED: All %d runs were race-free.\n", total_runs);
    return 0;
}
