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
#include <instances\PointLightInstance.hpp>

/*-------------------- TEMPORARY TEST VARIABLES ----------------------------------*/
// TODO move camera manage in a app::Camera object
static qc::render::Camera camera;
static qc::controller::Controller* cameraController;
static qc::controller::FPSCameraController fpsCameraController;

static std::vector<std::unique_ptr<qc::render::Mesh>> meshCollection;

static app::PointLightInstance pointLight;

static GLuint uMVPMatrix;
static GLuint uNormalMatrix;
static GLuint uViewMatrix;
static GLuint uMVMatrix;

static GLuint uPointLightPos;
static GLuint uPointLightCol;
static GLuint uPointLightInt;


/*-------------------- TEST FUNCTIONS PROTOTYPE ----------------------------------*/
static std::unique_ptr<qc::render::Mesh> makeCube();
static std::unique_ptr<qc::render::Mesh> makePlan();
static bool generateTestVertexShader(qc::render::program::Shader& vs);
static bool generateTestFragmentShader(qc::render::program::Shader& fs);
static bool makeTestProgram(qc::render::program::Program& program);

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
    fpsCameraController = qc::controller::FPSCameraController(window, camera, 10, 0.05f);
    cameraController = &fpsCameraController;

        //Init Program
    //qc::render::program::Program defaultProgram = qc::render::program::GetDefaultProgram();
    qc::render::program::Program defaultProgram;
    makeTestProgram(defaultProgram);

        // Init Uniform
    uMVPMatrix = defaultProgram.getUniformLocation("uMVPMatrix");
    uNormalMatrix = defaultProgram.getUniformLocation("uNormalMatrix");
    uViewMatrix = defaultProgram.getUniformLocation("uViewMatrix"); 
    uMVMatrix = defaultProgram.getUniformLocation("uMVMatrix");
    uPointLightPos = defaultProgram.getUniformLocation("uPointLight.position");
    uPointLightCol = defaultProgram.getUniformLocation("uPointLight.color");
    uPointLightInt = defaultProgram.getUniformLocation("uPointLight.intensity");

        // Init Meshs
    meshCollection.emplace_back(makeCube());
    //meshCollection.emplace_back(makePlan());
    const std::unique_ptr<qc::render::Mesh>& mesh = meshCollection.back();

        // Init Objects
    std::vector<app::Object3d> objects;
    objects.emplace_back(mesh.get());

    {
        auto& obj = objects.back();
        auto& trans = obj.getTransformation();
        trans.translate({ 0,0,-2 });
        trans.scale(2);
    }

    pointLight = app::PointLightInstance(glm::vec3(0, 0, 5), 5, glm::vec3(0, 1, 0));

    // Save viewport size
    glm::ivec2 viewportSize = window.getViewportSize(); // CAUTION: Valid because the viewport is unsizeable

    // Loop
    while (this->window.getShouldClose() == false)
    {
        this->window.update();

        cameraController->update();

        renderScene(objects, viewportSize, defaultProgram);
        /*
        for (auto& obj : objects)
        {
            auto& trans = obj.getTransformation();
            const float angleX = 1 / 100.f;
            const float angleY = 1 / 200.f;
            const float angleZ = 1 / 400.f;
            trans.rotate(glm::vec3(angleX, angleY, angleZ));
        }
        */
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
        qc::render::Vertex(points[0], glm::vec4(1,0,0,0), glm::vec2(1,0)),
        qc::render::Vertex(points[1], glm::vec4(1,0,0,0), glm::vec2(0,0)),
        qc::render::Vertex(points[2], glm::vec4(1,0,0,0), glm::vec2(1,1)),
        qc::render::Vertex(points[3], glm::vec4(1,0,0,0), glm::vec2(0,1)),
        qc::render::Vertex(points[4], glm::vec4(-1,0,0,0), glm::vec2(0,1)),
        qc::render::Vertex(points[5], glm::vec4(-1,0,0,0), glm::vec2(1,1)),
        qc::render::Vertex(points[6], glm::vec4(-1,0,0,0), glm::vec2(0,0)),
        qc::render::Vertex(points[7], glm::vec4(-1,0,0,0), glm::vec2(1,0)),

        qc::render::Vertex(points[0], glm::vec4(0,0,1,0), glm::vec2(0,1)),
        qc::render::Vertex(points[1], glm::vec4(0,0,1,0), glm::vec2(0,0)),
        qc::render::Vertex(points[7], glm::vec4(0,0,1,0), glm::vec2(1,0)),
        qc::render::Vertex(points[6], glm::vec4(0,0,1,0), glm::vec2(1,1)),
        qc::render::Vertex(points[2], glm::vec4(0,0,-1,0), glm::vec2(0,0)),
        qc::render::Vertex(points[3], glm::vec4(0,0,-1,0), glm::vec2(0,1)),
        qc::render::Vertex(points[5], glm::vec4(0,0,-1,0), glm::vec2(1,1)),
        qc::render::Vertex(points[4], glm::vec4(0,0,-1,0), glm::vec2(1,0)),

        qc::render::Vertex(points[0], glm::vec4(0,1,0,0), glm::vec2(0,0)),
        qc::render::Vertex(points[2], glm::vec4(0,1,0,0), glm::vec2(0,1)),
        qc::render::Vertex(points[4], glm::vec4(0,1,0,0), glm::vec2(1,1)),
        qc::render::Vertex(points[6], glm::vec4(0,1,0,0), glm::vec2(1,0)),
        qc::render::Vertex(points[1], glm::vec4(0,-1,0,0), glm::vec2(0,1)),
        qc::render::Vertex(points[3], glm::vec4(0,-1,0,0), glm::vec2(0,0)),
        qc::render::Vertex(points[5], glm::vec4(0,-1,0,0), glm::vec2(1,0)),
        qc::render::Vertex(points[7], glm::vec4(0,-1,0,0), glm::vec2(1,1))
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
        20, 22, 21,
        20, 23, 22
    };

    return std::make_unique<qc::render::Mesh>(vertices, indices);
}

