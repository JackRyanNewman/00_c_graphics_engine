/*┌─────────────────────────────────────────────────────────────────────────────┐
  │ Engine - Main Entry Point                                                   │
  │   • © 2025 Jack Ryan Newman                                                 │
  ├─────────────────────────────────────────────────────────────────────────────┤
  └─────────────────────────────────────────────────────────────────────────────┘*/

/*=================================================================================================*/
/* Includes */
#include "0_engine/0_engine/engine.h"
#include "0_engine/6_debugger/debugger.h"
#define NUM_THREADS 4
/*=================================================================================================*/
/* Main Entry Point */

int main() {
    omp_set_num_threads(4);
    printf("\n=== OpenGL 3D Graphics Engine ===\n");
    printf(" - Controls: Arrow keys to adjust camera\n");
    printf(" - Using OpenMP with %d threads\n\n", omp_get_max_threads());
    
    //Most likely some subroutines, to grab all scenes and write static data. 
    //Meta-compiling phase

    val_exit_error_1(!engine_start_up(), "Failed to initialize engine\n");
    return 0;
}