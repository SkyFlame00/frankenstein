#include "GameController.h"

#include <iostream>
#include <list>
#include <algorithm>
#include <math.h>
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "../vendor/imgui/imgui_impl_glfw.h"

//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Delaunay_triangulation_3.h>
//#include <CGAL/Delaunay_triangulation_cell_base_3.h>
//#include <CGAL/Triangulation_vertex_base_with_info_3.h>
//#include <vector>
//typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
//typedef CGAL::Triangulation_vertex_base_with_info_3<unsigned, K>    Vb;
//typedef CGAL::Delaunay_triangulation_cell_base_3<K>                 Cb;
//typedef CGAL::Triangulation_data_structure_3<Vb, Cb>                Tds;
////Use the Fast_location tag. Default or Compact_location works too.
//typedef CGAL::Delaunay_triangulation_3<K, Tds, CGAL::Fast_location> Delaunay;
//typedef Delaunay::Point                                             Point;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/algorithm.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Polyhedron_3<K>                     Polyhedron_3;
typedef K::Segment_3                              Segment_3;
// define point creator
typedef K::Point_3                                Point_3;





#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool LineSegmentAndPlaneIntersection(glm::vec3 planePoint, glm::vec3 planeNorm, glm::vec3 segmentV1, glm::vec3 segmentV2, glm::vec3 &intersection)
{
    glm::vec3 lineDir = segmentV2 - segmentV1;
    float prod = glm::dot(lineDir, planeNorm);

    if (prod == 0)
    {
        // Then line segment and plane are parallel, no intersection
        return false;
    }

    float d = glm::dot(planePoint - segmentV1, planeNorm) / prod;
    intersection = segmentV1 + lineDir * d;
    return true;
}

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos = glm::vec3(2.0f, 0.0f, 1.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool cursor_enabled = true;

struct Triangle
{
    glm::vec3 v0, v1, v2;
};

struct Edge
{
    glm::vec3 v0, v1;
};

struct Polygon
{
    float a, b, c, d;
    int count = 0;
    glm::vec3 norm;
    std::list<Triangle> triangles;
    std::list<glm::vec3> vertices;
    std::list< std::pair<glm::vec3, int> > indexedVertices;
    std::list<Edge> allEdges;
    std::list<Edge> borderEdges;

    auto FindIndexedVertex(glm::vec3 vertex)
    {
        for (auto& pair : indexedVertices)
        {
            if (pair.first == vertex)
            {
                return pair.second;
            }
        }
    }
};



/* Static members definition */
GameController* GameController::m_GameController;

GameController* GameController::Init(int screenWidth, int screenHeight)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return m_GameController = new GameController(window, screenWidth, screenHeight);
}

void GameController::Cleanup()
{
	delete m_GameController;
}

GameController::GameController(GLFWwindow* window, int screenWidth, int screenHeight)
    : m_GlfwWindow(window), m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight),
    m_DeltaTime(0.0f), m_LastFrame(0.0f), gameState(GameState::LOADING)
{
}

GameController::~GameController()
{
}

