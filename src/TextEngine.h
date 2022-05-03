#ifndef TEXTENGINE_H
#define TEXTENGINE_H



#include <ft2build.h>
#include FT_FREETYPE_H
//#include "GL/glew.h"

#include<GL/glew.h>
#include <GLFW/glfw3.h>




#include "ShaderObject.h"

//#include "/local1/delliott/SOIL/src/SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"



//Tent engine can render text for us using its own texture and render to screen.
//Also should provide an overriden renderText() function for writing into a provided texture (using a framebuffer) so that UIViews can keep them.
struct point
    {
        point() : x(0.0f), y(0.0f), s(0.0f), t(0.0f) {}
        point(GLfloat X, GLfloat Y, GLfloat S, GLfloat T) : x(X), y(Y), s(S), t(T) {}
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    };
struct point2
{
        point2() : x(0.0f), y(0.0f) {}
        point2(GLfloat X, GLfloat Y) : x(X), y(Y) {}
        GLfloat x;
        GLfloat y;
};

    struct character_info
    {
        float ax; // advance.x
        float ay; // advance.y

        float bw; // bitmap.width;
        float bh; // bitmap.rows;

        float bl; // bitmap_left;
        float bt; // bitmap_top;

        float tx; // x offset of glyph in texture coordinates
        float ty;

        float hY; //horibearingY
    };

    struct atlas
        {
            GLuint tex;
            FT_GlyphSlot g;

            int w;
            int h;
            //int maxYOffset;

            int atlas_width;
            character_info c[128];

            atlas(FT_Face face, int height);
            ~atlas();

        };
class TextEngine {


public:






    TextEngine();
    ~TextEngine();
    void render_text(const char *text, float x, float y, float sx, float sy);
    //atlas * get48Atlas();
    static void checkGLError();
    void initResources();

private:


    GLint get_uniform(GLuint program, const char *name);
    GLint get_attrib(GLuint program, const char *name);


    FT_Library ft;
    FT_Face face;

    int atlas_width;

    GLuint vbo;
    GLuint vao;// remember all of the states that is usually set by vertexarraypointer etc.
    GLint attribute_position;
    GLint uniform_tex;
    GLint uniform_color;
    GLint vPosition;
    GLint shaderMatrix;

    ShaderObject *textShader;

    atlas *a48;

    glm::mat4 model;
    glm::mat4 view;//cam/world matrix. move in opposite direction to simulate moving camera
    glm::mat4 proj;
    glm::mat4 modelViewProjMat;







};

#endif