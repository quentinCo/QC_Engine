#include <Application.hpp>

#include <iostream>

#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <qc_graphic/Geometry/Transformation.hpp>
#include <qc_graphic/Render/Shapes/Shape.hpp>
#include <qc_graphic/Render/Object3d.hpp>


/*-------------------- APPLICATION  CONSTRUCTOR ----------------------------------*/

Application::Application()
{
    glm::vec3 pos       = { 0,0,0 };
    glm::vec3 front     = { 0,0,-1 };
    camera              = qc_graphic::Camera(pos, 0.01, 100, 70, window.getWidth(), window.getHeight(), front);
    cameraController    = app::controller::CameraController(camera, window);
    

    time                = clock();
    elapsedTime         = 0.0;
}

Application::~Application()
{}

bool Application::waitKey()
{
	std::cin.clear();
	std::cout << "Please press any key to continue" << std::endl;
	std::cout << "..." << std::endl;
	std::cin.get();
	return true;
}


/*-------------------------------  RUN  ------------------------------------------*/

int Application::run()
{
	Cube cube = makeCube();
    Plan plan = makePlan();
	
	std::cout << "point" << std::endl;
    for (auto& it : cube.points)
    {
		std::cout << "[" << it.position.r << ", " << it.position.g << ", " << it.position.b << "]";
        std::cout << "[" << it.normal.r << ", " << it.normal.g << ", " << it.normal.b << "]" << std::endl;
    }
	std::cout << "indices" << std::endl;
    std::cout << "[" << std::endl;
    for (size_t i = 0; i < cube.indices.size(); i += 3)
    {
        std::cout << cube.indices[i] << ", " << cube.indices[i + 1] << ", " << cube.indices[i + 2] << std::endl;
    }
    std::cout << "]" << std::endl;
	
	// Object
    static qc_graphic::render::shape::Shape cubeShape = qc_graphic::render::shape::Shape(cube.points, cube.indices);
    static qc_graphic::render::shape::Shape planShape = qc_graphic::render::shape::Shape(plan.points, plan.indices);

    qc_graphic::render::Object3d cubeObject = qc_graphic::render::Object3d(&cubeShape);
    qc_graphic::render::Object3d planObject = qc_graphic::render::Object3d(&planShape);


    // Program
    GLuint vs   = makeVertexShader();
    GLuint fs   = makeFragmentShader();
    GLuint prog = makeProgram({ vs,fs });

    // Uniforms
    GLuint uMVPMatrix       = glGetUniformLocation(prog, "uMVPMatrix");
    GLuint uColor           = glGetUniformLocation(prog, "uColor");
    GLuint uNormalMatrix    = glGetUniformLocation(prog, "uNormalMatrix");

    // Matrices
    auto& cubeTransform = cubeObject.getTransfomation();
    cubeTransform.setPosition(0, 0, -4);

    auto& planTransform = planObject.getTransfomation();
    planTransform.setScale(25, 0, 25);
    planTransform.setPosition(0, -1, -4);


    while (window.shouldClose())
    {
        clock_t currentTime = clock();
        elapsedTime         = (static_cast<float>(currentTime - time)) / CLOCKS_PER_SEC;
        time                = currentTime;

        // Update
        cameraController.updateController(elapsedTime);


        // Draw
        glViewport(0, 0, static_cast<GLsizei>(window.getWidth()), static_cast<GLsizei>(window.getHeight()));

        // Render here 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.25, 0, 0, 1);

        glEnable(GL_DEPTH_TEST);

        glUseProgram(prog);

        glm::mat4 viewMatrix = camera.getViewMatrix();
        glm::mat4 projMatrix = camera.getProjMatrix();

        auto& drawObject = [&](qc_graphic::render::Object3d& object, const glm::vec4& color)
        {
            auto& transform         = object.getTransfomation();
            glm::mat4 modelMatrix   = transform.getTransformMatrix();
            glm::mat4 mvMatix       = viewMatrix * modelMatrix;
            glm::mat4 mvpMatrix     = projMatrix * mvMatix;
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mvMatix));
            //glm::mat4 normalMatrix  = glm::transpose(glm::inverse(modelMatrix));

            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

            glUniform4fv(uColor, 1, glm::value_ptr(color));

            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

            const auto& mesh = object.getShape();
            const auto& vao = mesh->getVao();
            vao.bindVertexArray();

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->getNbIndices()), GL_UNSIGNED_INT, nullptr);

            vao.unbindVertexArray();
        };

        // Draw cube
        drawObject(cubeObject, glm::vec4(1, 1, 0, 1));

        const float angleX = 1 / 100.f;
        const float angleY = 1 / 200.f;
        const float angleZ = 1 / 400.f;
        cubeTransform.rotate(angleX, angleY, angleZ);


        // Draw plan
        drawObject(planObject, glm::vec4(0, 1, 1, 1));
