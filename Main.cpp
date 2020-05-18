#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void processInput( GLFWwindow* window );

int main()
{
	//setting up glfw
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	//create a window
	GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", NULL, NULL );
	if( window == NULL ) //if failed create window
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window ); //tell glfw to make context with current "window"

	//GLAD manages function pointers for OpenGL
	//initialize glad
	if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) ) //to load the adress of the OpenGL function pointers which is OS-specific
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//to tell OpenGL the size of the rendering window so OpenGL knows how we want to display the data and coordinates with respect to the window
	glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );

	//if user resizes the window, the viewport should be adjusted as well
	framebuffer_size_callback( window, WINDOW_WIDTH, WINDOW_HEIGHT );
	//to tell GLFW we want to call framebuffer_size_callback function on every window resize by registering it
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );


	//render loop
	while( !glfwWindowShouldClose( window ) )
	{
		//input
		processInput( window );

		//render command here
		glClearColor( 0.1f, 0.2f, 0.7f, 0.3f );
		glClear( GL_COLOR_BUFFER_BIT );

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
	glViewport( 0, 0, width, height );
}

void processInput( GLFWwindow* window )
{
	if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( window, true );
	}
}