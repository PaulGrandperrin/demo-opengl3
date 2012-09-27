#include "graphicEngine.hpp"
#include <SDL/SDL.h>
#include <IL/il.h>

#include <vector>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <algorithm>

//#define PI 3.141592653589793238462

int ExtensionSupportee ( const char *nom );
int InitExtensions ( void );
static char* LoadSource ( const char *filename );
GLuint LoadShader ( GLenum type, const char *filename );
int loadTexture ( char* imagePath );


void matrix4Inv ( float M[4][4],float MS[4][4] );
float inline vecteur3Dot ( float u[3],float v[3] );
float inline vecteur3Norme ( float a[3] );
void matrix3Inv ( float M[3][3],float MS[3][3] );
void inline matrix4Mul ( float a[4][4],float b[4][4] );
void inline matrix4Id ( float a[4][4] );


using namespace std;


struct v
{
    float x,y,z;
};
struct vn
{
    float x,y,z;
};
struct vt
{
    float x,y;
};
struct vertex
{
    unsigned int vId,vnId,vtId;
};
struct vertexf
{
    int vertexId;
    v vV;
    vn vVn;
    vt vVt;
};



struct vertexFinal
{
    v vV;
    vn vVn;
    vt vVt;
};

bool vertexEgal ( const vertexf & a,const vertexFinal & b )
{
    return
        a.vV.x==b.vV.x && a.vV.y==b.vV.y && a.vV.z==b.vV.z &&
        a.vVn.x==b.vVn.x && a.vVn.y==b.vVn.y && a.vVn.z==b.vVn.z &&
        a.vVt.x==b.vVt.x && a.vVt.y==b.vVt.y;
}

bool vertexfLess ( const vertexf &a, const vertexf &b )
{
    if ( a.vV.x<b.vV.x )
        return true;
    else if ( a.vV.x>b.vV.x )
        return false;
    else
        if ( a.vV.y<b.vV.y )
            return true;
        else if ( a.vV.y>b.vV.y )
            return false;
        else
            if ( a.vV.z<b.vV.z )
                return true;
            else if ( a.vV.z>b.vV.z )
                return false;
            else
                if ( a.vVn.x<b.vVn.x )
                    return true;
                else if ( a.vVn.x>b.vVn.x )
                    return false;
                else
                    if ( a.vVn.y<b.vVn.y )
                        return true;
                    else if ( a.vVn.y>b.vVn.y )
                        return false;
                    else
                        if ( a.vVn.z<b.vVn.z )
                            return true;
                        else if ( a.vVn.z>b.vVn.z )
                            return false;
                        else
                            if ( a.vVt.x<b.vVt.x )
                                return true;
                            else if ( a.vVt.x>b.vVt.x )
                                return false;
                            else
                                if ( a.vVt.y<b.vVt.y )
                                    return true;
                                else
                                    return false;
}


GraphicEngine::GraphicEngine()
{
    GraphicEngine ( 640,480,false );
}

GraphicEngine::GraphicEngine ( unsigned int winWidth,unsigned int winHeight,bool fullscreen )
{
    m_window.width=winWidth;
    m_window.height=winHeight;
    m_window.fullscreen=fullscreen;

    m_camera.x=1;
    m_camera.y=1;
    m_camera.z=1;
    m_camera.ax=0;
    m_camera.ay=0;
    m_camera.az=0;
    m_camera.fovy=70;

    m_backgroundColor.R=0;
    m_backgroundColor.G=0;
    m_backgroundColor.B=0;
    m_backgroundColor.A=1;


    SDL_Init ( SDL_INIT_VIDEO );
    SDL_SetVideoMode ( m_window.width, m_window.height, 32, m_window.fullscreen? ( SDL_OPENGL|SDL_FULLSCREEN ) :SDL_OPENGL );

    //const char* glVersion=glGetString(GL_VERSION);
    //if(glVersion[0]!=3 || glVersion[2] < '1' || glVersion[2] > '2')
    //    return false;

    SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );

    glViewport ( 0,0,m_window.width,m_window.height );

    glClearColor ( m_backgroundColor.R,m_backgroundColor.G,m_backgroundColor.B,m_backgroundColor.A );

    ilInit();
    glewInit();

    //On calcule la matrice de projection

    float f=1.0/tan ( ( M_PI*m_camera.fovy ) / ( 360.0 ) );
    float zFar=1000;
    float zNear=0.01;
    projectionMatrix[0][0]=f/ ( ( float ) winWidth/winHeight );
    projectionMatrix[0][1]=0.0;
    projectionMatrix[0][2]=0.0;
    projectionMatrix[0][3]=0.0;
    projectionMatrix[1][0]=0.0;
    projectionMatrix[1][1]=f;
    projectionMatrix[1][2]=0.0;
    projectionMatrix[1][3]=0.0;
    projectionMatrix[2][0]=0.0;
    projectionMatrix[2][1]=0.0;
    projectionMatrix[2][2]= ( zFar+zNear ) / ( zNear-zFar );
    projectionMatrix[2][3]=-1.0;
    projectionMatrix[3][0]=0.0;
    projectionMatrix[3][1]=0.0;
    projectionMatrix[3][2]= ( 2.0*zFar*zNear ) / ( zNear-zFar );
    projectionMatrix[3][3]=0.0;


    //On met une lumière de base

    m_lightDir[0]=1.0;
    m_lightDir[1]=0;
    m_lightDir[2]=0;

    glEnable ( GL_TEXTURE_2D );
}