//        planTransform.rotate(angleX, 0, 0);

        glDisable(GL_DEPTH_TEST);

        window.swapBuffer();

        // Poll for and process events
        glfwPollEvents();
    }
	return 0;
}

GLuint Application::makeProgram(const std::vector<GLuint>& shaders)
{
    if (!shaders.size())
        return 0;

    GLuint prog = glCreateProgram();
    for(auto& it : shaders)
        glAttachShader(prog, it);

    glLinkProgram(prog);

    GLint linkStatus;
    glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);

    if (linkStatus != GL_TRUE) {
        GLint logLength;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);

        std::unique_ptr<char[]> buffer(new char[logLength]);
        glGetProgramInfoLog(prog, logLength, 0, buffer.get());

        std::cerr << "Program link error:" << buffer.get() << std::endl;
        throw std::runtime_error("Program link error:" + std::string(buffer.get()));
    }

    return prog;
}

GLuint Application::makeVertexShader()
{
	std::string vertexShaderSrc =
R"(
	#version 430

	layout(location = 0) in vec4 aPosition;
    layout(location = 1) in vec4 aNormal; 

	uniform mat4 uMVPMatrix;
    uniform mat4 uNormalMatrix;

    out vec4 vNormal;

	void main()
	{
        vNormal = uNormalMatrix * aNormal;
        gl_Position = uMVPMatrix * aPosition;
	}
)";

    return makeShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str());
}

GLuint Application::makeFragmentShader()
{
    std::string fragmentShaderSrc =
R"(
    #version 430
    
    in vec4 vNormal;

    uniform vec4 uColor;

    out vec4 fColor;

    void main()
    {
        fColor = dot(vNormal, vec4(0,1,0,1)) * uColor;
        //fColor = vec4(vNormal.xyz, 1);
    }
)";

    return makeShader(GL_FRAGMENT_SHADER, fragmentShaderSrc.c_str());
}

GLuint Application::makeShader(GLuint type, const char* src)
{
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);

	return shader;
}

Application::Cube Application::makeCube()
{
    Cube cube;
    float x = -0.5;
    float y = -0.5;
    float z = -0.5;
    std::vector<glm::vec4> vertex;
    for (int i = 0; i < 8; ++i)
    {
        if (i % 4 == 0) x = -x;
        y = -y;
        if (i < 2 || i >= 6)
            z = 0.5;
        else
            z = -0.5;

        vertex.push_back(glm::vec4(x, y, z, 1));
    }


    cube.points = {
        qc_graphic::geometry::Vertex(vertex[0], glm::vec4(1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[1], glm::vec4(1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[2], glm::vec4(1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[3], glm::vec4(1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[4], glm::vec4(-1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[5], glm::vec4(-1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[6], glm::vec4(-1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[7], glm::vec4(-1,0,0,0)),

        qc_graphic::geometry::Vertex(vertex[0], glm::vec4(0,0,1,0)),
        qc_graphic::geometry::Vertex(vertex[1], glm::vec4(0,0,1,0)),
        qc_graphic::geometry::Vertex(vertex[7], glm::vec4(0,0,1,0)),
        qc_graphic::geometry::Vertex(vertex[6], glm::vec4(0,0,1,0)),
        qc_graphic::geometry::Vertex(vertex[2], glm::vec4(0,0,-1,0)),
        qc_graphic::geometry::Vertex(vertex[3], glm::vec4(0,0,-1,0)),
        qc_graphic::geometry::Vertex(vertex[5], glm::vec4(0,0,-1,0)),
        qc_graphic::geometry::Vertex(vertex[4], glm::vec4(0,0,-1,0)),

        qc_graphic::geometry::Vertex(vertex[0], glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(vertex[2], glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(vertex[4], glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(vertex[6], glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(vertex[1], glm::vec4(0,-1,0,0)),
        qc_graphic::geometry::Vertex(vertex[3], glm::vec4(0,-1,0,0)),
        qc_graphic::geometry::Vertex(vertex[5], glm::vec4(0,-1,0,0)),
        qc_graphic::geometry::Vertex(vertex[7], glm::vec4(0,-1,0,0))
    };

    cube.indices = {
        0, 1, 2,
        3, 2, 1,
        4, 5, 6,
        7, 6, 5,

        8, 11, 10,
        8, 10, 9,
        12, 13, 15,
        13, 14, 15,

        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
    };

    return cube;
}


Application::Plan Application::makePlan()
{
    Plan plan;
    plan.points = {
        qc_graphic::geometry::Vertex(glm::vec4(0.5,0,0.5,1),    glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(glm::vec4(-0.5,0,0.5,1),   glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(glm::vec4(-0.5,0,-0.5,1),  glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(glm::vec4(0.5,0,-0.5,1),   glm::vec4(0,1,0,0))
    };

    plan.indices = {
        0,1,2,
        2,3,0
    };

    return plan;
}