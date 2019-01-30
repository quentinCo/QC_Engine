#include <Application.hpp>

// Native C++
#include <iostream>

#include <memory>

#include <string>
#include <chrono>

// Glm
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Qc_graphic
#include <qc_graphic\Useful.hpp>
#include <qc_graphic\render\Camera.hpp>
#include <qc_graphic\controler\FPSCameraController.hpp>

#include <qc_graphic\render\programs\Program.hpp>

// Application
#include <instances\Object3d.hpp>

/*-------------------- TEMPORARY TEST VARIABLES ----------------------------------*/
// TODO move camera manage in a app::Camera object
static qc::render::Camera camera;
static qc::controller::Controller* cameraController;
static qc::controller::FPSCameraController fpsCameraController;

static std::vector<std::unique_ptr<qc::render::Mesh>> meshCollection;

static GLuint uMVPMatrix;
static GLuint uNormalMatrix;


/*-------------------- TEST FUNCTIONS PROTOTYPE ----------------------------------*/
static std::unique_ptr<qc::render::Mesh> makeCube();
static bool generateDefaultVertexShader(qc::render::program::Shader& vs);
static bool generateDefaultFragmentShader(qc::render::program::Shader& fs);
static bool makeDefaultProgram(qc::render::program::Program& program);

static void renderScene(std::vector<app::Object3d>& objs, const glm::ivec2& viewportSize, const qc::render::program::Program& prog);



/*-------------------- APPLICATION  CONSTRUCTOR ----------------------------------*/

Application::Application()
    : window(1600, 900, "QC Engine")
{
    camera.setPosition({ 0,0,5 });
}

Application::~Application()
{}

/*-------------------------------  RUN  ------------------------------------------*/

int Application::run()
{
    // Init phase
        // Init window
    bool isInit = this->window.init();
    if (isInit == false)
        return 0;

        // Init Camera
    fpsCameraController = qc::controller::FPSCameraController(window, camera);
    cameraController = &fpsCameraController;

        //Init Program
    qc::render::program::Program defaultProgram;
    isInit = makeDefaultProgram(defaultProgram);
    if (isInit == false)
    {
        qc::Useful::PrintError("default program failed to init");
        return 0;
    }

        // Init Uniform
    uMVPMatrix = defaultProgram.getUniformLocation("uMVPMatrix");
    uNormalMatrix = defaultProgram.getUniformLocation("uNormalMatrix");

        // Init Meshs
    meshCollection.emplace_back(makeCube());
    const std::unique_ptr<qc::render::Mesh>& cube = meshCollection.back();

        // Init Objects
    std::vector<app::Object3d> objects;
    objects.emplace_back(cube.get());

    // Save viewport size
    glm::ivec2 viewportSize = window.getViewportSize(); // CAUTION: Valid because the viewport is unsizeable

    // Loop
    while (this->window.getShouldClose() == false)
    {
        this->window.update();

        //cameraController->update();

        renderScene(objects, viewportSize, defaultProgram);


        for (auto& obj : objects)
        {
            auto& trans = obj.getTransformation();
            const float angleX = 0;// 1 / 100.f;
            const float angleY = 1 / 200.f;
            const float angleZ = 0;// 1 / 400.f;
            trans.rotate(glm::vec3(angleX, angleY, angleZ));
        }

    }

    return 0;
}



