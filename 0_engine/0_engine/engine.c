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
#include "../3_camera/camera.h"

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
    
    // Enable VSync - locks frame rate to monitor refresh rate (60Hz, 144Hz, etc.)
    glfwSwapInterval(1);  // 1 = vsync enabled, 0 = vsync disabled
    
    double last_time = glfwGetTime();  // Time of previous frame (seconds since startup)

    do {
        double now = glfwGetTime();    // Current time (seconds since startup)
        double dt = now - last_time;   // Delta time = elapsed seconds since last frame
        last_time = now;               // Update for next iteration

        // Cap dt to prevent huge jumps (e.g., if window was dragged)
        if (dt > 0.1) dt = 0.1;        // Max 100ms per frame (prevents large dt spikes)
        
        // Handle WASD camera movement
        float move_speed = 1.0f;       // Units per second
        float move_distance = move_speed * (float)dt;  // Distance to move this frame
        
        Vec3 current_eye = camera_get_eye();
        // W = Move forward(toward center), Move closer to center (z decreases)
        // S = Move backward (away from center) Move away from center (z increases)
        if (glfwGetKey(engine_get_window(), GLFW_KEY_W) == GLFW_PRESS) current_eye.z -= move_distance;
        if (glfwGetKey(engine_get_window(), GLFW_KEY_S) == GLFW_PRESS) current_eye.z += move_distance;

        // A = Move left  Move left (x decreases)
        // D = Move right Move right (x increases)
        if (glfwGetKey(engine_get_window(), GLFW_KEY_A) == GLFW_PRESS) {
            current_eye.x -= move_distance;
        }
        if (glfwGetKey(engine_get_window(), GLFW_KEY_D) == GLFW_PRESS) current_eye.x += move_distance;
        
        camera_set_eye(current_eye);
        
        scene_update((float)dt);       // Pass dt so movement/physics scale with real time
        renderer_render_frame();       // Render the scene

        glfwSwapBuffers(window);       // Display frame (blocks until vsync if enabled)
        glfwPollEvents();              // Check for keyboard/mouse input

    } while (!glfwWindowShouldClose(window));  // Loop until window close requested
    
    if (DEBUG_MODE) printf("* Engine stopped\n");
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