void GraphicEngine::setCameraPosition ( float x,float y, float z, float ax,float ay, float az )
{
    m_camera.x=x;
    m_camera.y=y;
    m_camera.z=z;
    m_camera.ax=ax;
    m_camera.ay=ay;
    m_camera.az=az;
}

GraphicEngine::~GraphicEngine()
{

}

void GraphicEngine::setWindowTitle ( const char* title )
{
    SDL_WM_SetCaption ( title, NULL );
}

void GraphicEngine::setShowCursor ( bool show )
{
    SDL_ShowCursor ( show );
}

void GraphicEngine::setBackgroundColor ( unsigned char R,unsigned char G, unsigned char B, unsigned char A )
{
    glClearColor ( R,G,B,A );
}

bool GraphicEngine::loadMesh ( GEint* meshId,const char* pathName )
{


    vector<v> vVect; //le vecteur contenant les donnees brutes de positions
    vector<vn> vnVect;//le vecteur contenant les donnees brutes de normales
    vector<vt> vtVect;//le vecteur contenant les donnees brutes de etxtures
    vector<vertex> fVect; //le vecteur contenant les donnees brutes des faces

    ifstream OBJFile ( pathName,ios::in ); //on ouvre le fichier en lecture
    if ( !OBJFile )
        return false;

    string buffer,key;
    while ( getline ( OBJFile,buffer ) ) //tant qu'on peut lire une ligne
    {
        istringstream line ( buffer );
        key="";
        line >> key;

        if ( key=="v" )
        {
            v vTemp;
            line >> vTemp.x >> vTemp.y >> vTemp.z;
            vVect.push_back ( vTemp );
        }
        else if ( key=="vn" )
        {
            vn vnTemp;
            line >> vnTemp.x >> vnTemp.y >> vnTemp.z;
            vnVect.push_back ( vnTemp );
        }
        else if ( key=="vt" )
        {
            vt vtTemp;
            line >> vtTemp.x >> vtTemp.y;
            vtVect.push_back ( vtTemp );
        }
        else if ( key=="g" )
        {
            //a implémenter
        }
        else if ( key=="f" )
        {
            vertex fTemp;
            string vertex;
            int lim;

            for ( int i=0;i<3;i++ )
            {
                line >> vertex;


                lim=vertex.find_first_of ( '/' );
                fTemp.vId=atoi ( vertex.substr ( 0,lim ).c_str() );
                vertex=vertex.substr ( lim+1,vertex.length()-lim-1 );

                lim=vertex.find_first_of ( '/' );
                if ( lim!=0 )
                {
                    fTemp.vtId=atoi ( vertex.substr ( 0,lim ).c_str() );
                }
                else
                {
                    fTemp.vtId=0;
                }
                vertex=vertex.substr ( lim+1,vertex.length()-lim-1 );

                lim=vertex.find_first_of ( '/' );
                if ( lim!=0 )
                {
                    fTemp.vnId=atoi ( vertex.substr ( 0,lim ).c_str() );
                }
                else
                {
                    fTemp.vnId=0;
                }
                vertex=vertex.substr ( lim+1,vertex.length()-lim-1 );

                fVect.push_back ( fTemp );
            }

        }
    }
    OBJFile.close();
    //On a fini de charger les donn�es du fichier
    //Maintenant, faut organiser les donn�es de maniere optimale pour les utiliser avec des VBO et IBO.

    //On va d'abors construire tout les vertex possibles
    vector<vertexf> allVertex;
    for ( unsigned int i=0;i<fVect.size();i++ )
    {
        vertexf vertexfTemp;
        vertexfTemp.vertexId=i;
        vertexfTemp.vV=vVect[fVect[i].vId-1];
        vertexfTemp.vVn=vnVect[fVect[i].vnId-1];

        if ( fVect[i].vtId != 0 )
            vertexfTemp.vVt=vtVect[fVect[i].vtId-1];
        else
        {
            vertexfTemp.vVt.x=0;
            vertexfTemp.vVt.y=0;
        }

        allVertex.push_back ( vertexfTemp );
    }

    //On va trier ce vecteur pour virer les doublons

    sort ( allVertex.begin(),allVertex.end(),vertexfLess );

    //On peut dore et deja cree le vecteur d'ibo final pour opengl

    unsigned int *arrayIbo= ( unsigned int* ) malloc ( fVect.size() *sizeof ( unsigned int ) );

    //On creer le vecteur des vertex finaux sans doublons

    vector<vertexFinal> vectFinal;
    vertexFinal finalTemp;

    finalTemp.vV=allVertex[0].vV;
    finalTemp.vVn=allVertex[0].vVn;
    finalTemp.vVt=allVertex[0].vVt;

    vectFinal.push_back ( finalTemp );

    arrayIbo[allVertex[0].vertexId]=0;

    for ( unsigned int i=1; i< allVertex.size();i++ )
    {
        if ( !vertexEgal ( allVertex[i],finalTemp ) )
        {
            finalTemp.vV=allVertex[i].vV;
            finalTemp.vVn=allVertex[i].vVn;
            finalTemp.vVt=allVertex[i].vVt;
            vectFinal.push_back ( finalTemp );
        }
        arrayIbo[allVertex[i].vertexId]=vectFinal.size()-1;
    }

    //maintenant, on peut remplir les vbo

    float* arrayVertex= ( float* ) malloc ( allVertex.size() * ( 3+3+2 ) *sizeof ( float ) );


    for ( unsigned int i=0;i<vectFinal.size();i++ )
    {
        arrayVertex[i*8]=vectFinal[i].vV.x;
        arrayVertex[i*8+1]=vectFinal[i].vV.y;
        arrayVertex[i*8+2]=vectFinal[i].vV.z;
        arrayVertex[i*8+3]=vectFinal[i].vVn.x;
        arrayVertex[i*8+4]=vectFinal[i].vVn.y;
        arrayVertex[i*8+5]=vectFinal[i].vVn.z;
        arrayVertex[i*8+6]=vectFinal[i].vVt.x;
        arrayVertex[i*8+7]=vectFinal[i].vVt.y;
    }

    //Et on bind

    GLuint vboVertex;

    glGenBuffers ( 1, &vboVertex );

    glBindBuffer ( GL_ARRAY_BUFFER, vboVertex );
    glBufferData ( GL_ARRAY_BUFFER,
                   allVertex.size() * ( 3+3+2 ) *sizeof ( float ),
                   NULL,
                   GL_STATIC_DRAW );                           // à remplacer en STREAM
    glBufferSubData ( GL_ARRAY_BUFFER,
                      0,
                      allVertex.size() * ( 3+3+2 ) *sizeof ( float ),
                      arrayVertex );


    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

    //Puis l'ibo

    GLuint ibo;
    glGenBuffers ( 1,&ibo );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER,ibo );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER,fVect.size() *sizeof ( unsigned int ),NULL,GL_STATIC_DRAW );
    glBufferSubData ( GL_ELEMENT_ARRAY_BUFFER,0,fVect.size() *sizeof ( unsigned int ),arrayIbo );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER,0 );


    m_mesh.ibo=ibo;
    m_mesh.vboVertex=vboVertex;
    m_mesh.x=0;
    m_mesh.y=0;
    m_mesh.z=0;
    m_mesh.ax=0;
    m_mesh.ay=0;
    m_mesh.az=0;
    m_mesh.sx=1;
    m_mesh.sy=1;
    m_mesh.sz=1;
    m_mesh.sizeofVertex=allVertex.size();
    m_mesh.sizeofIbo=fVect.size();
    m_mesh.program=0;
    m_mesh.textureMap= ( GLuint ) loadTexture ( ( char* ) "textures/demon.png" );
    //m_mesh.normalMap= ( GLuint ) loadTexture ( ( char* ) "textures/gcanyon.dds" );

    free ( arrayIbo );
    free ( arrayVertex );

    cout << "Nb Vertex: " << allVertex.size() << endl;
    cout << "Nb Triangles: " << fVect.size() /3 << endl;

    return true;
}