static std::unique_ptr<qc::render::Mesh> makePlan()
{
    glm::vec4 points[4] =
    {
        glm::vec4(-0.5, 0, -0.5, 1),
        glm::vec4(0.5, 0, -0.5, 1),
        glm::vec4(0.5, 0, 0.5, 1),
        glm::vec4(-0.5, 0, 0.5, 1)
    };

    qc::render::Vertex vertices[4] = {
        qc::render::Vertex(points[0], glm::normalize(glm::vec4(0,1,0,0)), glm::vec2(0,0)),
        qc::render::Vertex(points[1], glm::normalize(glm::vec4(0,1,0,0)), glm::vec2(0,1)),
        qc::render::Vertex(points[2], glm::normalize(glm::vec4(0,1,0,0)), glm::vec2(1,1)),
        qc::render::Vertex(points[3], glm::normalize(glm::vec4(0,1,0,0)), glm::vec2(1,0))
    };

    unsigned int indices[6] = {
        0, 3, 2,
        0, 2, 1
    };

    return std::make_unique<qc::render::Mesh>(vertices, 4, indices, 6);
}


static bool generateTestVertexShader(qc::render::program::Shader& vs)
{
    std::string vertexShaderSrc =
R"(
	#version 430

	layout(location = 0) in vec4 aPosition;
    layout(location = 1) in vec4 aNormal; 
    layout(location = 2) in vec2 aTexCoord; 

	uniform mat4 uMVPMatrix;
    uniform mat4 uNormalMatrix;
    uniform mat4 uViewMatrix;
    uniform mat4 uMVMatrix;

    out vec4 vPosition;
    out vec4 vNormal;
    out vec2 vTexCoord;

	void main()
	{
        vPosition = uMVMatrix * aPosition;
        vNormal = normalize(uNormalMatrix * aNormal);
        vTexCoord = aTexCoord;
        gl_Position = uMVPMatrix * aPosition;
	}
)";

    vs.appendCode(vertexShaderSrc);
    return vs.compile();
}

