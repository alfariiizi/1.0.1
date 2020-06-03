#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// if user resizes the window, the viewport should be adjusted as well
void framebuffer_size_callback( GLFWwindow* window, int width, int height );
// untuk memprosses jika ada suatu input
void processInput( GLFWwindow* window );

// settings
// -------------------------------------
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4( aPos.x, aPos.y, aPos.z, 1.0 );"
"}\0";
const char* fragmentShaderSourceOrange = "#version 330 core\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
"	fragColor = vec4( 1.0f, 0.5f, 0.2f, 1.0f );"
"}\0";
const char* fragmentShaderSourceYellow = "#version 330 core\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
"	fragColor = vec4( 1.0f, 0.9f, 0.1f, 1.0f );"
"}\0";
// -------------------------------------


int main()
{
	// glfw : Menginisialisasi dan mengatur konfigurasinya
	// ---------------------------------------------------
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	// ---------------------------------------------------

	// Membuat window
	// --------------
	GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", NULL, NULL );
	if( window == NULL ) //if failed create window
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window ); //tell glfw to make context with current "window"
	//to tell GLFW we want to call framebuffer_size_callback function on every window resize by registering it
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
	// --------------

	// GLAD manages function pointers for OpenGL
	// glad : load semua OpenGL function pointer
	// -----------------------------------------
	if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) ) //to load the adress of the OpenGL function pointers which is OS-specific
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// -----------------------------------------



	/* --- Build dan compile vertexShader, fragmentShader, dan programShader nya --- */

	// membuat vertex Shader dan compile vertex shader
	// --------------------------------------------------------
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
	unsigned int fragmentShaderOrange = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShaderOrange, 1, &fragmentShaderSourceOrange, NULL );
	glCompileShader( fragmentShaderOrange );

	glGetShaderiv( fragmentShaderOrange, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetShaderInfoLog( vertexShader, sizeof( infoLog ), NULL, infoLog );
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
	}
	// ---------------------------------------------------

	// membuat fragment shader dan compile fragment shader
	// ---------------------------------------------------
	unsigned int fragmentShaderYellow = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL );
	glCompileShader( fragmentShaderYellow );

	glGetShaderiv( fragmentShaderYellow, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetShaderInfoLog( vertexShader, sizeof( infoLog ), NULL, infoLog );
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
	}
	// ---------------------------------------------------


	// membuat shader Program dan menggunakannya
	// -----------------------------------------
	unsigned int shaderProgramOrange = glCreateProgram();
	glAttachShader( shaderProgramOrange, vertexShader );
	glAttachShader( shaderProgramOrange, fragmentShaderOrange );
	glLinkProgram( shaderProgramOrange );

	glGetProgramiv( shaderProgramOrange, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetProgramInfoLog( shaderProgramOrange, sizeof( infoLog ), NULL, infoLog );
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// -----------------------------------------

	// membuat shader Program dan menggunakannya
	// -----------------------------------------
	unsigned int shaderProgramYellow = glCreateProgram();
	glAttachShader( shaderProgramYellow, vertexShader );
	glAttachShader( shaderProgramYellow, fragmentShaderYellow );
	glLinkProgram( shaderProgramYellow );

	glGetProgramiv( shaderProgramYellow, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetProgramInfoLog( shaderProgramYellow, sizeof( infoLog ), NULL, infoLog );
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// -----------------------------------------

	glDeleteShader( vertexShader ); // karena sudah tidak di-linking ke program shader, sehingga sudah tidak dipakai lagi
	glDeleteShader( fragmentShaderOrange ); // karena sudah tidak di-linking ke program shader, sehingga sudah tidak dipakai lagi
	glDeleteShader( fragmentShaderYellow ); // karena sudah tidak di-linking ke program shader, sehingga sudah tidak dipakai lagi

	/* ----------------------------------------------------------------------------- */


// Membuat vertex data (dan buffer(s)) dan mengatur vertex attribute
// -----------------------------------------------------------------

	//create vertices
	float firstTriangle[] = {
		//first triagle
		-0.7f, -0.3f, 0.0f,
		-0.4f, 0.5f, 0.0f,
		0.0f, -0.4f, 0.0f,
	};
	float secondTriangle[] = {
		//second triagle
		0.0f, -0.4f, 0.0f,
		0.6f, 0.9f, 0.0f,
		0.9f, -0.2f, 0.0f
	};

	// Vertex shader
	// ----------------------------------------
	unsigned int VAOs[2];
	unsigned int VBOs[2];
	glGenVertexArrays( 2, VAOs );
	glGenBuffers( 2, VBOs );

	// setup the first triangle
	// -----------------------
	glBindVertexArray( VAOs[0] );
	glBindBuffer( GL_ARRAY_BUFFER, VBOs[0] ); // . . . bind VBO ke VAO
	glBufferData( GL_ARRAY_BUFFER, sizeof( firstTriangle ), firstTriangle, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void* )0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	//glBindVertexArray( 0 ); // no need to unbind at all as we directly bind a different VAO the next few lines
	glEnableVertexAttribArray( 0 );
	// -----------------------

	// setup the second triangle
	// -------------------------
	glBindVertexArray( VAOs[1] );
	glBindBuffer( GL_ARRAY_BUFFER, VBOs[1] ); // . . . bind VBO ke VAO
	glBufferData( GL_ARRAY_BUFFER, sizeof( secondTriangle ), secondTriangle, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void* )0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	//glBindVertexArray( 0 ); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)
	glEnableVertexAttribArray( 0 ); 
	// -------------------------

	// ----------------------------------
// ----------------------------------------


	//render loop
	while( !glfwWindowShouldClose( window ) )
	{
		//input
		processInput( window );

	// render command here
	// ------------------
		glClearColor( 0.2f, 0.3f, 0.5f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		// draw first triangle dengan warna orange
		// -------------------
		glUseProgram( shaderProgramOrange );
		glBindVertexArray( VAOs[0] );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		// -------------------

		// draw second triangle dengan warna kuning
		// --------------------
		glUseProgram( shaderProgramYellow );
		glBindVertexArray( VAOs[1] );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		// --------------------
	// ------------------

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	// dealocate semua resources yang sudah digunakan
	// ----------------------------------------------
	glDeleteBuffers( 2, VBOs );
	//glDeleteBuffers( 1, &EBO );
	glDeleteVertexArrays( 2, VAOs );
	glDeleteProgram( shaderProgramOrange );
	glDeleteProgram( shaderProgramYellow );
	// ----------------------------------------------

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
	//to tell OpenGL the size of the rendering window so OpenGL knows how we want to display the data and coordinates with respect to the window
	glViewport( 0, 0, width, height );
}

void processInput( GLFWwindow* window )
{
	if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( window, true );
	}
	else if( glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else if( glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}