void GraphicEngine::setMeshAttribute ( unsigned int program, float x,float y, float z, float ax,float ay, float az,float sx, float sy, float sz )
{
    m_mesh.x=x;
    m_mesh.y=y;
    m_mesh.z=z;
    m_mesh.ax=ax;
    m_mesh.ay=ay;
    m_mesh.az=az;
    m_mesh.sx=sx;
    m_mesh.sy=sy;
    m_mesh.sz=sz;
    m_mesh.program=program;
}

void GraphicEngine::setLightDir ( float x,float y,float z )
{
    m_lightDir[0]=x;
    m_lightDir[1]=y;
    m_lightDir[2]=z;
}

void GraphicEngine::render ( float time )
{
    glEnable ( GL_DEPTH_TEST );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //Calcul de la matrice de vue

    float camrotateX[4][4]=
    {
        {1.0, 0.0, 0.0,0.0},
        {0.0, cos ( m_camera.ax ), sin ( m_camera.ax ),0.0},
        {0.0, -sin ( m_camera.ax ), cos ( m_camera.ax ),0.0},
        {0.0, 0.0, 0.0,1.0}
    };

    float camrotateY[4][4]=
    {
        {cos ( m_camera.ay ), sin ( m_camera.ay ), 0.0,0.0},
        {-sin ( m_camera.ay ), cos ( m_camera.ay ), 0.0,0.0},
        {0.0, 0.0, 1.0,0.0},
        {0.0, 0.0, 0.0,1.0}
    };

    float camrotateZ[4][4]=
    {
        {cos ( m_camera.az ), 0.0, -sin ( m_camera.az ),0.0},
        {0.0, 1.0, 0.0,0.0},
        {sin ( m_camera.az ), 0.0, cos ( m_camera.az ),0.0},
        {0.0, 0.0, 0.0,1.0}
    };

    float camtranslation[4][4]=
    {
        {1.0, 0.0, 0.0,0.0},
        {0.0, 1.0, 0.0,0.0},
        {0.0, 0.0, 1.0,0.0},
        {m_camera.x, m_camera.y, m_camera.z,1.0}
    };

    matrix4Id ( viewMatrix );

    matrix4Mul ( camrotateX,viewMatrix );
    matrix4Mul ( camrotateY,viewMatrix );
    matrix4Mul ( camrotateZ,viewMatrix );
    matrix4Mul ( camtranslation,viewMatrix );

    //On calcule la matrix du model

    float modelrotateX[4][4]=
    {
        {1.0, 0.0, 0.0,0.0},
        {0.0, cos ( m_mesh.ax ), sin ( m_mesh.ax ),0.0},
        {0.0, -sin ( m_mesh.ax ), cos ( m_mesh.ax ),0.0},
        {0.0, 0.0, 0.0,1.0}
    };

    float modelrotateY[4][4]=
    {
        {cos ( m_mesh.ay ), sin ( m_mesh.ay ), 0.0,0.0},
        {-sin ( m_mesh.ay ), cos ( m_mesh.ay ), 0.0,0.0},
        {0.0, 0.0, 1.0,0.0},
        {0.0, 0.0, 0.0,1.0}
    };

    float modelrotateZ[4][4]=
    {
        {cos ( m_mesh.az ), 0.0, -sin ( m_mesh.az ),0.0},
        {0.0, 1.0, 0.0,0.0},
        {sin ( m_mesh.az ), 0.0, cos ( m_mesh.az ),0.0},
        {0.0, 0.0, 0.0,1.0}
    };

    float modeltranslation[4][4]=
    {
        {1.0, 0.0, 0.0,0.0},
        {0.0, 1.0, 0.0,0.0},
        {0.0, 0.0, 1.0,0.0},
        {m_mesh.x, m_mesh.y, m_mesh.z,1.0}
    };

    float modelscale[4][4]=
    {
        {m_mesh.sx, 0.0, 0.0,0.0},
        {0.0, m_mesh.sy, 0.0,0.0},
        {0.0, 0.0, m_mesh.sz,0.0},
        {0.0, 0.0, 0.0,1.0}
    };

    matrix4Id ( modelMatrix );

    matrix4Mul ( modelrotateX,modelMatrix );
    matrix4Mul ( modelrotateY,modelMatrix );
    matrix4Mul ( modelrotateZ,modelMatrix );
    matrix4Mul ( modelscale,modelMatrix );
    matrix4Mul ( modeltranslation,modelMatrix );

    //On calcul les matrices manquantes

    matrix4Id ( modelViewMatrix );
    matrix4Mul ( viewMatrix,modelViewMatrix );
    matrix4Mul ( modelMatrix,modelViewMatrix );

    matrix4Id ( modelViewProjectionMatrix );
    matrix4Mul ( projectionMatrix,modelViewProjectionMatrix );
    matrix4Mul ( modelViewMatrix,modelViewProjectionMatrix );

    float modelMatrix3x3[3][3]=
    {
        {modelMatrix[0][0],modelMatrix[0][1],modelMatrix[0][2]},
        {modelMatrix[1][0],modelMatrix[1][1],modelMatrix[1][2]},
        {modelMatrix[2][0],modelMatrix[2][1],modelMatrix[2][2]}
    };
    matrix3Inv ( modelMatrix3x3,normalMatrix );

    //On envoie les donnees

    glUseProgram ( m_mesh.program );


    //rien a foutre ici, ou pas
    modelMatrixId = glGetUniformLocation ( m_mesh.program, "modelMatrix" );
    viewMatrixId = glGetUniformLocation ( m_mesh.program, "viewMatrix" );
    modelViewMatrixId = glGetUniformLocation ( m_mesh.program, "modelViewMatrix" );
    projectionMatrixId = glGetUniformLocation ( m_mesh.program, "projectionMatrix" );
    modelViewProjectionMatrixId = glGetUniformLocation ( m_mesh.program, "modelViewProjectionMatrix" );
    normalMatrixId = glGetUniformLocation ( m_mesh.program, "normalMatrix" );
    lightDirId = glGetUniformLocation ( m_mesh.program, "lightDir" );
    cameraDirId = glGetUniformLocation ( m_mesh.program, "cameraDir" );


    glUniformMatrix4fv ( modelMatrixId, 1, 0, ( GLfloat* ) modelMatrix );
    glUniformMatrix4fv ( viewMatrixId, 1, 0, ( GLfloat* ) viewMatrix );
    glUniformMatrix4fv ( modelViewMatrixId, 1, 0, ( GLfloat* ) modelViewMatrix );
    glUniformMatrix4fv ( projectionMatrixId, 1, 0, ( GLfloat* ) projectionMatrix );
    glUniformMatrix4fv ( modelViewProjectionMatrixId, 1, 0, ( GLfloat* ) modelViewProjectionMatrix );
    glUniformMatrix3fv ( normalMatrixId, 1, 0, ( GLfloat* ) modelMatrix3x3 );
    glUniform3fv ( lightDirId, 1, ( GLfloat* ) m_lightDir );
    glUniform3f ( cameraDirId,m_camera.x,m_camera.y,m_camera.z );


    //On lie les objects buffers aux shaders


    glBindBuffer ( GL_ARRAY_BUFFER, m_mesh.vboVertex );

    GLuint positionId = glGetAttribLocation ( m_mesh.program, "vertexPosition" ) ;
    glBindAttribLocation ( m_mesh.program,0,"vertexPosition" );
    glEnableVertexAttribArray ( 0 ); //positions
    glVertexAttribPointer ( ( GLuint ) 0,3, GL_FLOAT,GL_FALSE, 32, ( char * ) NULL + 0 );

    GLuint normalId = glGetAttribLocation ( m_mesh.program, "vertexNormal" ) ;
    glBindAttribLocation ( m_mesh.program,1,"vertexNormal" );
    glEnableVertexAttribArray ( 1 ); //normales
    glVertexAttribPointer ( ( GLuint ) 1,3, GL_FLOAT,GL_TRUE, 32, ( char * ) NULL + 12 );

    GLuint textureId = glGetAttribLocation ( m_mesh.program, "vertexTexture" ) ;
    glBindAttribLocation ( m_mesh.program,2,"vertexTexture" );
    glEnableVertexAttribArray ( 2 ); //textures
    glVertexAttribPointer ( ( GLuint ) 2,2, GL_FLOAT,GL_FALSE, 32, ( char * ) NULL + 24 );

    glBindTexture ( GL_TEXTURE_2D, m_mesh.textureMap );


    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER,m_mesh.ibo );
    glDrawElements ( GL_TRIANGLES, m_mesh.sizeofIbo, GL_UNSIGNED_INT,0 );


    glFlush();
    SDL_GL_SwapBuffers();


}