/*----------------------------- TEST FUNCTIONS ----------------------------------*/
static std::unique_ptr<qc::render::Mesh> makeCube()
{
    float x = -0.5;
    float y = -0.5;
    float z = -0.5;
    
    std::vector<qc::render::Vertex> vertices;
    std::vector<unsigned int>       indices;
    glm::vec4                       points[8];

    for (int i = 0; i < 8; ++i)
    {
        if (i % 4 == 0) x = -x;
        y = -y;
        if (i < 2 || i >= 6)
            z = 0.5;
        else
            z = -0.5;

        points[i] = glm::vec4(x, y, z, 1);
    }


    vertices = {
        qc::render::Vertex(points[0], glm::vec4(1,0,0,0)),
        qc::render::Vertex(points[1], glm::vec4(1,0,0,0)),
        qc::render::Vertex(points[2], glm::vec4(1,0,0,0)),
        qc::render::Vertex(points[3], glm::vec4(1,0,0,0)),
        qc::render::Vertex(points[4], glm::vec4(-1,0,0,0)),
        qc::render::Vertex(points[5], glm::vec4(-1,0,0,0)),
        qc::render::Vertex(points[6], glm::vec4(-1,0,0,0)),
        qc::render::Vertex(points[7], glm::vec4(-1,0,0,0)),

        qc::render::Vertex(points[0], glm::vec4(0,0,1,0)),
        qc::render::Vertex(points[1], glm::vec4(0,0,1,0)),
        qc::render::Vertex(points[7], glm::vec4(0,0,1,0)),
        qc::render::Vertex(points[6], glm::vec4(0,0,1,0)),
        qc::render::Vertex(points[2], glm::vec4(0,0,-1,0)),
        qc::render::Vertex(points[3], glm::vec4(0,0,-1,0)),
        qc::render::Vertex(points[5], glm::vec4(0,0,-1,0)),
        qc::render::Vertex(points[4], glm::vec4(0,0,-1,0)),

        qc::render::Vertex(points[0], glm::vec4(0,1,0,0)),
        qc::render::Vertex(points[2], glm::vec4(0,1,0,0)),
        qc::render::Vertex(points[4], glm::vec4(0,1,0,0)),
        qc::render::Vertex(points[6], glm::vec4(0,1,0,0)),
        qc::render::Vertex(points[1], glm::vec4(0,-1,0,0)),
        qc::render::Vertex(points[3], glm::vec4(0,-1,0,0)),
        qc::render::Vertex(points[5], glm::vec4(0,-1,0,0)),
        qc::render::Vertex(points[7], glm::vec4(0,-1,0,0))
    };

    indices = {
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

    return std::unique_ptr<qc::render::Mesh>(new qc::render::Mesh(vertices, indices));
}


static bool generateDefaultVertexShader(qc::render::program::Shader& vs)
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

    vs.appendCode(vertexShaderSrc);
    return vs.compile();
}

static bool generateDefaultFragmentShader(qc::render::program::Shader& fs)
{
    std::string fragmentShaderSrc =
R"(
    #version 430
    
    in vec4 vNormal;
    out vec4 fColor;
    void main()
    {
        vec4 color = vec4(1, 0.5, 0, 1);
        fColor = dot(vNormal, vec4(0,1,0,0)) * color;
        fColor = vNormal;
        //fColor = vec4(0,1,0, 1);
    }
)";

    fs.appendCode(fragmentShaderSrc);
    return fs.compile();
}

static bool makeDefaultProgram(qc::render::program::Program& program)
{
    qc::render::program::Shader vs = qc::render::program::Shader(qc::render::program::ShaderType::VERTEX_SHADER);
    qc::render::program::Shader fs = qc::render::program::Shader(qc::render::program::ShaderType::FRAGMENT_SHADER);

    bool res = false;

    res = generateDefaultVertexShader(vs);
    if (!res)
        return res;

    res = generateDefaultFragmentShader(fs);
    if (!res)
        return res;

    res = program.attach({ vs, fs });
    if (!res)
        return res;

    return program.link();
}

static void renderScene(std::vector<app::Object3d>& objs, const glm::ivec2& viewportSize, const qc::render::program::Program& prog) // objs not const due to the possible update of the matrix from transform
{
    // Get common matrices
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projMatrix = camera.getProjMatrix(viewportSize);

    // Object rendering lambda
    auto renderObject = [&](app::Object3d& obj)
    {
        qc::Transform& transform = obj.getTransformation();

        glm::mat4 modelMatrix   = transform.getMatrix();

        glm::mat4 mvMatrix      = viewMatrix * modelMatrix;
        glm::mat4 mvpMatrix     = projMatrix * mvMatrix;
        glm::mat4 normalMatrix  = glm::transpose(glm::inverse(mvMatrix)); // TODO: check the normal must be only affected and so by rotation -> rotateMatrix^-1 = rotateMatrix^T

        glUniformMatrix4fv(uMVPMatrix   , 1, GL_FALSE, glm::value_ptr(mvpMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        const qc::render::Mesh* mesh    = obj.getMesh();
        const qc::render::VAO& vao      = mesh->getVAO();
        
        if (vao.bind() == false)
            return;

        glDrawElements(GL_TRIANGLES, mesh->getNbIndices(), GL_UNSIGNED_INT, 0);

        vao.unbind();
    };

    // Init view port size
    glViewport(0, 0, static_cast<GLsizei>(viewportSize.x), static_cast<GLsizei>(viewportSize.y));

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 0, 0, 1);

    // Enable options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    // Use rendering program
    prog.use();

    // Iterate through objects and draw
    for (auto& obj : objs)
        renderObject(obj);

    // Disable options
    glDisable(GL_DEPTH_TEST);
}

