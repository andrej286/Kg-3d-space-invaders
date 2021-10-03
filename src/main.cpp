#include <OpenGLPrj.hpp>

#include <GLFW/glfw3.h>

#include <Camera.hpp>
#include <Shader.hpp>

#include <iostream>
#include <string>
#include <vector>

const std::string program_name = ("Camera");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// timing
static float deltaTime = 0.0f; // time between current frame and last frame
static float lastFrame = 0.0f;

 float moveY = 0.0f;
 float moveX = 0.0f;

 //brojaci za sekoj enemy posebno
 //za sekoj enemy shto go dodavash mora da stavish i za nego brojac
 float enemyMoveZ[] = {
   0.0f,//1
   0.0f,//2
   0.0f,//3
   0.0f,//4
   0.0f,//5
   0.0f,//6
   0.0f,//7
   0.0f,//8
   0.0f,//9
   0.0f,//10
 };

  // float  allZ[] = {
  //         -70.0f,  //1
  //     -50.0f,  //2
  //       -220.5f,  //3
  //      -142.3f,  //4
  //    -130.5f,  //5
  //     -170.5f,  //6
  //      -320.5f,  //7
  //     -120.5f,  //8
  //       -190.5f,  //9
  //     -150.5f  //10
  //   };


    float shipL = -3.0f; //momentalno leva granica na brodot
     float shipR = 3.0f; //momentalno desna granica na brodot
      float shipU = -2.0f; //momentalno gorna granica na brodot
       float shipD = -4.0f;  //momentalno dolna granica na brodot

  bool endOfGame = false; //dali zavrshi igrata 

  

 //float vrakjaj = 1.0f;

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                        program_name.c_str(), nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile our shader program
  // ------------------------------------
  std::string shader_location("../res/shaders/");

  std::string used_shaders("shader");

  Shader ourShader(shader_location + used_shaders + std::string(".vert"),
                   shader_location + used_shaders + std::string(".frag"));

   //SHIP shder
  std::string used_shipshaders("shipshader");

  Shader ourShipShader(shader_location + used_shipshaders + std::string(".vert"),
                   shader_location + used_shipshaders + std::string(".frag"));

  //ENEMY shader
  std::string used_enemyshaders("enemy");

  Shader ourEnemyShader(shader_location + used_enemyshaders + std::string(".vert"),
                   shader_location + used_enemyshaders + std::string(".frag"));

  //END shader
  std::string used_endshaders("end");

  Shader ourEndShader(shader_location + used_endshaders + std::string(".vert"),
                   shader_location + used_endshaders + std::string(".frag"));


  

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      -40.5f, -40.5f, -0.5f, 0.0f, 0.0f, 40.5f,  -40.5f, -0.5f, 1.0f, 0.0f,
      40.5f,  40.5f,  -0.5f, 1.0f, 1.0f, 40.5f,  40.5f,  -0.5f, 1.0f, 1.0f,
      -40.5f, 40.5f,  -0.5f, 0.0f, 1.0f, -40.5f, -40.5f, -0.5f, 0.0f, 0.0f,
      };

  float endvertices[] = {
     -1.5f, -1.5f, -0.5f, 0.0f, 0.0f,
      1.5f,  -1.5f, -0.5f, 1.0f, 0.0f,
      1.5f,  1.5f,  -0.5f, 1.0f, 1.0f, 
      1.5f,  1.5f,  -0.5f, 1.0f, 1.0f,
      -1.5f, 1.5f,  -0.5f, 0.0f, 1.0f, 
      -1.5f, -1.5f, -0.5f, 0.0f, 0.0f,
  };

   float daleku = -30.0f; //-30.0f

  float shipvertices[] {

    //klun gore
    // positions         // colors
     1.0f, -2.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom right
    -1.0f, -2.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom left
     -0.0f,  -4.0f, -12.0f + daleku,  1.0f, 1.0f, 0.0f,    // top 
    //klun levo
       -1.5f, -3.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom right
    -1.0f, -2.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom left
     -0.0f,  -4.0f, -12.0f + daleku,  1.0f, 1.0f, 0.0f,    // top 
     //klun desno
        1.5f, -3.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom right
    1.0f, -2.5f , -10.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom left
     -0.0f,  -4.0f, -12.0f + daleku,  1.0f, 1.0f, 0.0f,    // top 
       //klun dole
        1.5f, -3.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom right
     -1.5f, -3.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom right
     -0.0f,  -4.0f, -12.0f + daleku,  1.0f, 1.0f, 0.0f,    // top 

     //telo gore 1

        0.8f, -2.7f, -10.0f + daleku,  0.45f, 0.45f, 0.45f,   // bottom right
    -0.8f, -2.7f, -10.0f + daleku,   0.45f, 0.45f, 0.45f,   // bottom left
     -0.8f,  -2.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 

     //telo gore 2

        0.8f, -2.7f, -10.0f + daleku,   0.45f, 0.45f, 0.45f,   // bottom right
    -0.8f, -2.7f, -7.0f + daleku,    0.45f, 0.45f, 0.45f,   // bottom left
     0.8f,  -2.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 



     //telo gore gore 1

        0.5f, -2.68f, -9.0f + daleku,  0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -2.68f, -9.0f + daleku,    0.0f, 1.0f, 0.0f,   // bottom left
     -0.5f,  -2.68f, -7.0f + daleku,   0.0f, 1.0f, 0.0f,    // top 

     //telo gore gore 2

        0.5f, -2.68f, -9.0f + daleku,   0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -2.68f, -7.0f + daleku,    0.0f, 1.0f, 0.0f,   // bottom left
     0.5f,  -2.68f, -7.0f + daleku,   0.0f, 1.0f, 0.0f,    // top 


      //telo gore gore gore 1

        0.6f, -2.69f, -9.5f + daleku,  0.15f, 0.15f, 0.15f,   // bottom right
    -0.6f, -2.69f, -9.5f + daleku,    0.15f, 0.15f, 0.15f,   // bottom left
     -0.6f,  -2.69f, -7.0f + daleku,   0.15f, 0.15f, 0.15f,  // top 

     //telo gore gore gore 2

        0.6f, -2.69f, -9.5f + daleku,  0.15f, 0.15f, 0.15f,  // bottom right
    -0.6f, -2.69f, -7.0f + daleku,    0.15f, 0.15f, 0.15f,   // bottom left
     0.6f,  -2.69f, -7.0f + daleku,  0.15f, 0.15f, 0.15f,    // top 




      //telo levo 1

           -1.3f, -3.7f, -10.0f + daleku,   0.45f, 0.45f, 0.45f,   // bottom right
    -0.8f, -2.7f, -10.0f + daleku,   0.45f, 0.45f, 0.45f,   // bottom left
     -0.8f,  -2.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 

       //telo levo 2

           -1.3f, -3.7f, -10.0f + daleku,  0.45f, 0.45f, 0.45f,   // bottom right
        -1.3f, -3.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,   // bottom right
     -0.8f,  -2.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 


       //telo desno 2

         1.3f, -3.7f, -10.0f + daleku, 0.45f, 0.45f, 0.45f,   // bottom right
      0.8f, -2.7f, -10.0f + daleku,  0.45f, 0.45f, 0.45f,   // bottom right
     0.8f,  -2.7f, -7.0f + daleku,  0.45f, 0.45f, 0.45f,    // top 
      //telo desno 2

         1.3f, -3.7f, -7.0f + daleku,  0.45f, 0.45f, 0.45f,   // bottom right
      1.3f, -3.7f, -10.0f + daleku,  0.45f, 0.45f, 0.45f,  // bottom right
     0.8f,  -2.7f, -7.0f + daleku,  0.45f, 0.45f, 0.45f,    // top 

      //telo dole 1
   
        1.3f, -3.7f, -10.0f + daleku,   0.45f, 0.45f, 0.45f,   // bottom right
    -1.3f, -3.7f, -10.0f + daleku,  0.45f, 0.45f, 0.45f,   // bottom left
     -1.3f, -3.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 

     //telo dole 1
   
        1.3f, -3.7f, -10.0f + daleku,   0.45f, 0.45f, 0.45f,   // bottom right
    -1.3f, -3.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,   // bottom left
     1.3f, -3.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 

      //telo pozadi 1

         1.3f, -3.7f, -7.0f + daleku,  0.45f, 0.45f, 0.45f,   // bottom right
       -1.3f, -3.7f, -7.0f + daleku,  0.45f, 0.45f, 0.45f,  // bottom right
     0.8f,  -2.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 

      //telo pozadi 2

       -1.3f, -3.7f, -7.0f + daleku,  0.45f, 0.45f, 0.45f,   // bottom right
     -0.8f,  -2.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 
   0.8f,  -2.7f, -7.0f + daleku,   0.45f, 0.45f, 0.45f,    // top 




    //ogan pozadi 1

         1.0f, -3.5f, -6.9f + daleku,  1.0f, 0.2f, 0.0f,  // bottom right
       -1.0f, -3.5f, -6.9f + daleku,1.0f, 0.2f, 0.0f,    // bottom right
     0.8f,  -2.9f, -6.9f + daleku, 1.0f, 0.2f, 0.0f,     // top 

      //ogan pozadi 2

       -1.0f, -3.5f, -6.9f + daleku,  1.0f, 0.2f, 0.0f,    // bottom right
     -0.8f,  -2.9f, -6.9f + daleku, 1.0f, 0.2f, 0.0f,     // top 
   0.8f,  -2.9, -6.9f + daleku, 1.0f, 0.2f, 0.0f,     // top 

      //plamen pozadi 1

         0.7f, -3.3f, -6.8f + daleku,  0.956f, 0.949f, 0.145f,   // bottom right
       -0.7f, -3.3f, -6.8f + daleku, 0.956f, 0.949f, 0.145f,   // bottom right
     0.6f,  -3.0f, -6.8f + daleku,  0.956f, 0.949f, 0.145f,  // top 

      //plamen pozadi 2

       -0.7f, -3.3f, -6.8f + daleku,   0.956f, 0.949f, 0.145f,   // bottom right
     -0.6f,  -3.0f, -6.8f + daleku,  0.956f, 0.949f, 0.145f,   // top 
   0.6f,  -3.0, -6.8f + daleku, 0.956f, 0.949f, 0.145f,   // top 




   //levo krilo 1

       -3.0f, -3.7f, -7.0f + daleku,  1.0f, 1.0f, 0.0f,   // bottom right
     -0.8f,  -3.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,    // top 
   -1.0f,  -3.5f, -7.0f + daleku,  0.0f, 1.0f, 0.0f,    // top 

   //levo krilo 2

       -3.0f, -3.0f, -7.0f + daleku,  1.0f, 1.0f, 0.0f,   // bottom right
       -0.8f,  -3.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,    // top 
   -0.6f,  -3.0f, -7.0f + daleku,  0.0f, 1.0f, 0.0f,    // top 

    //desno krilo 1

       3.0f, -3.5f, -7.0f + daleku,  1.0f, 1.0f, 0.0f,   // bottom right
     0.8f,  -3.7f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,    // top 
   1.0f,  -3.5f, -7.0f + daleku,  0.0f, 1.0f, 0.0f,    // top 

    //desno krilo 2

       3.0f, -3.0f, -7.0f + daleku,  1.0f, 1.0f, 0.0f,   // bottom right
       0.8f,  -3.5f, -10.0f + daleku,  0.0f, 1.0f, 0.0f,    // top 
   0.6f,  -3.0f, -7.0f + daleku,  0.0f, 1.0f, 0.0f,    // top 
     

     //12 * 3 * 6
  };

   float enemyvertices[] = {
      -2.5f, -2.5f, -0.5f, 0.2f, 0.2f,
       2.5f,  -2.5f, -0.5f, 0.8f, 0.2f,
      2.5f,  2.5f,  -0.5f, 0.8f, 0.8f,
       2.5f,  2.5f,  -0.5f, 0.8f, 0.8f,
      -2.5f, 2.5f,  -0.5f, 0.2f, 0.8f, 
      -2.5f, -2.5f, -0.5f, 0.2f, 0.2f,

      };

  // world space positions of our cubes
  //glm::vec3 enemyPositions =  glm::vec3(0.0f, 0.0f, -40.0f);

 
    glm::vec3  enemyPositions[] = {
        glm::vec3( 0.0f,  0.0f,  -90.0f),  //1
        glm::vec3( 6.0f,  15.0f, -50.0f),  //2
        glm::vec3(-23.5f, -12.2f, -220.5f),  //3
        glm::vec3(-6.8f, -21.0f, -142.3f),  //4
        glm::vec3( 4.4f, -10.4f, -130.5f),  //5
        glm::vec3(-3.7f,  13.0f, -170.5f),  //6
        glm::vec3( 1.3f, -12.0f, -320.5f),  //7
        glm::vec3( 3.5f,  12.0f, -120.5f),  //8
        glm::vec3( 1.5f,  10.2f, -190.5f),  //9
        glm::vec3(-11.3f,  1.0f, -150.5f)  //10
    };
    




  // world space positions of our cubes
 // glm::vec3 cubePositions =  glm::vec3(0.0f, 0.0f, -80.0f);

  //POZADINA Vao
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);


    //Ship Vao
   unsigned int shipVBO, shipVAO;
    glGenVertexArrays(1, &shipVAO);
    glGenBuffers(1, &shipVBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(shipVAO);

    glBindBuffer(GL_ARRAY_BUFFER, shipVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipvertices), shipvertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    

    //ENEMY Vao
     unsigned int enemyVBO, enemyVAO;
    glGenVertexArrays(1, &enemyVAO);
    glGenBuffers(1, &enemyVBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(enemyVAO);

    glBindBuffer(GL_ARRAY_BUFFER, enemyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(enemyvertices), enemyvertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //END Vao
  unsigned int endVBO, endVAO;
  glGenVertexArrays(1, &endVAO);
  glGenBuffers(1, &endVBO);

  glBindVertexArray(endVAO);

  glBindBuffer(GL_ARRAY_BUFFER, endVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(endvertices), endvertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // load and create a texture
  // -------------------------
  //za pozadina tekstura
  unsigned int texture1;
  // texture 1
  // ---------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  // The FileSystem::getPath(...) is part of the GitHub repository so we can
  // find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("../res/textures/slikaKg2.jpg", &width,
                                  &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  
  ourShader.use();
  ourShader.setInt("ourTexture", 0);


  //Za enemy tekstura
  unsigned int texture2;
  // texture 1
  // ---------
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int widthE, heightE, nrChannelsE;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  // The FileSystem::getPath(...) is part of the GitHub repository so we can
  // find files on any IDE/platform; replace it with your own image path.
  unsigned char *dataE = stbi_load("../res/textures/enemy.jpg", &widthE,
                                  &heightE, &nrChannelsE, 0);
  if (dataE) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthE, heightE, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, dataE);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(dataE);
  
  ourEnemyShader.use();
  ourEnemyShader.setInt("ourEnemyTexture", 0);






  //za END tekstura
  unsigned int texture3;
  // texture 1
  // ---------
  glGenTextures(1, &texture3);
  glBindTexture(GL_TEXTURE_2D, texture3);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int widthEnd, heightEnd, nrChannelsEnd;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  // The FileSystem::getPath(...) is part of the GitHub repository so we can
  // find files on any IDE/platform; replace it with your own image path.
  unsigned char *dataEnd = stbi_load("../res/textures/slikaKg4.jpg", &widthEnd,
                                  &heightEnd, &nrChannelsEnd, 0);
  if (dataEnd) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthEnd, heightEnd, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, dataEnd);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(dataEnd);
  
  ourEndShader.use();
  ourEndShader.setInt("ourTexture", 0);

  


  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //ZA ENEMY
    
     glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2);

     ourEnemyShader.use();

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projectionEnemy = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    ourEnemyShader.setMat4("projection", projectionEnemy);

    // camera/view transformation
    glm::mat4 viewEnemy = camera.GetViewMatrix();
    ourEnemyShader.setMat4("view", viewEnemy);

    // render boxes
    glBindVertexArray(enemyVAO);

  
 for (unsigned int i = 0; i < 10; i++){
      glm::mat4 modelEnemy = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

      //enemyMozeZ[i] pretstavuva kolku distanca ima pominato
      //ako od distancata dodademe pocetnata pozicija moze da kazeme kade e momentalno 
      //so toa moze da go vratime nazad
      enemyMoveZ[i] += 1.0f/30.0f;
     // allZ[i] += 1.0f/30.0f;

      modelEnemy = glm::translate(modelEnemy, enemyPositions[i]);
      modelEnemy =  glm::translate(modelEnemy, glm::vec3(0.0f, 0.0f, enemyMoveZ[i]));

          int temp = enemyPositions[i].z + enemyMoveZ[i];
         //  std::cout <<temp<< std::endl;

          if(temp>=-50.0f && temp<=-35.0f){
               if((enemyPositions[i].x)>= (shipL-1.5f) && (enemyPositions[i].x) <= (shipR+1.5f)){
                if(enemyPositions[i].y>= (shipD-1.0f) && (enemyPositions[i].y) <= (shipU+2.5f)){
                  // std::cout <<"                                                                                      POGODOK"<<shipL<<"/"<<shipR<< std::endl;
                  //  glfwSetWindowShouldClose(window, true);
                  endOfGame=true;
                 }
               }
              //  else{
              //       std::cout <<"                                                                                         ne"<<enemyPositions[i].x<<"/"<<shipL<<"/"<<shipR<< std::endl;

              //  }
         }//brodot e 5.0f vo dolzina ,a enemy vo shirina e 5
        


           if((enemyPositions[i].z + enemyMoveZ[i])>= -30.0f){
           //  allZ[i] -= enemyMoveZ[i];
             enemyMoveZ[i] = enemyPositions[i].z;
           //   glfwSetWindowShouldClose(window, true);
           }


      ourEnemyShader.setMat4("model", modelEnemy);

      glDrawArrays(GL_TRIANGLES, 0, 8);
 }
  
    //ZA BRODOT

        // bind textures on corresponding texture units
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texture2);

    // activate shader
    ourShipShader.use();

    // create transformations
    glm::mat4 projectionShip = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projectionShip);

    // camera/view transformation
    //glm::mat4 
   glm::mat4 viewShip = camera.GetViewMatrix();
    ourShader.setMat4("view", viewShip);

    // render boxes
    glBindVertexArray(shipVAO);

     glm::mat4 modelShip = glm::mat4(
          1.0f); // make sure to initialize matrix to identity matrix first
      modelShip = glm::translate(modelShip, 
      //cubePositions);
      // glm::vec3(0.0f, 0.0f, 0.0f));
       glm::vec3(moveX, moveY, 0.0f));
    
      // float angle = 20.0f;
      // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      ourShader.setMat4("model", modelShip);
    // render container
  //  glBindVertexArray(shipVAO);

    glDrawArrays(GL_TRIANGLES, 0, 190);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------

     // ZA POZADINATA

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    ourShader.use();
     glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);
     glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("view", view);
    // render boxes
    glBindVertexArray(VAO);
       glm::mat4 model = glm::mat4(1.0f); 
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
      ourShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 8);
    //}



    //Za END

    if(endOfGame==true){

        glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texture2);

    // activate shader

    ourEndShader.use();

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
   // glm::mat4 
     glm::mat4 projectionEnd = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projectionEnd);

    // camera/view transformation
    //glm::mat4 
     glm::mat4 viewEnd = camera.GetViewMatrix();
    ourShader.setMat4("view", viewEnd);

    // render boxes
    glBindVertexArray(endVAO);
   // for (unsigned int i = 0; i < 1; i++) {
      // calculate the model matrix for each object and pass it to shader before
      // drawing
      //glm::mat4 
       glm::mat4 modelEnd = glm::mat4(
          1.0f); // make sure to initialize matrix to identity matrix first
      model = glm::translate(modelEnd, glm::vec3(0.0f, 0.0f, -40.0f));
      // float angle = 20.0f;
      // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      ourShader.setMat4("model", modelEnd);

      glDrawArrays(GL_TRIANGLES, 0, 8);
    }
   








    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
   glDeleteVertexArrays(1, &shipVAO);
  glDeleteBuffers(1, &shipVBO);
  glDeleteVertexArrays(1, &enemyVAO);
  glDeleteBuffers(1, &enemyVBO);
  glDeleteVertexArrays(1, &endVAO);
  glDeleteBuffers(1, &endVBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

float speed = 0.04f;
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
   // camera.ProcessKeyboard(FORWARD, deltaTime);
   if(moveY>=17.0f){
     moveY=17.0f;
   }
   else{
    moveY += speed;
    shipU +=speed;
    shipD +=speed;
   }
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    //camera.ProcessKeyboard(BACKWARD, deltaTime);
   if(moveY<=-13.0f){
     moveY=-13.0f;
   }
   else{
    moveY -= speed;
     shipU -=speed;
     shipD -=speed;
   }
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
   // camera.ProcessKeyboard(LEFT, deltaTime);
   if(moveX<=-15.0f){
     moveX=-15.0f;
   }
   else{
    moveX -= speed;
     shipL -=speed;
     shipR -=speed;
   }
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
   // camera.ProcessKeyboard(RIGHT, deltaTime);
    if(moveX>=15.0f){
     moveX=15.0f;
   }
   else{
    moveX += speed;
     shipL +=speed;
     shipR +=speed;
   }
     
  }
}
//Za gledanje
// void processInput(GLFWwindow *window) {
//   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//     glfwSetWindowShouldClose(window, true);
//   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//   {
//    camera.ProcessKeyboard(FORWARD, deltaTime);
//   }
//   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//   {
//     camera.ProcessKeyboard(BACKWARD, deltaTime);
//   }
//   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//   {
//    camera.ProcessKeyboard(LEFT, deltaTime);
//   }
//   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//   {
//    camera.ProcessKeyboard(RIGHT, deltaTime);
//   }
// }

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposd, double yposd) {
  // float xpos = static_cast<float>(xposd);
  // float ypos = static_cast<float>(yposd);
  // if (firstMouse) {
  //   lastX = xpos;
  //   lastY = ypos;
  //   firstMouse = false;
  // }

  // float xoffset = xpos - lastX;
  // float yoffset =
  //     lastY - ypos; // reversed since y-coordinates go from bottom to top

  // lastX = xpos;
  // lastY = ypos;

  // camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