int loadTexture ( char* imagePath )
{
    //Chargement de la texture
    ILuint ilTextId;
    GLuint glTextId;

    ilGenImages ( 1,&ilTextId );
    ilBindImage ( ilTextId );
    if ( !ilLoadImage ( imagePath ) )
        exit ( 11 );
    if ( !ilConvertImage ( IL_RGBA,IL_UNSIGNED_BYTE ) )
        exit ( 11 );

    glGenTextures ( 1,&glTextId );
    glBindTexture ( GL_TEXTURE_2D,glTextId );

    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexImage2D (
        GL_TEXTURE_2D,
        0,
        ilGetInteger ( IL_IMAGE_BPP ),
        ilGetInteger ( IL_IMAGE_WIDTH ),
        ilGetInteger ( IL_IMAGE_HEIGHT ),
        0,
        ilGetInteger ( IL_IMAGE_FORMAT ),
        GL_UNSIGNED_BYTE,
        ilGetData()
    );

    ilDeleteImages ( 1, &ilTextId );


    return glTextId;
}

int ExtensionSupportee ( const char *nom )
{
    if ( glewIsSupported ( nom ) )
        return 1;
    else
    {
        fprintf ( stderr, "extension %s non supportee\n", nom );
        return 0;
    }
}


int InitExtensions ( void )
{
    int state = 1;

    if ( !ExtensionSupportee ( "GL_ARB_shading_language_100" ) ||
         !ExtensionSupportee ( "GL_ARB_shader_objects" ) ||
         !ExtensionSupportee ( "GL_ARB_vertex_shader" ) ||
         !ExtensionSupportee ( "GL_ARB_fragment_shader" ) ||
         !ExtensionSupportee ( "GL_EXT_geometry_shader4" )
       )
    {
        state = 0;
    }

    return state;
}



