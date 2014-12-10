#include "TextEngine.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>




using namespace std;

#define MAXWIDTH 1024

TextEngine::TextEngine()
{

    cout << "textEng constructor" << endl;


}

TextEngine::~TextEngine()
{





}

atlas::atlas(FT_Face face, int height)
{

    cout << "creating new atlas" << endl;
    FT_Set_Pixel_Sizes(face, 0, height);
    g = face->glyph;

    memset(c, 0, sizeof c); //set character info struct data to 0
    w = 0;
    h = 0;


    //load face into memory
    for(int i = 32; i < 128; i++)
    {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER	))
        {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }



        w += g->bitmap.width;
        h = std::max(h, g->bitmap.rows);



        atlas_width = w;
        //cout << "height " << h << endl;

    }




    glActiveTexture(GL_TEXTURE0);
    TextEngine::checkGLError();

    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     /* Paste all glyph bitmaps into the texture, remembering the offset */
    int x = 0;
    int y = 0;
    //maxYOffset = 0;


    for (int i = 32; i < 128; i++)
    {
        if (FT_Load_Char(face, i,FT_LOAD_DEFAULT))
        {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }

        if(FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL))
        {
            fprintf(stderr, "rendering character %c failed!\n", i);
            continue;
        }

        //FT_Render_Glyph (g, FT_RENDER_MODE_NORMAL );

        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        x,
                        0,
                        g->bitmap.width,
                        g->bitmap.rows,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        g->bitmap.buffer);


        //int copySize = g->bitmap.width*g->bitmap.rows;


        if (i==32)
            c[i].ax = g->advance.x >> 6;
        else
            c[i].ax = (g->advance.x >> 6) *0.99;
        c[i].ay = g->advance.y >> 6;
        //c[i].ay = 0.0;
        c[i].bw = g->bitmap.width;
        c[i].bh = g->bitmap.rows;
        c[i].bl = g->bitmap_left;
        c[i].bt = g->bitmap_top;
        c[i].tx = (float)x / (float)w;
        c[i].ty = (float)y / (float)h;
        c[i].hY = g->metrics.horiBearingY;
        //cout << "bitmap top is " << c[i].bt << endl;
        //cout << "glyph metric vertical bearing is " << (float)g->metrics.horiBearingY/64.0 << endl;
        //cout << "bitmap height is " << g->bitmap.rows << endl;
        //cout << "glyph metric height is " << g->metrics.height/64.0 << endl;;



        x += g->bitmap.width;
        //maxYOffset = std::min(maxYOffset, g->bitmap_top-g->bitmap.rows);
        //y = std::max(y, g->bitmap.rows);
        //cout << "maxOffset is " << maxYOffset << endl;
        //cout << "total texture width is" << x << endl;

    }



    fprintf(stderr, "Generated a %d x %d (%d kb) texture atlas\n", w, h, w * h / 1024);

}
    //atlas_width = w;


atlas::~atlas()
{
    if (glIsTexture(tex))
    {
        glDeleteTextures(1, &tex);
    }


}

GLint TextEngine::get_uniform(GLuint program, const char *name)
{
    GLint uniform = glGetUniformLocation(program, name);
    if(uniform == -1)
    fprintf(stderr, "Could not bind uniform %s\n", name);
    return uniform;
}


GLint TextEngine::get_attrib(GLuint program, const char *name)
{
    GLint attribute = glGetAttribLocation(program, name);
    if(attribute == -1)
    fprintf(stderr, "Could not bind attribute %s\n", name);
    return attribute;
}

void TextEngine::initResources()
{

    cout << "in init" << endl;

     if(FT_Init_FreeType(&ft)) {
    fprintf(stderr, "Could not init freetype library\n");
    return;
     }


    if(FT_New_Face(ft, "Helvetica.ttf", 0, &face)) {
      fprintf(stderr, "Could not open font\n");
      return;
    }
    cout << "fonts loaded" << endl;
    textShader = new ShaderObject();

    textShader->loadProgram("textShader.vert", "textShader.frag");
    //checkGLError();

    //textShader->printProgramLog(textShader->getProgramID());

    cout << "shader created" << endl;

    glGenVertexArrays(1, &vao);
    //checkGLError();
    glBindVertexArray(vao);
    //checkGLError();


    glGenBuffers(1, &vbo);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);

    textShader->bind();

    attribute_position = get_attrib(textShader->getProgramID(), "vPosition");
    //checkGLError();

    shaderMatrix = get_uniform(textShader->getProgramID(), "modelViewProjectionMatrix");
    //checkGLError();


    uniform_tex = get_uniform(textShader->getProgramID(), "tex");
    checkGLError();
    textShader->unbind();
    //
    //

    //uniform_color = get_uniform(textShader->getProgramID(), "color");
    //
    //



    if(attribute_position == -1)
        cout << "coord is messed" << endl;
    //if(uniform_tex == -1 )
        //cout << "tex is messed" << endl;
    //if(uniform_color == -1)
        //cout << "color is messed" << endl;




    cout << "creating altas" << endl;

    a48 = new atlas(face,  20);

    model = glm::mat4(1.0f); //identity
    view = glm::mat4(1.0f); //identity
    proj = glm::ortho(0.0f, 1024.0f, 0.0f, 576.0f, 100.0f, -100.0f); //proj of main window
    //proj = glm::mat4(1.0f);
    modelViewProjMat = proj*view*model;

}

/*
atlas* TextEngine::get48Atlas()
{

    return a48;

}
*/


