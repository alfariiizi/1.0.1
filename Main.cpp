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
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
"	fragColor = vec4( 1.0f, 0.5f, 0.2f, 1.0f );"
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
	// -----------------------------------------
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	glLinkProgram( shaderProgram );

	glGetProgramiv( shaderProgram, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetProgramInfoLog( shaderProgram, sizeof( infoLog ), NULL, infoLog );
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader( vertexShader ); // karena sudah tidak di-linking ke program shader, sehingga sudah tidak dipakai lagi
	glDeleteShader( fragmentShader ); // karena sudah tidak di-linking ke program shader, sehingga sudah tidak dipakai lagi
	// -----------------------------------------

	/* ----------------------------------------------------------------------------- */


// Membuat vertex data (dan buffer(s)) dan mengatur vertex attribute
// -----------------------------------------------------------------
	//create vertices
	float vertices[] = {
		-0.5f, 0.5f, 0.0f,	// top left
		0.5f, 0.5f, 0.0f,	// top right
		-0.5f, -0.5f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f	// bottom right
	};

	//create indices
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	// Vertex shader
	// ----------------------------------------
	unsigned int VAO, VBO, EBO; // vertex arrays object (VAO), vertex buffer objects (VBO), element buffer object (EBO)
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glGenBuffers( 1, &EBO );

	glBindVertexArray( VAO ); // bind VAO terlebih dahulu kemudian . . . [lanjut bawahnya]
	glBindBuffer( GL_ARRAY_BUFFER, VBO ); // . . . bind VBO ke VAO
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), &vertices, GL_STATIC_DRAW );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

	// set the vertex attributes pointers
	// ----------------------------------
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void* )0 );
	glEnableVertexAttribArray( 0 );
	// ----------------------------------

	// karena VBO sudah ter-bind ke VAO, maka VBO yang ada di GL_ARRAY_BUFFER dapat di unbind
	// begitu pula VAO juga dapat di unbind terlebih dahulu dari glBindVertexArray
	// ---------------------------------------------------------------------------------------
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray( 0 );
	// ---------------------------------------------------------------------------------------
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

		//draw rectangle
		glUseProgram( shaderProgram );
		glBindVertexArray( VAO );
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
	// ------------------

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	// dealocate semua resources yang sudah digunakan
	// ----------------------------------------------
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &EBO );
	glDeleteVertexArrays( 1, &VAO );
	glDeleteProgram( shaderProgram );
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