/* charge le code source d'un shader */
static char* LoadSource ( const char *filename )
{
    char *src = NULL;   /* code source de notre shader */
    FILE *fp = NULL;    /* fichier */
    long size;          /* taille du fichier */
    long i;             /* compteur */


    /* on ouvre le fichier */
    fp = fopen ( filename, "r" );
    /* on verifie si l'ouverture a echoue */
    if ( fp == NULL )
    {
        fprintf ( stderr, "impossible d'ouvrir le fichier '%s'\n", filename );
        return NULL;
    }

    /* on recupere la longueur du fichier */
    fseek ( fp, 0, SEEK_END );
    size = ftell ( fp );

    /* on se replace au debut du fichier */
    rewind ( fp );

    /* on alloue de la memoire pour y placer notre code source */
    src = ( char* ) malloc ( size+1 ); /* +1 pour le caractere de fin de chaine '\0' */
    if ( src == NULL )
    {
        fclose ( fp );
        fprintf ( stderr, "erreur d'allocation de memoire!\n" );
        return NULL;
    }

    /* lecture du fichier */
    for ( i=0; i<size; i++ )
        src[i] = fgetc ( fp );

    /* on place le dernier caractere a '\0' */
    src[size] = '\0';

    fclose ( fp );

    return src;
}


