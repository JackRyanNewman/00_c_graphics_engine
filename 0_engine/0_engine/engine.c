/*┌─────────────────────────────────────────────────────────────────────────────┐
  │ CNN - Engine Core                                                           │
  │   • © 2025 Jack Ryan Newman                                                 │
  ├─────────────────────────────────────────────────────────────────────────────┤
  └─────────────────────────────────────────────────────────────────────────────┘*/

/*=================================================================================================*/
/* Includes */
#include "engine.h"
#include "../1_renderer/renderer.h"
#include "../4_input/input.h"
#include "../6_debugger/debugger.h"

/*=================================================================================================*/
/* Internal State */
static GLFWwindow* window;


/*=================================================================================================*/
/* Engine Lifecycle */
bool engine_start_up(){    
    if (DEBUG_MODE) printf("* Engine: Intilization\n");
    val_crit_error_1(!engine_init_backend(),0, "Failed to Intilize graphics libaries GFLW and GL"); 
    val_crit_error_1(!renderer_load_scene(1), 0, "Failed to load start screen\n");
    engine_run(); 
    engine_clean();
    return 0;
}


bool engine_init_backend(){
    //GL and GFLW Intilizations    
    val_crit_error_1(!glfwInit(), 0, "Failed to initialize GLFW\n");            // Initialize GLFW library; fail if it returns false
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                              // Request OpenGL version 3.x (major)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                              // Request OpenGL version x.3 (minor)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);              // Request core profile (no deprecated functions)

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Graphics Engine", NULL, NULL); // Create a window and OpenGL context
    val_crit_error_2(!window, 0, glfwTerminate(), "Failed to create GLFW window\n");              // Check if window creation failed; terminate GLFW if so
    glfwMakeContextCurrent(window);                                                               // Make the window's OpenGL context current on this thread
    glfwSetFramebufferSizeCallback(window, input_framebuffer_callback);                           // Set callback for window resize events (adjust viewport)

    glewExperimental = GL_TRUE;                                               // Enable modern OpenGL function loading in GLEW
    val_crit_error_1(glewInit() != GLEW_OK, 0,"Failed to initialize GLEW\n"); // Initialize GLEW; fail if initialization fails

    // Set up OpenGL state
    
    glEnable(GL_DEPTH_TEST);                        //Ensures layers are resepecte. Things closer to camera will appear over. IF false, it will render most recently drawn first. 
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  //Defines window area. 
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black  //glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //Sets background, to colors. 
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    return 1;
}

void engine_run() {
    if (DEBUG_MODE) printf("* Start engine\n");
    
    
    if (DEBUG_MODE) printf("* Start engine\n");
    double last_time = glfwGetTime();

    do {
        double now = glfwGetTime();
        double dt = now - last_time;
        last_time = now;

        // Cap dt to prevent huge jumps (e.g., if window was dragged)
        if (dt > 0.1) dt = 0.1;  // Max 100ms per frame
        
        scene_update((float)dt);
        renderer_render_frame();

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (!glfwWindowShouldClose(window));
       
  
    
    
}

//Waiting options
// - glfwWaitEvents(); waits until IO eeveent? thread block. 
// - glfwPollEvents(): Unwaiting check on IO eevents
// - glfwSwapBuffers(); Shows the frame you just rendered  
    // - IF glfwSwapInterval(1), is set then sets interval and makees swapbbufferway.




void engine_clean() {
    renderer_cleanup();
    scene_cleanup();
    
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

/*=================================================================================================*/
/* Window Management */
GLFWwindow* engine_get_window() {return window;}

//PHYSICS!
    //   double last_time = glfwGetTime();
    //     const double fixed_dt = 1.0 / 60.0;
    //     const double max_accumulator = fixed_dt * 5;  // Max 5 physics frames per render
    //     double accumulator = 0.0;

    //     do {
    //         double now = glfwGetTime();
    //         double frame_dt = now - last_time;
    //         last_time = now;

    //         accumulator += frame_dt;
    //         if (accumulator > max_accumulator) accumulator = max_accumulator;  // Cap it

    //         while (accumulator >= fixed_dt) {
    //             scene_update((float)fixed_dt);
    //             accumulator -= fixed_dt;
    //         }

    //         renderer_render_frame();

    //         glfwSwapBuffers(window);
    //         glfwPollEvents();

    //     } while (!glfwWindowShouldClose(window));
    // }