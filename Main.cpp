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


	//create vertices
	float vertices[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	// Vertex Input
	// ----------------------------------------
	unsigned int VBO; // vertex buffer objects
	glGenBuffers( 1, &VBO );
		unsigned int VAO; // vertex arrays object
		glGenVertexArrays( 1, &VAO );
		glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), &vertices, GL_STATIC_DRAW );
		// set the vertex attributes pointers
		// ----------------------------------
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void* )0 );
		glEnableVertexAttribArray( 0 );
		// ----------------------------------
	// ----------------------------------------


	// membuat vertex Shader dan compile vertex shader
	// --------------------------------------------------------
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4( aPos.x, aPos.y, aPos.z, 1.0 );"
		"}\0";
	
	unsigned int vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
	glCompileShader( vertexShader );

	int success;
	char infoLog[512];
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetShaderInfoLog( vertexShader, sizeof( infoLog ), NULL, infoLog );
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
	}
	// --------------------------------------------------------


	// membuat fragment shader dan compile fragment shader
	// ---------------------------------------------------
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 fragColor;\n"
		"void main()\n"
		"{\n"
		"	fragColor = vec4( 1.0f, 0.5f, 0.2f, 1.0f );"
		"}\0";
	unsigned int fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
	glCompileShader( fragmentShader );

	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetShaderInfoLog( vertexShader, sizeof( infoLog ), NULL, infoLog );
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
	}
	// ---------------------------------------------------


	// membuat shader Program dan menggunakannya
	// ----------------------
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	glLinkProgram( shaderProgram );

	glGetProgramiv( shaderProgram, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetProgramInfoLog( shaderProgram, sizeof( infoLog ), NULL, infoLog );
		std::cout << "ERROR::PROGRAM::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader( vertexShader ); // karena sudah tidak di-linking ke program shader, sehingga sudah tidak dipakai lagi
	glDeleteShader( fragmentShader ); // karena sudah tidak di-linking ke program shader, sehingga sudah tidak dipakai lagi
	// ----------------------


	//render loop
	while( !glfwWindowShouldClose( window ) )
	{
		//input
		processInput( window );

		//render command here
		glClearColor( 0.1f, 0.2f, 0.7f, 0.3f );
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram( shaderProgram );
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 3 );

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