unsigned int GraphicEngine::loadProgram ( const char *vsname,const char* gsname, const char *psname )
{
    GLuint prog = 0;
    GLuint vs = 0, ps = 0, gs = 0;
    GLint link_status = GL_TRUE;
    GLint logsize = 0;
    char *log = NULL;


    /* verification des arguments */
    if ( vsname == NULL && gsname == NULL && psname == NULL )
    {
        fprintf ( stderr, "creation d'un program demande, mais aucuns "
                  "noms de fichiers source envoye, arret.\n" );

        return 0;
    }


    /* chargement des shaders */
    if ( vsname != NULL )
    {
        vs = LoadShader ( GL_VERTEX_SHADER, vsname );
        if ( vs == 0 )
            return 0;
    }
    if ( gsname != NULL )
    {
        gs = LoadShader ( GL_GEOMETRY_SHADER_ARB, gsname );
        if ( gs == 0 )
            return 0;
    }
    if ( psname != NULL )
    {
        ps = LoadShader ( GL_FRAGMENT_SHADER, psname );
        if ( ps == 0 )
        {
            if ( glIsShader ( vs ) )
                glDeleteShader ( vs );
            return 0;
        }
    }


    /* creation du program */
    prog = glCreateProgram();

    /* on envoie nos shaders a notre program */
    if ( vs )
        glAttachShader ( prog, vs );
    if ( gs )
        glAttachShader ( prog, gs );
    if ( ps )
        glAttachShader ( prog, ps );

    glProgramParameteriARB ( prog,GL_GEOMETRY_INPUT_TYPE_ARB,GL_TRIANGLES );
    glProgramParameteriARB ( prog,GL_GEOMETRY_OUTPUT_TYPE_ARB,GL_TRIANGLES );
    int temp;
    glGetIntegerv ( GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB,&temp );
    glProgramParameteriARB ( prog,GL_GEOMETRY_VERTICES_OUT_ARB,temp );

    /* on lie le tout */
    glLinkProgram ( prog );

    /* on verifie que tout s'est bien passe */
    glGetProgramiv ( prog, GL_LINK_STATUS, &link_status );
    if ( link_status != GL_TRUE )
    {
        glGetProgramiv ( prog, GL_INFO_LOG_LENGTH, &logsize );
        log = ( char* ) malloc ( logsize + 1 );
        if ( log == NULL )
        {
            glDeleteProgram ( prog );
            glDeleteShader ( vs );
            glDeleteShader ( gs );
            glDeleteShader ( ps );

            fprintf ( stderr, "impossible d'allouer de la memoire!\n" );
            return 0;
        }
        memset ( log, '\0', logsize + 1 );
        glGetProgramInfoLog ( prog, logsize, &logsize, log );

        fprintf ( stderr, "impossible de lier le program :\n%s", log );

        free ( log );
        glDeleteProgram ( prog );
        glDeleteShader ( vs );
        glDeleteShader ( gs );
        glDeleteShader ( ps );

        return 0;
    }

    /* les shaders sont dans le program maintenant, on en a plus besoin */
    glDeleteShader ( vs );
    glDeleteShader ( gs );
    glDeleteShader ( ps );
    return ( unsigned int ) prog;
}