#if 1
static bool generateTestFragmentShader(qc::render::program::Shader& fs)
{
    std::string fragmentShaderSrc =
        R"(
    #version 430

    struct PointLight
    {
        vec4        position;
        vec4        color;
        float       intensity;
    };

    layout(origin_upper_left) in vec4 gl_FragCoord;    

    uniform PointLight uPointLight;
    uniform mat4 uViewMatrix;

    in vec4 vPosition;
    in vec4 vNormal;
    in vec2 vTexCoord;

    out vec4 fColor;

    void checkerboard(in vec2 texCoord, inout vec4 color)
    {
        float scale = 3.0;
        float sum = floor(texCoord.x * scale) + floor(texCoord.y * scale);
        float modulo = mod(sum, 2); 
        color =  vec4(color.xyz * modulo, 1);
    } 

    void main()
    {
        // Light
        vec4 lightPos = uViewMatrix * uPointLight.position;
        vec4 lightDir = normalize(lightPos - vPosition);

        // Main vectors
        //vec4 reflection = 2 * dot(lightDir, vNormal) * vNormal - lightDir;
        vec4 reflection = 2 * dot(-lightDir, vNormal) * vNormal + lightDir;
        //vec4 reflection = normalize(reflect(-lightDir, vNormal));
        vec4 eyeDir = normalize(-vPosition);
        
        // Factors
        float s = clamp((100 * dot(reflection, eyeDir) - 97), 0, 1);
        float t = (dot(lightDir, vNormal) + 1) * 0.5;

        // Colors
        vec4 color = vec4(1);
        checkerboard(vTexCoord, color);

        vec4 colorWarm          = glm::vec4(0.5,0.5,0,1) + 0.25 * color;
        vec4 colorCool          = glm::vec4(0,0,0.5,1) + 0.25 * color;
        vec4 colorHightLight    = glm::vec4(1);
        
//color = vec4(reflection.xyz, 1);
        color = s * uPointLight.color + ( 1 - s) * (t * colorWarm + (1 - t) * colorCool);


        fColor = color;
    }
)";

    fs.appendCode(fragmentShaderSrc);
    return fs.compile();
}
#else
static bool generateTestFragmentShader(qc::render::program::Shader& fs)
{
    std::string fragmentShaderSrc =
R"(
    #version 430

    struct PointLight
    {
        vec4        position;
        vec4        color;
        float       intensity;
    };

    layout(origin_upper_left) in vec4 gl_FragCoord;    

    uniform PointLight uPointLight;
    uniform mat4 uViewMatrix;

    in vec4 vPosition;
    in vec4 vNormal;
    in vec2 vTexCoord;

    out vec4 fColor;

    void checkerboard(in vec2 texCoord, inout vec4 color)
    {
        float scale = 3.0;
        float sum = floor(texCoord.x * scale) + floor(texCoord.y * scale);
        float modulo = mod(sum, 2); 
        color =  vec4(color.xyz * modulo, 1);
    } 

    void main()
    {
        // Light
        vec4 lightPos = uViewMatrix * uPointLight.position;
        vec4 lightDir = lightPos - vPosition;
        float fact = max(0, dot(lightDir, vNormal));

        vec4 light = uPointLight.color * fact;


        vec4 color = vec4(1);

        vec4 dir = normalize(vec4(0.25,0.25,0.75,0));

        //fColor = dot(vNormal, dir) * color;
        //fColor = vNormal;
        //fColor = vec4(0,1,0, 1);

        //vec2 uv = floor(gl_FragCoord.xy / 60.0);
        //
        //if(mod(uv.x + uv.y, 2.0) == 0.0){
        //    fColor = vec4(0,0,0, 1.0);
        //}
        //else{
        //    fColor = vec4(1,1,1, 1.0);
        //}

        //fColor = vec4(vTexCoord, 0, 1);

        checkerboard(vTexCoord, color);

        fColor = light * color;
    }
)";

    fs.appendCode(fragmentShaderSrc);
    return fs.compile();
}
#endif

static bool makeTestProgram(qc::render::program::Program& program)
{
    qc::render::program::Shader vs = qc::render::program::Shader(qc::render::program::ShaderType::VERTEX_SHADER);
    qc::render::program::Shader fs = qc::render::program::Shader(qc::render::program::ShaderType::FRAGMENT_SHADER);

    bool res = false;

    res = generateTestVertexShader(vs);
    if (!res)
        return res;

    res = generateTestFragmentShader(fs);
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
        glUniformMatrix4fv(uMVMatrix    , 1, GL_FALSE, glm::value_ptr(mvMatrix));        
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

    // Bind general uniform
    glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    auto& light = pointLight.getLight();
    auto color = light.getColor();

    glUniform4fv(uPointLightPos, 1, glm::value_ptr(light.getPosition()));
    glUniform4fv(uPointLightCol, 1, glm::value_ptr(light.getColor()));
    glUniform1f(uPointLightInt,     light.getIntensity());

    // Iterate through objects and draw
    for (auto& obj : objs)
        renderObject(obj);

    // Disable options
    glDisable(GL_DEPTH_TEST);
}