void TextEngine::render_text(const char *text, float x, float y, float sx, float sy)
{

    //checkGLError();
    const uint8_t *p;

    atlas *a = a48;

    //cout << "rendering text" << endl;
    //glPushAttrib(GL_ALL_ATTRIB_BITS);
    //glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, a->tex);
    //glUniform1i(uniform_tex, 0);//set the programs uniform variable that we got from the name to the texture unit 0


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);










    //GLfloat black[4] = { 0, 0, 0, 1 };
    //glUniform4fv(uniform_color, 1, black);

    //glGetError();
    /* Set up the VBO for our vertex data */







    point coords[6 * strlen(text)];
    //point2 *coords = (point2*)malloc(sizeof(point2) * 6 * strlen(text));
    //glBufferData(GL_ARRAY_BUFFER, sizeof (coords), coords, GL_DYNAMIC_DRAW);

    //point coords[6 * strlen(text)];
    int c = 0;

    /* Loop through all characters */
    for (p = (const uint8_t *)text; *p; p++)
    {
        /* Calculate the vertex and texture coordinates */
        float bt = 0.0 ;


        //float bt =  (a->c[*p].bt)-(a->c[*p].bh);
        //if (bt < -6)
        //    bt = -1;

        //cout << "ydiff is " << (a->c[*p].bt)-(a->c[*p].bh) << endl;
        float x2 =  x + (a->c[*p].bl) * sx;
        //float y2 =  - y - (a->c[*p].bh) * sy;

        //float y2 =  - y - (a->c[*p].bt) * sy;
        float y2 =   -y - (a->c[*p].bt) * sy ;

        //cout <<"bt is " << (a->c[*p].bt) << endl;
        //cout << y2 << endl;


        float w = a->c[*p].bw * sx;
        float h = a->c[*p].bh * sy;

        x += a->c[*p].ax * sx;
        //y += a->c[*p].ay * sy;
        y += 0.0;


        if(!w || !h)
            continue;


        coords[c++] = point(x2,    -y2    ,      a->c[*p].tx,                a->c[*p].ty);
        coords[c++] =  point(x2 + w, -y2 ,  a->c[*p].tx + a->c[*p].bw / (float)a->atlas_width   , a->c[*p].ty);
        coords[c++] =     point(x2 ,    -y2 -h,        a->c[*p].tx,         a->c[*p].ty + a->c[*p].bh / (float)a->h);
        coords[c++] = point(x2 + w, -y2  , a->c[*p].tx + a->c[*p].bw / (float)a->atlas_width, a->c[*p].ty);
        coords[c++] = point(x2,  -y2 -h ,       a->c[*p].tx,          a->c[*p].ty + a->c[*p].bh / (float)a->h);
        coords[c++] = point(x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->atlas_width,a->c[*p].ty + a->c[*p].bh / (float)a->h);


        /*
        coords[c++] = point2(x2,    -y2   );
        coords[c++] =  point2(x2 + w, -y2 );
        coords[c++] =     point2(x2 ,    -y2 - h);
        coords[c++] = point2(x2 + w, -y2  );
        coords[c++] = point2(x2,  -y2 - h );
        coords[c++] = point2(x2 + w, -y2 - h );
        */

        /*
        coords[c++] = point2(0.0f,    0.0f   );
        coords[c++] = point2(100.0f , 100.0f );
        coords[c++] = point2(0.0f ,    100.0f );
        coords[c++] = point2(0.0f, 0.0f  );
        coords[c++] = point2(100.0f,  0.0f );
        coords[c++] = point2(100.0f, 100.0f );
        */


    }

    //cout << "size of coords is " << sizeof(coords) << endl;


    /* Draw all the character on the screen in one go */

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //checkGLError();
    glBufferData(GL_ARRAY_BUFFER, sizeof (coords), coords, GL_DYNAMIC_DRAW);
    //checkGLError();
    glVertexAttribPointer(attribute_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //checkGLError();
    glEnableVertexAttribArray(attribute_position);
    //checkGLError();
    //glUniformMatrix4fv(shaderMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjMat) );
    //checkGLError();

    //point2 *coords2 = (point2*)malloc(sizeof(point2) * 6 * strlen(text));
    //point2 coords2[6 * strlen(text)];
    //glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof coords2, coords2);
    //checkGLError();
    //cout << "orig coords data is " << coords[2].x << " " << coords[2].y << endl;
    //cout << "copied buffer data is " << coords2[2].x << " " << coords2[2].y << endl;

   // if (glIsBuffer(vbo) != GL_TRUE)
    //{
    //    cout << "buffer is not buffer" << endl;
    //}





    //glVertexPointer(2, GL_FLOAT, 4*sizeof(GLfloat), &coords[0]);
    //checkGLError();

    textShader->bind();




    glUniformMatrix4fv(shaderMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjMat) );
    //checkGLError();




    //checkGLError();
    glDrawArrays(GL_TRIANGLES, 0, c);
    //checkGLError();
    glDisableVertexAttribArray(attribute_position);
    //checkGLError();





/*
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < c; i++)
    {
        glTexCoord2f(coords[i].s, coords[i].t);
        glVertex3f(coords[i].x, coords[i].y, -50.0);

    }
    glEnd();
    */
    textShader->unbind();
    //delete coords;
    //glPopAttrib();



    //glBindTexture(GL_TEXTURE_2D, NULL);
}


void TextEngine::checkGLError()
{
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error! %s\n", gluErrorString( error ) );
    }

}