GLuint LoadShader ( GLenum type, const char *filename )
{
    GLuint shader = 0;
    GLsizei logsize = 0;
    GLint compile_status = GL_TRUE;
    char *log = NULL;
    char *src = NULL;

    /* creation d'un shader de sommet */
    shader = glCreateShader ( type );
    if ( shader == 0 )
    {
        fprintf ( stderr, "impossible de creer le shader\n" );
        return 0;
    }

    /* chargement du code source */
    src = LoadSource ( filename );
    if ( src == NULL )
    {
        /* theoriquement, la fonction LoadSource a deja affiche un message
        d'erreur, nous nous contenterons de supprimer notre shader
        et de retourner 0 */

        glDeleteShader ( shader );
        return 0;
    }

    /* assignation du code source */
    glShaderSource ( shader, 1, ( const GLchar** ) &src, NULL );


    /* compilation du shader */
    glCompileShader ( shader );

    /* liberation de la memoire du code source */
    free ( src );
    src = NULL;

    /* verification du succes de la compilation */
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compile_status );
    if ( compile_status != GL_TRUE )
    {
        /* erreur a la compilation recuperation du log d'erreur */

        /* on recupere la taille du message d'erreur */
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &logsize );

        /* on alloue un esapce memoire dans lequel OpenGL ecrira le message */
        log = ( char * ) malloc ( logsize + 1 );
        if ( log == NULL )
        {
            fprintf ( stderr, "impossible d'allouer de la memoire!\n" );
            return 0;
        }
        /* initialisation du contenu */
        memset ( log, '\0', logsize + 1 );

        glGetShaderInfoLog ( shader, logsize, &logsize, log );
        fprintf ( stderr, "impossible de compiler le shader '%s' :\n%s",
                  filename, log );

        /* ne pas oublier de liberer la memoire et notre shader */
        free ( log );
        glDeleteShader ( shader );

        return 0;
    }

    return shader;
}

void inline matrix4Mul ( float a[4][4],float b[4][4] )
{
    float t[4][4]={{0}};
    for ( int x=0;x<4;x++ )
    {
        for ( int y=0;y<4;y++ )
        {
            for ( int i=0;i<4;i++ )
                t[y][x]+=a[y][i]*b[i][x];
        }
    }

    for ( int x=0;x<4;x++ )
    {
        for ( int y=0;y<4;y++ )
        {
            b[y][x]=t[y][x];
        }
    }
}

float inline vecteur3Norme ( float a[3] )
{
    return sqrtf ( a[0]*a[0]+a[1]*a[1]+a[2]*a[2] );
}

float inline vecteur3Dot ( float u[3],float v[3] )
{
    return u[0]*v[0]+u[1]*v[1]+u[2]*u[2];
}