void GameController::Run()
{
    /* ----------------------- IMGUI -----------------------*/
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(m_GlfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();

    /* Cube */
    //std::vector<Point> points;
    //points.push_back(Point(0, 0, 0));
    //points.push_back(Point(1, 0, 0));
    //points.push_back(Point(0, 0, 1));
    //points.push_back(Point(1, 0, 1));
    //points.push_back(Point(0, 1, 0));
    //points.push_back(Point(1, 1, 0));
    //points.push_back(Point(0, 1, 1));
    //points.push_back(Point(1, 1, 1));

    std::vector<Point_3> points_before2;
    points_before2.push_back(Point_3(0, 0, 0));
    points_before2.push_back(Point_3(1, 0, 0));
    points_before2.push_back(Point_3(0, 0, 1));
    points_before2.push_back(Point_3(1, 0, 1));
    points_before2.push_back(Point_3(0, 1, 0));
    points_before2.push_back(Point_3(1, 1, 0));
    points_before2.push_back(Point_3(0, 1, 1));
    points_before2.push_back(Point_3(1, 1, 1));
    points_before2.push_back(Point_3(0.5, 2.0, 0.5));

    glm::mat4 transform(1.0);
    transform = glm::rotate(transform, glm::radians(20.0f), glm::vec3(1.0, 0.5, 0.3));

    std::vector<Point_3> points_before;
    for (auto& point : points_before2)
    {
        glm::vec4 p(point[0], point[1], point[2], 1.0f);
        p = transform * p;
        points_before.push_back(Point_3(p.x, p.y, p.z));
    }

    std::cout << "Hello";
    /* Tetrahedra */
    //std::vector<Point> points;
    //points.push_back(Point(0, 0, 0));
    //points.push_back(Point(1, 0, 0));
    //points.push_back(Point(0, 0, 1));
    //points.push_back(Point(0, 1, 0));

    //std::vector<Point_3> points;
    //points.push_back(Point_3(0, 0, 0));
    //points.push_back(Point_3(1, 0, 0));
    //points.push_back(Point_3(0, 0, 1));
    //points.push_back(Point_3(0, 1, 0));
    //points.push_back(Point_3(0, 0, 0));
    //points.push_back(Point_3(1, 0, 0));
    //points.push_back(Point_3(0, 0, 1));
    //points.push_back(Point_3(1, 0, 1));
    //points.push_back(Point_3(0.5, 1, 0.5));
    //points.push_back(Point_3(0.5, -1, 0.5));

    //std::vector<std::pair<Point, int>> points;
    //points.push_back(std::make_pair(Point(0, 0, 0), 0));
    //points.push_back(std::make_pair(Point(1, 0, 0), 1));
    //points.push_back(std::make_pair(Point(0, 1, 0), 2));
    //points.push_back(std::make_pair(Point(0, 0, 1), 3));

    //std::vector<Point> points;
    //points.push_back(Point(0, 0, 0));
    //points.push_back(Point(1, 0, 0));
    //points.push_back(Point(0, 1, 0));
    //points.push_back(Point(0, 0, 1));


    //Delaunay triangulation(points.begin(), points.end());
    //Delaunay::Finite_facets_iterator fit;
    //int i = 0;
    //int num = triangulation.number_of_finite_facets();
    //int size = num * 3 * 8;
    //float *vertices = new float[size];
    //for (fit = triangulation.finite_facets_begin(); fit != triangulation.finite_facets_end(); ++fit) {
    //    
    //    //std::cout << "Facet " << i++ << std::endl;



    //    Delaunay::Triangle t = triangulation.triangle(*fit);
    //    glm::vec3 norm;
    //    float v1_x = t.vertex(0).x();
    //    float v1_y = t.vertex(0).y();
    //    float v1_z = t.vertex(0).z();
    //    glm::vec3 v1(v1_x, v1_y, v1_z);

    //    float v2_x = t.vertex(1).x();
    //    float v2_y = t.vertex(1).y();
    //    float v2_z = t.vertex(1).z();
    //    glm::vec3 v2(v2_x, v2_y, v2_z);

    //    float v3_x = t.vertex(2).x();
    //    float v3_y = t.vertex(2).y();
    //    float v3_z = t.vertex(2).z();
    //    glm::vec3 v3(v3_x, v3_y, v3_z);

    //    norm = glm::cross(v1 - v2, v1 - v3);
    //    norm = glm::normalize(norm);

    //    vertices[i++] = v1_x;
    //    vertices[i++] = v1_y;
    //    vertices[i++] = v1_z;
    //    vertices[i++] = norm.x;
    //    vertices[i++] = norm.y;
    //    vertices[i++] = norm.z;
    //    vertices[i++] = 0.0f;
    //    vertices[i++] = 0.0f;

    //    vertices[i++] = v2_x;
    //    vertices[i++] = v2_y;
    //    vertices[i++] = v2_z;
    //    vertices[i++] = norm.x;
    //    vertices[i++] = norm.y;
    //    vertices[i++] = norm.z;
    //    vertices[i++] = 0.0f;
    //    vertices[i++] = 0.0f;

    //    vertices[i++] = v3_x;
    //    vertices[i++] = v3_y;
    //    vertices[i++] = v3_z;
    //    vertices[i++] = norm.x;
    //    vertices[i++] = norm.y;
    //    vertices[i++] = norm.z;
    //    vertices[i++] = 0.0f;
    //    vertices[i++] = 0.0f;
    //    //t.vertex(i);
    //    //std::cout << t.vertex(0) << std::endl << t.vertex(1) << std::endl << t.vertex(2) << "\n\n";

    //    //Triangulation::Facet_handle face = fit;
    //    //triangulation.triangle(fit);
    //    //std::cout << "Triangle:\t" << triangulation.triangle(fit) << std::endl;
    //    //std::cout << "Vertex 0:\t" << triangulation.triangle(fit)[0] << std::endl;
    //    //std::cout << "Vertex 0:\t" << face->vertex(0)->info() << std::endl;
    //    
    //}


    Polyhedron_3 poly_before;
    CGAL::convex_hull_3(points_before2.begin(), points_before2.end(), poly_before);
    
    /* Plane and line segment intersection test */
    glm::vec3 v1(1.0f, 0.0f, 0.5f), v2(0.5f, 0.0f, 1.0f);
    float k = (v2.z - v1.z) / (v2.x - v1.x);
    float b = v1.z - k * v1.x;

    auto getZ = [&](float x) {
        return k * x + b;
    };

    glm::vec3 pv1(0.0f, 0.0f, getZ(0.0f)), pv2(1.0f, 1.0f, getZ(1.0f)), pv3(-1.0f, -1.0f, getZ(-1.0f));
    glm::vec3 planeNorm = glm::cross(pv2 - pv1, pv2 - pv3);

    
    //std::vector<Point_3> points;
    Polyhedron_3::Edge_iterator eit;
    int j = 0;
    for (eit = poly_before.edges_begin(); eit != poly_before.edges_end(); eit++)
    {
        //std::cout << "Edge " << j++ << ": (" << eit->vertex()->point() << ", " << eit->next()->vertex()->point() << ")" << std::endl;
        //std::cout << "Edge " << j++ << std::endl;
        //LineSegmentAndPlaneIntersection(pv1, planeNorm, , );
    }
    //std::cout << "Edge " << j << std::endl;
    //std::cout << "Facets " << poly_before.size_of_halfedges() << std::endl;

    //std::vector<Point_3> points;
    //points.push_back(Point_3(0, 0, 0));
    //points.push_back(Point_3(1, 0, 0));
    //points.push_back(Point_3(0, 0, 1));
    //points.push_back(Point_3(1, 0, 1));
    //points.push_back(Point_3(0, 1, 0));
    //points.push_back(Point_3(1, 1, 0));
    //points.push_back(Point_3(0, 1, 1));
    //points.push_back(Point_3(1, 1, 1));
    Polyhedron_3 poly;
    CGAL::convex_hull_3(points_before.begin(), points_before.end(), poly);
    int num = poly.size_of_facets();
    int size = num * 3 * 8;
    float *vertices = new float[size];

    Polyhedron_3::Facet_iterator fit;

    int i = 0;

    std::list<Polygon> polygons;
    std::list<Edge> edges;

    auto round = [](float num) {
        return std::round(num * 1000) / 1000;
    };

    for (fit = poly.facets_begin(); fit != poly.facets_end(); fit++)
    {
        Polyhedron_3::Facet facet = *fit;
        auto v1_raw = facet.halfedge()->vertex()->point();
        auto v2_raw = facet.halfedge()->next()->vertex()->point();
        auto v3_raw = facet.halfedge()->opposite()->vertex()->point();
        
        //std::cout << "Facet " << i++ << ": " << v1[0] << " " << v1[1] << " " << v1[2] << std::endl;

        glm::vec3 v1(v1_raw[0], v1_raw[1], v1_raw[2]);
        glm::vec3 v2(v2_raw[0], v2_raw[1], v2_raw[2]);
        glm::vec3 v3(v3_raw[0], v3_raw[1], v3_raw[2]);
        glm::vec3 norm;

        norm = glm::cross(v1 - v2, v1 - v3);
        norm = glm::normalize(norm);

        Edge edge1{ v1, v2 }, edge2{ v1, v3 }, edge3{ v2, v3 };
        auto containsEdge = [](std::list<Edge> edges, Edge target_edge) {
            auto res = std::find_if(edges.begin(), edges.end(), [&](Edge edge) {
                return
                    target_edge.v0 == edge.v0 && target_edge.v1 == edge.v1 ||
                    target_edge.v1 == edge.v0 && target_edge.v0 == edge.v1;
            });
            return res != edges.end();
        };
        auto containsVertex = [](std::list<glm::vec3> vertices, glm::vec3 target_vertex) {
            auto res = std::find_if(vertices.begin(), vertices.end(), [&](glm::vec3 vertex) {
                return vertex == target_vertex;
                });
            return res != vertices.end();
        };
        //auto containsEdge1 = [&](Edge edge) { return edge.v0 == edge1.v0 && edge.v1 == edge1.v1 || edge.v1 == edge1.v0 && edge.v0 == edge1.v1; };
        if (!containsEdge(edges, edge1))
            edges.push_back(edge1);
        if (!containsEdge(edges, edge2))
            edges.push_back(edge2);
        if (!containsEdge(edges, edge3))
            edges.push_back(edge3);

        //float a = norm.x;
        //float b = norm.y;
        //float c = norm.z;
        //float d = -glm::dot(norm, v1);
        float a = round(norm.x);
        float b = round(norm.y);
        float c = round(norm.z);
        float d = round(-glm::dot(norm, v1));
        std::cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << std::endl;

        bool found = false;
        Triangle triangle{ v1, v2, v3 };
        for (auto& poly : polygons)
        {
            if (poly.a == a && poly.b == b && poly.c == c && poly.d == d)
            {
                found = true;
                if (!containsEdge(poly.allEdges, edge1))
                    poly.allEdges.push_back(edge1);
                if (!containsEdge(poly.allEdges, edge2))
                    poly.allEdges.push_back(edge2);
                if (!containsEdge(poly.allEdges, edge3))
                    poly.allEdges.push_back(edge3);
                if (!containsVertex(poly.vertices, v1))
                    poly.vertices.push_back(v1);
                if (!containsVertex(poly.vertices, v2))
                    poly.vertices.push_back(v2);
                if (!containsVertex(poly.vertices, v3))
                    poly.vertices.push_back(v3);
                poly.triangles.push_back(triangle);
                poly.count++;
            }
        }

        if (!found)
        {
            Polygon polygon{ a, b, c, d };
            if (!containsEdge(polygon.allEdges, edge1))
                polygon.allEdges.push_back(edge1);
            if (!containsEdge(polygon.allEdges, edge2))
                polygon.allEdges.push_back(edge2);
            if (!containsEdge(polygon.allEdges, edge3))
                polygon.allEdges.push_back(edge3);
            if (!containsVertex(polygon.vertices, v1))
                polygon.vertices.push_back(v1);
            if (!containsVertex(polygon.vertices, v2))
                polygon.vertices.push_back(v2);
            if (!containsVertex(polygon.vertices, v3))
                polygon.vertices.push_back(v3);
            polygon.norm = norm;
            polygon.triangles.push_back(triangle);
            polygon.count++;
            polygons.push_back(polygon);
        }

        //vertices[i++] = v1.x;
        //vertices[i++] = v1.y;
        //vertices[i++] = v1.z;
        //vertices[i++] = norm.x;
        //vertices[i++] = norm.y;
        //vertices[i++] = norm.z;
        //vertices[i++] = 0.0f;
        //vertices[i++] = 0.0f;

        //vertices[i++] = v2.x;
        //vertices[i++] = v2.y;
        //vertices[i++] = v2.z;
        //vertices[i++] = norm.x;
        //vertices[i++] = norm.y;
        //vertices[i++] = norm.z;
        //vertices[i++] = 0.0f;
        //vertices[i++] = 0.0f;

        //vertices[i++] = v3.x;
        //vertices[i++] = v3.y;
        //vertices[i++] = v3.z;
        //vertices[i++] = norm.x;
        //vertices[i++] = norm.y;
        //vertices[i++] = norm.z;
        //vertices[i++] = 0.0f;
        //vertices[i++] = 0.0f;
    }

    //std::cout << "Edges num = " << edges.size() << std::endl;

    //float nn = 1.0;
    //glm::vec3 n(0, 1, 0), _v1(nn, 0, 0), _v2(0, 0, nn), _v3(0, 0, -nn), _v4(-nn, 0, 0), center(0.01f, 0, 0.01f),
    //    _v5(-0.5f, 0.0f, -0.5f), _v6(0.5f, 0.0f, -0.5f),
    //    _v7(-0.5f, 0.0f, 0.5f), _v8(0.5f, 0.0f, 0.5f);
    //float res = glm::dot(n, glm::cross(_v1 - center, _v5 - center));
    //float res2 = glm::dot(n, glm::cross(_v1 - center, _v6 - center));
    //float res3 = glm::dot(n, glm::cross(_v1 - center, _v7 - center));
    //float res4 = glm::dot(n, glm::cross(_v1 - center, _v8 - center));
    //float res5 = glm::dot(n, glm::cross(_v1 - center, _v4 - center));
    //std::cout << "Order is " << res << std::endl;


    //std::list<glm::vec3> pts{
    //    glm::vec3(0.0, 0.0, 0.0),
    //    glm::vec3(1.0, 0.0, 0.0),
    //    glm::vec3(0.0, 0.0, 1.0),
    //    glm::vec3(1.0, 0.0, 1.0)
    //};
    //glm::vec3 ctr(0.5, 0.0, 0.5), norm(0.0, 1.0, 0.0);

    //pts.sort([&](glm::vec3 v1, glm::vec3 v2) {
    //    return glm::dot(norm, glm::cross(v1 - ctr, v2 - ctr)) < 0;
    //});

    for (auto& poly : polygons)
    {
        glm::vec3 center(0.0);
        for (auto& v : poly.vertices)
        {
            center += v;
        }
        center /= poly.vertices.size();
        float delta = 0.1f; // TODO: when the minimum unit is defined, adjust this value to be smaller than min unit
        center += delta;

        // https://stackoverflow.com/questions/14370636/sorting-a-list-of-3d-coplanar-points-to-be-clockwise-or-counterclockwise
        poly.vertices.sort([&](glm::vec3 v1, glm::vec3 v2) {
            return glm::dot(poly.norm, glm::cross(v1 - center, v2 - center)) < 0;
        });
    }

    for (auto& poly : polygons)
    {
        int i = 0;
        for (auto& vertex : poly.vertices)
        {
            poly.indexedVertices.push_back(std::make_pair(vertex, i++));
        }
    }

    int linesVerticesNum = 0;
    for (auto& poly : polygons)
    {
        for (auto& edge : poly.allEdges)
        {
            int v0Index = poly.FindIndexedVertex(edge.v0);
            int v1Index = poly.FindIndexedVertex(edge.v1);
            if (std::abs(v0Index - v1Index) == 1 ||
                std::abs(v0Index - v1Index) == poly.indexedVertices.size() - 1)
            {
                poly.borderEdges.push_back(edge);
                linesVerticesNum += 2;
            }
        }
    }

    i = 0;
    int linesSize = linesVerticesNum * 8;
    float* linesVertices = new float[linesSize];
    for (auto& poly : polygons)
    {
        glm::vec3 norm = poly.norm;

        for (auto& edge : poly.borderEdges)
        {
            glm::vec3 v1 = edge.v0;
            glm::vec3 v2 = edge.v1;

            linesVertices[i++] = v1.x;
            linesVertices[i++] = v1.y;
            linesVertices[i++] = v1.z;
            linesVertices[i++] = norm.x;
            linesVertices[i++] = norm.y;
            linesVertices[i++] = norm.z;
            linesVertices[i++] = 0.0f;
            linesVertices[i++] = 0.0f;

            linesVertices[i++] = v2.x;
            linesVertices[i++] = v2.y;
            linesVertices[i++] = v2.z;
            linesVertices[i++] = norm.x;
            linesVertices[i++] = norm.y;
            linesVertices[i++] = norm.z;
            linesVertices[i++] = 0.0f;
            linesVertices[i++] = 0.0f;
        }
    }

    Shader ourShader("shaders/object2.vert.glsl", "shaders/object2.frag.glsl");

    unsigned int objectVBO, objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glGenBuffers(1, &objectVBO);

    glBindVertexArray(objectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
    glBufferData(GL_ARRAY_BUFFER, linesSize * sizeof(float), linesVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /* Lamp */
    Shader lampShader("shaders/lamp.vert.glsl", "shaders/lamp.frag.glsl");
    unsigned int lampVBO, lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glGenBuffers(1, &lampVBO);

    glBindVertexArray(lampVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
    glBufferData(GL_ARRAY_BUFFER, linesSize * sizeof(float), linesVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 containerModel = glm::mat4(1.0);

    ourShader.use();
    ourShader.setInt("texture_diffuse0", 0);
    ourShader.setInt("texture_specular0", 1);
    ourShader.setInt("material.emission", 2);

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
    };

    float ambient = 0.5f;
    float diffuse = 0.5f;
    float specular = 1.0f;

    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(m_GlfwWindow))
    {
        /*float currentFrame = glfwGetTime();
        m_DeltaTime = currentFrame - m_LastFrame;
        m_LastFrame = currentFrame;*/

        /* ImGui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(m_GlfwWindow);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        /* Draw a container */
        ourShader.use();

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        ourShader.setFloat("material.shininess", 64.0f);

        ourShader.setVec3("dirLight.direction", -0.1f, -0.5f, -0.2f);
        ourShader.setVec3("dirLight.ambient", ambient, ambient, ambient);
        ourShader.setVec3("dirLight.diffuse", diffuse, diffuse, diffuse);
        ourShader.setVec3("dirLight.specular", specular, specular, specular);

        ourShader.setVec3("pointLights[0].position", lightPos[0], lightPos[1], lightPos[2]);
        ourShader.setVec3("pointLights[0].ambient", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[0].constant", 1.0f);
        ourShader.setFloat("pointLights[0].linear", 0.09f);
        ourShader.setFloat("pointLights[0].quadratic", 0.032f);

        ourShader.setVec3("viewPos", cameraPos);

        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            ourShader.setMat4("model", model);
            glBindVertexArray(objectVAO);
            glDrawArrays(GL_LINES, 0, linesVerticesNum);
        }
        

        /* Draw a lamp */
        {
            lampShader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.1f));
            lampShader.setMat4("model", model);
            lampShader.setMat4("projection", projection);
            lampShader.setMat4("view", view);
            glBindVertexArray(lampVAO);
            glDrawArrays(GL_LINES, 0, linesVerticesNum);
        }
        
        ImGui::Begin("Global light");
        ImGui::SliderFloat("Light position x", &lightPos.x, -2.0f, 2.0f);
        ImGui::SliderFloat("Light position y", &lightPos.y, -2.0f, 2.0f);
        ImGui::SliderFloat("Light position z", &lightPos.z, -2.0f, 2.0f);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* End */


        Update(m_DeltaTime);
        Render();

        glfwSwapBuffers(m_GlfwWindow);
        glfwPollEvents();
    }

    delete vertices;
    delete linesVertices;
    glfwTerminate();
}

void GameController::Update(float deltaTime)
{
    switch (gameState)
    {
    case GameState::LOADING:
    {
        /* nothing yet to load, go immediately to the next stage */
        gameState = GameState::PLAYING;
        break;
    }
    case GameState::MENU:
    {
        break;
    }
    case GameState::PLAYING:
    {
        break;
    }
    }
}

void GameController::Render()
{
    switch (gameState)
    {
    case GameState::LOADING:
    {
        break;
    }
    case GameState::MENU:
    {
        break;
    }
    case GameState::PLAYING:
    {
        break;
    }
    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        lightPos.x -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        lightPos.x += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        lightPos.z -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        lightPos.z += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        lightPos.y -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        lightPos.y += 0.01f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if (cursor_enabled)
    {
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        firstMouse = true;
        cursor_enabled = !cursor_enabled;
        glfwSetInputMode(window, GLFW_CURSOR, cursor_enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}