void matrix4Inv ( float M[4][4],float MS[4][4] )
{

    float MM[4][4],M1[4][4];// d�claration de matrices de taille identique � celle � inverser

    for ( int i=0;i<4;i++ )   // copie de la matrice � inverser
    {
        for ( int j=0;j<4;j++ )
        {
            MM[i][j]=M[i][j];
            M1[i][j]=M[i][j];
        }
    }

    float Mi1[4][4]=
    {
        {1.0, 0.0, 0.0,0.0},
        {0.0, 1.0, 0.0,0.0},
        {0.0, 0.0, 1.0,0.0},
        {0.0, 0.0, 0.0,1.0}
    };

    float Minv[4][4]=
    {
        {1.0, 0.0, 0.0,0.0},
        {0.0, 1.0, 0.0,0.0},
        {0.0, 0.0, 1.0,0.0},
        {0.0, 0.0, 0.0,1.0}
    };


    for ( int i=0;i<4;i++ )
    {
        for ( int j=0;j<4;j++ )
        {
            if ( MM[i][i]==0 )
            {
                cerr << "inversion pivot GAUSS impossible : division par 0\n"<< endl;
                return;
            }
            M1[i][j]=MM[i][j]/MM[i][i];
            Mi1[i][j]=Minv[i][j]/MM[i][i];
        }

        for ( int ii=0;ii<4;ii++ )
        {
            for ( int j=0;j<4;j++ )
            {
                MM[ii][j]=M1[ii][j];
            }
        }
        for ( int ii=0;ii<4;ii++ )
        {
            for ( int j=0;j<4;j++ )
            {
                Minv[ii][j]=Mi1[ii][j];
            }
        }

        for ( int k=0;k<4;k++ ) // mise z�ros de la colonne
            if ( k!=i )
                for ( int j=0;j<4;j++ )
                {
                    M1[k][j]=MM[k][j]-MM[i][j]*MM[k][i];
                    Mi1[k][j]=Minv[k][j]-Minv[i][j]*MM[k][i];
                }


        for ( int i=0;i<4;i++ )
        {
            for ( int j=0;j<4;j++ )
            {
                MM[i][j]=M1[i][j];
            }
        }
        for ( int i=0;i<4;i++ )
        {
            for ( int j=0;j<4;j++ )
            {
                Minv[i][j]=Mi1[i][j];
            }
        }

    }

    for ( int i=0;i<4;i++ )
    {
        for ( int j=0;j<4;j++ )
        {
            MS[i][j]=Minv[i][j];
        }
    }

}


void matrix3Inv ( float M[3][3],float MS[3][3] )
{

    float MM[3][3],M1[3][3];// d�claration de matrices de taille identique � celle � inverser

    for ( int i=0;i<3;i++ )   // copie de la matrice � inverser
    {
        for ( int j=0;j<3;j++ )
        {
            MM[i][j]=M[i][j];
            M1[i][j]=M[i][j];
        }
    }

    float Mi1[3][3]=
    {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    float Minv[3][3]=
    {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };


    for ( int i=0;i<3;i++ )
    {
        for ( int j=0;j<3;j++ )
        {
            if ( MM[i][i]==0 )
            {
                cerr << "inversion pivot GAUSS impossible : division par 0\n"<< endl;
                return;
            }
            M1[i][j]=MM[i][j]/MM[i][i];
            Mi1[i][j]=Minv[i][j]/MM[i][i];
        }

        for ( int ii=0;ii<3;ii++ )
        {
            for ( int j=0;j<3;j++ )
            {
                MM[ii][j]=M1[ii][j];
            }
        }
        for ( int ii=0;ii<3;ii++ )
        {
            for ( int j=0;j<3;j++ )
            {
                Minv[ii][j]=Mi1[ii][j];
            }
        }

        for ( int k=0;k<3;k++ ) // mise z�ros de la colonne
            if ( k!=i )
                for ( int j=0;j<3;j++ )
                {
                    M1[k][j]=MM[k][j]-MM[i][j]*MM[k][i];
                    Mi1[k][j]=Minv[k][j]-Minv[i][j]*MM[k][i];
                }


        for ( int i=0;i<3;i++ )
        {
            for ( int j=0;j<3;j++ )
            {
                MM[i][j]=M1[i][j];
            }
        }
        for ( int i=0;i<3;i++ )
        {
            for ( int j=0;j<3;j++ )
            {
                Minv[i][j]=Mi1[i][j];
            }
        }

    }

    for ( int i=0;i<3;i++ )
    {
        for ( int j=0;j<3;j++ )
        {
            MS[i][j]=Minv[i][j];
        }
    }

}

void inline matrix4Id ( float a[4][4] )
{
    for ( int i=0;i<4;i++ )
        for ( int j=0;j<4;j++ )
            if ( i==j )
                a[i][j]=1;
            else
                a[i][j]=0;
}

